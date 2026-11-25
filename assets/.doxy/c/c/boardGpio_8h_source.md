

# File boardGpio.h

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**boardGpio.h**](boardGpio_8h.md)

[Go to the documentation of this file](boardGpio_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_BOARDGPIO_H_
#define SRC_BOARDGPIO_H_

#include <type.h>

#define GPIO_IN_VCCO_PS_EN_BIT   0  
#define GPIO_IN_VCCO_PL_EN_BIT   1  
#define GPIO_IN_SDPWR_EN_BIT     2  
#define GPIO_IN_HDA14P_BIT       3  
#define GPIO_IN_HDA14N_BIT       4  
#define GPIO_IN_MIO35_BIT        5  
#define GPIO_IN_FMC1_SUP_PG_BIT  6  
#define GPIO_IN_FMC2_SUP_PG_BIT  7  
#define GPIO_IN_FMC1_MOD_PG_BIT  8  
#define GPIO_IN_FMC2_MOD_PG_BIT  9  
#define GPIO_IN_2V5_PG_BIT      10  
#define GPIO_IN_SYS_PG_BIT      11  
#define GPIO_IN_SZG_PG_BIT      12  
#define GPIO_IN_LMK_INTR_BIT    13  
#define GPIO_IN_ETH_PME_BIT     14  

#define GPIO_OUT_HPC_VCCIO_SELECT_BIT   0   
#define GPIO_OUT_HDB_VCCIO_SELECT_BIT   1   
#define GPIO_OUT_FMC1_SUP_EN_BIT        2   
#define GPIO_OUT_FMC2_SUP_EN_BIT        3   
#define GPIO_OUT_FMC1_PG_RLS_BIT        4   
#define GPIO_OUT_FMC2_PG_RLS_BIT        5   
#define GPIO_OUT_FMC1_MOD_EN_BIT        6   
#define GPIO_OUT_FMC2_MOD_EN_BIT        7   
#define GPIO_OUT_POR_B_BIT              8   
#define GPIO_OUT_ETH_RSTN_BIT           9   
#define GPIO_OUT_STATUS_LED_BIT        31   

#define GPIO_PG_SYS_MASK        (1u << GPIO_IN_SYS_PG_BIT)      
#define GPIO_PG_FMC1_SUP_MASK   (1u << GPIO_IN_FMC1_SUP_PG_BIT) 
#define GPIO_PG_FMC2_SUP_MASK   (1u << GPIO_IN_FMC2_SUP_PG_BIT) 

#define GPIO_SOM_READY_MASK     ((1u << GPIO_IN_VCCO_PL_EN_BIT) | \
                     (1u << GPIO_IN_VCCO_PS_EN_BIT) | \
                     GPIO_PG_SYS_MASK)

#define GPIO_STATUS_LED_MASK    (1u << GPIO_OUT_STATUS_LED_BIT) 

void boardGpio_init(void);

u32 boardGpio_getInput(void);

void boardGpio_waitSomReady(void);

u32 boardGpio_getPgStatus(void);

void boardGpio_setFmcSupplyEnable(int enable);

void boardGpio_setFmcPgoodRelease(int release);

void boardGpio_setFmcModuleEnable(int enable);

void boardGpio_releaseSomPor(void);

void boardGpio_releaseEthPhyReset(void);

void boardGpio_pulseEthPhyReset(void);

void boardGpio_setHpcVccio(int sourceFmc1);

void boardGpio_setHdbVccio(int source2V5);

void boardGpio_setStatusLed(int state);

#endif /* SRC_BOARDGPIO_H_ */
```


