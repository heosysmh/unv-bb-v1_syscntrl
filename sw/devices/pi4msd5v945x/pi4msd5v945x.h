/*
 * si5332.h
 *
 *  Created on: 22.09.2026
 *      Author: matthias
 */

#include <type.h>

#ifndef SRC_PI4MSD5V945X_H_
#define SRC_PI4MSD5V945X_H_

// Channel definitions for device that support any channel combination
#define PI4MSD5V945X_CH0	0x1u
#define PI4MSD5V945X_CH1	0x2u
#define PI4MSD5V945X_CH2	0x4u
#define PI4MSD5V945X_CH3	0x8u

typedef enum {
	PI4MSD5V945X_OK = 0,
	PI4MSD5V945X_E_PARAM = -1,
	PI4MSD5V945X_E_I2C = -2
} pi4msd5v945x_err_t;

/* Low-Level HAL: to be provided by BSP */
typedef pi4msd5v945x_err_t (*pi4msd5v945x_i2c_write_f)(void *ctx, u8 i2c_addr, const u8 *data, u32 length);
typedef pi4msd5v945x_err_t (*pi4msd5v945x_i2c_read_f) (void *ctx, u8 i2c_addr,       u8 *data, u32 length);

typedef struct pi4msd5v945x_t pi4msd5v945x_t;

pi4msd5v945x_t* pi4msd5v945x_initialize(u8 i2c_addr, pi4msd5v945x_i2c_write_f w, pi4msd5v945x_i2c_read_f r, void *ctx);
pi4msd5v945x_err_t pi4msd5v945x_setChannels(pi4msd5v945x_t *pDev, u8 channels);

#endif /* SRC_PI4MSD5V945X_H_ */
