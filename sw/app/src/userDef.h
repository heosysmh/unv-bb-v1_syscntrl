/*******************************************************************************
*
* @file userDef.h
*
* @brief Instance-pool sizes for the drivers and device drivers used by this
*        application.
*
*        This header is included by BSP drivers (which hard-code the name
*        "userDef.h") and by the device drivers in devices/. It is therefore
*        deliberately kept free of any BSP or board includes so that the
*        device drivers stay host-testable (see tests/).
*
*        The board itself is configured in boardConfig.h. The counts below
*        must match the board configuration; power.c contains compile-time
*        checks that enforce this.
*
*******************************************************************************/

#ifndef SRC_USERDEF_H_
#define SRC_USERDEF_H_

/* drivers/i2c/i2cWrapper.c: number of I2C controller instances (PMBus, SysCfg) */
#define I2C_NUM_OF_INSTANCES		2

/* devices/tpsm8286x: FMC1_VADJ, FMC2_VADJ */
#define TPSM8286_NUM_OF_INSTANCES	2

/* devices/ina745a: one instance per monitored rail (see BOARD_PWRMON_TABLE) */
#define INA745A_NUM_INSTANCES		6

#endif /* SRC_USERDEF_H_ */
