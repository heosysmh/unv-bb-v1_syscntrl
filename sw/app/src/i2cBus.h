/*******************************************************************************
*
* @file i2cBus.h
*
* @brief Ownership of the I2C infrastructure of the board.
*
*        Owns the interrupt controller handle, both I2C controller instances
*        (PMBus and SysCfg) and the PI4MSD5V945X bus switch that segments the
*        SysCfg bus. Addresses and channels are assigned here.
*
*        Note: this file is deliberately not called i2c.h - that name is
*        already used by the BSP driver (mcu/software/standalone/driver/i2c.h)
*        and every src directory is on the include path.
*
*******************************************************************************/

#ifndef SRC_I2CBUS_H_
#define SRC_I2CBUS_H_

#include "pi4msd5v945x.h"
#include "i2cWrapper.h"
#include "interrupt.h"

/* --- Device I2C addresses -------------------------------------------------- */
#define I2CBUS_FMC1_VADJ_ADDR		0x4E
#define I2CBUS_FMC2_VADJ_ADDR		0x4F
#define I2CBUS_CLKGEN_ADDR		0x6A
#define I2CBUS_BUS_SWITCH_ADDR	0x77

/* --- I2C bus switch channels (SysCfg bus) ---------------------------------- */
#define I2CBUS_CH_SYZYGY	PI4MSD5V945X_CH0
#define I2CBUS_CH_FMC2		PI4MSD5V945X_CH1
#define I2CBUS_CH_FMC1		PI4MSD5V945X_CH2
#define I2CBUS_CH_MBRD		PI4MSD5V945X_CH3

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
