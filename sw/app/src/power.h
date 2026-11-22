
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file power.h
* @brief Power functions of the board: FMC Vadj regulators (tpsm8286x) and
*        the power monitors (ina745a).
* @details
*        Rails, I2C addresses, power-good groups and protection limits are
*        defined by #PWR_MON_TABLE below; the rail enum is generated from it.
*
*        Protection concept:
*          - Over-voltage, over-current and over-temperature limits are armed
*            with a latched alert (ALATCH = 1). Under-voltage/under-current
*            protection is not used on this board.
*          - The ALERT pins are wired-OR onto the power-good inputs, so a
*            fault appears as a power-good line going low. pwr_checkAlerts()
*            reads those lines first (no bus traffic) and only queries the
*            devices of an affected group over I2C.
*          - Reading DIAG_ALRT clears the latch in the device, therefore the
*            module keeps a sticky fault shadow (see pwr_mon_getFaults()).
*/

#ifndef SRC_POWER_H_
#define SRC_POWER_H_

#include <type.h>

#include "boardConfig.h"

/* =========================================================================== */
/* 5. Power monitors: per-rail configuration                                   */
/* =========================================================================== */
/**
 * @brief Rail configuration table (X-macro).
 *
 * One row per monitored rail. This table is the only place where rails are
 * defined; power.h/power.c derive the rail enum, the name strings, the I2C
 * addresses, the power-good group and the protection limits from it.
 *
 * Columns:
 *   - name: identifier suffix, also used as display name
 *   - addr: 7-bit I2C address on the PMBus (A0/A1 strapping)
 *   - pg:   power-good / ALERT net the device is wired to (#pwrPgLine_t)
 *   - vov:  bus over-voltage limit [V]
 *   - iov:  over-current limit [A]
 *   - tov:  over-temperature limit [degC]
 *
 * @note The number of rows must match #INA745A_NUM_INSTANCES in userDef.h.
 */
/*	  name,        addr, 			  		pg,						  				  vov,   iov,   tov */
#define PWR_MON_TABLE \
	X(12V,         I2CBUS_PWRMON_12V_ADDR,	PWR_PG_SYS,							   13.20f,  5.0f, 95.0f) \
	X(3V3,         I2CBUS_PWRMON_3V3_ADDR,	PWR_PG_SYS,								3.60f, 12.0f, 95.0f) \
	X(5V0_SOM,     I2CBUS_PWRMON_5V0_ADDR, 	PWR_PG_SYS,								5.50f,  5.0f, 95.0f) \
	X(1V8,         I2CBUS_PWRMON_1V8_ADDR,	PWR_PG_SYS,     						1.98f,  5.0f, 95.0f) \
	X(VCCADJ_FMC1, I2CBUS_PWRMON_FMC1_ADDR, PWR_PG_FMC1_SUP,  (BOARD_FMC_VADJ_MAX_V+0.2f),  4.5f, 95.0f) \
	X(VCCADJ_FMC2, I2CBUS_PWRMON_FMC2_ADDR, PWR_PG_FMC2_SUP,  (BOARD_FMC_VADJ_MAX_V+0.2f),  4.5f, 95.0f)

/* --------------------------------------------------------------------------- */
/* Status codes                                                                 */
/* --------------------------------------------------------------------------- */
/** Status codes of the power function group. */
typedef enum {
	PWR_OK          =  0,	/**< Success. */
	PWR_E_PARAM     = -1,	/**< Invalid argument. */
	PWR_E_INIT      = -2,	/**< Handle could not be created / module not initialized. */
	PWR_E_DEVICE    = -3,	/**< Device reported an error (I2C, ID mismatch, ...). */
	PWR_E_RANGE     = -4,	/**< Configured voltage outside the allowed range. */
	PWR_E_REGULATION= -5	/**< Regulator does not report a valid output voltage. */
} pwr_err_t;

/* --------------------------------------------------------------------------- */
/* Power-good / ALERT nets                                                      */
/* --------------------------------------------------------------------------- */
/** Power-good / ALERT nets the monitors are wired-OR onto. */
typedef enum {
	PWR_PG_SYS = 0,		/**< System power-good net (SYS_PG). */
	PWR_PG_FMC1_SUP,	/**< FMC1 supply power-good net (FMC1_SUP_PG). */
	PWR_PG_FMC2_SUP,	/**< FMC2 supply power-good net (FMC2_SUP_PG). */
	PWR_PG_COUNT		/**< Number of power-good nets. */
} pwrPgLine_t;

/* --------------------------------------------------------------------------- */
/* Monitored rails (generated from PWR_MON_TABLE)                               */
/* --------------------------------------------------------------------------- */
/** Monitored rails, generated from #PWR_MON_TABLE (one PWR_RAIL_<name> per row). */
#define X(name, addr, pg, vov, iov, tov)	PWR_RAIL_##name,
typedef enum {
	PWR_MON_TABLE
	PWR_RAIL_COUNT		/**< Number of monitored rails. */
} pwrRail_t;
#undef X

/* --------------------------------------------------------------------------- */
/* FMC Vadj regulators                                                          */
/* --------------------------------------------------------------------------- */
/** FMC Vadj regulator identifiers. */
typedef enum {
	PWR_FMC1 = 0,		/**< FMC1 Vadj regulator. */
	PWR_FMC2,			/**< FMC2 Vadj regulator. */
	PWR_FMC_COUNT		/**< Number of FMC Vadj regulators. */
} pwrFmc_t;

/* --------------------------------------------------------------------------- */
/* Measurements and faults                                                      */
/* --------------------------------------------------------------------------- */
/** Measurement snapshot of one rail (converted engineering units). */
typedef struct {
	float bus_voltage_V;	/**< Bus voltage [V]. */
	float current_A;		/**< Current [A]. */
	float die_temp_C;		/**< Monitor die temperature [degC]. */
} pwrMon_values_t;

/** Sticky fault flags of one rail (accumulated since the last clear). */
typedef struct {
	u8 over_voltage;	/**< BUSOL: bus over-voltage. */
	u8 over_current;	/**< CURRENTOL: over-current. */
	u8 over_temp;		/**< TMPOL: over-temperature. */
	u8 over_power;		/**< POL: over-power. */
	u8 memory_fail;		/**< MEMSTAT reported a checksum error. */
} pwrMon_fault_t;

/* --------------------------------------------------------------------------- */
/* API                                                                          */
/* --------------------------------------------------------------------------- */

/**
 * @brief Creates all device handles (regulators and monitors).
 *
 * No bus traffic, no device configuration. Requires i2cBus_init() to have
 * succeeded.
 *
 * @return #PWR_OK on success, #PWR_E_INIT if a handle could not be created.
 */
pwr_err_t pwr_init(void);

/**
 * @name FMC Vadj regulators
 * @{
 */

/**
 * @brief Writes control register and voltage setpoint of both FMC Vadj regulators.
 *
 * Outputs stay disabled. The setpoints are taken from #BOARD_FMC1_VADJ_V /
 * #BOARD_FMC2_VADJ_V.
 *
 * @return #PWR_OK on success, #PWR_E_INIT if not initialized,
 *         #PWR_E_RANGE if a configured setpoint exceeds #BOARD_FMC_VADJ_MAX_V,
 *         #PWR_E_DEVICE on a device error.
 */
pwr_err_t pwr_fmcVadj_configure(void);

/**
 * @brief Checks whether the regulator reports a valid output voltage setting.
 * @param fmc Regulator to check.
 * @return #PWR_OK if the voltage register reads back the programmed value,
 *         #PWR_E_PARAM if @p fmc is invalid, #PWR_E_INIT if not initialized,
 *         #PWR_E_DEVICE on a device error, #PWR_E_REGULATION on a mismatch.
 */
pwr_err_t pwr_fmcVadj_isRegulating(pwrFmc_t fmc);

/**
 * @brief Enables/disables both FMC Vadj regulator outputs.
 * @param enable Non-zero to enable, zero to disable.
 * @return #PWR_OK on success, #PWR_E_INIT if not initialized,
 *         #PWR_E_DEVICE on a device error.
 */
pwr_err_t pwr_fmcVadj_enable(int enable);
/** @} */

/**
 * @name Power monitors
 * @{
 */

/**
 * @brief Configures and arms all monitors.
 *
 * ADC configuration, over-voltage, over-current and over-temperature limits,
 * latched alert, and a clearing read of DIAG_ALRT.
 *
 * @note Call order matters: the ALERT outputs sit on the power-good nets, so
 *       this must run after boardGpio_waitSomReady() (a latched alert would
 *       otherwise keep SYS_PG low) and before the FMC supply is switched on.
 *
 * @return #PWR_OK if all monitors were configured, #PWR_E_INIT if not
 *         initialized, #PWR_E_DEVICE if at least one monitor failed
 *         (the remaining monitors are still configured).
 */
pwr_err_t pwr_mon_configure(void);

/**
 * @brief Polls bus voltage, current and die temperature of a single rail.
 * @param rail     Rail to read.
 * @param[out] values Receives the measurement snapshot on success.
 * @return #PWR_OK on success, #PWR_E_PARAM if @p values is NULL or @p rail
 *         is invalid, #PWR_E_INIT if not initialized/configured,
 *         #PWR_E_DEVICE on a device error.
 */
pwr_err_t pwr_mon_read(pwrRail_t rail, pwrMon_values_t *values);

/**
 * @brief Polls all rails and prints the measured values.
 * @return #PWR_OK if all rails were read, otherwise the last error code.
 */
pwr_err_t pwr_mon_poll(void);

/**
 * @brief Checks the power-good / ALERT lines.
 *
 * Only if a line is low, the devices of that group are queried over I2C;
 * faults found are merged into the sticky fault shadow. If a line is low
 * but no device reports a fault, the supply itself failed and
 * #PWR_E_REGULATION is returned.
 *
 * @return #PWR_OK if all lines are high, #PWR_E_INIT if the monitors are not
 *         configured, #PWR_E_REGULATION if a line is low,
 *         #PWR_E_DEVICE if a monitor query failed.
 */
pwr_err_t pwr_checkAlerts(void);

/**
 * @brief Reads the sticky fault shadow of a rail.
 *
 * Accumulated over all queries since the last clear.
 *
 * @param rail     Rail to query.
 * @param[out] faults Receives the accumulated fault flags.
 * @return #PWR_OK on success, #PWR_E_PARAM if @p faults is NULL or @p rail
 *         is invalid.
 */
pwr_err_t pwr_mon_getFaults(pwrRail_t rail, pwrMon_fault_t *faults);

/**
 * @brief Clears the sticky fault shadow of a rail.
 * @param rail Rail to clear (invalid values are ignored).
 */
void pwr_mon_clearFaults(pwrRail_t rail);

/**
 * @brief Prints all accumulated faults of all rails.
 */
void pwr_mon_reportFaults(void);

/**
 * @brief Returns the display name of a rail, e.g. "VCCADJ_FMC1".
 * @param rail Rail to name.
 * @return Name string, or "?" if @p rail is invalid.
 */
const char* pwr_railName(pwrRail_t rail);
/** @} */

#endif /* SRC_POWER_H_ */
