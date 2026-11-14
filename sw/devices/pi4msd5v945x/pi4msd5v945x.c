/*#
 * pi4msd5v945x.c
 *
 *  Created on: 13.09.2026
 *      Author: matthias
 */


#include "pi4msd5v945x.h"
#include "userDef.h"




struct pi4msd5v945x_t {
	int isReady;
	u8 i2cAddr;
	void* hal_ctx;
	pi4msd5v945x_i2c_write_f i2c_write;
	pi4msd5v945x_i2c_read_f i2c_read;
};



static pi4msd5v945x_t gDevice;

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

