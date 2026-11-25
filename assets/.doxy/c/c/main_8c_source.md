

# File main.c

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**main.c**](main_8c.md)

[Go to the documentation of this file](main_8c.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#include "bsp.h"
#include "riscv.h"

#include "boardConfig.h"
#include "boardGpio.h"
#include "clkGen.h"
#include "i2cBus.h"
#include "power.h"

typedef enum {
    INIT_OK         =  0,   
    INIT_E_CLKGEN   = -1,   
    INIT_E_PWRMON   = -2,   
    INIT_E_FMCADJ   = -3    
} init_err_t;

void trap_entry();

static void cpu_enableInterrupts(void)
{
    csr_write(mtvec, trap_entry);
    csr_write(mie, MIE_MEIE);
    csr_write(mstatus, csr_read(mstatus) | MSTATUS_MPP | MSTATUS_MIE);
}

static int board_bringUp(void)
{
    int regOk = 0;

    if (i2cBus_init() != I2CBUS_OK) {
        bsp_printf("ERROR: I2C initialization failed\r\n");
        return -1;
    }

    if (pwr_init() != PWR_OK) {
        bsp_printf("ERROR: Power device initialization failed\r\n");
        return -1;
    }

    if (clkGen_init() != CLKGEN_OK) {
        bsp_printf("ERROR: Clock-Generator initialization failed\r\n");
        return -1;
    }

    cpu_enableInterrupts();

    bsp_printf("Wait for SoM supplies\r\n");
    boardGpio_waitSomReady();
    bsp_uDelay(BOARD_POWER_SETTLE_US);

    bsp_printf("Configure Clock-Generator\r\n");
    if (clkGen_configure() != CLKGEN_OK) {
        bsp_printf("ERROR: Clock-Generator configuration failed\r\n");
        return INIT_E_CLKGEN;
    }
    bsp_uDelay(BOARD_CLK_SETTLE_US);

    bsp_printf("Release ETH-PHY reset\r\n");
    boardGpio_releaseEthPhyReset();

    bsp_printf("Configure Power-Monitors\r\n");
    if (pwr_mon_configure() != PWR_OK) {
        bsp_printf("WARNING: Power-Monitor configuration incomplete\r\n");
        return INIT_E_PWRMON;
    }

    bsp_printf("Set VCCIO sources\r\n");
    boardGpio_setHpcVccio(BOARD_VCCIO_SELECT_HPC);
    boardGpio_setHdbVccio(BOARD_VCCIO_SELECT_HDB);

    bsp_printf("Enable FMC supplies\r\n");
    boardGpio_setFmcSupplyEnable(1);
    bsp_uDelay(BOARD_POWER_SETTLE_US);

    if (pwr_fmcVadj_configure() != PWR_OK) {
        bsp_printf("ERROR: FMC Vadj configuration failed\r\n");
        return INIT_E_FMCADJ;
    }

    if (pwr_fmcVadj_isRegulating(PWR_FMC1) != PWR_OK ||
        pwr_fmcVadj_isRegulating(PWR_FMC2) != PWR_OK) {
        bsp_printf("ERROR: FMC Vadj voltage register check failed\r\n");
        return INIT_E_FMCADJ;
    }

    bsp_printf("Enable FMCs\r\n");
    if (pwr_fmcVadj_enable(1) != PWR_OK) {
        bsp_printf("ERROR: FMC Vadj enable failed\r\n");
        return INIT_E_FMCADJ;
    }
    /* Release FMC power-good enable */
    boardGpio_setFmcPgoodRelease(1);

    bsp_uDelay(BOARD_POWER_SETTLE_US);

    /* Reports a supply failure as well as any monitor alert of the FMC and
     * system rails. */
    (void)pwr_checkAlerts();

    bsp_printf("Release POR\r\n");
    boardGpio_releaseSomPor();

    return INIT_OK;
}

/******************************************************************************
*
* @brief Entry point: configures the board and then supervises the supplies.
*
******************************************************************************/
void main(void)
{
    bsp_init();
    u32 LedState = 0;

    boardGpio_init();

    bsp_printf("Board configuration start\r\n");

    if (board_bringUp() != 0) {
        bsp_printf("Board configuration FAILED\r\n");
        while (1) {}
    }

    bsp_printf("Board configuration finished\r\n");

    /* Supervision loop: the power-good / ALERT lines are checked on every
     * pass (GPIO only); the measured values are reported periodically. */
    while (1) {
        if (pwr_checkAlerts() != PWR_OK)
            pwr_mon_reportFaults();

        pwr_mon_poll();
        /* Toggle LED*/
        LedState = !LedState;
        boardGpio_setStatusLed(LedState);
        bsp_uDelay(BOARD_MONITOR_PERIOD_US);
    }
}
```


