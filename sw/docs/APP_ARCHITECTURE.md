# Application architecture (`app/src`)

The system controller application is split into function groups. `main.c` contains only the
CPU-level startup and the bring-up sequence; every device is owned by exactly one module.

## Layering

```
main.c
  -> power.c / clkGen.c / boardGpio.c / i2cBus.c      (function groups, app/src)
     -> devices/   (tpsm8286x, ina745a, si5332, pi4msd5v945x)
        -> drivers/  (i2cWrapper)
           -> bsp / mcu driver
```

A module never reaches around a layer: device modules talk to devices, devices talk to the
injected HAL callbacks, and only `i2cBus.c` knows the controller instances.

| File | Owns | Responsibility |
|---|---|---|
| [`boardConfig.h`](../app/src/boardConfig.h) | - | **Single** place for board values: wiring, timing, setpoints, limits, clock config |
| [`userDef.h`](../app/src/userDef.h) | - | Instance-pool sizes for drivers/devices (name required by BSP drivers) |
| [`i2cBus.c`](../app/src/i2cBus.c) | PLIC handle, 2 I2C controllers, `pi4msd5v945x` | Bus infrastructure and segment selection |
| [`boardGpio.c`](../app/src/boardGpio.c) | GPIO output shadow | Named signal access, the only place evaluating GPIO bit positions |
| [`power.c`](../app/src/power.c) | 2x `tpsm8286x`, 6x `ina745a` | FMC Vadj regulators, power monitoring, protection, fault shadow |
| [`clkGen.c`](../app/src/clkGen.c) | 1x `si5332` | Clock generator configuration and output control |
| [`interrupt.c`](../app/src/interrupt.c) | PLIC registers | Generic interrupt controller driver |
| [`main.c`](../app/src/main.c) | - | `bsp_init`, trap vector/CSR setup, bring-up sequence, supervision loop |

### Module contract

- Handles and configuration tables are `static` inside the `.c` file; headers expose functions only.
- `<mod>_init()` creates handles without bus traffic; `<mod>_configure()` performs device access.
- Every function returns a status enum; the caller decides how to report.
- Board values are read from `boardConfig.h` only — no addresses, bit positions or setpoints
  anywhere else in `app/src`.

### Naming constraint

`app/src` must not contain `i2c.h` or `gpio.h`: the BSP already provides those names
([`i2c.h`](../mcu/software/standalone/driver/i2c.h),
[`gpio.h`](../mcu/software/standalone/driver/gpio.h)) and the makefile puts every `src`
directory on the include path. Hence `i2cBus.*` and `boardGpio.*`.

## Configuration

Everything configurable lives in [`boardConfig.h`](../app/src/boardConfig.h):

| Section | Contents |
|---|---|
| 1. Wiring | GPIO bases and bit positions, PG/ALERT masks, PLIC, both I2C controllers, device addresses, bus-switch channels |
| 2. Timing | Supply/clock settling delays, ETH reset pulse, monitoring period |
| 3. FMC Vadj | `BOARD_FMC1_VADJ_V`, `BOARD_FMC2_VADJ_V`, `BOARD_FMC_VADJ_MAX_V`, control register word |
| 4. Power monitors | ADC mode/conversion times/averaging, alert behaviour, disabled under-limit values |
| 5. Rail table | `BOARD_PWRMON_TABLE`: name, I2C address, PG group, over-voltage, over-current, over-temperature |
| 6. Clock generator | Input source plus enable/frequency/format for OUT0, OUT2, OUT4, OUT7 |

`BOARD_PWRMON_TABLE` is an X-macro. It generates the `pwrRail_t` enum in
[`power.h`](../app/src/power.h) and the configuration array in
[`power.c`](../app/src/power.c), so rail names, addresses, groups and limits can never drift
apart. The number of rows must equal `INA745A_NUM_INSTANCES` in `userDef.h`; `power.c` enforces
this at compile time.

## Power monitoring and protection

### ALERT is wired onto the power-good nets

The INA745A ALERT outputs are open-drain and wired-OR onto the power-good inputs:

| Power-good input | Monitors |
|---|---|
| `SYS_PG` | 0x40 (12V), 0x41 (3V3), 0x43 (5V0_SOM), 0x44 (1V8) |
| `FMC1_SUP_PG` | 0x4C (VCCADJ_FMC1) |
| `FMC2_SUP_PG` | 0x4D (VCCADJ_FMC2) |

Two configuration values follow directly from this wiring and must not be changed:

- `BOARD_PWRMON_ALERT_POL = INA745A_ALERT_ACTIVE_LOW` — the open-drain output pulls the net low.
- `BOARD_PWRMON_ALERT_CNVR = 0` — a conversion-ready alert would pull the power-good net low
  after every conversion.

### Armed limits

Over-voltage, over-current and over-temperature are armed with `ALATCH = 1`, so a short event
stays visible until the device is read. Under-voltage and under-current protection are not used;
since the driver API always writes both thresholds, non-trippable values are programmed
(`BUVL = 0 V`, `CURRENTUL = -39 A`, both clamped to the register minimum by the driver). The
corresponding `BUSUL`/`CURRENTUL` status bits are ignored by `power.c`.

### Arming order

`pwr_mon_configure()` is called **after** `boardGpio_waitSomReady()` and **before** the FMC
supply is switched on:

- After the wait, because once armed, a latched alert holds `SYS_PG` low and would stall the
  wait loop.
- Before the FMC supply, because all monitors are powered as soon as the MCU runs, so the FMC
  rails are already protected while they ramp up.

### Polling

`pwr_checkAlerts()` reads the three power-good lines through `boardGpio_getPgStatus()` — a
single GPIO read, no bus traffic. Only when a line is low are the monitors of that group queried
over I2C:

- at least one monitor reports a flag → monitor alert, cause is in the fault shadow
- no monitor reports a flag → the supply itself failed (`PWR_E_REGULATION`)

Reading `DIAG_ALRT` clears the latch in the device (and releases the net if the condition is
gone), so `power.c` keeps a sticky fault shadow that is OR-accumulated across all queries.
Use `pwr_mon_getFaults()` / `pwr_mon_reportFaults()` to inspect it and `pwr_mon_clearFaults()`
to reset it. `pwr_mon_read()` / `pwr_mon_poll()` provide the plain voltage, current and
temperature readings.

## Bring-up sequence

```
bsp_init
boardGpio_init                 all outputs off, POR_B asserted, ETH PHY in reset
i2cBus_init                    PLIC, both I2C controllers, bus switch
pwr_init                       create regulator and monitor handles
clkGen_init                    create clock generator handle
cpu_enableInterrupts           trap vector, MIE/MSTATUS
boardGpio_waitSomReady         wait for VCCO_PS_EN, VCCO_PL_EN, SYS_PG
clkGen_configure               source, per-output frequency/format/enable, single commit
boardGpio_releaseEthPhyReset
pwr_mon_configure              ADC config, OV/OC/OT limits, arm latched alert, clear latches
boardGpio_setFmcSupplyEnable(1)
pwr_fmcVadj_configure          control word and setpoints (outputs still disabled)
pwr_fmcVadj_isRegulating       voltage register plausibility check
pwr_fmcVadj_enable(1)
pwr_checkAlerts                FMC and system rails
boardGpio_releaseSomPor
-> supervision loop: pwr_checkAlerts + pwr_mon_poll every BOARD_MONITOR_PERIOD_US
```
