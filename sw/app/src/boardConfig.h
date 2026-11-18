/*******************************************************************************
*
* @file boardConfig.h
*
* @brief Single configuration point of the system controller application.
*        Set your FMC voltages and clock config here.
*
*        Sections:*          
*          1. Startup Timing
*          2. FMC Vadj          (tpsm8286x)
*          3. FMC Vadj hard upper bound
*          4. Clock generator   (si5332)
*
*******************************************************************************/

#ifndef SRC_BOARDCONFIG_H_
#define SRC_BOARDCONFIG_H_

#include "si5332.h"

/* =========================================================================== */
/* 1. Startup Timing                                                           */
/* =========================================================================== */
#define BOARD_POWER_SETTLE_US		1000u	/* wait for supplies to stabilize */
#define BOARD_CLK_SETTLE_US			1000u	/* wait for clock outputs */
#define BOARD_ETH_RESET_PULSE_US	10u	/* ETH PHY reset pulse width */
#define BOARD_MONITOR_PERIOD_US		1000000u/* period of the monitoring loop */

/* =========================================================================== */
/* 2. FMC Vadj (tpsm8286x)                                                     */
/* =========================================================================== */
#define BOARD_FMC1_VADJ_V		1.8f
#define BOARD_FMC2_VADJ_V		1.8f

/* =========================================================================== */
/* 3. FMC Vadj hard upper bound (IO protection)                                */
/* =========================================================================== */
/*
 * Set according to the FPGAs VCCIO limits, checked at startup and used to 
 * define power monitor limits.    
 */
#define BOARD_FMC_VADJ_MAX_V	1.8f	

/* =========================================================================== */
/* 4. Clock generator (si5332)                                                 */
/* =========================================================================== */
/*
 * Primarily used to provide reference clocks for the GTR transceivers and the
 * ethernet PHY.
 *
 * OUT6 (25 MHz CMOS PYH clock) is fixed and always enabled; 
 * 
 * Use SI5332_CLK_SRC_XTAL_27M when no sychronization to the Sync-Board is needed,
 * SI5332_CLK_SRC_CLKIN2_100M otherwise. 
 *
 * Note: Frequencies that are integer divisors of 125 MHz or 100 MHz are derived
 * directly from these; other frequencies are synthesized with the two 
 * MultiSynths, of which only two different frequencies can be active at a time.
 */
#define BOARD_CLKGEN_SRC			SI5332_CLK_SRC_XTAL_27M

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
