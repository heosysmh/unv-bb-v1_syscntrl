/*******************************************************************************
*
* @file boardConfig.h
*
* @brief Single configuration point of the system controller application.
*        Set your FMC voltages and clock config here.
*
*        Sections:
*          1. Startup Timing
*          2. VCCIO selection (HPC/HDB)
*          3. FMC Vadj        (tpsm8286x)
*          4. Clock generator (si5332)
*
*******************************************************************************/

#ifndef SRC_BOARDCONFIG_H_
#define SRC_BOARDCONFIG_H_

#include "si5332.h"

/* =========================================================================== */
/* 1. Startup Timing                                                           */
/* =========================================================================== */
#define BOARD_POWER_SETTLE_US		1000u	/**< Wait for supplies to stabilize [us]. */
#define BOARD_CLK_SETTLE_US			1000u	/**< Wait for clock outputs [us]. */
#define BOARD_ETH_RESET_PULSE_US	10u		/**< ETH PHY reset pulse width [us]. */
#define BOARD_MONITOR_PERIOD_US		1000000u/**< Period of the monitoring loop [us]. */

/* =========================================================================== */
/* 2. VCCIO selection (HPC/HDB)                                                */
/* =========================================================================== */
/**
 * @name VCCIO source selection
 *
 * Kria HPC bank can be connected to either FMC2-LA00..19 (SW1002.2 Off) or
 * FMC1-HA00..19 (SW1002.2 On) via DIP-Switch on the carrier board.
 * Kria HDB bank is always connected to FMC2-LA20..31. It can be sourced with
 * either FMC2-Vadj or 2.5V. With 2.5V LVDS-inputs are supported. Be carefull
 * to make the correct settings for your FMC module.
 * @{
 */
#define BOARD_VCCIO_SELECT_HPC		0	/**< HPC VCCIO source: 0 = FMC2-Vadj, 1 = FMC1-Vadj. */
#define BOARD_VCCIO_SELECT_HDB		0	/**< HDB VCCIO source: 0 = FMC2-Vadj, 1 = 2.5V. */
/** @} */

/* =========================================================================== */
/* 3.1 FMC Vadj (tpsm8286x)                                                     */
/* =========================================================================== */
#define BOARD_FMC1_VADJ_V		1.80f	/**< FMC1 Vadj setpoint [V]. */
#define BOARD_FMC2_VADJ_V		1.80f	/**< FMC2 Vadj setpoint [V]. */

/* =========================================================================== */
/* 3.2 FMC Vadj hard upper bound (IO protection)                                */
/* =========================================================================== */
/**
 * Set according to the FPGAs VCCIO limits, checked at startup and used to
 * define power monitor limits.
 */
#define BOARD_FMC_VADJ_MAX_V	1.80f	/**< Hard upper bound for both FMC Vadj rails [V]. */

/* =========================================================================== */
/* 4. Clock generator (si5332)                                                 */
/* =========================================================================== */
/**
 * @name Clock generator configuration
 *
 * Primarily used to provide reference clocks for the GTR transceivers and the
 * ethernet PHY.
 *
 * OUT6 (25 MHz CMOS PHY clock) is fixed and always enabled.
 *
 * Use #SI5332_CLK_SRC_XTAL_27M when no synchronization to the Sync-Board is
 * needed, #SI5332_CLK_SRC_CLKIN2_100M otherwise.
 *
 * @note Frequencies that are integer divisors of 125 MHz or 100 MHz are derived
 *       directly from these; other frequencies are synthesized with the two
 *       MultiSynths, of which only two different frequencies can be active at
 *       a time.
 * @{
 */
#define BOARD_CLKGEN_SRC			SI5332_CLK_SRC_XTAL_27M	/**< PLL input clock source. */

#define BOARD_CLKGEN_OUT0_EN		0						/**< OUT0 enable at startup. */
#define BOARD_CLKGEN_OUT0_FREQ_HZ	SI5332_100MHZ			/**< OUT0 frequency [Hz]. */
#define BOARD_CLKGEN_OUT0_FORMAT	SI5332_FMT_LVDS_FAST	/**< OUT0 signal format. */

#define BOARD_CLKGEN_OUT2_EN		0						/**< OUT2 enable at startup. */
#define BOARD_CLKGEN_OUT2_FREQ_HZ	SI5332_125MHZ			/**< OUT2 frequency [Hz]. */
#define BOARD_CLKGEN_OUT2_FORMAT	SI5332_FMT_LVDS_FAST	/**< OUT2 signal format. */

#define BOARD_CLKGEN_OUT4_EN		0						/**< OUT4 enable at startup. */
#define BOARD_CLKGEN_OUT4_FREQ_HZ	SI5332_100MHZ			/**< OUT4 frequency [Hz]. */
#define BOARD_CLKGEN_OUT4_FORMAT	SI5332_FMT_LVDS_FAST	/**< OUT4 signal format. */

#define BOARD_CLKGEN_OUT7_EN		0						/**< OUT7 enable at startup. */
#define BOARD_CLKGEN_OUT7_FREQ_HZ	SI5332_125MHZ			/**< OUT7 frequency [Hz]. */
#define BOARD_CLKGEN_OUT7_FORMAT	SI5332_FMT_LVDS_FAST	/**< OUT7 signal format. */
/** @} */

#endif /* SRC_BOARDCONFIG_H_ */
