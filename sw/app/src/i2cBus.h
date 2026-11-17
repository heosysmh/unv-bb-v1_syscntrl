/*******************************************************************************
*
* @file i2cBus.h
*
* @brief Ownership of the I2C infrastructure of the board.
*
*        Owns the interrupt controller handle, both I2C controller instances
*        (PMBus and SysCfg) and the PI4MSD5V945X bus switch that segments the
*        SysCfg bus. All addresses, interrupt IDs and channel assignments come
*        from boardConfig.h.
*
*        Note: this file is deliberately not called i2c.h - that name is
*        already used by the BSP driver (mcu/software/standalone/driver/i2c.h)
*        and every src directory is on the include path.
*
*******************************************************************************/

#ifndef SRC_I2CBUS_H_
#define SRC_I2CBUS_H_

#include "i2cWrapper.h"
#include "interrupt.h"

typedef enum {
	I2CBUS_OK        =  0,
	I2CBUS_E_INIT    = -1,	/* a handle could not be created */
	I2CBUS_E_SELECT  = -2	/* bus switch did not accept the channel selection */
} i2cBus_err_t;

/*
 * Creates the interrupt controller, both I2C controllers and the bus switch
 * handle. Must be called before any device module is initialized.
 */
i2cBus_err_t i2cBus_init(void);

/*
 * Selects one or more channels of the SysCfg bus switch, e.g.
 * BOARD_I2C_CH_MBRD. The channel mask is passed through unchanged, so
 * several channels may be combined.
 */
i2cBus_err_t i2cBus_select(u8 channelMask);

/* Controller handles for the device modules (NULL before i2cBus_init()). */
i2cHandle_t* i2cBus_pmbus(void);
i2cHandle_t* i2cBus_sysCfg(void);

/* Interrupt controller handle, for future users besides the I2C controllers. */
intrHandle_t* i2cBus_intr(void);

#endif /* SRC_I2CBUS_H_ */
