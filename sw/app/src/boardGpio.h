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
 * Power-good / ALERT nets, masked to BOARD_PG_SYS_MASK,
 * BOARD_PG_FMC1_SUP_MASK and BOARD_PG_FMC2_SUP_MASK. A bit reads 0 either
 * because the supply is not good or because an INA745A on that net has
 * asserted its (active low, open-drain) ALERT output.
 */
u32 boardGpio_getPgStatus(void);

/* FMC supply enable (both FMC slots). */
void boardGpio_setFmcSupplyEnable(int enable);

/* FMC module enable (both FMC slots). */
void boardGpio_setFmcModuleEnable(int enable);

/* Releases POR_B of the SoM (cannot be re-asserted by this API on purpose). */
void boardGpio_releaseSomPor(void);

/* ETH PHY reset handling. */
void boardGpio_releaseEthPhyReset(void);
void boardGpio_pulseEthPhyReset(void);

#endif /* SRC_BOARDGPIO_H_ */
