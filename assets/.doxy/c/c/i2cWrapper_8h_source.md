

# File i2cWrapper.h

[**File List**](files.md) **>** [**drivers**](dir_258f8fd52823f65304266634174f6293.md) **>** [**i2c**](dir_6bfd9f8c6bb0ff18ae4323cf33a90ad8.md) **>** [**i2cWrapper.h**](i2cWrapper_8h.md)

[Go to the documentation of this file](i2cWrapper_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_I2CCOM_H_
#define SRC_I2CCOM_H_

#include "i2c.h"
#include "interrupt.h"

typedef void (*intrHandler_t)(void *data);

typedef struct i2cHandle_t i2cHandle_t;

typedef enum {
    I2C_WRAPPER_OK          =  0,   
    I2C_WRAPPER_ERROR       = -1,   
    I2C_WRAPPER_TIMEOUT     = -2,   
    I2C_WRAPPER_ARB_LOSS    = -3,   
    I2C_WRAPPER_NACK        = -4    
} i2c_wrapper_err_t;

typedef struct
{
    u8 deviceId;        
    u32 baseAddress;    
    u8 intrId;      
    u8 intrPrio;        
    u8 intrHart;        
    I2c_Config cfg;     
} i2cHandle_config_t;


i2cHandle_t* i2c_initialize(i2cHandle_config_t *pDevCfg, intrHandle_t *pIntr);

int i2c_writeReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data);

int i2c_readReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data);


int i2c_writeRegMultipleBytes(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length);

int i2c_readRegMultipleBytes (i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length);

int i2c_write(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length);

int i2c_read(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length);

#endif /* SRC_I2CCOM_H_ */
```


