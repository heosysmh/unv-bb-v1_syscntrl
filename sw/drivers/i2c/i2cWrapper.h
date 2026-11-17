/*
 * i2cCom.h
 *
 *  Created on: 13.09.2026
 *      Author: matthias
 */

#ifndef SRC_I2CCOM_H_
#define SRC_I2CCOM_H_

#include "i2c.h"
#include "interrupt.h"

typedef void (*intrHandler_t)(void *data);

typedef struct i2cHandle_t i2cHandle_t;


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
/*
 * Generic multi-byte register access (1-byte register pointer, 'length'
 * data bytes, MSB first). Reads use a genuine repeated start between the
 * pointer write and the data read (see i2c_readData_b_ack() in
 * mcu/software/standalone/driver/i2c.h). i2c_writeReg()/i2c_readReg()
 * above are 1-byte special cases of these.
 */
int i2c_writeRegBuf(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length);
int i2c_readRegBuf (i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length);
int i2c_writeDirect(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length);
int i2c_readDirect(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length);

#endif /* SRC_I2CCOM_H_ */
