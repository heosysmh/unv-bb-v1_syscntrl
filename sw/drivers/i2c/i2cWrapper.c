/*
 * i2cCom.c
 *
 *  Created on: 13.09.2026
 *      Author: matthias
 */

#include "i2cWrapper.h"
#include "userDef.h"
#include "bsp.h"

struct i2cHandle_t
{
	u8 deviceId;
	u32 baseAddress;
	u8 intrId;
	u8 intrPrio;
	u8 intrHart;
	I2c_Config cfg;
};

static i2cHandle_t gI2cInstances[I2C_NUM_OF_INSTANCES];

static void i2c_isr();

i2cHandle_t* i2c_initialize(i2cHandle_config_t *pDevCfg, intrHandle_t *pIntr)
{
	i2cHandle_t *pInst = &gI2cInstances[pDevCfg->deviceId];

	pInst->baseAddress 				= pDevCfg->baseAddress;
	pInst->deviceId 				= pDevCfg->deviceId;
	pInst->intrHart					= pDevCfg->intrHart;
	pInst->intrId					= pDevCfg->intrId;
	pInst->intrPrio					= pDevCfg->intrPrio;
	pInst->cfg.samplingClockDivider = pDevCfg->cfg.samplingClockDivider;
	pInst->cfg.tBuf 				= pDevCfg->cfg.tBuf;
	pInst->cfg.tHigh 				= pDevCfg->cfg.tHigh;
	pInst->cfg.tLow 				= pDevCfg->cfg.tLow;
	pInst->cfg.timeout 			 	= pDevCfg->cfg.timeout;
	pInst->cfg.tsuDat 				= pDevCfg->cfg.tsuDat;

	i2c_applyConfig(pInst->baseAddress, &pInst->cfg);

	intr_connect(pIntr, pInst->intrId, &i2c_isr, pInst);

    //enable PLIC I2C interrupts
	intr_serPriority(pIntr, pInst->intrId, pInst->intrPrio);
	intr_setEnable(pIntr, pInst->intrHart, pInst->intrId);

	return pInst;
}

int i2c_writeReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data) {

	i2c_writeData_b_ack(pInst->baseAddress, (slaveAddr<<1), regAddr, data, 0x1);

	return 0;
}


int i2c_readReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data) {

	i2c_readData_b_ack(pInst->baseAddress, (slaveAddr<<1), regAddr, data , 0x1);

	return 0;
}

int i2c_writeDirect(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length) {

	volatile u32 reg;
	i2c_masterStartBlocking(pInst->baseAddress);                   // Start condition
	//write_u32(slaveAddr | I2C_TX_VALID | I2C_TX_ENABLE , pInst->baseAddress + I2C_TX_DATA);
	//reg = i2c_getMasterStatus(pInst->baseAddress);
	TX_AND_CHECK(pInst->baseAddress, ((slaveAddr<<1) | I2C_WRITE));     // Send slave address (write mode)
	for(u32 i = 0; i < length; i++){
		TX_AND_CHECK(pInst->baseAddress, data[i]);                 // Send each byte
	}

	i2c_masterStopBlocking(pInst->baseAddress);                    // Stop condition

	return 0;
}


int i2c_readDirect(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length) {

	i2c_masterStartBlocking(pInst->baseAddress);               		 // Send start sequence
	TX_AND_CHECK(pInst->baseAddress, (slaveAddr<<1) | I2C_READ);     // write device address byte with read bit
	if(length > 1){
		for(u32 i = 0; i < length - 1; i++){
			i2c_txByte(pInst->baseAddress, 0xFF);              // send 0xFF (Release SDA line) to the slave while generate 8-bit SCL pulses
			i2c_txAckBlocking(pInst->baseAddress);             // send ack bit to ask slave to continue send the next byte
			data[i] = i2c_rxData(pInst->baseAddress);          // read the data from rx data register and place it into data array
		}
	}
	i2c_txByte(pInst->baseAddress, 0xFF);                      // send 0xFF (Release SDA line) to the slave while generate 8-bit SCL pulses
	i2c_txNackBlocking(pInst->baseAddress);                    // send nack bit
	data[length-1] = i2c_rxData(pInst->baseAddress);           // read the data from rx data register and place it into last data array
	i2c_masterStopBlocking(pInst->baseAddress);                // send stop sequence

	return 0;
}

/******************************************************************************
*
* @brief This function handles I2C interrupts and clear interrupt flag.
*
******************************************************************************/
static void i2c_isr(void *callBackRef){

	i2cHandle_t *pInst = (i2cHandle_t*)callBackRef;

	if (i2c_getInterruptFlag(pInst->baseAddress) & I2C_INTERRUPT_DROP)
		bsp_printf("I2C timeout!\r\n");
	else
		bsp_printf("I2C Interrupt is triggered!\r\n");

	i2c_clearInterruptFlag(pInst->baseAddress, I2C_INTERRUPT_DROP);
	while(1);

}
