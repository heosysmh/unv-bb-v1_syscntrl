
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file pi4msd5v945x.c
* @brief Driver implementation for the Diodes PI4MSD5V945x I2C bus switch
*/


#include "pi4msd5v945x.h"

/**
 * @brief Opaque device handle (definition), see pi4msd5v945x_t in pi4msd5v945x.h.
 */
struct pi4msd5v945x_t {
	int isReady;	/**< Non-zero once pi4msd5v945x_initialize() has set up the HAL callbacks. */
	u8 i2cAddr;	/**< 7-bit I2C address of the device. */
	void* hal_ctx;	/**< Opaque context pointer passed through to the HAL callbacks. */
	pi4msd5v945x_i2c_write_f i2c_write;	/**< HAL write callback. */
	pi4msd5v945x_i2c_read_f i2c_read;	/**< HAL read callback. */
};



/** Single static device instance backing pi4msd5v945x_initialize(). */
static pi4msd5v945x_t gDevice;

/* Public API: documented in pi4msd5v945x.h only. */
pi4msd5v945x_t* pi4msd5v945x_initialize(u8 i2c_addr, pi4msd5v945x_i2c_write_f w, pi4msd5v945x_i2c_read_f r, void *ctx )
{
	pi4msd5v945x_t* pDev = &gDevice;

	pDev->i2cAddr = i2c_addr;
	pDev->i2c_write = w;
	pDev->i2c_read = r;
	pDev->hal_ctx = ctx;
	pDev->isReady = 1;

	return pDev;
}


pi4msd5v945x_err_t pi4msd5v945x_setChannels(pi4msd5v945x_t *pDev, u8 channels)
{
	if (!pDev) return PI4MSD5V945X_E_PARAM;

	pDev->i2c_write(pDev->hal_ctx, pDev->i2cAddr,&channels,1);

	return PI4MSD5V945X_OK;
}

