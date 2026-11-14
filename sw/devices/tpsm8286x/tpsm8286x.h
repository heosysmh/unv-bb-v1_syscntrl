/*
 * tpsm8286.h
 *
 *  Created on: 13.09.2026
 *      Author: matthias
 */

#include <type.h>

#ifndef SRC_TPSM8286_H_
#define SRC_TPSM8286_H_

#define TPSM8286_VOUT1_REG_ADDR		0x01
#define TPSM8286_VOUT2_REG_ADDR		0x02
#define TPSM8286_CNTRL_REG_ADDR		0x03
#define TPSM8286_STATUS_REG_ADDR	0x05

#define	TPSM8286_VOLTAGE_RAMP_SPEED_MASK	0x03
#define	TPSM8286_VOLTAGE_RAMP_SPEED_20MV	0x00
#define	TPSM8286_VOLTAGE_RAMP_SPEED_10MV	0x01
#define	TPSM8286_VOLTAGE_RAMP_SPEED_5MV		0x10
#define	TPSM8286_VOLTAGE_RAMP_SPEED_1MV		0x11

#define TPSM8286_HICCUP_EN_MASK			0x04
#define TPSM8286_OUTPUT_DISCHARGE_MASK	0x08
#define TPSM8286_FPWM_MASK				0x10
#define TPSM8286_DEVICE_ENABLE_MASK		0x20
#define TPSM8286_FPWM_AT_VC_MASK		0x40
#define TPSM8286_RESET_REGS_MASK		0x80

#define TPSM8286_STAT_UVLO_MASK			0x01
#define TPSM8286_STAT_HICCUP_MASK		0x04
#define TPSM8286_STAT_TEMPWARM_MASK		0x10

#define TPSM8286_XXX3_V_TO_REG


typedef enum {
	TPSM8286_OK = 0,
	TPSM8286_E_PARAM = -1,
	TPSM8286_E_I2C = -2
} tpsm8286_err_t;

/* Low-Level HAL: to be provided by BSP */
typedef tpsm8286_err_t (*tpsm8286_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
typedef tpsm8286_err_t (*tpsm8286_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr,       u8 *data);

typedef struct tpsm8286_t tpsm8286_t;

tpsm8286_t* tpsm8286_initialize(u32 devId, u8 i2c_addr, tpsm8286_i2c_write_f w, tpsm8286_i2c_read_f r, void *ctx);
tpsm8286_err_t tpsm8286_set_voltage(tpsm8286_t *dev, float voltage);
tpsm8286_err_t tpsm8286_set_cntrl(tpsm8286_t *pInst, u8 regVal);
tpsm8286_err_t tpsm8286_set_enable(tpsm8286_t *pInst, int enable);
tpsm8286_err_t tpsm8286_check_voltageReg(tpsm8286_t *pInst, int *regOk);

#endif /* SR_TPSM8286_H_ */
