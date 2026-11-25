

# File boardGpio.c

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**boardGpio.c**](boardGpio_8c.md)

[Go to the documentation of this file](boardGpio_8c.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#include "boardGpio.h"

#include "bsp.h"
#include "gpio.h"
#include "boardConfig.h"

/* --- GPIO controllers ----------------------------------------------------- */
#define GPIO_IN     SYSTEM_GPIO_0_IO_CTRL
#define GPIO_OUT    SYSTEM_GPIO_1_IO_CTRL


#define GPIO_HPC_VCCIO_SELECT_MASK  (1u << GPIO_OUT_HPC_VCCIO_SELECT_BIT)
#define GPIO_HDB_VCCIO_SELECT_MASK  (1u << GPIO_OUT_HDB_VCCIO_SELECT_BIT)

#define GPIO_FMC_SUP_EN_MASK    ((1u << GPIO_OUT_FMC1_SUP_EN_BIT) | \
                                 (1u << GPIO_OUT_FMC2_SUP_EN_BIT))
#define GPIO_FMC_MOD_EN_MASK    ((1u << GPIO_OUT_FMC1_MOD_EN_BIT) | \
                                 (1u << GPIO_OUT_FMC2_MOD_EN_BIT))
#define GPIO_FMC_PG_RLS_MASK    ((1u << GPIO_OUT_FMC1_PG_RLS_BIT) | \
                                 (1u << GPIO_OUT_FMC2_PG_RLS_BIT))

#define GPIO_POR_B_MASK     (1u << GPIO_OUT_POR_B_BIT)
#define GPIO_ETH_RSTN_MASK  (1u << GPIO_OUT_ETH_RSTN_BIT)

#define GPIO_PG_MASK    (GPIO_PG_SYS_MASK | \
                    GPIO_PG_FMC1_SUP_MASK | \
                    GPIO_PG_FMC2_SUP_MASK)

static u32 gOutShadow = 0;

static void boardGpio_modifyOutput(u32 mask, u32 value)
{
    gOutShadow = (gOutShadow & ~mask) | (value & mask);
    gpio_setOutput(GPIO_OUT, gOutShadow);
}

void boardGpio_init(void)
{
    gOutShadow = 0x0;   /* supplies off, POR_B asserted, ETH PHY in reset */
    gpio_setOutput(GPIO_OUT, gOutShadow);
}

u32 boardGpio_getInput(void)
{
    return gpio_getInput(GPIO_IN);
}

void boardGpio_waitSomReady(void)
{
    u32 in = 0;

    while ((in & GPIO_SOM_READY_MASK) != GPIO_SOM_READY_MASK)
        in = gpio_getInput(GPIO_IN);
}

u32 boardGpio_getPgStatus(void)
{
    return gpio_getInput(GPIO_IN) & GPIO_PG_MASK;
}

void boardGpio_setFmcSupplyEnable(int enable)
{
    boardGpio_modifyOutput(GPIO_FMC_SUP_EN_MASK, enable ? GPIO_FMC_SUP_EN_MASK : 0u);
}

void boardGpio_setFmcPgoodRelease(int release)
{
    boardGpio_modifyOutput(GPIO_FMC_PG_RLS_MASK, release ? GPIO_FMC_PG_RLS_MASK : 0u);
}

void boardGpio_setFmcModuleEnable(int enable)
{
    boardGpio_modifyOutput(GPIO_FMC_MOD_EN_MASK, enable ? GPIO_FMC_MOD_EN_MASK : 0u);
}

void boardGpio_releaseSomPor(void)
{
    boardGpio_modifyOutput(GPIO_POR_B_MASK, GPIO_POR_B_MASK);
}

void boardGpio_releaseEthPhyReset(void)
{
    boardGpio_modifyOutput(GPIO_ETH_RSTN_MASK, GPIO_ETH_RSTN_MASK);
}

void boardGpio_pulseEthPhyReset(void)
{
    boardGpio_modifyOutput(GPIO_ETH_RSTN_MASK, 0u);
    bsp_uDelay(BOARD_ETH_RESET_PULSE_US);
    boardGpio_modifyOutput(GPIO_ETH_RSTN_MASK, GPIO_ETH_RSTN_MASK);
}

void boardGpio_setHpcVccio(int sourceFmc1)
{
    boardGpio_modifyOutput(GPIO_HPC_VCCIO_SELECT_MASK, sourceFmc1 ? GPIO_HPC_VCCIO_SELECT_MASK : 0u);
}

void boardGpio_setHdbVccio(int source2V5)
{
    boardGpio_modifyOutput(GPIO_HDB_VCCIO_SELECT_MASK, source2V5 ? GPIO_HDB_VCCIO_SELECT_MASK : 0u);
}

void boardGpio_setStatusLed(int state)
{
    boardGpio_modifyOutput(GPIO_STATUS_LED_MASK, state ? GPIO_STATUS_LED_MASK : 0u);
}
```


