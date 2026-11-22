
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file boardGpio.h
* @brief Implementation of GPIO access (interface to FPGA).
* @details
*        This is the only place where GPIO bit positions are evaluated; the
*        input/output bit positions are defined below. The output register is
*        shadowed so that read-modify-write sequences cannot lose bits.
*
* @note  This file is deliberately not called gpio.h - that name is
*        already used by the BSP driver (mcu/software/standalone/driver/gpio.h)
*        and every src directory is on the include path.
*/

#ifndef SRC_BOARDGPIO_H_
#define SRC_BOARDGPIO_H_

#include <type.h>

/**
 * @name GPIO input bit positions
 * @{
 */
#define GPIO_IN_VCCO_PS_EN_BIT	 0	/**< SoM PS supply enable indication. */
#define GPIO_IN_VCCO_PL_EN_BIT	 1	/**< SoM PL supply enable indication. */
#define GPIO_IN_SDPWR_EN_BIT	 2	/**< SD card power enable indication. */
#define GPIO_IN_HDA14P_BIT		 3	/**< HDA14P input. */
#define GPIO_IN_HDA14N_BIT		 4	/**< HDA14N input. */
#define GPIO_IN_MIO35_BIT		 5	/**< MIO35 input. */
#define GPIO_IN_FMC1_SUP_PG_BIT	 6	/**< FMC1 supply power-good / ALERT net. */
#define GPIO_IN_FMC2_SUP_PG_BIT	 7	/**< FMC2 supply power-good / ALERT net. */
#define GPIO_IN_FMC1_MOD_PG_BIT	 8	/**< FMC1 module power-good. */
#define GPIO_IN_FMC2_MOD_PG_BIT	 9	/**< FMC2 module power-good. */
#define GPIO_IN_2V5_PG_BIT		10	/**< 2.5V rail power-good. */
#define GPIO_IN_SYS_PG_BIT		11	/**< System power-good / ALERT net. */
#define GPIO_IN_SZG_PG_BIT		12	/**< SYZYGY power-good. */
#define GPIO_IN_LMK_INTR_BIT	13	/**< LMK interrupt input. */
#define GPIO_IN_ETH_PME_BIT		14	/**< ETH PHY power-management event. */
/** @} */

/**
 * @name GPIO output bit positions
 * @{
 */
#define GPIO_OUT_HPC_VCCIO_SELECT_BIT	0	/**< HPC VCCIO source select (0 = FMC2, 1 = FMC1). */
#define GPIO_OUT_HDB_VCCIO_SELECT_BIT	1	/**< HDB VCCIO source select (0 = FMC2-Vadj, 1 = 2.5V). */
#define GPIO_OUT_FMC1_SUP_EN_BIT		2	/**< FMC1 supply enable. */
#define GPIO_OUT_FMC2_SUP_EN_BIT		3	/**< FMC2 supply enable. */
#define GPIO_OUT_FMC1_PG_RLS_BIT		4	/**< FMC1 power-good release. */
#define GPIO_OUT_FMC2_PG_RLS_BIT		5	/**< FMC2 power-good release. */
#define GPIO_OUT_FMC1_MOD_EN_BIT		6	/**< FMC1 module enable. */
#define GPIO_OUT_FMC2_MOD_EN_BIT		7	/**< FMC2 module enable. */
#define GPIO_OUT_POR_B_BIT				8	/**< SoM POR_B (active low, 0 = reset asserted). */
#define GPIO_OUT_ETH_RSTN_BIT			9	/**< ETH PHY reset (active low, 0 = in reset). */
#define GPIO_OUT_STATUS_LED_BIT		   31	/**< Status LED. */
/** @} */

/**
 * @name Power-good / ALERT nets
 *
 * The ALERT outputs of the INA745A power monitors are open-drain and are
 * wired-OR onto the power-good inputs below. A line therefore reads low
 * either because the supply is not good or because an INA745A on that net
 * has asserted ALERT. See PWR_MON_TABLE (power.h) for the rail-to-net mapping
 * and pwr_checkAlerts() for how the two causes are distinguished.
 * @{
 */
#define GPIO_PG_SYS_MASK		(1u << GPIO_IN_SYS_PG_BIT)		/**< SYS_PG net mask. */
#define GPIO_PG_FMC1_SUP_MASK	(1u << GPIO_IN_FMC1_SUP_PG_BIT)	/**< FMC1_SUP_PG net mask. */
#define GPIO_PG_FMC2_SUP_MASK	(1u << GPIO_IN_FMC2_SUP_PG_BIT)	/**< FMC2_SUP_PG net mask. */
/** @} */

/** Signals that indicate that the SoM has switched on its PL/PS supplies. */
#define GPIO_SOM_READY_MASK		((1u << GPIO_IN_VCCO_PL_EN_BIT) | \
					 (1u << GPIO_IN_VCCO_PS_EN_BIT) | \
					 GPIO_PG_SYS_MASK)

#define GPIO_STATUS_LED_MASK	(1u << GPIO_OUT_STATUS_LED_BIT)	/**< Status LED output mask. */

/**
 * @brief Drives all outputs to their reset state.
 *
 * Everything off: FMC supplies and modules disabled, POR_B asserted,
 * ETH PHY in reset.
 */
void boardGpio_init(void);

/**
 * @brief Reads the raw input register of the board.
 * @return Input register value (bit positions: GPIO_IN_*_BIT).
 */
u32 boardGpio_getInput(void);

/**
 * @brief Blocks until the SoM has switched on its PL/PS supplies.
 *
 * Waits until VCCO_PL_EN, VCCO_PS_EN and SYS_PG are all asserted
 * (see #GPIO_SOM_READY_MASK).
 */
void boardGpio_waitSomReady(void);

/**
 * @brief Reads the power-good / ALERT nets.
 *
 * Masked to #GPIO_PG_SYS_MASK, #GPIO_PG_FMC1_SUP_MASK and
 * #GPIO_PG_FMC2_SUP_MASK. A bit reads 0 either because the supply is not
 * good or because an INA745A on that net has asserted its (active low,
 * open-drain) ALERT output.
 *
 * @return Masked power-good status bits.
 */
u32 boardGpio_getPgStatus(void);

/**
 * @brief Enables/disables the FMC supply (both FMC slots).
 * @param enable Non-zero to enable, zero to disable.
 */
void boardGpio_setFmcSupplyEnable(int enable);

/**
 * @brief Releases/locks the FMC power-good (both FMC slots).
 *
 * Power-good is forced low if locked.
 *
 * @param release Non-zero to release, zero to force low.
 */
void boardGpio_setFmcPgoodRelease(int release);

/**
 * @brief Enables/disables the FMC modules (both FMC slots).
 * @param enable Non-zero to enable, zero to disable.
 */
void boardGpio_setFmcModuleEnable(int enable);

/**
 * @brief Releases POR_B of the SoM.
 *
 * Cannot be re-asserted by this API on purpose.
 */
void boardGpio_releaseSomPor(void);

/**
 * @brief Releases the ETH PHY reset (ETH_RSTN high).
 */
void boardGpio_releaseEthPhyReset(void);

/**
 * @brief Generates a reset pulse on the ETH PHY reset line.
 *
 * Pulse width is #BOARD_ETH_RESET_PULSE_US; the line is high afterwards.
 */
void boardGpio_pulseEthPhyReset(void);

/**
 * @brief Selects the VCCIO source of the HPC bank.
 * @param sourceFmc1 Non-zero: FMC1-Vadj, zero: FMC2-Vadj.
 */
void boardGpio_setHpcVccio(int sourceFmc1);

/**
 * @brief Selects the VCCIO source of the HDB bank.
 * @param source2V5 Non-zero: 2.5V, zero: FMC2-Vadj.
 */
void boardGpio_setHdbVccio(int source2V5);

/**
 * @brief Sets the state of the status LED.
 * @param state Non-zero: LED on, zero: LED off.
 */
void boardGpio_setStatusLed(int state);

#endif /* SRC_BOARDGPIO_H_ */
