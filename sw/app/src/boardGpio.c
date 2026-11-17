/*******************************************************************************
*
* @file boardGpio.c
*
* @brief Implementation of the named board GPIO access, see boardGpio.h.
*
*******************************************************************************/

#include "boardGpio.h"

#include "bsp.h"
#include "gpio.h"
#include "boardConfig.h"

#define FMC_SUP_EN_MASK		((1u << BOARD_GPIO_OUT_FMC1_SUP_EN_SHIFT) | \
				 (1u << BOARD_GPIO_OUT_FMC2_SUP_EN_SHIFT))
#define FMC_MOD_EN_MASK		((1u << BOARD_GPIO_OUT_FMC1_MOD_EN_SHIFT) | \
				 (1u << BOARD_GPIO_OUT_FMC2_MOD_EN_SHIFT))
#define POR_B_MASK		(1u << BOARD_GPIO_OUT_POR_B_SHIFT)
#define ETH_RSTN_MASK		(1u << BOARD_GPIO_OUT_ETH_RSTN_SHIFT)

#define PG_MASK			(BOARD_PG_SYS_MASK | \
				 BOARD_PG_FMC1_SUP_MASK | \
				 BOARD_PG_FMC2_SUP_MASK)

/** Shadow of the output register; the hardware register is write-only safe. */
static u32 gOutShadow = 0;

/**
 * @brief Applies @p value to the masked output bits via the shadow register.
 */
static void boardGpio_modifyOutput(u32 mask, u32 value)
{
	gOutShadow = (gOutShadow & ~mask) | (value & mask);
	gpio_setOutput(BOARD_GPIO_OUT, gOutShadow);
}

void boardGpio_init(void)
{
	gOutShadow = 0x0;	/* supplies off, POR_B asserted, ETH PHY in reset */
	gpio_setOutput(BOARD_GPIO_OUT, gOutShadow);
}

u32 boardGpio_getInput(void)
{
	return gpio_getInput(BOARD_GPIO_IN);
}

void boardGpio_waitSomReady(void)
{
	u32 in = 0;

	while ((in & BOARD_SOM_READY_MASK) != BOARD_SOM_READY_MASK)
		in = gpio_getInput(BOARD_GPIO_IN);
}

u32 boardGpio_getPgStatus(void)
{
	return gpio_getInput(BOARD_GPIO_IN) & PG_MASK;
}

void boardGpio_setFmcSupplyEnable(int enable)
{
	boardGpio_modifyOutput(FMC_SUP_EN_MASK, enable ? FMC_SUP_EN_MASK : 0u);
}

void boardGpio_setFmcModuleEnable(int enable)
{
	boardGpio_modifyOutput(FMC_MOD_EN_MASK, enable ? FMC_MOD_EN_MASK : 0u);
}

void boardGpio_releaseSomPor(void)
{
	boardGpio_modifyOutput(POR_B_MASK, POR_B_MASK);
}

void boardGpio_releaseEthPhyReset(void)
{
	boardGpio_modifyOutput(ETH_RSTN_MASK, ETH_RSTN_MASK);
}

void boardGpio_pulseEthPhyReset(void)
{
	boardGpio_modifyOutput(ETH_RSTN_MASK, 0u);
	bsp_uDelay(BOARD_ETH_RESET_PULSE_US);
	boardGpio_modifyOutput(ETH_RSTN_MASK, ETH_RSTN_MASK);
}
