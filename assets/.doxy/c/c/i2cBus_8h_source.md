

# File i2cBus.h

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**i2cBus.h**](i2cBus_8h.md)

[Go to the documentation of this file](i2cBus_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_I2CBUS_H_
#define SRC_I2CBUS_H_

#include "pi4msd5v945x.h"
#include "i2cWrapper.h"
#include "interrupt.h"

#define I2CBUS_CLKGEN_ADDR      0x6A    
#define I2CBUS_BUS_SWITCH_ADDR  0x77    

#define I2CBUS_PWRMON_12V_ADDR  0x40    
#define I2CBUS_PWRMON_3V3_ADDR  0x41    
#define I2CBUS_PWRMON_5V0_ADDR  0x43    
#define I2CBUS_PWRMON_1V8_ADDR  0x44    
#define I2CBUS_PWRMON_FMC1_ADDR 0x4C    
#define I2CBUS_PWRMON_FMC2_ADDR 0x4D    
#define I2CBUS_FMC1_VADJ_ADDR   0x4E    
#define I2CBUS_FMC2_VADJ_ADDR   0x4F    

#define I2CBUS_CH_SYZYGY    PI4MSD5V945X_CH0    
#define I2CBUS_CH_FMC2      PI4MSD5V945X_CH1    
#define I2CBUS_CH_FMC1      PI4MSD5V945X_CH2    
#define I2CBUS_CH_MBRD      PI4MSD5V945X_CH3    

typedef enum {
    I2CBUS_OK        =  0,  
    I2CBUS_E_INIT    = -1,  
    I2CBUS_E_SELECT  = -2   
} i2cBus_err_t;

i2cBus_err_t i2cBus_init(void);

i2cBus_err_t i2cBus_select(u8 channelMask);

i2cHandle_t* i2cBus_pmbus(void);

i2cHandle_t* i2cBus_sysCfg(void);

intrHandle_t* i2cBus_intr(void);

#endif /* SRC_I2CBUS_H_ */
```


