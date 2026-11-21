/*******************************************************************************
*
* @file ina745a.h
*
* @brief Driver for the Texas Instruments INA745A/B 40V, 16-Bit, Precision I2C
*        Output Digital Power Monitor with integrated 800µΩ shunt (EZShunt).
*
* @details
* Concept:
*  - The driver core is bus-agnostic: it only uses injected HAL callbacks
*    (#ina745a_i2c_write_f / #ina745a_i2c_read_f), never a concrete I2C
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
*
*******************************************************************************/

#ifndef SRC_INA745A_H_
#define SRC_INA745A_H_

#include <type.h>

/* ------------------------------------------------------------------------- */
/* Status codes                                                              */
/* ------------------------------------------------------------------------- */
/** Status codes of the power monitor driver. */
typedef enum {
	INA745A_OK         =  0,	/**< Success. */
	INA745A_E_PARAM    = -1,	/**< Invalid argument (NULL pointer or out-of-range field). */
	INA745A_E_I2C      = -2,	/**< HAL/bus error. */
	INA745A_E_NOT_INIT = -3,	/**< Handle not created / ina745a_init() not completed. */
	INA745A_E_MANUF_ID = -4		/**< MANUFACTURER_ID register != 0x5449 ("TI"). */
} ina745a_err_t;

/* ------------------------------------------------------------------------- */
/* Low-level HAL                                                              */
/* ------------------------------------------------------------------------- */
/**
 * @name Low-level HAL callbacks
 *
 * Writes/reads one logical register of @p len bytes (1, 2, 3 or 5), MSB
 * first, at address @p reg_addr of the 7-bit I2C target @p i2c_addr.
 * @p ctx is passed through unchanged (e.g. an i2cHandle_t*).
 *
 * This signature is deliberately identical to i2c_writeRegMultipleBytes()/
 * i2c_readRegMultipleBytes() in drivers/i2c/i2cWrapper.h, so they can be passed to
 * ina745a_initialize() directly via a function-pointer cast, e.g.:
 * @code
 *   ina745a_initialize(addr, (ina745a_i2c_write_f)i2c_writeRegMultipleBytes,
 *                             (ina745a_i2c_read_f)i2c_readRegMultipleBytes, i2cHandle);
 * @endcode
 * (same pattern already used for si5332 with i2c_writeReg/i2c_readReg).
 * No adapter module is required.
 * @{
 */
typedef ina745a_err_t (*ina745a_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);
typedef ina745a_err_t (*ina745a_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);
/** @} */

/** Opaque device handle (defined in ina745a.c). */
typedef struct ina745a_t ina745a_t;

/* ------------------------------------------------------------------------- */
/* I2C address range (datasheet Table 6-2: 16 addresses from A0/A1 strapping) */
/* ------------------------------------------------------------------------- */
/**
 * @name I2C address range
 *
 * The 7-bit I2C address is passed as a plain parameter to
 * ina745a_initialize() (determined by the A0/A1 pin strapping on the
 * board, see datasheet Table 6-2). The driver validates it against this
 * range (0x40..0x4F) and refuses to create a handle for addresses outside
 * of it.
 * @{
 */
#define INA745A_ADDR_MIN	0x40u	/**< Lowest valid 7-bit I2C address. */
#define INA745A_ADDR_MAX	0x4Fu	/**< Highest valid 7-bit I2C address. */
/** @} */

/** Expected content of the MANUFACTURER_ID register ("TI" in ASCII). */
#define INA745A_MANUFACTURER_ID	0x5449u

/* ------------------------------------------------------------------------- */
/* Fixed LSB / conversion factors (datasheet Table 8-1)                      */
/* ------------------------------------------------------------------------- */
/**
 * @name Fixed LSB / conversion factors (datasheet Table 8-1)
 * @{
 */
#define INA745A_CURRENT_LSB_A		0.0012f		/**< 1.2 mA/LSB */
#define INA745A_BUS_VOLTAGE_LSB_V	0.003125f	/**< 3.125 mV/LSB */
#define INA745A_TEMP_LSB_C			0.125f		/**< 125 m°C/LSB */
#define INA745A_POWER_LSB_W			0.00024f	/**< 240 µW/LSB */
#define INA745A_ENERGY_LSB_J		0.00384		/**< 3.84 mJ/LSB (double) */
#define INA745A_CHARGE_LSB_C		0.000075	/**< 75 µC/LSB (double) */
#define INA745A_PWR_LIMIT_LSB_W		0.06144f	/**< 61.44 mW/LSB (= 256 x Power-LSB) */
/** @} */

/* ------------------------------------------------------------------------- */
/* ADC configuration types (register field values match the datasheet)      */
/* ------------------------------------------------------------------------- */

/** ADC_CONFIG.MODE (bits 15-12), Table 7-4. */
typedef enum {
	INA745A_MODE_SHUTDOWN               = 0x0,	/**< Shutdown. */
	INA745A_MODE_TRIG_BUSV              = 0x1,	/**< Triggered bus voltage. */
	INA745A_MODE_TRIG_TEMP              = 0x4,	/**< Triggered temperature. */
	INA745A_MODE_TRIG_TEMP_BUSV         = 0x5,	/**< Triggered temperature + bus voltage. */
	INA745A_MODE_TRIG_TEMP_CURRENT      = 0x6,	/**< Triggered temperature + current. */
	INA745A_MODE_TRIG_TEMP_CURRENT_BUSV = 0x7,	/**< Triggered temperature + current + bus voltage. */
	INA745A_MODE_CONT_BUSV              = 0x9,	/**< Continuous bus voltage. */
	INA745A_MODE_CONT_TEMP              = 0xC,	/**< Continuous temperature. */
	INA745A_MODE_CONT_TEMP_BUSV         = 0xD,	/**< Continuous temperature + bus voltage. */
	INA745A_MODE_CONT_TEMP_CURRENT      = 0xE,	/**< Continuous temperature + current. */
	INA745A_MODE_CONT_TEMP_CURRENT_BUSV = 0xF	/**< Continuous temperature + current + bus voltage. */
} ina745a_mode_t;

/** ADC_CONFIG.VBUSCT / VSENCT / TCT conversion times (3 bit each), Table 7-4. */
typedef enum {
	INA745A_CT_50US   = 0,	/**< 50 us conversion time. */
	INA745A_CT_84US   = 1,	/**< 84 us conversion time. */
	INA745A_CT_150US  = 2,	/**< 150 us conversion time. */
	INA745A_CT_280US  = 3,	/**< 280 us conversion time. */
	INA745A_CT_540US  = 4,	/**< 540 us conversion time. */
	INA745A_CT_1052US = 5,	/**< 1052 us conversion time. */
	INA745A_CT_2074US = 6,	/**< 2074 us conversion time. */
	INA745A_CT_4120US = 7	/**< 4120 us conversion time. */
} ina745a_convtime_t;

/** ADC_CONFIG.AVG averaging setting (3 bit), Table 7-4. */
typedef enum {
	INA745A_AVG_1    = 0,	/**< No averaging. */
	INA745A_AVG_4    = 1,	/**< 4 samples. */
	INA745A_AVG_16   = 2,	/**< 16 samples. */
	INA745A_AVG_64   = 3,	/**< 64 samples. */
	INA745A_AVG_128  = 4,	/**< 128 samples. */
	INA745A_AVG_256  = 5,	/**< 256 samples. */
	INA745A_AVG_512  = 6,	/**< 512 samples. */
	INA745A_AVG_1024 = 7	/**< 1024 samples. */
} ina745a_avg_t;

/** Complete ADC_CONFIG register content. */
typedef struct {
	ina745a_mode_t     mode;	/**< Conversion mode (MODE field). */
	ina745a_convtime_t vbusct;	/**< Bus voltage conversion time (VBUSCT field). */
	ina745a_convtime_t vsenct;	/**< Shunt current conversion time (VSENCT field). */
	ina745a_convtime_t tct;		/**< Temperature conversion time (TCT field). */
	ina745a_avg_t      avg;		/**< Averaging setting (AVG field). */
} ina745a_adc_config_t;

/* ------------------------------------------------------------------------- */
/* Alert / diagnostic types (DIAG_ALRT register, Table 7-11)                 */
/* ------------------------------------------------------------------------- */
/** ALERT pin polarity (DIAG_ALRT.APOL). */
typedef enum {
	INA745A_ALERT_ACTIVE_LOW  = 0,	/**< Open-drain, active low (default). */
	INA745A_ALERT_ACTIVE_HIGH = 1	/**< Active high. */
} ina745a_alert_pol_t;

/** Writable DIAG_ALRT configuration bits (15-12). */
typedef struct {
	u8 alatch;		/**< 1 = latched alert, 0 = transparent. */
	u8 conv_ready_en;	/**< CNVR: assert ALERT on conversion-ready. */
	u8 slow_alert;		/**< 1 = compare thresholds against the averaged value. */
	ina745a_alert_pol_t polarity;	/**< ALERT pin polarity (APOL). */
} ina745a_alert_cfg_t;

/** Read-only DIAG_ALRT status/diagnostic bits (11-0). */
typedef struct {
	u8 energy_overflow;	/**< ENERGYOF: energy accumulator overflow. */
	u8 charge_overflow;	/**< CHARGEOF: charge accumulator overflow. */
	u8 math_overflow;	/**< MATHOF: internal math overflow. */
	u8 temp_over_limit;	/**< TMPOL: temperature over limit. */
	u8 current_over_limit;	/**< CURRENTOL: current over limit. */
	u8 current_under_limit;	/**< CURRENTUL: current under limit. */
	u8 bus_over_limit;	/**< BUSOL: bus voltage over limit. */
	u8 bus_under_limit;	/**< BUSUL: bus voltage under limit. */
	u8 power_over_limit;	/**< POL: power over limit. */
	u8 conversion_ready;	/**< CNVRF: conversion ready. */
	u8 memory_ok;		/**< MEMSTAT, 1 = normal operation. */
} ina745a_diag_flags_t;

/* ------------------------------------------------------------------------- */
/* Measurement results                                                        */
/* ------------------------------------------------------------------------- */
/** Complete measurement snapshot (converted values and raw registers). */
typedef struct {
	/** @name Converted engineering values */
	/** @{ */
	float  bus_voltage_V;	/**< Bus voltage [V]. */
	float  die_temp_C;	/**< Die temperature [degC]. */
	float  current_A;	/**< Current [A]. */
	float  power_W;		/**< Power [W]. */
	double energy_J;	/**< Accumulated energy [J]. */
	double charge_C;	/**< Accumulated charge [C]. */
	/** @} */

	/** @name Raw register values (for integer-only consumers, e.g. ISRs) */
	/** @{ */
	u16 vbus_raw;		/**< VBUS,    16 bit, unsigned */
	s16 dietemp_raw;	/**< DIETEMP, 12-bit field, sign-extended to 16 bit */
	s16 current_raw;	/**< CURRENT, 16 bit, two's complement */
	u32 power_raw;		/**< POWER,   24 bit, unsigned (in u32) */
	u64 energy_raw;		/**< ENERGY,  40 bit, unsigned (in u64) */
	s64 charge_raw;		/**< CHARGE,  40 bit, two's complement, sign-extended to 64 bit */
	/** @} */
} ina745a_measurements_t;

/* ------------------------------------------------------------------------- */
/* API                                                                        */
/* ------------------------------------------------------------------------- */

/**
 * @brief Creates the device handle for instance @p devId (no I2C access).
 *
 * Analogous to tpsm8286_initialize(): @p devId directly indexes the
 * static instance pool (size #INA745A_NUM_INSTANCES, defined in
 * userDef.h), allowing multiple INA745A devices to be managed
 * simultaneously.
 *
 * @param devId    Instance index, 0 .. (#INA745A_NUM_INSTANCES - 1).
 * @param i2c_addr 7-bit I2C address determined by the A0/A1 pin strapping
 *                 on the board (datasheet Table 6-2), validated against
 *                 #INA745A_ADDR_MIN .. #INA745A_ADDR_MAX.
 * @param w        HAL register write callback, must not be NULL.
 * @param r        HAL register read callback, must not be NULL.
 * @param ctx      Opaque context pointer passed through to @p w and @p r.
 * @return Pointer to the device handle, or NULL if @p devId or @p i2c_addr
 *         is out of range, or @p w/@p r is NULL.
 */
ina745a_t* ina745a_initialize(u32 devId, u8 i2c_addr, ina745a_i2c_write_f w, ina745a_i2c_read_f r, void *ctx);

/**
 * @brief Verifies MANUFACTURER_ID and applies the ADC configuration.
 *
 * Fails hard with #INA745A_E_MANUF_ID if the ID does not read 0x5449
 * ("TI") (wrong address / wrong device) - the driver stays unconfigured
 * in that case.
 *
 * @param pDev Device handle from ina745a_initialize().
 * @param cfg  Desired ADC configuration.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_init(ina745a_t *pDev, const ina745a_adc_config_t *cfg);

/**
 * @brief Software reset (CONFIG.RST), equivalent to a power-on reset.
 * @param pDev Device handle.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_reset(ina745a_t *pDev);

/**
 * @brief Clears the ENERGY and CHARGE accumulator registers (CONFIG.RSTACC).
 * @param pDev Device handle.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_reset_accumulators(ina745a_t *pDev);

/**
 * @brief Sets the initial-conversion delay in steps of 2 ms.
 * @param pDev      Device handle.
 * @param steps_2ms Delay steps (0 = no delay, 255 = 510 ms).
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_set_conversion_delay(ina745a_t *pDev, u8 steps_2ms);

/**
 * @brief Writes the complete ADC_CONFIG register.
 * @param pDev Device handle.
 * @param cfg  Desired ADC configuration.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_set_adc_config(ina745a_t *pDev, const ina745a_adc_config_t *cfg);

/**
 * @brief Reads back the complete ADC_CONFIG register.
 * @param pDev     Device handle.
 * @param[out] cfg Receives the decoded configuration on success.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_adc_config(ina745a_t *pDev, ina745a_adc_config_t *cfg);

/**
 * @brief Changes only ADC_CONFIG.MODE (read-modify-write).
 *
 * Conversion times and AVG are preserved.
 *
 * @param pDev Device handle.
 * @param mode Desired conversion mode.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_set_mode(ina745a_t *pDev, ina745a_mode_t mode);

/**
 * @name Measurement getters (converted engineering units)
 * @{
 */

/**
 * @brief Reads the bus voltage.
 * @param pDev       Device handle.
 * @param[out] volts Receives the bus voltage in volts.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_bus_voltage(ina745a_t *pDev, float *volts);

/**
 * @brief Reads the die temperature.
 * @param pDev         Device handle.
 * @param[out] celsius Receives the die temperature in °C.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_die_temperature(ina745a_t *pDev, float *celsius);

/**
 * @brief Reads the current.
 * @param pDev      Device handle.
 * @param[out] amps Receives the current in amperes.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_current(ina745a_t *pDev, float *amps);

/**
 * @brief Reads the power.
 * @param pDev       Device handle.
 * @param[out] watts Receives the power in watts.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_power(ina745a_t *pDev, float *watts);

/**
 * @brief Reads the accumulated energy.
 * @param pDev        Device handle.
 * @param[out] joules Receives the energy in joules.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_energy(ina745a_t *pDev, double *joules);

/**
 * @brief Reads the accumulated charge.
 * @param pDev          Device handle.
 * @param[out] coulombs Receives the charge in coulombs.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_charge(ina745a_t *pDev, double *coulombs);
/** @} */

/**
 * @brief Reads all measurement registers (VBUS, DIETEMP, CURRENT, POWER,
 *        ENERGY, CHARGE).
 * @param pDev     Device handle.
 * @param[out] out Receives all measurement values (converted and raw).
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_measurements(ina745a_t *pDev, ina745a_measurements_t *out);

/**
 * @name Alert / threshold configuration
 * @{
 */

/**
 * @brief Writes the current over-/under-limit thresholds (COL/CUL).
 * @param pDev          Device handle.
 * @param over_limit_A  Overcurrent threshold in amperes.
 * @param under_limit_A Undercurrent threshold in amperes.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_set_current_limits(ina745a_t *pDev, float over_limit_A, float under_limit_A);

/**
 * @brief Writes the bus over-/under-voltage thresholds (BOVL/BUVL).
 * @param pDev           Device handle.
 * @param overvoltage_V  Overvoltage threshold in volts.
 * @param undervoltage_V Undervoltage threshold in volts.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_set_bus_voltage_limits(ina745a_t *pDev, float overvoltage_V, float undervoltage_V);

/**
 * @brief Writes the temperature over-limit threshold (TEMP_LIMIT).
 * @param pDev       Device handle.
 * @param overtemp_C Over-temperature threshold in °C.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_set_temperature_limit(ina745a_t *pDev, float overtemp_C);

/**
 * @brief Writes the power over-limit threshold (PWR_LIMIT).
 * @param pDev        Device handle.
 * @param overpower_W Over-power threshold in watts.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_set_power_limit(ina745a_t *pDev, float overpower_W);

/**
 * @brief Writes the DIAG_ALRT configuration bits (ALATCH/CNVR/SLOWALERT/APOL).
 *
 * @note Writing this register also clears any latched alert flags
 *       (same documented side effect as reading it while ALATCH = 1).
 *
 * @param pDev Device handle.
 * @param cfg  Desired alert configuration.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_configure_alert(ina745a_t *pDev, const ina745a_alert_cfg_t *cfg);

/**
 * @brief Reads and decodes the DIAG_ALRT register.
 *
 * @note If ALATCH = 1, reading this register clears the latched alert
 *       flags (documented device behaviour).
 *
 * @param pDev     Device handle.
 * @param[out] out Receives the decoded diagnostic flags on success.
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_diag_flags(ina745a_t *pDev, ina745a_diag_flags_t *out);
/** @} */

/**
 * @brief Reads the MANUFACTURER_ID register.
 * @param pDev   Device handle.
 * @param[out] id Receives the ID (expected: #INA745A_MANUFACTURER_ID).
 * @return #INA745A_OK on success, negative ina745a_err_t code on error.
 */
ina745a_err_t ina745a_get_manufacturer_id(ina745a_t *pDev, u16 *id);

#endif /* SRC_INA745A_H_ */
