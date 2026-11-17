/*******************************************************************************
*
* @file boardConfig.h
*
* @brief Single configuration point of the system controller application.
*
*        Everything board specific lives here: pin assignment, controller
*        instances, device addresses, timing, voltage setpoints, protection
*        limits and the clock generator configuration. No other file in
*        app/src/ is allowed to contain board values.
*
*        Layering:
*          main.c -> power / clkGen / boardGpio / i2cBus -> devices -> drivers -> bsp
*
*        Sections:
*          1. Wiring            (fixed by the schematic, do not change)
*          2. Timing
*          3. FMC Vadj          (tpsm8286x)
*          4. Power monitors    (ina745a, ADC + alert behaviour)
*          5. Power monitors    (per rail: address, PG group, protection limits)
*          6. Clock generator   (si5332)
*
*******************************************************************************/

#ifndef SRC_BOARDCONFIG_H_
#define SRC_BOARDCONFIG_H_

#include "soc.h"

#include "ina745a.h"
#include "pi4msd5v945x.h"
#include "si5332.h"
#include "tpsm8286x.h"

/* =========================================================================== */
/* 1. Wiring (fixed by the schematic)                                          */
/* =========================================================================== */

/* --- Interrupt controller ------------------------------------------------- */
#define BOARD_PLIC_BASE			SYSTEM_PLIC_CTRL

/* --- GPIO controllers ----------------------------------------------------- */
#define BOARD_GPIO_IN			SYSTEM_GPIO_0_IO_CTRL
#define BOARD_GPIO_OUT			SYSTEM_GPIO_1_IO_CTRL

/* --- GPIO input bit positions --------------------------------------------- */
#define BOARD_GPIO_IN_VCCO_PS_EN_SHIFT	0
#define BOARD_GPIO_IN_VCCO_PL_EN_SHIFT	1
#define BOARD_GPIO_IN_SDPWR_EN_SHIFT	2
#define BOARD_GPIO_IN_HDA14P_SHIFT	3
#define BOARD_GPIO_IN_HDA14N_SHIFT	4
#define BOARD_GPIO_IN_MIO35_SHIFT	5
#define BOARD_GPIO_IN_FMC1_SUP_PG_SHIFT	6
#define BOARD_GPIO_IN_FMC2_SUP_PG_SHIFT	7
#define BOARD_GPIO_IN_FMC1_MOD_PG_SHIFT	8
#define BOARD_GPIO_IN_FMC2_MOD_PG_SHIFT	9
#define BOARD_GPIO_IN_2V5_PG_SHIFT	10
#define BOARD_GPIO_IN_SYS_PG_SHIFT	11
#define BOARD_GPIO_IN_SZG_PG_SHIFT	12
#define BOARD_GPIO_IN_LMK_INTR_SHIFT	13
#define BOARD_GPIO_IN_ETH_PME_SHIFT	14

/* --- GPIO output bit positions -------------------------------------------- */
#define BOARD_GPIO_OUT_FMC1_SUP_EN_SHIFT	0
#define BOARD_GPIO_OUT_FMC2_SUP_EN_SHIFT	1
#define BOARD_GPIO_OUT_FMC1_MOD_EN_SHIFT	2
#define BOARD_GPIO_OUT_FMC2_MOD_EN_SHIFT	3
#define BOARD_GPIO_OUT_POR_B_SHIFT		4
#define BOARD_GPIO_OUT_ETH_RSTN_SHIFT		5

/*
 * Power-good / ALERT nets.
 *
 * The ALERT outputs of the INA745A power monitors are open-drain and are
 * wired-OR onto the power-good inputs below. A line therefore reads low
 * either because the supply is not good or because an INA745A on that net
 * has asserted ALERT. See BOARD_PWRMON_TABLE for the rail-to-net mapping
 * and pwr_checkAlerts() for how the two causes are distinguished.
 */
#define BOARD_PG_SYS_MASK		(1u << BOARD_GPIO_IN_SYS_PG_SHIFT)
#define BOARD_PG_FMC1_SUP_MASK		(1u << BOARD_GPIO_IN_FMC1_SUP_PG_SHIFT)
#define BOARD_PG_FMC2_SUP_MASK		(1u << BOARD_GPIO_IN_FMC2_SUP_PG_SHIFT)

/* Signals that indicate that the SoM has switched on its PL/PS supplies */
#define BOARD_SOM_READY_MASK		((1u << BOARD_GPIO_IN_VCCO_PL_EN_SHIFT) | \
					 (1u << BOARD_GPIO_IN_VCCO_PS_EN_SHIFT) | \
					 BOARD_PG_SYS_MASK)

/* --- I2C controller: PMBus (DC/DC converters and power monitors) ----------- */
#define BOARD_I2C_PMBUS_BASE		SYSTEM_I2C_1_IO_CTRL
#define BOARD_I2C_PMBUS_DEV_ID		0x1
#define BOARD_I2C_PMBUS_CORE_FREQ	SYSTEM_CLINT_HZ
#define BOARD_I2C_PMBUS_IF_FREQ		100000U
#define BOARD_I2C_PMBUS_INTR_ID		SYSTEM_PLIC_SYSTEM_I2C_1_IO_INTERRUPT
#define BOARD_I2C_PMBUS_INTR_PRIO	1U
#define BOARD_I2C_PMBUS_INTR_HART	0U

/* --- I2C controller: system configuration (clock generator, add-on modules) - */
#define BOARD_I2C_SYSCFG_BASE		SYSTEM_I2C_0_IO_CTRL
#define BOARD_I2C_SYSCFG_DEV_ID		0x0
#define BOARD_I2C_SYSCFG_CORE_FREQ	SYSTEM_CLINT_HZ
#define BOARD_I2C_SYSCFG_IF_FREQ	100000U
#define BOARD_I2C_SYSCFG_INTR_ID	SYSTEM_PLIC_SYSTEM_I2C_0_IO_INTERRUPT
#define BOARD_I2C_SYSCFG_INTR_PRIO	2U
#define BOARD_I2C_SYSCFG_INTR_HART	0U

/* Common controller timing (sampling divider and bus timeout) */
#define BOARD_I2C_SAMPLING_DIVIDER	3
#define BOARD_I2C_TIMEOUT_DIVIDER	10	/* timeout = coreFreq / 10 = 0.1 s */

/* --- Device I2C addresses -------------------------------------------------- */
#define BOARD_FMC1_VADJ_I2C_ADDR	0x4E
#define BOARD_FMC2_VADJ_I2C_ADDR	0x4F
#define BOARD_CLKGEN_I2C_ADDR		0x6A
#define BOARD_BUS_SWITCH_I2C_ADDR	0x77

/* --- I2C bus switch channels (SysCfg bus) ---------------------------------- */
#define BOARD_I2C_CH_SYZYGY		PI4MSD5V945X_CH0
#define BOARD_I2C_CH_FMC2		PI4MSD5V945X_CH1
#define BOARD_I2C_CH_FMC1		PI4MSD5V945X_CH2
#define BOARD_I2C_CH_MBRD		PI4MSD5V945X_CH3

/* =========================================================================== */
/* 2. Timing                                                                   */
/* =========================================================================== */
#define BOARD_POWER_SETTLE_US		10000u	/* wait for supplies to stabilize */
#define BOARD_CLK_SETTLE_US		10000u	/* wait for clock outputs */
#define BOARD_ETH_RESET_PULSE_US	10u	/* ETH PHY reset pulse width */
#define BOARD_MONITOR_PERIOD_US		1000000u/* period of the monitoring loop */

/* =========================================================================== */
/* 3. FMC Vadj (tpsm8286x)                                                     */
/* =========================================================================== */
#define BOARD_FMC1_VADJ_V		1.8f
#define BOARD_FMC2_VADJ_V		1.8f
#define BOARD_FMC_VADJ_MAX_V		1.8f	/* hard upper bound, checked at startup */

#define BOARD_FMC_VADJ_CNTRL		(TPSM8286_FPWM_AT_VC_MASK | \
					 TPSM8286_FPWM_MASK | \
					 TPSM8286_OUTPUT_DISCHARGE_MASK | \
					 TPSM8286_HICCUP_EN_MASK | \
					 TPSM8286_VOLTAGE_RAMP_SPEED_1MV)

/* =========================================================================== */
/* 4. Power monitors: ADC and alert behaviour (ina745a)                        */
/* =========================================================================== */
#define BOARD_PWRMON_MODE		INA745A_MODE_CONT_TEMP_CURRENT_BUSV
#define BOARD_PWRMON_VBUSCT		INA745A_CT_1052US
#define BOARD_PWRMON_VSENCT		INA745A_CT_1052US
#define BOARD_PWRMON_TCT		INA745A_CT_1052US
#define BOARD_PWRMON_AVG		INA745A_AVG_1

/*
 * Alert configuration. Both values are dictated by the wiring (ALERT on the
 * power-good nets, see section 1):
 *   - ALATCH = 1: a fault is latched until the DIAG_ALRT register is read,
 *     so short events cannot be missed between two polls.
 *   - CNVR = 0: a conversion-ready alert would pull the power-good net low
 *     after every conversion and must stay disabled.
 *   - APOL = active low: the open-drain output pulls the net low on fault.
 */
#define BOARD_PWRMON_ALERT_LATCH	1
#define BOARD_PWRMON_ALERT_CNVR		0
#define BOARD_PWRMON_ALERT_SLOW		0	/* compare against the measured value */
#define BOARD_PWRMON_ALERT_POL		INA745A_ALERT_ACTIVE_LOW

/*
 * Under-voltage and under-current protection are not used on this board.
 * The driver API always writes both thresholds, so non-trippable values are
 * programmed: BUVL = 0 V (VBUS is unsigned, never below 0) and
 * CURRENTUL = the most negative value the 16-bit register can hold.
 */
#define BOARD_PWRMON_UV_DISABLED_V	0.0f
#define BOARD_PWRMON_UI_DISABLED_A	(-39.0f)

/* =========================================================================== */
/* 5. Power monitors: per-rail configuration                                   */
/* =========================================================================== */
/*
 * One row per monitored rail. This table is the only place where rails are
 * defined; power.h/power.c derive the rail enum, the name strings, the I2C
 * addresses, the power-good group and the protection limits from it.
 *
 * Columns:
 *   name  identifier suffix, also used as display name
 *   addr  7-bit I2C address on the PMBus (A0/A1 strapping)
 *   pg    power-good / ALERT net the device is wired to (pwrPgLine_t)
 *   vov   bus over-voltage limit [V]
 *   iov   over-current limit [A]
 *   tov   over-temperature limit [degC]
 *
 * The number of rows must match INA745A_NUM_INSTANCES in userDef.h.
 */
/*	  name,        addr, pg,                 vov,   iov,   tov */
#define BOARD_PWRMON_TABLE \
	X(12V,         0x40, PWR_PG_SYS,      13.20f,  5.0f, 95.0f) \
	X(3V3,         0x41, PWR_PG_SYS,       3.60f,  6.0f, 95.0f) \
	X(5V0_SOM,     0x43, PWR_PG_SYS,       5.50f,  5.0f, 95.0f) \
	X(1V8,         0x44, PWR_PG_SYS,       1.98f,  5.0f, 95.0f) \
	X(VCCADJ_FMC1, 0x4C, PWR_PG_FMC1_SUP,  1.50f,  4.0f, 95.0f) \
	X(VCCADJ_FMC2, 0x4D, PWR_PG_FMC2_SUP,  1.98f,  4.0f, 95.0f)

/* =========================================================================== */
/* 6. Clock generator (si5332)                                                 */
/* =========================================================================== */
/*
 * Input source and per-output configuration. OUT6 (25 MHz CMOS system clock)
 * is fixed in hardware and always enabled; it cannot be configured here.
 *
 * Note: OUT0 is routed through HSDIV0 (100 MHz) and OUT2 through HSDIV1
 * (125 MHz); other frequencies are synthesized with the two MultiSynths,
 * of which only two different frequencies can be active at a time.
 */
#define BOARD_CLKGEN_SRC		SI5332_CLK_SRC_XTAL_27M

#define BOARD_CLKGEN_OUT0_EN		0
#define BOARD_CLKGEN_OUT0_FREQ_HZ	SI5332_100MHZ
#define BOARD_CLKGEN_OUT0_FORMAT	SI5332_FMT_LVDS_FAST

#define BOARD_CLKGEN_OUT2_EN		0
#define BOARD_CLKGEN_OUT2_FREQ_HZ	SI5332_125MHZ
#define BOARD_CLKGEN_OUT2_FORMAT	SI5332_FMT_LVDS_FAST

#define BOARD_CLKGEN_OUT4_EN		0
#define BOARD_CLKGEN_OUT4_FREQ_HZ	SI5332_100MHZ
#define BOARD_CLKGEN_OUT4_FORMAT	SI5332_FMT_LVDS_FAST

#define BOARD_CLKGEN_OUT7_EN		0
#define BOARD_CLKGEN_OUT7_FREQ_HZ	SI5332_125MHZ
#define BOARD_CLKGEN_OUT7_FORMAT	SI5332_FMT_LVDS_FAST

#endif /* SRC_BOARDCONFIG_H_ */
