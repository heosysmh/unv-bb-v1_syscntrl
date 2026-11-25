

# File i2cBus.c

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**i2cBus.c**](i2cBus_8c.md)

[Go to the documentation of this file](i2cBus_8c.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#include "i2cBus.h"

#include "bsp.h"
#include "boardConfig.h"

/* --- Interrupt controller ------------------------------------------------- */
#define BOARD_PLIC_BASE         SYSTEM_PLIC_CTRL

/* --- I2C controller: PMBus (DC/DC converters and power monitors) ----------- */
/* --- Connected to the PMBus of the SoM ------------------------------------- */
#define I2C_PMBUS_BASE          SYSTEM_I2C_1_IO_CTRL
#define I2C_PMBUS_DEV_ID        0x1
#define I2C_PMBUS_CORE_FREQ     SYSTEM_CLINT_HZ
#define I2C_PMBUS_IF_FREQ       100000U
#define I2C_PMBUS_INTR_ID       SYSTEM_PLIC_SYSTEM_I2C_1_IO_INTERRUPT
#define I2C_PMBUS_INTR_PRIO     1U
#define I2C_PMBUS_INTR_HART     0U

/* --- I2C controller: system configuration (clock generator, add-on modules) - */
#define I2C_SYSCFG_BASE         SYSTEM_I2C_0_IO_CTRL
#define I2C_SYSCFG_DEV_ID       0x0
#define I2C_SYSCFG_CORE_FREQ    SYSTEM_CLINT_HZ
#define I2C_SYSCFG_IF_FREQ      100000U
#define I2C_SYSCFG_INTR_ID      SYSTEM_PLIC_SYSTEM_I2C_0_IO_INTERRUPT
#define I2C_SYSCFG_INTR_PRIO    2U
#define I2C_SYSCFG_INTR_HART    0U



/* Common controller timing (sampling divider and bus timeout) */
#define I2C_SAMPLING_DIVIDER    3
#define I2C_TIMEOUT_DIVIDER 10  /* timeout = coreFreq / 10 = 0.1 s */

static intrHandle_t   *gIntr     = 0;   
static i2cHandle_t    *gPmbus    = 0;   
static i2cHandle_t    *gSysCfg   = 0;   
static pi4msd5v945x_t *gBusSwitch = 0;  

static void i2cBus_buildConfig(i2cHandle_config_t *cfg, u32 base, u8 devId,
                               u8 intrId, u8 intrPrio, u8 intrHart,
                               u32 coreFreq, u32 ifFreq)
{
    cfg->baseAddress = base;
    cfg->deviceId    = devId;
    cfg->intrId      = intrId;
    cfg->intrPrio    = intrPrio;
    cfg->intrHart    = intrHart;

    cfg->cfg.samplingClockDivider = I2C_SAMPLING_DIVIDER;
    cfg->cfg.timeout = coreFreq / I2C_TIMEOUT_DIVIDER;  /* 0.1 s */
    cfg->cfg.tsuDat  = coreFreq / ifFreq / 3;           /* 3.33 us */
    cfg->cfg.tLow    = coreFreq / ifFreq / 2;           /* 5 us */
    cfg->cfg.tHigh   = coreFreq / ifFreq / 2;           /* 5 us */
    cfg->cfg.tBuf    = coreFreq / ifFreq;               /* 10 us */
}

i2cBus_err_t i2cBus_init(void)
{
    i2cHandle_config_t cfg;

    gIntr = intr_initialize(BOARD_PLIC_BASE);
    if (!gIntr)
        return I2CBUS_E_INIT;

    intr_setThreshold(gIntr, 0, 0); 

    /* I2C for power management (DC/DC converters and power monitors) */
    i2cBus_buildConfig(&cfg,
                       I2C_PMBUS_BASE, I2C_PMBUS_DEV_ID,
                       I2C_PMBUS_INTR_ID, I2C_PMBUS_INTR_PRIO,
                       I2C_PMBUS_INTR_HART,
                       I2C_PMBUS_CORE_FREQ, I2C_PMBUS_IF_FREQ); 

    gPmbus = i2c_initialize(&cfg, gIntr);
    if (!gPmbus)
        return I2CBUS_E_INIT;

    /* I2C for system configuration (clock generator, add-on modules) */
    i2cBus_buildConfig(&cfg,
                       I2C_SYSCFG_BASE, I2C_SYSCFG_DEV_ID,
                       I2C_SYSCFG_INTR_ID, I2C_SYSCFG_INTR_PRIO,
                       I2C_SYSCFG_INTR_HART,
                       I2C_SYSCFG_CORE_FREQ, I2C_SYSCFG_IF_FREQ);                  

    gSysCfg = i2c_initialize(&cfg, gIntr);
    if (!gSysCfg)
        return I2CBUS_E_INIT;

    /* Bus switch that segments the SysCfg bus */
    gBusSwitch = pi4msd5v945x_initialize(I2CBUS_BUS_SWITCH_ADDR,
                                        (pi4msd5v945x_i2c_write_f)i2c_write,
                                        (pi4msd5v945x_i2c_read_f)i2c_read,
                                        (void*)gSysCfg);
    if (!gBusSwitch)
        return I2CBUS_E_INIT;

    return I2CBUS_OK;
}

i2cBus_err_t i2cBus_select(u8 channelMask)
{
    if (!gBusSwitch)
        return I2CBUS_E_INIT;

    if (pi4msd5v945x_setChannels(gBusSwitch, channelMask) != PI4MSD5V945X_OK)
        return I2CBUS_E_SELECT;

    return I2CBUS_OK;
}

i2cHandle_t* i2cBus_pmbus(void)
{
    return gPmbus;
}

i2cHandle_t* i2cBus_sysCfg(void)
{
    return gSysCfg;
}

intrHandle_t* i2cBus_intr(void)
{
    return gIntr;
}
```


