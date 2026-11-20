/*******************************************************************************
*
* @file boardGpio.h
*
* @brief Named access to the board GPIO signals.
*
*        This is the only place where GPIO bit positions are evaluated; all
*        shifts come from boardConfig.h. The output register is shadowed so
*        that read-modify-write sequences cannot lose bits.
*
*        Note: this file is deliberately not called gpio.h - that name is
*        already used by the BSP driver (mcu/software/standalone/driver/gpio.h)
*        and every src directory is on the include path.
*
*******************************************************************************/

#ifndef SRC_BOARDGPIO_H_
#define SRC_BOARDGPIO_H_

#include <type.h>

/* --- GPIO input bit positions --------------------------------------------- */
#define GPIO_IN_VCCO_PS_EN_BIT	 0
#define GPIO_IN_VCCO_PL_EN_BIT	 1
#define GPIO_IN_SDPWR_EN_BIT	 2
#define GPIO_IN_HDA14P_BIT		 3
#define GPIO_IN_HDA14N_BIT		 4
#define GPIO_IN_MIO35_BIT		 5
#define GPIO_IN_FMC1_SUP_PG_BIT	 6
#define GPIO_IN_FMC2_SUP_PG_BIT	 7
#define GPIO_IN_FMC1_MOD_PG_BIT	 8
#define GPIO_IN_FMC2_MOD_PG_BIT	 9
#define GPIO_IN_2V5_PG_BIT		10
#define GPIO_IN_SYS_PG_BIT		11
#define GPIO_IN_SZG_PG_BIT		12
#define GPIO_IN_LMK_INTR_BIT	13
#define GPIO_IN_ETH_PME_BIT		14

/* --- GPIO output bit positions -------------------------------------------- */
#define GPIO_OUT_HPC_VCCIO_SELECT_BIT	0
#define GPIO_OUT_HDB_VCCIO_SELECT_BIT	1
#define GPIO_OUT_FMC1_SUP_EN_BIT		2
#define GPIO_OUT_FMC2_SUP_EN_BIT		3
#define GPIO_OUT_FMC1_PG_RLS_BIT		4
#define GPIO_OUT_FMC2_PG_RLS_BIT		5
#define GPIO_OUT_FMC1_MOD_EN_BIT		6
#define GPIO_OUT_FMC2_MOD_EN_BIT		7
#define GPIO_OUT_POR_B_BIT				8
#define GPIO_OUT_ETH_RSTN_BIT			9
#define GPIO_OUT_STATUS_LED_BIT		   31

/*
 * Power-good / ALERT nets.
 *
 * The ALERT outputs of the INA745A power monitors are open-drain and are
 * wired-OR onto the power-good inputs below. A line therefore reads low
 * either because the supply is not good or because an INA745A on that net
 * has asserted ALERT. See BOARD_PWRMON_TABLE for the rail-to-net mapping
 * and pwr_checkAlerts() for how the two causes are distinguished.
 */
#define GPIO_PG_SYS_MASK		(1u << GPIO_IN_SYS_PG_BIT)
#define GPIO_PG_FMC1_SUP_MASK	(1u << GPIO_IN_FMC1_SUP_PG_BIT)
#define GPIO_PG_FMC2_SUP_MASK	(1u << GPIO_IN_FMC2_SUP_PG_BIT)

/* Signals that indicate that the SoM has switched on its PL/PS supplies */
#define GPIO_SOM_READY_MASK		((1u << GPIO_IN_VCCO_PL_EN_BIT) | \
					 (1u << GPIO_IN_VCCO_PS_EN_BIT) | \
					 GPIO_PG_SYS_MASK)

#define GPIO_STATUS_LED_MASK	(1u << GPIO_OUT_STATUS_LED_BIT)

/*
 * Drives all outputs to their reset state (everything off: FMC supplies and
 * modules disabled, POR_B asserted, ETH PHY in reset).
 */
void boardGpio_init(void);

/* Raw input register of the board (bit positions: BOARD_GPIO_IN_*). */
u32 boardGpio_getInput(void);

/*
 * Blocks until the SoM has switched on its PL/PS supplies and SYS_PG is
 * asserted (BOARD_SOM_READY_MASK).
 */
void boardGpio_waitSomReady(void);

/*
 * Power-good / ALERT nets, masked to GPIO_PG_SYS_MASK,
 * GPIO_PG_FMC1_SUP_MASK and GPIO_PG_FMC2_SUP_MASK. A bit reads 0 either
 * because the supply is not good or because an INA745A on that net has
 * asserted its (active low, open-drain) ALERT output.
 */
u32 boardGpio_getPgStatus(void);

/* FMC supply enable (both FMC slots). */
void boardGpio_setFmcSupplyEnable(int enable);

/* FMC power-good release (both FMC slots). Power-good is forced low if locked. */
void boardGpio_setFmcPgoodRelease(int release);

/* FMC module enable (both FMC slots). */
void boardGpio_setFmcModuleEnable(int enable);

/* Releases POR_B of the SoM (cannot be re-asserted by this API on purpose). */
void boardGpio_releaseSomPor(void);

/* ETH PHY reset handling. */
void boardGpio_releaseEthPhyReset(void);
void boardGpio_pulseEthPhyReset(void);

/* VCCIO selection for HPC and HDB banks. */
void boardGpio_setHpcVccio(int sourceFmc1);
void boardGpio_setHdbVccio(int source2V5);

/* Sets the state of the status LED. */
void boardGpio_setStatusLed(int state);

#endif /* SRC_BOARDGPIO_H_ */
