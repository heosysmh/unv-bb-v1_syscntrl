
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file tpsm8286x.h
* @brief Driver for the TI TPSM8286x step-down power module with I2C
*        (PMBus-style) output voltage control.
*
*        Bus-agnostic core driver: it only uses injected HAL callbacks
*        (tpsm8286_i2c_write_f / tpsm8286_i2c_read_f), never a concrete I2C
*        driver directly. The callback signatures match
*        i2c_writeReg()/i2c_readReg() in drivers/i2c/i2cWrapper.h, so they
*        can be passed directly via a function-pointer cast.
*
*        Multiple simultaneous instances are supported: tpsm8286_initialize()
*        takes an explicit @c devId that indexes the static instance pool
*        (size #TPSM8286_NUM_OF_INSTANCES, defined in userDef.h).
*/

#include <type.h>

#ifndef SRC_TPSM8286_H_
#define SRC_TPSM8286_H_

/**
 * @name Register addresses
 * @{
 */
#define TPSM8286_VOUT1_REG_ADDR		0x01	/**< Output voltage setpoint register 1. */
#define TPSM8286_VOUT2_REG_ADDR		0x02	/**< Output voltage setpoint register 2. */
#define TPSM8286_CNTRL_REG_ADDR		0x03	/**< Control register. */
#define TPSM8286_STATUS_REG_ADDR	0x05	/**< Status register. */
/** @} */

/**
 * @name CNTRL register: voltage ramp speed field
 * @{
 */
#define	TPSM8286_VOLTAGE_RAMP_SPEED_MASK	0x03	/**< Mask of the ramp-speed field. */
#define	TPSM8286_VOLTAGE_RAMP_SPEED_20MV	0x00	/**< 20 mV/step ramp speed. */
#define	TPSM8286_VOLTAGE_RAMP_SPEED_10MV	0x01	/**< 10 mV/step ramp speed. */
#define	TPSM8286_VOLTAGE_RAMP_SPEED_5MV		0x10	/**< 5 mV/step ramp speed. */
#define	TPSM8286_VOLTAGE_RAMP_SPEED_1MV		0x11	/**< 1 mV/step ramp speed. */
/** @} */

/**
 * @name CNTRL register: control bits
 * @{
 */
#define TPSM8286_HICCUP_EN_MASK			0x04	/**< Enable hiccup mode on over-current. */
#define TPSM8286_OUTPUT_DISCHARGE_MASK	0x08	/**< Enable output discharge when disabled. */
#define TPSM8286_FPWM_MASK				0x10	/**< Force PWM mode (no pulse skipping). */
#define TPSM8286_DEVICE_ENABLE_MASK		0x20	/**< Device/output enable. */
#define TPSM8286_FPWM_AT_VC_MASK		0x40	/**< Force PWM at voltage change. */
#define TPSM8286_RESET_REGS_MASK		0x80	/**< Reset registers to defaults. */
/** @} */

/**
 * @name STATUS register bits
 * @{
 */
#define TPSM8286_STAT_UVLO_MASK			0x01	/**< Under-voltage lockout active. */
#define TPSM8286_STAT_HICCUP_MASK		0x04	/**< Hiccup mode active. */
#define TPSM8286_STAT_TEMPWARM_MASK		0x10	/**< Temperature warning. */
/** @} */

#define TPSM8286_XXX3_V_TO_REG


/** Status codes of the regulator driver. */
typedef enum {
	TPSM8286_OK = 0,		/**< Success. */
	TPSM8286_E_PARAM = -1,	/**< Invalid argument (e.g. NULL handle). */
	TPSM8286_E_I2C = -2	/**< HAL/bus error. */
} tpsm8286_err_t;

/**
 * @name Low-level HAL (to be provided by the BSP)
 *
 * Write/read one byte at register @p reg_addr of the 7-bit I2C target
 * @p i2c_addr. @p ctx is passed through unchanged (e.g. an i2cHandle_t*).
 * @{
 */
typedef tpsm8286_err_t (*tpsm8286_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
typedef tpsm8286_err_t (*tpsm8286_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr,       u8 *data);
/** @} */

/** Opaque device handle (defined in tpsm8286x.c). */
typedef struct tpsm8286_t tpsm8286_t;

/**
 * @brief Creates the device handle for instance @p devId (no I2C access).
 *
 * @p devId directly indexes the static instance pool
 * (size #TPSM8286_NUM_OF_INSTANCES, defined in userDef.h).
 *
 * @param devId    Instance index.
 * @param i2c_addr 7-bit I2C address of the device.
 * @param w        HAL register write callback.
 * @param r        HAL register read callback.
 * @param ctx      Opaque context pointer passed through to @p w and @p r.
 * @return Pointer to the device handle.
 */
tpsm8286_t* tpsm8286_initialize(u32 devId, u8 i2c_addr, tpsm8286_i2c_write_f w, tpsm8286_i2c_read_f r, void *ctx);

/**
 * @brief Sets the output voltage setpoint (VOUT1 register).
 *
 * The voltage is mapped to the 10 mV/LSB register scale and clamped to the
 * supported range (0.8 V .. 3.35 V). Takes effect according to the
 * configured ramp speed; the output enable state is not changed.
 *
 * @param pDev    Device handle.
 * @param voltage Desired output voltage [V].
 * @return #TPSM8286_OK on success, #TPSM8286_E_PARAM if @p pDev is NULL,
 *         #TPSM8286_E_I2C on a HAL/bus error.
 */
tpsm8286_err_t tpsm8286_set_voltage(tpsm8286_t *pDev, float voltage);

/**
 * @brief Writes the complete CNTRL register.
 * @param pDev   Device handle.
 * @param regVal Raw control register value (OR of TPSM8286_*_MASK bits).
 * @return #TPSM8286_OK on success, #TPSM8286_E_PARAM if @p pDev is NULL,
 *         otherwise the HAL error code.
 */
tpsm8286_err_t tpsm8286_set_cntrl(tpsm8286_t *pDev, u8 regVal);

/**
 * @brief Enables/disables the output (CNTRL.DEVICE_ENABLE, read-modify-write).
 * @param pDev   Device handle.
 * @param enable Non-zero to enable, zero to disable.
 * @return #TPSM8286_OK on success, #TPSM8286_E_PARAM if @p pDev is NULL,
 *         #TPSM8286_E_I2C on a HAL/bus error.
 */
tpsm8286_err_t tpsm8286_set_enable(tpsm8286_t *pDev, int enable);

/**
 * @brief Checks whether the VOUT1 register reads back the programmed value.
 * @param pDev      Device handle.
 * @param[out] regOk Receives 1 if the register matches the last setpoint,
 *                   0 otherwise.
 * @return #TPSM8286_OK on success, #TPSM8286_E_PARAM if @p pDev is NULL,
 *         #TPSM8286_E_I2C on a HAL/bus error.
 */
tpsm8286_err_t tpsm8286_check_voltageReg(tpsm8286_t *pDev, int *regOk);

#endif /* SR_TPSM8286_H_ */
