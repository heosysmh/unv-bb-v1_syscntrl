

# File tpsm8286x.c

[**File List**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**tpsm8286x**](dir_d611db536405966ae215d07169916749.md) **>** [**tpsm8286x.c**](tpsm8286x_8c.md)

[Go to the documentation of this file](tpsm8286x_8c.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys



#include "tpsm8286x.h"
#include "userDef.h"    /* provides TPSM8286_NUM_OF_INSTANCES */


typedef struct {
    float   min_volt;      
    float   max_volt;      
    float   step_volt;     
    u8 reg_min;     
    u8 reg_max;     
} tpsm8286_voltage_map_t;

struct tpsm8286_t {
    u8 isReady; 
    u8 control; 
    u8 status;  
    u8 isEnabled;   
    float voltage;  
    u8 voltage_raw; 
    u8 i2cAddr; 
    void* hal_ctx;  
    tpsm8286_i2c_write_f i2c_write; 
    tpsm8286_i2c_read_f i2c_read;   
};

const tpsm8286_voltage_map_t TPSM8286_VOLTAGEMAP_10MV = {
    .min_volt = 0.800f, .max_volt = 3.350f, .step_volt = 0.01f,
    .reg_min = 0x00, .reg_max = 0xFF
};

static tpsm8286_t gInstances[TPSM8286_NUM_OF_INSTANCES];

static inline float clampf(float x, float a, float b) { return (x < a) ? a : (x > b) ? b : x; }



/* Public API: documented in tpsm8286x.h only. */
tpsm8286_t* tpsm8286_initialize(u32 devId, u8 i2c_addr, tpsm8286_i2c_write_f w, tpsm8286_i2c_read_f r, void *ctx )
{
    tpsm8286_t* pInst = &gInstances[devId];

    pInst->i2cAddr = i2c_addr;
    pInst->i2c_write = w;
    pInst->i2c_read = r;
    pInst->hal_ctx = ctx;
    pInst->control = 0;
    pInst->status = 0;
    pInst->voltage = 0.0f;
    pInst->voltage_raw = 0;
    pInst->isEnabled = 0;
    pInst->isReady = 1;

    return pInst;
}


tpsm8286_err_t tpsm8286_voltage_to_reg(float voltage, u8 *reg_out) {
    if (!reg_out) return TPSM8286_E_PARAM;
    tpsm8286_voltage_map_t m = TPSM8286_VOLTAGEMAP_10MV;
    float volt_c = clampf(voltage, m.min_volt, m.max_volt);
    float steps = (volt_c - m.min_volt) / m.step_volt;
    if (steps < 0) steps = 0;
    float steps_max = (m.max_volt - m.min_volt) / m.step_volt;
    if (steps > steps_max) steps = steps_max;

    /* Map steps to register range */
    float span = (float)((int)m.reg_max - (int)m.reg_min);
    u8 reg = (u8)((m.reg_min) + (steps / steps_max) * span + 0.5f);
    *reg_out = reg;
    return TPSM8286_OK;
}

tpsm8286_err_t tpsm8286_set_voltage(tpsm8286_t *pDev, float voltage)
{
    if (!pDev) return TPSM8286_E_PARAM;
    tpsm8286_err_t status;

    status = tpsm8286_voltage_to_reg(voltage,&pDev->voltage_raw);
    if(status != 0) return TPSM8286_E_I2C;

    pDev->voltage = voltage;

    status = pDev->i2c_write(pDev->hal_ctx, pDev->i2cAddr, TPSM8286_VOUT1_REG_ADDR, &pDev->voltage_raw);

    return status;
}

tpsm8286_err_t tpsm8286_check_voltageReg(tpsm8286_t *pDev, int *regOk)
{
    if (!pDev) return TPSM8286_E_PARAM;
    tpsm8286_err_t status;
    u8 regVal;

    status = pDev->i2c_read(pDev->hal_ctx, pDev->i2cAddr, TPSM8286_VOUT1_REG_ADDR, &regVal);
    if(status != TPSM8286_OK) return TPSM8286_E_I2C;

    *regOk = (regVal == pDev->voltage_raw) ? 1 : 0;

    return status;
}


tpsm8286_err_t tpsm8286_set_cntrl(tpsm8286_t *pDev, u8 regVal)
{
    if (!pDev) return TPSM8286_E_PARAM;
    tpsm8286_err_t status;

    status = pDev->i2c_write(pDev->hal_ctx, pDev->i2cAddr, TPSM8286_CNTRL_REG_ADDR, &regVal);

    return status;
}

tpsm8286_err_t tpsm8286_set_enable(tpsm8286_t *pDev, int enable)
{
    if (!pDev) return TPSM8286_E_PARAM;
    tpsm8286_err_t status;
    u8 regVal;

    enable = enable != 0 ? TPSM8286_DEVICE_ENABLE_MASK : 0;

    status = pDev->i2c_read(pDev->hal_ctx, pDev->i2cAddr, TPSM8286_CNTRL_REG_ADDR, &regVal);
    if(status != 0) return TPSM8286_E_I2C;

    regVal = (regVal & ~TPSM8286_DEVICE_ENABLE_MASK) | enable;

    status = pDev->i2c_write(pDev->hal_ctx, pDev->i2cAddr, TPSM8286_CNTRL_REG_ADDR, &regVal);

    return status;
}

```


