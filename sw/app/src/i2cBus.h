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
* @note  This file is deliberately not called i2c.h - that name is
*        already used by the BSP driver (mcu/software/standalone/driver/i2c.h)
*        and every src directory is on the include path.
*
*******************************************************************************/

#ifndef SRC_I2CBUS_H_
#define SRC_I2CBUS_H_

#include "pi4msd5v945x.h"
#include "i2cWrapper.h"
#include "interrupt.h"

/**
 * @name Device I2C addresses (SysCfg bus)
 * @{
 */
#define I2CBUS_CLKGEN_ADDR		0x6A	/**< Si5332 clock generator (behind channel #I2CBUS_CH_MBRD). */
#define I2CBUS_BUS_SWITCH_ADDR	0x77	/**< PI4MSD5V945X bus switch. */
/** @} */

/**
 * @name PMBus addresses (DC/DC converters and power monitors)
 * @{
 */
#define I2CBUS_PWRMON_12V_ADDR	0x40	/**< INA745A on the 12V rail. */
#define I2CBUS_PWRMON_3V3_ADDR	0x41	/**< INA745A on the 3.3V rail. */
#define I2CBUS_PWRMON_5V0_ADDR	0x43	/**< INA745A on the 5.0V SoM rail. */
#define I2CBUS_PWRMON_1V8_ADDR	0x44	/**< INA745A on the 1.8V rail. */
#define I2CBUS_PWRMON_FMC1_ADDR	0x4C	/**< INA745A on FMC1 Vadj. */
#define I2CBUS_PWRMON_FMC2_ADDR	0x4D	/**< INA745A on FMC2 Vadj. */
#define I2CBUS_FMC1_VADJ_ADDR	0x4E	/**< TPSM8286x FMC1 Vadj regulator. */
#define I2CBUS_FMC2_VADJ_ADDR	0x4F	/**< TPSM8286x FMC2 Vadj regulator. */
/** @} */

/**
 * @name I2C bus switch channels (SysCfg bus)
 * @{
 */
#define I2CBUS_CH_SYZYGY	PI4MSD5V945X_CH0	/**< SYZYGY segment. */
#define I2CBUS_CH_FMC2		PI4MSD5V945X_CH1	/**< FMC2 segment. */
#define I2CBUS_CH_FMC1		PI4MSD5V945X_CH2	/**< FMC1 segment. */
#define I2CBUS_CH_MBRD		PI4MSD5V945X_CH3	/**< Motherboard segment (clock generator). */
/** @} */

/** Status codes of the I2C infrastructure module. */
typedef enum {
	I2CBUS_OK        =  0,	/**< Success. */
	I2CBUS_E_INIT    = -1,	/**< A handle could not be created. */
	I2CBUS_E_SELECT  = -2	/**< Bus switch did not accept the channel selection. */
} i2cBus_err_t;

/**
 * @brief Creates the I2C infrastructure handles.
 *
 * Creates the interrupt controller, both I2C controllers and the bus switch
 * handle. Must be called before any device module is initialized.
 *
 * @return #I2CBUS_OK on success, #I2CBUS_E_INIT if a handle could not be
 *         created.
 */
i2cBus_err_t i2cBus_init(void);

/**
 * @brief Selects one or more channels of the SysCfg bus switch.
 *
 * The channel mask is passed through unchanged, so several channels may be
 * combined (e.g. #I2CBUS_CH_MBRD).
 *
 * @param channelMask Bitmask of PI4MSD5V945X_CHx channels.
 * @return #I2CBUS_OK on success, #I2CBUS_E_INIT if not initialized,
 *         #I2CBUS_E_SELECT if the bus switch rejected the selection.
 */
i2cBus_err_t i2cBus_select(u8 channelMask);

/**
 * @brief Returns the PMBus controller handle (power devices).
 * @return Handle, or NULL before i2cBus_init().
 */
i2cHandle_t* i2cBus_pmbus(void);

/**
 * @brief Returns the SysCfg controller handle (clock generator, add-on modules).
 * @return Handle, or NULL before i2cBus_init().
 */
i2cHandle_t* i2cBus_sysCfg(void);

/**
 * @brief Returns the interrupt controller handle.
 *
 * For future users besides the I2C controllers.
 *
 * @return Handle, or NULL before i2cBus_init().
 */
intrHandle_t* i2cBus_intr(void);

#endif /* SRC_I2CBUS_H_ */
