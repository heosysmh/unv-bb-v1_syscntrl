
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file pi4msd5v945x.h
* @brief Driver for the Diodes PI4MSD5V945x 4-channel I2C bus switch.
*
*        Bus-agnostic core driver: it only uses injected HAL callbacks
*        (pi4msd5v945x_i2c_write_f / pi4msd5v945x_i2c_read_f), never a
*        concrete I2C driver directly. The callback signatures match
*        i2c_write()/i2c_read() in drivers/i2c/i2cWrapper.h, so they can be
*        passed directly via a function-pointer cast.
*
*        The device supports any combination of the four downstream
*        channels; the channel mask is written as a single control byte.
*/

#include <type.h>

#ifndef SRC_PI4MSD5V945X_H_
#define SRC_PI4MSD5V945X_H_

/**
 * @name Channel definitions
 * Channel bits of the control register; any combination may be selected.
 * @{
 */
#define PI4MSD5V945X_CH0	0x1u	/**< Downstream channel 0. */
#define PI4MSD5V945X_CH1	0x2u	/**< Downstream channel 1. */
#define PI4MSD5V945X_CH2	0x4u	/**< Downstream channel 2. */
#define PI4MSD5V945X_CH3	0x8u	/**< Downstream channel 3. */
/** @} */

/** Status codes of the bus switch driver. */
typedef enum {
	PI4MSD5V945X_OK = 0,		/**< Success. */
	PI4MSD5V945X_E_PARAM = -1,	/**< Invalid argument (e.g. NULL handle). */
	PI4MSD5V945X_E_I2C = -2		/**< HAL/bus error. */
} pi4msd5v945x_err_t;

/**
 * @name Low-level HAL (to be provided by the BSP)
 *
 * Write/read @p length bytes to/from the 7-bit I2C target @p i2c_addr
 * without a register pointer. @p ctx is passed through unchanged
 * (e.g. an i2cHandle_t*).
 * @{
 */
typedef pi4msd5v945x_err_t (*pi4msd5v945x_i2c_write_f)(void *ctx, u8 i2c_addr, const u8 *data, u32 length);
typedef pi4msd5v945x_err_t (*pi4msd5v945x_i2c_read_f) (void *ctx, u8 i2c_addr,       u8 *data, u32 length);
/** @} */

/** Opaque device handle (defined in pi4msd5v945x.c). */
typedef struct pi4msd5v945x_t pi4msd5v945x_t;

/**
 * @brief Creates the device handle (no I2C access).
 *
 * The driver manages a single static instance.
 *
 * @param i2c_addr 7-bit I2C address of the bus switch.
 * @param w        HAL write callback.
 * @param r        HAL read callback.
 * @param ctx      Opaque context pointer passed through to @p w and @p r.
 * @return Pointer to the device handle.
 */
pi4msd5v945x_t* pi4msd5v945x_initialize(u8 i2c_addr, pi4msd5v945x_i2c_write_f w, pi4msd5v945x_i2c_read_f r, void *ctx);

/**
 * @brief Selects the active downstream channels.
 *
 * Writes the channel mask as the single control byte; previously selected
 * channels are replaced, not added.
 *
 * @param pDev     Device handle from pi4msd5v945x_initialize().
 * @param channels Bitmask of PI4MSD5V945X_CHx bits (0 = all channels off).
 * @return #PI4MSD5V945X_OK on success, #PI4MSD5V945X_E_PARAM if @p pDev is
 *         NULL.
 */
pi4msd5v945x_err_t pi4msd5v945x_setChannels(pi4msd5v945x_t *pDev, u8 channels);

#endif /* SRC_PI4MSD5V945X_H_ */
