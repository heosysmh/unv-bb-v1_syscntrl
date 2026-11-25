

# File boardConfig.h

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**boardConfig.h**](boardConfig_8h.md)

[Go to the documentation of this file](boardConfig_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_BOARDCONFIG_H_
#define SRC_BOARDCONFIG_H_

#include "si5332.h"

/* =========================================================================== */
/* 1. Startup Timing                                                           */
/* =========================================================================== */
#define BOARD_POWER_SETTLE_US       1000u   
#define BOARD_CLK_SETTLE_US         1000u   
#define BOARD_ETH_RESET_PULSE_US    10u     
#define BOARD_MONITOR_PERIOD_US     1000000u

/* =========================================================================== */
/* 2. VCCIO selection (HPC/HDB)                                                */
/* =========================================================================== */
#define BOARD_VCCIO_SELECT_HPC      0   
#define BOARD_VCCIO_SELECT_HDB      0   

/* =========================================================================== */
/* 3.1 FMC Vadj (tpsm8286x)                                                     */
/* =========================================================================== */
#define BOARD_FMC1_VADJ_V       1.80f   
#define BOARD_FMC2_VADJ_V       1.80f   

/* =========================================================================== */
/* 3.2 FMC Vadj hard upper bound (IO protection)                                */
/* =========================================================================== */
#define BOARD_FMC_VADJ_MAX_V    1.80f   

/* =========================================================================== */
/* 4. Clock generator (si5332)                                                 */
/* =========================================================================== */
#define BOARD_CLKGEN_SRC            SI5332_CLK_SRC_XTAL_27M 

#define BOARD_CLKGEN_OUT0_EN        0                       
#define BOARD_CLKGEN_OUT0_FREQ_HZ   SI5332_100MHZ           
#define BOARD_CLKGEN_OUT0_FORMAT    SI5332_FMT_LVDS_FAST    

#define BOARD_CLKGEN_OUT2_EN        0                       
#define BOARD_CLKGEN_OUT2_FREQ_HZ   SI5332_125MHZ           
#define BOARD_CLKGEN_OUT2_FORMAT    SI5332_FMT_LVDS_FAST    

#define BOARD_CLKGEN_OUT4_EN        0                       
#define BOARD_CLKGEN_OUT4_FREQ_HZ   SI5332_100MHZ           
#define BOARD_CLKGEN_OUT4_FORMAT    SI5332_FMT_LVDS_FAST    

#define BOARD_CLKGEN_OUT7_EN        0                       
#define BOARD_CLKGEN_OUT7_FREQ_HZ   SI5332_125MHZ           
#define BOARD_CLKGEN_OUT7_FORMAT    SI5332_FMT_LVDS_FAST    

#endif /* SRC_BOARDCONFIG_H_ */
```


