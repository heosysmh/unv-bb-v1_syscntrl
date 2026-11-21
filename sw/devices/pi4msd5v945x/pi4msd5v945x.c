/*******************************************************************************
*
* @file pi4msd5v945x.c
*
* @brief Driver implementation for the Diodes PI4MSD5V945x I2C bus switch,
*        see pi4msd5v945x.h.
*
*******************************************************************************/


#include "pi4msd5v945x.h"
#include "userDef.h"




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

/**
 * @brief Creates the device handle (no I2C access), see pi4msd5v945x.h.
 *
 * @param i2c_addr 7-bit I2C address of the bus switch.
 * @param w        HAL write callback.
 * @param r        HAL read callback.
 * @param ctx      Opaque context pointer passed through to @p w and @p r.
 * @return Pointer to the device handle.
 */
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


/**
 * @brief Selects the active downstream channels, see pi4msd5v945x.h.
 *
 * @param pDev     Device handle.
 * @param channels Bitmask of PI4MSD5V945X_CHx bits (0 = all channels off).
 * @return #PI4MSD5V945X_OK on success, #PI4MSD5V945X_E_PARAM if @p pDev is NULL.
 */
pi4msd5v945x_err_t pi4msd5v945x_setChannels(pi4msd5v945x_t *pDev, u8 channels)
{
	if (!pDev) return PI4MSD5V945X_E_PARAM;

	pDev->i2c_write(pDev->hal_ctx, pDev->i2cAddr,&channels,1);

	return PI4MSD5V945X_OK;
}

