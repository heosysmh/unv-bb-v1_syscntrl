/*#
 * tpsm82866.c
 *
 *  Created on: 13.09.2026
 *      Author: matthias
 */


#include "si5332.h"
#include "si5332_regs.h"
#include "userDef.h"




struct si5332_t {
	u8 isReady;
	u8 i2cAddr;
	void* hal_ctx;
	si5332_i2c_write_f i2c_write;
	si5332_i2c_read_f i2c_read;
};



static si5332_t gDevice;

si5332_t* si5332_initialize(u8 i2c_addr, si5332_i2c_write_f w, si5332_i2c_read_f r, void *ctx )
{
	si5332_t* pDev = &gDevice;

	pDev->i2cAddr = i2c_addr;
	pDev->i2c_write = w;
	pDev->i2c_read = r;
	pDev->hal_ctx = ctx;
	pDev->isReady = 1;

	return pDev;
}

si5332_err_t si5332_apply_config(si5332_t *pDev)
{
	if (!pDev) return SI5332_E_PARAM;

	si5332_err_t status;
	int i;

	for(i=0;i<SI5332_GM2_REVD_REG_CONFIG_NUM_REGS;i++)
	{
		status = pDev->i2c_write(pDev->hal_ctx, pDev->i2cAddr, (u8)si5332_gm2_revd_registers[i].address, &si5332_gm2_revd_registers[i].value);
		if(status != SI5332_OK) return SI5332_E_I2C;
	}


	return SI5332_OK;
}


