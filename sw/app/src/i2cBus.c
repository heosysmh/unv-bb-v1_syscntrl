/*******************************************************************************
*
* @file i2cBus.c
*
* @brief Implementation of the I2C infrastructure ownership, see i2cBus.h.
*
*******************************************************************************/

#include "i2cBus.h"

#include "bsp.h"
#include "boardConfig.h"

static intrHandle_t   *gIntr     = 0;
static i2cHandle_t    *gPmbus    = 0;
static i2cHandle_t    *gSysCfg   = 0;
static pi4msd5v945x_t *gBusSwitch = 0;

/**
 * @brief Fills a controller configuration from the board definition.
 */
static void i2cBus_buildConfig(i2cHandle_config_t *cfg, u32 base, u8 devId,
                               u8 intrId, u8 intrPrio, u8 intrHart,
                               u32 coreFreq, u32 ifFreq)
{
	cfg->baseAddress = base;
	cfg->deviceId    = devId;
	cfg->intrId      = intrId;
	cfg->intrPrio    = intrPrio;
	cfg->intrHart    = intrHart;

	cfg->cfg.samplingClockDivider = BOARD_I2C_SAMPLING_DIVIDER;
	cfg->cfg.timeout = coreFreq / BOARD_I2C_TIMEOUT_DIVIDER;	/* 0.1 s */
	cfg->cfg.tsuDat  = coreFreq / ifFreq / 3;			/* 3.33 us */
	cfg->cfg.tLow    = coreFreq / ifFreq / 2;			/* 5 us */
	cfg->cfg.tHigh   = coreFreq / ifFreq / 2;			/* 5 us */
	cfg->cfg.tBuf    = coreFreq / ifFreq;				/* 10 us */
}

i2cBus_err_t i2cBus_init(void)
{
	i2cHandle_config_t cfg;

	gIntr = intr_initialize(BOARD_PLIC_BASE);
	if (!gIntr)
		return I2CBUS_E_INIT;

	intr_setThreshold(gIntr, 0, 0);

	/* I2C for power management (DC/DC converters and power monitors) */
	i2cBus_buildConfig(&cfg,
	                   BOARD_I2C_PMBUS_BASE, BOARD_I2C_PMBUS_DEV_ID,
	                   BOARD_I2C_PMBUS_INTR_ID, BOARD_I2C_PMBUS_INTR_PRIO,
	                   BOARD_I2C_PMBUS_INTR_HART,
	                   BOARD_I2C_PMBUS_CORE_FREQ, BOARD_I2C_PMBUS_IF_FREQ);

	gPmbus = i2c_initialize(&cfg, gIntr);
	if (!gPmbus)
		return I2CBUS_E_INIT;

	/* I2C for system configuration (clock generator, add-on modules) */
	i2cBus_buildConfig(&cfg,
	                   BOARD_I2C_SYSCFG_BASE, BOARD_I2C_SYSCFG_DEV_ID,
	                   BOARD_I2C_SYSCFG_INTR_ID, BOARD_I2C_SYSCFG_INTR_PRIO,
	                   BOARD_I2C_SYSCFG_INTR_HART,
	                   BOARD_I2C_SYSCFG_CORE_FREQ, BOARD_I2C_SYSCFG_IF_FREQ);

	gSysCfg = i2c_initialize(&cfg, gIntr);
	if (!gSysCfg)
		return I2CBUS_E_INIT;

	/* Bus switch that segments the SysCfg bus */
	gBusSwitch = pi4msd5v945x_initialize(BOARD_BUS_SWITCH_I2C_ADDR,
	                                    (pi4msd5v945x_i2c_write_f)i2c_writeDirect,
	                                    (pi4msd5v945x_i2c_read_f)i2c_readDirect,
	                                    (void*)gSysCfg);
	if (!gBusSwitch)
		return I2CBUS_E_INIT;

	return I2CBUS_OK;
}

i2cBus_err_t i2cBus_select(u8 channelMask)
{
	if (!gBusSwitch)
		return I2CBUS_E_INIT;

	if (pi4msd5v945x_setChannels(gBusSwitch, channelMask) != PI4MSD5V945X_OK)
		return I2CBUS_E_SELECT;

	return I2CBUS_OK;
}

i2cHandle_t* i2cBus_pmbus(void)
{
	return gPmbus;
}

i2cHandle_t* i2cBus_sysCfg(void)
{
	return gSysCfg;
}

intrHandle_t* i2cBus_intr(void)
{
	return gIntr;
}
