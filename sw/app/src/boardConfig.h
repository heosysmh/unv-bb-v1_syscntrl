/*******************************************************************************
*
* @file boardConfig.h
*
* @brief Single configuration point of the system controller application.
*        Set your FMC voltages and clock config here.
*
*        Sections:*          
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
#define BOARD_POWER_SETTLE_US		1000u	/* wait for supplies to stabilize */
#define BOARD_CLK_SETTLE_US			1000u	/* wait for clock outputs */
#define BOARD_ETH_RESET_PULSE_US	10u	/* ETH PHY reset pulse width */
#define BOARD_MONITOR_PERIOD_US		1000000u/* period of the monitoring loop */

/* =========================================================================== */
/* 2. VCCIO selection (HPC/HDB)                                                */
/* =========================================================================== */
/*
 * Kria HPC bank can be connected to either FMC2-LA00..19 (SW1002.2 Off) or 
 * FMC1-HA00..19 (SW1002.2 On) via DIP-Switch on the carrier board. 
 * Kria HDB bank is always connected to FMC2-LA20..31. It can be sourced with 
 * either FMC2-Vadj or 2.5V. With 2.5V LVDS-inputs are supported. Be carefull
 * to make the correct settings for your FMC module.
 */
#define BOARD_VCCIO_SELECT_HPC		0	/* 0: FMC2-Vadj, 1: FMC1-Vadj */
#define BOARD_VCCIO_SELECT_HDB		10	/* 0: FMC2-Vadj, 1: 2.5V */

/* =========================================================================== */
/* 3.1 FMC Vadj (tpsm8286x)                                                     */
/* =========================================================================== */
#define BOARD_FMC1_VADJ_V		1.80f
#define BOARD_FMC2_VADJ_V		1.80f

/* =========================================================================== */
/* 3.2 FMC Vadj hard upper bound (IO protection)                                */
/* =========================================================================== */
/*
 * Set according to the FPGAs VCCIO limits, checked at startup and used to 
 * define power monitor limits.    
 */
#define BOARD_FMC_VADJ_MAX_V	1.80f	

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
