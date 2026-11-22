
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file i2cWrapper.h
* @brief I2C master wrapper around the BSP I2C driver.
*
*        Wraps the low-level BSP driver (mcu/software/standalone/driver/i2c.h)
*        into a handle-based API with error checking. Transfers are polled,
*        but bus timeout and arbitration loss are detected asynchronously by
*        the I2C interrupt service routine, so a stuck bus cannot hang the
*        caller forever.
*
*        The register-access functions use a 1-byte register pointer and
*        transfer data MSB first. Their signatures are intentionally
*        compatible with the HAL callback types of the device drivers
*        (si5332, tpsm8286x, ina745a, pi4msd5v945x), so they can be passed
*        directly via a function-pointer cast.
*
* @note  This file is deliberately not called i2c.h - that name is already
*        used by the BSP driver and every src directory is on the include
*        path.
*
*/

#ifndef SRC_I2CCOM_H_
#define SRC_I2CCOM_H_

#include "i2c.h"
#include "interrupt.h"

/** Interrupt handler callback type (see interrupt.h). */
typedef void (*intrHandler_t)(void *data);

/** Opaque I2C controller handle (defined in i2cWrapper.c). */
typedef struct i2cHandle_t i2cHandle_t;

/** Status codes returned by the I2C wrapper functions. */
typedef enum {
    I2C_WRAPPER_OK          =  0,	/**< Transfer completed successfully. */
    I2C_WRAPPER_ERROR       = -1,	/**< Generic error. */
    I2C_WRAPPER_TIMEOUT     = -2,	/**< Bus timeout detected by the ISR (DROP interrupt). */
    I2C_WRAPPER_ARB_LOSS    = -3,	/**< Arbitration loss detected by the ISR. */
    I2C_WRAPPER_NACK        = -4	/**< NACK received from the target. */
} i2c_wrapper_err_t;

/** Configuration of one I2C controller instance, passed to i2c_initialize(). */
typedef struct
{
	u8 deviceId;		/**< Instance index into the static pool (0 .. I2C_NUM_OF_INSTANCES-1). */
	u32 baseAddress;	/**< Base address of the I2C controller. */
	u8 intrId;		/**< PLIC interrupt source ID of the controller. */
	u8 intrPrio;		/**< PLIC interrupt priority. */
	u8 intrHart;		/**< Hart (CPU) the interrupt is routed to. */
	I2c_Config cfg;		/**< Controller timing configuration (BSP driver). */
} i2cHandle_config_t;


/**
 * @brief Initializes one I2C controller instance.
 *
 * Applies the timing configuration, connects the I2C ISR to the interrupt
 * controller and enables the DROP (timeout) interrupt.
 *
 * @param pDevCfg Controller configuration; @c deviceId selects the instance.
 * @param pIntr   Interrupt controller handle from intr_initialize().
 * @return Pointer to the controller instance.
 */
i2cHandle_t* i2c_initialize(i2cHandle_config_t *pDevCfg, intrHandle_t *pIntr);

/**
 * @brief Writes a single byte to an 8-bit register (1-byte special case of
 *        i2c_writeRegMultipleBytes()).
 * @param pInst     Controller handle.
 * @param slaveAddr 7-bit I2C target address.
 * @param regAddr   Register address.
 * @param data      Pointer to the byte to write.
 * @return #I2C_WRAPPER_OK on success, negative i2c_wrapper_err_t code on error.
 */
int i2c_writeReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data);

/**
 * @brief Reads a single byte from an 8-bit register (1-byte special case of
 *        i2c_readRegMultipleBytes()).
 * @param pInst     Controller handle.
 * @param slaveAddr 7-bit I2C target address.
 * @param regAddr   Register address.
 * @param[out] data Receives the register byte on success.
 * @return #I2C_WRAPPER_OK on success, negative i2c_wrapper_err_t code on error.
 */
int i2c_readReg(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data);

/**
 * @name Generic multi-byte register access
 *
 * 1-byte register pointer, @c length data bytes, MSB first. Reads use a
 * genuine repeated start between the pointer write and the data read
 * (see i2c_readData_b_ack() in mcu/software/standalone/driver/i2c.h).
 * i2c_writeReg()/i2c_readReg() are 1-byte special cases of these.
 * @{
 */

/**
 * @brief Writes @p length bytes to a register, MSB first.
 * @param pInst     Controller handle.
 * @param slaveAddr 7-bit I2C target address.
 * @param regAddr   Register address.
 * @param data      Buffer with the bytes to write.
 * @param length    Number of data bytes.
 * @return #I2C_WRAPPER_OK on success, negative i2c_wrapper_err_t code on error.
 */
int i2c_writeRegMultipleBytes(i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length);

/**
 * @brief Reads @p length bytes from a register, MSB first, using a repeated start.
 * @param pInst     Controller handle.
 * @param slaveAddr 7-bit I2C target address.
 * @param regAddr   Register address.
 * @param[out] data Receives the data bytes on success.
 * @param length    Number of data bytes (>= 1).
 * @return #I2C_WRAPPER_OK on success, negative i2c_wrapper_err_t code on error.
 */
int i2c_readRegMultipleBytes (i2cHandle_t *pInst, u8 slaveAddr, u8 regAddr, u8 *data, u32 length);
/** @} */

/**
 * @brief Writes @p length bytes without a register pointer (raw write).
 * @param pInst     Controller handle.
 * @param slaveAddr 7-bit I2C target address.
 * @param data      Buffer with the bytes to write.
 * @param length    Number of data bytes.
 * @return #I2C_WRAPPER_OK on success, negative i2c_wrapper_err_t code on error.
 */
int i2c_write(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length);

/**
 * @brief Reads @p length bytes without a register pointer (raw read).
 * @param pInst     Controller handle.
 * @param slaveAddr 7-bit I2C target address.
 * @param[out] data Receives the data bytes on success.
 * @param length    Number of data bytes (>= 1).
 * @return #I2C_WRAPPER_OK on success, negative i2c_wrapper_err_t code on error.
 */
int i2c_read(i2cHandle_t *pInst, u8 slaveAddr, u8 *data, u32 length);

#endif /* SRC_I2CCOM_H_ */
