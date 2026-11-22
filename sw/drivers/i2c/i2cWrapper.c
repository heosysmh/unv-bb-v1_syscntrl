
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file i2cWrapper.c
* @brief Implementation of the I2C master wrapper.
*
* @details
* Transfers are polled; the ISR only sets error flags (timeout,
* arbitration loss) which the polling loops check, so a stuck bus
* aborts the transfer instead of hanging forever.
*
*******************************************************************************/

#include "i2cWrapper.h"
#include "userDef.h"
#include "bsp.h"

#define DEFAULT_TIMEOUT 100000  /**< Default timeout in ticks. */

/**
 * @brief I2C controller instance state (opaque handle, see i2cWrapper.h).
 */
struct i2cHandle_t
{
	u8 deviceId;		/**< Instance index into #gI2cInstances. */
	u32 baseAddress;	/**< Base address of the I2C controller. */
	u8 intrId;		/**< PLIC interrupt source ID. */
	u8 intrPrio;		/**< PLIC interrupt priority. */
	u8 intrHart;		/**< Hart (CPU) the interrupt is routed to. */
	I2c_Config cfg;		/**< Controller timing configuration. */

	/* Error flags set asynchronously by the ISR */
	volatile u8 timeout;      /**< Flag set by ISR when timeout detected. */
	volatile u8 arbLoss;      /**< Flag for arbitration loss. */
	volatile u8 nackRecvd;    /**< Flag for NACK reception. */
};

/** Static pool of controller instances (size from userDef.h). */
static i2cHandle_t gI2cInstances[I2C_NUM_OF_INSTANCES];

static void i2c_isr();




/**
 * @name Polling helpers with interrupt-based error check (WithEC)
 *
 * Each helper issues a low-level BSP operation and polls its completion
 * bit while checking the ISR error flags (timeout / arbitration loss).
 * @{
 */

/**
 * @brief Transmits a NACK and waits for completion.
 * @param pInst Controller handle.
 * @return #I2C_WRAPPER_OK, #I2C_WRAPPER_TIMEOUT or #I2C_WRAPPER_ARB_LOSS.
 */
static int i2c_txNackWithEC(i2cHandle_t *pInst){
    i2c_txNack(pInst->baseAddress);

    while(read_u32(pInst->baseAddress + I2C_TX_ACK) & I2C_TX_VALID) 
    {
        if (pInst->timeout) {
            return I2C_WRAPPER_TIMEOUT;
        }
        if (pInst->arbLoss) {
            return I2C_WRAPPER_ARB_LOSS;
        }       
    }
    return I2C_WRAPPER_OK;
}

/**
 * @brief Transmits an ACK and waits for completion.
 * @param pInst Controller handle.
 * @return #I2C_WRAPPER_OK, #I2C_WRAPPER_TIMEOUT or #I2C_WRAPPER_ARB_LOSS.
 */
static int i2c_txAckWithEC(i2cHandle_t *pInst){
    i2c_txAck(pInst->baseAddress);

    while(read_u32(pInst->baseAddress + I2C_TX_ACK) & I2C_TX_VALID) 
    {
        if (pInst->timeout) {
            return I2C_WRAPPER_TIMEOUT;
        }
        if (pInst->arbLoss) {
            return I2C_WRAPPER_ARB_LOSS;
        }       
    }
    return I2C_WRAPPER_OK;
}

/**
 * @brief Waits until the received ACK/NACK bit is valid.
 * @param pInst Controller handle.
 * @return #I2C_WRAPPER_OK, #I2C_WRAPPER_TIMEOUT or #I2C_WRAPPER_ARB_LOSS.
 */
static int i2c_rxAckWaitWithEC(i2cHandle_t *pInst){
    while ((read_u32(pInst->baseAddress + I2C_RX_ACK) & I2C_RX_VALUE) != 0)
    {
        if (pInst->timeout) {
        return I2C_WRAPPER_TIMEOUT;
        }
        if (pInst->arbLoss) {
            return I2C_WRAPPER_ARB_LOSS;
        }     
    }
    return I2C_WRAPPER_OK;
}



/**
 * @brief Issues a (repeated) START and waits for completion.
 *
 * Resets the timeout/arbitration-loss flags before polling.
 *
 * @param pInst Controller handle.
 * @return #I2C_WRAPPER_OK, #I2C_WRAPPER_TIMEOUT or #I2C_WRAPPER_ARB_LOSS.
 */
static int i2c_masterStartWithEC(i2cHandle_t *pInst) {
    i2c_masterStart(pInst->baseAddress);
    
    // Reset timeout flags
    pInst->timeout = 0;
    pInst->arbLoss = 0;
    
    // Poll with error check
    while(i2c_getMasterStatus(pInst->baseAddress) & I2C_MASTER_START) {
        if (pInst->timeout) {
            return I2C_WRAPPER_TIMEOUT;
        }
        if (pInst->arbLoss) {
            return I2C_WRAPPER_ARB_LOSS;
        }        
    }
    return I2C_WRAPPER_OK;
}

/**
 * @brief Issues a STOP and waits for completion.
 *
 * Resets the timeout/arbitration-loss flags before polling.
 *
 * @param pInst Controller handle.
 * @return #I2C_WRAPPER_OK, #I2C_WRAPPER_TIMEOUT or #I2C_WRAPPER_ARB_LOSS.
 */
static int i2c_masterStopWithEC(i2cHandle_t *pInst) {
    i2c_masterStop(pInst->baseAddress);
    
    // Reset timeout flags
    pInst->timeout = 0;
    pInst->arbLoss = 0;
    
    // Poll with error check
    while(i2c_getMasterStatus(pInst->baseAddress) & I2C_MASTER_STOP) {
        if (pInst->timeout) {
            return I2C_WRAPPER_TIMEOUT;
        }
        if (pInst->arbLoss) {
            return I2C_WRAPPER_ARB_LOSS;
        }       
    }
    return I2C_WRAPPER_OK;
}

/**
 * @brief Transmits one byte and waits for the target acknowledge.
 * @param pInst Controller handle.
 * @param byte  Byte to transmit.
 * @return #I2C_WRAPPER_OK, #I2C_WRAPPER_TIMEOUT or #I2C_WRAPPER_ARB_LOSS.
 */
static int TX_AND_CHECK_WITH_EC(i2cHandle_t *pInst, u8 byte) {
    int result;

    i2c_txByte(pInst->baseAddress, byte);
       
    result = i2c_txNackWithEC(pInst);  
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    i2c_rxAckWaitWithEC(pInst);     
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    return I2C_WRAPPER_OK;
}

/** @} */

/* Public API: documented in i2cWrapper.h only. */
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
	
	// Initialize timeout-related fields
	pInst->timeout = 0;
	pInst->arbLoss = 0;
	pInst->nackRecvd = 0;	

	i2c_applyConfig(pInst->baseAddress, &pInst->cfg);

	intr_connect(pIntr, pInst->intrId, &i2c_isr, pInst);

    //enable PLIC I2C interrupts
	intr_setPriority(pIntr, pInst->intrId, pInst->intrPrio);
	intr_setEnable(pIntr, pInst->intrHart, pInst->intrId);

    i2c_enableInterrupt(pInst->baseAddress, I2C_INTERRUPT_DROP);

	return pInst;
}

int i2c_writeRegMultipleBytes(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length) {
    int result;

    // Start condition
    result = i2c_masterStartWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    
    // Send slave address with write bit
    result = TX_AND_CHECK_WITH_EC(pInst, ((slaveAddr<<1) | I2C_WRITE));
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    
    // Send register address
    result = TX_AND_CHECK_WITH_EC(pInst, regAddr);
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  
        return result;
    }
    
    // Send data
    for(u32 i = 0; i < length; i++){
        result = TX_AND_CHECK_WITH_EC(pInst, data[i]);
        if (result != I2C_WRAPPER_OK) {
            i2c_masterStop(pInst->baseAddress);  
        }
    }
    
    i2c_masterStopWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    return I2C_WRAPPER_OK;
}

int i2c_readRegMultipleBytes(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length) {
    int result;     

    // Start condition
    result = i2c_masterStartWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    
    // Send slave address with write bit
    result = TX_AND_CHECK_WITH_EC(pInst, ((slaveAddr<<1) | I2C_WRITE));
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    
    // Send register address
    result = TX_AND_CHECK_WITH_EC(pInst, regAddr);
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    
    // Repeated start condition
    result = i2c_masterStartWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    
    // Send slave address with read bit
    result = TX_AND_CHECK_WITH_EC(pInst, ((slaveAddr<<1) | I2C_READ));
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    
    if(length > 1){
        for(u32 i = 0; i < length - 1; i++){
            i2c_txByte(pInst->baseAddress, 0xFF);
            result = i2c_txAckWithEC(pInst);
            if (result != I2C_WRAPPER_OK) {
                i2c_masterStop(pInst->baseAddress);  // Clean up
                return result;
            }
            data[i] = i2c_rxData(pInst->baseAddress);
        }
    }
    
    i2c_txByte(pInst->baseAddress, 0xFF);
    result = i2c_txNackWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    data[length-1] = i2c_rxData(pInst->baseAddress);
    
    result = i2c_masterStopWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    return I2C_WRAPPER_OK;
}

int i2c_writeReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data) {

	return i2c_writeRegMultipleBytes(pInst, slaveAddr, regAddr, data, 0x1);
}


int i2c_readReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data) {

	return i2c_readRegMultipleBytes(pInst, slaveAddr, regAddr, data, 0x1);
}

int i2c_write(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length) {
    int result;
    
    // Start condition
    result = i2c_masterStartWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    
    // Send slave address with write bit
    result = TX_AND_CHECK_WITH_EC(pInst, ((slaveAddr<<1) | I2C_WRITE));
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    
    // Send data
    for(u32 i = 0; i < length; i++){
        result = TX_AND_CHECK_WITH_EC(pInst, data[i]);
        if (result != I2C_WRAPPER_OK) {
            i2c_masterStop(pInst->baseAddress);  // Clean up
            return result;
        }
    }
    
    result = i2c_masterStopWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    return I2C_WRAPPER_OK;
}


int i2c_read(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length) {
    int result;
    
    // Start condition
    result = i2c_masterStartWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    
    // Send slave address with read bit
    result = TX_AND_CHECK_WITH_EC(pInst, ((slaveAddr<<1) | I2C_READ));
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    
    if(length > 1){
        for(u32 i = 0; i < length - 1; i++){
            i2c_txByte(pInst->baseAddress, 0xFF);
            result = i2c_txAckWithEC(pInst);
            if (result != I2C_WRAPPER_OK) {
                i2c_masterStop(pInst->baseAddress);  // Clean up
                return result;
            }
            data[i] = i2c_rxData(pInst->baseAddress);
        }
    }
    
    i2c_txByte(pInst->baseAddress, 0xFF);
    result = i2c_txNackWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        i2c_masterStop(pInst->baseAddress);  // Clean up
        return result;
    }
    data[length-1] = i2c_rxData(pInst->baseAddress);
    
    result = i2c_masterStopWithEC(pInst);
    if (result != I2C_WRAPPER_OK) {
        return result;
    }
    return I2C_WRAPPER_OK;
}

/**
 * @brief I2C interrupt service routine.
 *
 * Sets the timeout / arbitration-loss error flags evaluated by the polling
 * helpers and clears all interrupt flags.
 *
 * @param callBackRef Controller handle registered via intr_connect().
 */
static void i2c_isr(void *callBackRef){
    i2cHandle_t *pInst = (i2cHandle_t*)callBackRef;
    u32 interruptFlags = i2c_getInterruptFlag(pInst->baseAddress);
    
    // Check for drop interrupt (timeout)
    if (interruptFlags & I2C_INTERRUPT_DROP) {
        pInst->timeout = 1;
        bsp_printf("I2C timeout detected!\r\n");
    }
    
    // Check for arbitration loss
    if (interruptFlags & I2C_MASTER_START_DROPPED) {
        pInst->arbLoss = 1;
        bsp_printf("I2C arbitration loss detected!\r\n");
    }
    
    // Clear all interrupt flags
    i2c_clearInterruptFlag(pInst->baseAddress, interruptFlags);
}
