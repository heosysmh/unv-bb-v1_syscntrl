///////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2025 SaxonSoc contributors
//  SPDX license identifier: MIT
//  Full license header bsp/efinix/EfxSapphireSoc/include/LICENSE.MD
//////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
*
* @file main.c: system controller application
*
* @brief Board bring-up and supervision of the carrier board.
*
*        This file only contains the CPU-level startup (trap vector, interrupt
*        enable) and the bring-up sequence. All device handling lives in the
*        function groups:
*
*          i2cBus.c     I2C controllers, interrupt controller, bus switch
*          boardGpio.c  named GPIO access (enables, resets, power-good lines)
*          power.c      FMC Vadj regulators and power monitors
*          clkGen.c     clock generator
*
*        Everything configurable is in boardConfig.h.
*
******************************************************************************/
#include <stdint.h>

#include "bsp.h"
#include "riscv.h"
#include "soc.h"

#include "boardConfig.h"
#include "boardGpio.h"
#include "clkGen.h"
#include "i2cBus.h"
#include "power.h"

void trap_entry();

/**
 * @brief Enables machine external interrupts and installs the trap vector.
 */
static void cpu_enableInterrupts(void)
{
	csr_write(mtvec, trap_entry);
	csr_write(mie, MIE_MEIE);
	csr_write(mstatus, csr_read(mstatus) | MSTATUS_MPP | MSTATUS_MIE);
}

/**
 * @brief Brings the board up in the required order.
 *
 * Ordering constraints:
 *  - The power monitors must be armed after the SoM power-good wait: their
 *    ALERT outputs are wired onto the power-good nets, so a latched alert
 *    would keep SYS_PG low and stall the wait loop.
 *  - They are armed before the FMC supply is switched on so that the FMC
 *    rails are already protected while ramping up.
 *
 * @return PWR_OK / 0 if the whole sequence succeeded.
 */
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
		return -1;
	}
	bsp_uDelay(BOARD_CLK_SETTLE_US);

	bsp_printf("Release ETH-PHY reset\r\n");
	boardGpio_releaseEthPhyReset();

	bsp_printf("Configure Power-Monitors\r\n");
	if (pwr_mon_configure() != PWR_OK)
		bsp_printf("WARNING: Power-Monitor configuration incomplete\r\n");

	bsp_printf("Enable FMC supplies\r\n");
	boardGpio_setFmcSupplyEnable(1);
	bsp_uDelay(BOARD_POWER_SETTLE_US);

	if (pwr_fmcVadj_configure() != PWR_OK) {
		bsp_printf("ERROR: FMC Vadj configuration failed\r\n");
		return -1;
	}

	if (pwr_fmcVadj_isRegulating(PWR_FMC1, &regOk) != PWR_OK ||
	    pwr_fmcVadj_isRegulating(PWR_FMC2, &regOk) != PWR_OK) {
		bsp_printf("ERROR: FMC Vadj voltage register check failed\r\n");
		return -1;
	}

	bsp_printf("Enable FMCs\r\n");
	if (pwr_fmcVadj_enable(1) != PWR_OK) {
		bsp_printf("ERROR: FMC Vadj enable failed\r\n");
		return -1;
	}
	bsp_uDelay(BOARD_POWER_SETTLE_US);

	/* Reports a supply failure as well as any monitor alert of the FMC and
	 * system rails. */
	(void)pwr_checkAlerts();

	bsp_printf("Release POR\r\n");
	boardGpio_releaseSomPor();

	return 0;
}

/******************************************************************************
*
* @brief Entry point: configures the board and then supervises the supplies.
*
******************************************************************************/
void main(void)
{
	bsp_init();

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

		bsp_uDelay(BOARD_MONITOR_PERIOD_US);
	}
}
