/*
 * si5332.h
 *
 *  Created on: 22.09.2026
 *      Author: matthias
 */

#include <type.h>

#ifndef SRC_SI5332_H_
#define SRC_SI5332_H_




typedef enum {
	SI5332_OK = 0,
	SI5332_E_PARAM = -1,
	SI5332_E_I2C = -2
} si5332_err_t;

/* Low-Level HAL: to be provided by BSP */
typedef si5332_err_t (*si5332_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
typedef si5332_err_t (*si5332_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr,       u8 *data);

typedef struct si5332_t si5332_t;

si5332_t* si5332_initialize(u8 i2c_addr, si5332_i2c_write_f w, si5332_i2c_read_f r, void *ctx);
si5332_err_t si5332_apply_config(si5332_t *pDev);

#endif /* SRC_SI5332_H_ */
