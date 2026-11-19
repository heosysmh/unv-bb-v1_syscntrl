/*
 * ina745a.h
 *
 * Driver for the Texas Instruments INA745A/B 40V, 16-Bit, Precision I2C
 * Output Digital Power Monitor with integrated 800µΩ shunt (EZShunt).
 *
 * Concept:
 *  - The driver core is bus-agnostic: it only uses injected HAL callbacks
 *    (ina745a_i2c_write_f / ina745a_i2c_read_f), never a concrete I2C
 *    driver directly. This allows host-side unit testing with stubs
 *    (see tests/ina745a/) without any BSP/hardware dependency, exactly
 *    like the si5332 driver.
 *  - The HAL callbacks operate on whole logical registers (1/2/3/5 bytes),
 *    MSB first, matching the INA745x register map. Their signature is
 *    intentionally identical to drivers/i2c/i2cWrapper's
 *    i2c_writeRegMultipleBytes()/i2c_readRegMultipleBytes(), so they can be 
 *    passed directly to ina745a_initialize() via a function-pointer cast - 
 *    no adapter module needed (same pattern as si5332 with i2c_writeReg/i2c_readReg).
 *    The core itself never includes i2cWrapper.h or any BSP header.
 *  - The INA745x has no calibration register: the integrated shunt is
 *    factory-trimmed, all LSB sizes are fixed (see datasheet Table 8-1).
 *  - Register writes take effect immediately; unlike some other devices
 *    there is no READY/ACTIVE register-lock state machine to manage.
 *
 *  Created on: 15.11.2026
 *      Author: matthias
 */

#ifndef SRC_INA745A_H_
#define SRC_INA745A_H_

#include <type.h>

/* ------------------------------------------------------------------------- */
/* Status codes                                                              */
/* ------------------------------------------------------------------------- */
typedef enum {
	INA745A_OK         =  0,
	INA745A_E_PARAM    = -1,
	INA745A_E_I2C      = -2,
	INA745A_E_NOT_INIT = -3,
	INA745A_E_MANUF_ID = -4	/* MANUFACTURER_ID register != 0x5449 ("TI") */
} ina745a_err_t;

/* ------------------------------------------------------------------------- */
/* Low-level HAL                                                              */
/* ------------------------------------------------------------------------- */
/*
 * Writes/reads one logical register of 'len' bytes (1, 2, 3 or 5), MSB
 * first, at address 'reg_addr' of the 7-bit I2C target 'i2c_addr'.
 * 'ctx' is passed through unchanged (e.g. an i2cHandle_t*).
 *
 * This signature is deliberately identical to i2c_writeRegMultipleBytes()/
 * i2c_readRegMultipleBytes() in drivers/i2c/i2cWrapper.h, so they can be passed to
 * ina745a_initialize() directly via a function-pointer cast, e.g.:
 *   ina745a_initialize(addr, (ina745a_i2c_write_f)i2c_writeRegMultipleBytes,
 *                             (ina745a_i2c_read_f)i2c_readRegMultipleBytes, i2cHandle);
 * (same pattern already used for si5332 with i2c_writeReg/i2c_readReg).
 * No adapter module is required.
 */
typedef ina745a_err_t (*ina745a_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);
typedef ina745a_err_t (*ina745a_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);

typedef struct ina745a_t ina745a_t;

/* ------------------------------------------------------------------------- */
/* I2C address range (datasheet Table 6-2: 16 addresses from A0/A1 strapping) */
/* ------------------------------------------------------------------------- */
/*
 * The 7-bit I2C address is passed as a plain parameter to
 * ina745a_initialize() (determined by the A0/A1 pin strapping on the
 * board, see datasheet Table 6-2). The driver validates it against this
 * range (0x40..0x4F) and refuses to create a handle for addresses outside
 * of it.
 */
#define INA745A_ADDR_MIN	0x40u
#define INA745A_ADDR_MAX	0x4Fu

/* Expected content of the MANUFACTURER_ID register ("TI" in ASCII) */
#define INA745A_MANUFACTURER_ID	0x5449u

/* ------------------------------------------------------------------------- */
/* Fixed LSB / conversion factors (datasheet Table 8-1)                      */
/* ------------------------------------------------------------------------- */
#define INA745A_CURRENT_LSB_A		0.0012f		/* 1.2 mA/LSB */
#define INA745A_BUS_VOLTAGE_LSB_V	0.003125f	/* 3.125 mV/LSB */
#define INA745A_TEMP_LSB_C			0.125f		/* 125 m°C/LSB */
#define INA745A_POWER_LSB_W			0.00024f	/* 240 µW/LSB */
#define INA745A_ENERGY_LSB_J		0.00384		/* 3.84 mJ/LSB (double) */
#define INA745A_CHARGE_LSB_C		0.000075	/* 75 µC/LSB (double) */
#define INA745A_PWR_LIMIT_LSB_W		0.06144f	/* 61.44 mW/LSB (= 256 x Power-LSB) */

/* ------------------------------------------------------------------------- */
/* ADC configuration types (register field values match the datasheet)      */
/* ------------------------------------------------------------------------- */

/* ADC_CONFIG.MODE (bits 15-12), Table 7-4 */
typedef enum {
	INA745A_MODE_SHUTDOWN               = 0x0,
	INA745A_MODE_TRIG_BUSV              = 0x1,
	INA745A_MODE_TRIG_TEMP              = 0x4,
	INA745A_MODE_TRIG_TEMP_BUSV         = 0x5,
	INA745A_MODE_TRIG_TEMP_CURRENT      = 0x6,
	INA745A_MODE_TRIG_TEMP_CURRENT_BUSV = 0x7,
	INA745A_MODE_CONT_BUSV              = 0x9,
	INA745A_MODE_CONT_TEMP              = 0xC,
	INA745A_MODE_CONT_TEMP_BUSV         = 0xD,
	INA745A_MODE_CONT_TEMP_CURRENT      = 0xE,
	INA745A_MODE_CONT_TEMP_CURRENT_BUSV = 0xF
} ina745a_mode_t;

/* ADC_CONFIG.VBUSCT / VSENCT / TCT (3 bit each), Table 7-4 */
typedef enum {
	INA745A_CT_50US   = 0,
	INA745A_CT_84US   = 1,
	INA745A_CT_150US  = 2,
	INA745A_CT_280US  = 3,
	INA745A_CT_540US  = 4,
	INA745A_CT_1052US = 5,
	INA745A_CT_2074US = 6,
	INA745A_CT_4120US = 7
} ina745a_convtime_t;

/* ADC_CONFIG.AVG (3 bit), Table 7-4 */
typedef enum {
	INA745A_AVG_1    = 0,
	INA745A_AVG_4    = 1,
	INA745A_AVG_16   = 2,
	INA745A_AVG_64   = 3,
	INA745A_AVG_128  = 4,
	INA745A_AVG_256  = 5,
	INA745A_AVG_512  = 6,
	INA745A_AVG_1024 = 7
} ina745a_avg_t;

typedef struct {
	ina745a_mode_t     mode;
	ina745a_convtime_t vbusct;
	ina745a_convtime_t vsenct;
	ina745a_convtime_t tct;
	ina745a_avg_t      avg;
} ina745a_adc_config_t;

/* ------------------------------------------------------------------------- */
/* Alert / diagnostic types (DIAG_ALRT register, Table 7-11)                 */
/* ------------------------------------------------------------------------- */
typedef enum {
	INA745A_ALERT_ACTIVE_LOW  = 0,
	INA745A_ALERT_ACTIVE_HIGH = 1
} ina745a_alert_pol_t;

/* Writable DIAG_ALRT configuration bits (15-12) */
typedef struct {
	u8 alatch;		/* 1 = latched alert, 0 = transparent */
	u8 conv_ready_en;	/* CNVR: assert ALERT on conversion-ready */
	u8 slow_alert;		/* 1 = compare thresholds against the averaged value */
	ina745a_alert_pol_t polarity;
} ina745a_alert_cfg_t;

/* Read-only DIAG_ALRT status/diagnostic bits (11-0) */
typedef struct {
	u8 energy_overflow;	/* ENERGYOF */
	u8 charge_overflow;	/* CHARGEOF */
	u8 math_overflow;	/* MATHOF */
	u8 temp_over_limit;	/* TMPOL */
	u8 current_over_limit;	/* CURRENTOL */
	u8 current_under_limit;/* CURRENTUL */
	u8 bus_over_limit;	/* BUSOL */
	u8 bus_under_limit;	/* BUSUL */
	u8 power_over_limit;	/* POL */
	u8 conversion_ready;	/* CNVRF */
	u8 memory_ok;		/* MEMSTAT, 1 = normal operation */
} ina745a_diag_flags_t;

/* ------------------------------------------------------------------------- */
/* Measurement results                                                        */
/* ------------------------------------------------------------------------- */
typedef struct {
	/* Converted engineering values */
	float  bus_voltage_V;
	float  die_temp_C;
	float  current_A;
	float  power_W;
	double energy_J;
	double charge_C;

	/* Raw register values (for integer-only consumers, e.g. ISRs) */
	u16 vbus_raw;		/* VBUS,    16 bit, unsigned */
	s16 dietemp_raw;	/* DIETEMP, 12-bit field, sign-extended to 16 bit */
	s16 current_raw;	/* CURRENT, 16 bit, two's complement */
	u32 power_raw;		/* POWER,   24 bit, unsigned (in u32) */
	u64 energy_raw;		/* ENERGY,  40 bit, unsigned (in u64) */
	s64 charge_raw;		/* CHARGE,  40 bit, two's complement, sign-extended to 64 bit */
} ina745a_measurements_t;

/* ------------------------------------------------------------------------- */
/* API                                                                        */
/* ------------------------------------------------------------------------- */

/*
 * Creates the device handle for instance 'devId' (no I2C access),
 * analogous to tpsm8286_initialize(): 'devId' directly indexes the
 * static instance pool (size INA745A_NUM_INSTANCES, defined in
 * userDef.h), allowing multiple INA745A devices to be managed
 * simultaneously. 'i2c_addr' is the 7-bit I2C address determined by the
 * A0/A1 pin strapping on the board (datasheet Table 6-2) and is
 * validated against INA745A_ADDR_MIN..INA745A_ADDR_MAX. Returns NULL if
 * 'devId' is out of range, 'i2c_addr' is out of range, or 'w'/'r' is
 * NULL.
 */
ina745a_t* ina745a_initialize(u32 devId, u8 i2c_addr, ina745a_i2c_write_f w, ina745a_i2c_read_f r, void *ctx);

/*
 * Checks MANUFACTURER_ID (must read 0x5449, "TI") and applies the ADC
 * configuration. Fails hard with INA745A_E_MANUF_ID if the ID does not
 * match (wrong address / wrong device) - the driver stays unconfigured
 * in that case.
 */
ina745a_err_t ina745a_init(ina745a_t *pDev, const ina745a_adc_config_t *cfg);

/* Software reset (CONFIG.RST), equivalent to a power-on reset. */
ina745a_err_t ina745a_reset(ina745a_t *pDev);

/* Clears the ENERGY and CHARGE accumulator registers (CONFIG.RSTACC). */
ina745a_err_t ina745a_reset_accumulators(ina745a_t *pDev);

/* Sets the initial-conversion delay in steps of 2 ms (0 = no delay, 255 = 510 ms). */
ina745a_err_t ina745a_set_conversion_delay(ina745a_t *pDev, u8 steps_2ms);

/* Writes/reads the complete ADC_CONFIG register. */
ina745a_err_t ina745a_set_adc_config(ina745a_t *pDev, const ina745a_adc_config_t *cfg);
ina745a_err_t ina745a_get_adc_config(ina745a_t *pDev, ina745a_adc_config_t *cfg);

/* Changes only ADC_CONFIG.MODE (read-modify-write, conversion times/AVG are preserved). */
ina745a_err_t ina745a_set_mode(ina745a_t *pDev, ina745a_mode_t mode);

/* Measurement getters (converted engineering units). */
ina745a_err_t ina745a_get_bus_voltage(ina745a_t *pDev, float *volts);
ina745a_err_t ina745a_get_die_temperature(ina745a_t *pDev, float *celsius);
ina745a_err_t ina745a_get_current(ina745a_t *pDev, float *amps);
ina745a_err_t ina745a_get_power(ina745a_t *pDev, float *watts);
ina745a_err_t ina745a_get_energy(ina745a_t *pDev, double *joules);
ina745a_err_t ina745a_get_charge(ina745a_t *pDev, double *coulombs);

/* Reads all measurement registers (VBUS, DIETEMP, CURRENT, POWER, ENERGY, CHARGE). */
ina745a_err_t ina745a_get_measurements(ina745a_t *pDev, ina745a_measurements_t *out);

/* Alert / threshold configuration. */
ina745a_err_t ina745a_set_current_limits(ina745a_t *pDev, float over_limit_A, float under_limit_A);
ina745a_err_t ina745a_set_bus_voltage_limits(ina745a_t *pDev, float overvoltage_V, float undervoltage_V);
ina745a_err_t ina745a_set_temperature_limit(ina745a_t *pDev, float overtemp_C);
ina745a_err_t ina745a_set_power_limit(ina745a_t *pDev, float overpower_W);

/*
 * Writes the DIAG_ALRT configuration bits (ALATCH/CNVR/SLOWALERT/APOL).
 * Note: writing this register also clears any latched alert flags
 * (same documented side effect as reading it while ALATCH = 1).
 */
ina745a_err_t ina745a_configure_alert(ina745a_t *pDev, const ina745a_alert_cfg_t *cfg);

/*
 * Reads and decodes the DIAG_ALRT register. Note: if ALATCH = 1, reading
 * this register clears the latched alert flags (documented device
 * behaviour).
 */
ina745a_err_t ina745a_get_diag_flags(ina745a_t *pDev, ina745a_diag_flags_t *out);

/* Reads the MANUFACTURER_ID register (expected: INA745A_MANUFACTURER_ID). */
ina745a_err_t ina745a_get_manufacturer_id(ina745a_t *pDev, u16 *id);

#endif /* SRC_INA745A_H_ */
