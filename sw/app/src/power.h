/*******************************************************************************
*
* @file power.h
*
* @brief Power functions of the board: FMC Vadj regulators (tpsm8286x) and
*        the power monitors (ina745a).
*
*        Rails, I2C addresses, power-good groups and protection limits are
*        defined by BOARD_PWRMON_TABLE in boardConfig.h; the enum below is
*        generated from it.
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
*
*******************************************************************************/

#ifndef SRC_POWER_H_
#define SRC_POWER_H_

#include <type.h>

#include "boardConfig.h"

/* =========================================================================== */
/* 5. Power monitors: per-rail configuration                                   */
/* =========================================================================== */
/*
 * One row per monitored rail. This table is the only place where rails are
 * defined; power.h/power.c derive the rail enum, the name strings, the I2C
 * addresses, the power-good group and the protection limits from it.
 *
 * Columns:
 *   name  identifier suffix, also used as display name
 *   addr  7-bit I2C address on the PMBus (A0/A1 strapping)
 *   pg    power-good / ALERT net the device is wired to (pwrPgLine_t)
 *   vov   bus over-voltage limit [V]
 *   iov   over-current limit [A]
 *   tov   over-temperature limit [degC]
 *
 * The number of rows must match INA745A_NUM_INSTANCES in userDef.h.
 */
/*	  name,        addr, 			  pg,						  vov,   iov,   tov */
#define PWR_MON_TABLE \
	X(12V,         0x40,      PWR_PG_SYS,					   13.20f,  5.0f, 95.0f) \
	X(3V3,         0x41,      PWR_PG_SYS,						3.60f, 12.0f, 95.0f) \
	X(5V0_SOM,     0x43,      PWR_PG_SYS,						5.50f,  5.0f, 95.0f) \
	X(1V8,         0x44, 	  PWR_PG_SYS,     					1.98f,  5.0f, 95.0f) \
	X(VCCADJ_FMC1, 0x4C, PWR_PG_FMC1_SUP, (BOARD_FMC_VADJ_MAX_V+0.2f),  4.5f, 95.0f) \
	X(VCCADJ_FMC2, 0x4D, PWR_PG_FMC2_SUP, (BOARD_FMC_VADJ_MAX_V+0.2f),  4.5f, 95.0f)

/* --------------------------------------------------------------------------- */
/* Status codes                                                                 */
/* --------------------------------------------------------------------------- */
typedef enum {
	PWR_OK          =  0,
	PWR_E_PARAM     = -1,	/* invalid argument */
	PWR_E_INIT      = -2,	/* handle could not be created / module not initialized */
	PWR_E_DEVICE    = -3,	/* device reported an error (I2C, ID mismatch, ...) */
	PWR_E_RANGE     = -4,	/* configured voltage outside the allowed range */
	PWR_E_REGULATION= -5	/* regulator does not report a valid output voltage */
} pwr_err_t;

/* --------------------------------------------------------------------------- */
/* Power-good / ALERT nets                                                      */
/* --------------------------------------------------------------------------- */
typedef enum {
	PWR_PG_SYS = 0,
	PWR_PG_FMC1_SUP,
	PWR_PG_FMC2_SUP,
	PWR_PG_COUNT
} pwrPgLine_t;

/* --------------------------------------------------------------------------- */
/* Monitored rails (generated from BOARD_PWRMON_TABLE)                          */
/* --------------------------------------------------------------------------- */
#define X(name, addr, pg, vov, iov, tov)	PWR_RAIL_##name,
typedef enum {
	PWR_MON_TABLE
	PWR_RAIL_COUNT
} pwrRail_t;
#undef X

/* --------------------------------------------------------------------------- */
/* FMC Vadj regulators                                                          */
/* --------------------------------------------------------------------------- */
typedef enum {
	PWR_FMC1 = 0,
	PWR_FMC2,
	PWR_FMC_COUNT
} pwrFmc_t;

/* --------------------------------------------------------------------------- */
/* Measurements and faults                                                      */
/* --------------------------------------------------------------------------- */
typedef struct {
	float bus_voltage_V;
	float current_A;
	float die_temp_C;
} pwrMon_values_t;

typedef struct {
	u8 over_voltage;	/* BUSOL  */
	u8 over_current;	/* CURRENTOL */
	u8 over_temp;		/* TMPOL  */
	u8 over_power;		/* POL    */
	u8 memory_fail;		/* MEMSTAT reported a checksum error */
} pwrMon_fault_t;

/* --------------------------------------------------------------------------- */
/* API                                                                          */
/* --------------------------------------------------------------------------- */

/*
 * Creates all device handles (regulators and monitors). No bus traffic, no
 * device configuration. Requires i2cBus_init() to have succeeded.
 */
pwr_err_t pwr_init(void);

/* --- FMC Vadj ------------------------------------------------------------- */

/*
 * Writes the control register and the voltage setpoint of both FMC Vadj
 * regulators (outputs stay disabled). Returns PWR_E_RANGE if a configured
 * setpoint exceeds BOARD_FMC_VADJ_MAX_V.
 */
pwr_err_t pwr_fmcVadj_configure(void);

/* Checks whether the regulator reports a valid output voltage setting. */
pwr_err_t pwr_fmcVadj_isRegulating(pwrFmc_t fmc);

/* Enables/disables both FMC Vadj regulator outputs. */
pwr_err_t pwr_fmcVadj_enable(int enable);

/* --- Power monitors -------------------------------------------------------- */

/*
 * Configures and arms all monitors: ADC configuration, over-voltage,
 * over-current and over-temperature limits, latched alert, and a clearing
 * read of DIAG_ALRT.
 *
 * Call order matters: the ALERT outputs sit on the power-good nets, so this
 * must run after boardGpio_waitSomReady() (a latched alert would otherwise
 * keep SYS_PG low) and before the FMC supply is switched on.
 */
pwr_err_t pwr_mon_configure(void);

/* Polls bus voltage, current and die temperature of a single rail. */
pwr_err_t pwr_mon_read(pwrRail_t rail, pwrMon_values_t *values);

/* Polls all rails and prints the measured values. */
pwr_err_t pwr_mon_poll(void);

/*
 * Checks the power-good / ALERT lines. Only if a line is low, the devices of
 * that group are queried over I2C; faults found are merged into the sticky
 * fault shadow. If a line is low but no device reports a fault, the supply
 * itself failed and PWR_E_REGULATION is returned.
 */
pwr_err_t pwr_checkAlerts(void);

/* Sticky fault shadow (accumulated over all queries since the last clear). */
pwr_err_t pwr_mon_getFaults(pwrRail_t rail, pwrMon_fault_t *faults);
void      pwr_mon_clearFaults(pwrRail_t rail);
void      pwr_mon_reportFaults(void);

/* Display name of a rail, e.g. "VCCADJ_FMC1". */
const char* pwr_railName(pwrRail_t rail);

#endif /* SRC_POWER_H_ */
