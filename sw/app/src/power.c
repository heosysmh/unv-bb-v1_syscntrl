
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file power.c
* @brief Implementation of the power functions.
*/

#include "ina745a.h"
#include "tpsm8286x.h"

#include "power.h"
#include "bsp.h"
#include "userDef.h"
#include "boardGpio.h"
#include "i2cBus.h"


/* =========================================================================== */
/* Power monitors: ADC and alert behaviour (ina745a)                        */
/* =========================================================================== */
#define PWR_MON_MODE	INA745A_MODE_CONT_TEMP_CURRENT_BUSV
#define PWR_MON_VBUSCT	INA745A_CT_150US
#define PWR_MON_VSENCT	INA745A_CT_150US
#define PWR_MON_TCT		INA745A_CT_150US
#define PWR_MON_AVG		INA745A_AVG_128

/*
 * Alert configuration. Both values are dictated by the wiring (ALERT on the
 * power-good nets, see boardGpio.h):
 *   - ALATCH = 1: a fault is latched until the DIAG_ALRT register is read,
 *     so short events cannot be missed between two polls.
 *   - CNVR = 0: a conversion-ready alert would pull the power-good net low
 *     after every conversion and must stay disabled.
 *   - APOL = active low: the open-drain output pulls the net low on fault.
 */
#define PWR_MON_ALERT_LATCH	1
#define PWR_MON_ALERT_CNVR	0
#define PWR_MON_ALERT_SLOW	0	/* compare against the measured value */
#define PWR_MON_ALERT_POL	INA745A_ALERT_ACTIVE_LOW

/*
 * Under-voltage and under-current protection are not used on this board.
 * The driver API always writes both thresholds, so non-trippable values are
 * programmed: BUVL = 0 V (VBUS is unsigned, never below 0) and
 * CURRENTUL = the most negative value the 16-bit register can hold.
 */
#define PWR_MON_UV_DISABLED_V	0.0f
#define PWR_MON_UI_DISABLED_A	(-39.0f)

#define PWR_FMC_VADJ_CNTRL		(TPSM8286_FPWM_AT_VC_MASK | \
								 TPSM8286_FPWM_MASK | \
					 			 TPSM8286_OUTPUT_DISCHARGE_MASK | \
					 			 TPSM8286_HICCUP_EN_MASK | \
					 			 TPSM8286_VOLTAGE_RAMP_SPEED_1MV)

/* The rail table and the driver instance pool must describe the same number
 * of devices. */
typedef char pwr_assert_railCount[(PWR_RAIL_COUNT == INA745A_NUM_INSTANCES) ? 1 : -1];
typedef char pwr_assert_fmcCount[(PWR_FMC_COUNT == TPSM8286_NUM_OF_INSTANCES) ? 1 : -1];

/* --------------------------------------------------------------------------- */
/* Static configuration 						                               */
/* --------------------------------------------------------------------------- */

typedef struct {
	const char *name;
	u8          i2cAddr;
	pwrPgLine_t pgLine;
	float       overVoltage_V;
	float       overCurrent_A;
	float       overTemp_C;
} pwrMonCfg_t;

#define X(name, addr, pg, vov, iov, tov)	{ #name, addr, pg, vov, iov, tov },
static const pwrMonCfg_t gMonCfg[PWR_RAIL_COUNT] = {
	PWR_MON_TABLE
};
#undef X

typedef struct {
	const char *name;
	u8          i2cAddr;
	float       setpoint_V;
} pwrFmcCfg_t;

static const pwrFmcCfg_t gFmcCfg[PWR_FMC_COUNT] = {
	{ "FMC1_VADJ", I2CBUS_FMC1_VADJ_ADDR, BOARD_FMC1_VADJ_V },
	{ "FMC2_VADJ", I2CBUS_FMC2_VADJ_ADDR, BOARD_FMC2_VADJ_V }
};

/** Power-good / ALERT line masks, indexed by pwrPgLine_t. */
static const u32 gPgMask[PWR_PG_COUNT] = {GPIO_PG_SYS_MASK,
									GPIO_PG_FMC1_SUP_MASK,
									GPIO_PG_FMC2_SUP_MASK
};

static const char * const gPgName[PWR_PG_COUNT] = {
	"SYS_PG", "FMC1_SUP_PG", "FMC2_SUP_PG"
};

/* --------------------------------------------------------------------------- */
/* Module state                                                                 */
/* --------------------------------------------------------------------------- */

static tpsm8286_t *gFmcVadj[PWR_FMC_COUNT];
static ina745a_t  *gMon[PWR_RAIL_COUNT];

/** Sticky fault shadow: DIAG_ALRT clears on read, the application must not. */
static pwrMon_fault_t gFaultLatch[PWR_RAIL_COUNT];

static u8 gMonConfigured;

/* --------------------------------------------------------------------------- */
/* Helpers                                                                      */
/* --------------------------------------------------------------------------- */

/**
 * @brief Merges the device diagnostics into the sticky fault shadow.
 *
 * Under-voltage and under-current flags are ignored because those limits are
 * programmed to non-trippable values (see #PWR_MON_UV_DISABLED_V and
 * #PWR_MON_UI_DISABLED_A).
 *
 * @param rail  Rail the diagnostics belong to.
 * @param flags Decoded DIAG_ALRT flags of the device.
 * @return Non-zero if at least one fault bit was set in @p flags.
 */
static int pwr_mergeFaults(pwrRail_t rail, const ina745a_diag_flags_t *flags)
{
	pwrMon_fault_t *f = &gFaultLatch[rail];
	int any = 0;

	if (flags->bus_over_limit)     { f->over_voltage = 1; any = 1; }
	if (flags->current_over_limit) { f->over_current = 1; any = 1; }
	if (flags->temp_over_limit)    { f->over_temp    = 1; any = 1; }
	if (flags->power_over_limit)   { f->over_power   = 1; any = 1; }
	if (!flags->memory_ok)         { f->memory_fail  = 1; any = 1; }

	return any;
}

/**
 * @brief Queries all monitors of one power-good group.
 * @param line Power-good / ALERT net whose monitors are queried.
 * @return Number of devices that reported a fault, or -1 on a device error.
 */
static int pwr_queryGroup(pwrPgLine_t line)
{
	ina745a_diag_flags_t flags;
	int faults = 0;

	for (u32 i = 0; i < PWR_RAIL_COUNT; i++) {
		if (gMonCfg[i].pgLine != line)
			continue;

		if (ina745a_get_diag_flags(gMon[i], &flags) != INA745A_OK)
			return -1;

		if (pwr_mergeFaults((pwrRail_t)i, &flags))
			faults++;
	}

	return faults;
}

/* --------------------------------------------------------------------------- */
/* Initialization                                                               */
/* --------------------------------------------------------------------------- */

pwr_err_t pwr_init(void)
{
	i2cHandle_t *pmbus = i2cBus_pmbus();

	if (!pmbus)
		return PWR_E_INIT;

	/* FMC Vadj regulators */
	for (u32 i = 0; i < PWR_FMC_COUNT; i++) {
		gFmcVadj[i] = tpsm8286_initialize(i, gFmcCfg[i].i2cAddr,
		                                  (tpsm8286_i2c_write_f)i2c_writeReg,
		                                  (tpsm8286_i2c_read_f)i2c_readReg,
		                                  (void*)pmbus);
		if (!gFmcVadj[i])
			return PWR_E_INIT;
	}

	/* Power monitors; they share the PMBus with the regulators */
	for (u32 i = 0; i < PWR_RAIL_COUNT; i++) {
		gMon[i] = ina745a_initialize(i, gMonCfg[i].i2cAddr,
		                             (ina745a_i2c_write_f)i2c_writeRegMultipleBytes,
		                             (ina745a_i2c_read_f)i2c_readRegMultipleBytes,
		                             (void*)pmbus);
		if (!gMon[i])
			return PWR_E_INIT;
	}

	for (u32 i = 0; i < PWR_RAIL_COUNT; i++)
		pwr_mon_clearFaults((pwrRail_t)i);

	gMonConfigured = 0;

	return PWR_OK;
}

/* --------------------------------------------------------------------------- */
/* FMC Vadj                                                                     */
/* --------------------------------------------------------------------------- */

pwr_err_t pwr_fmcVadj_configure(void)
{
	for (u32 i = 0; i < PWR_FMC_COUNT; i++) {
		if (!gFmcVadj[i])
			return PWR_E_INIT;

		if (gFmcCfg[i].setpoint_V > BOARD_FMC_VADJ_MAX_V) {
			bsp_printf("ERROR: %s setpoint out of range\r\n", gFmcCfg[i].name);
			return PWR_E_RANGE;
		}

		if (tpsm8286_set_cntrl(gFmcVadj[i], PWR_FMC_VADJ_CNTRL) != TPSM8286_OK)
			return PWR_E_DEVICE;

		if (tpsm8286_set_voltage(gFmcVadj[i], gFmcCfg[i].setpoint_V) != TPSM8286_OK)
			return PWR_E_DEVICE;
	}

	return PWR_OK;
}

pwr_err_t pwr_fmcVadj_isRegulating(pwrFmc_t fmc)
{
	if (fmc >= PWR_FMC_COUNT)
		return PWR_E_PARAM;
	if (!gFmcVadj[fmc])
		return PWR_E_INIT;

	int ok = 0;
	if (tpsm8286_check_voltageReg(gFmcVadj[fmc], &ok) != TPSM8286_OK)
		return PWR_E_DEVICE;

	return (ok == 1) ? PWR_OK : PWR_E_REGULATION;
}

pwr_err_t pwr_fmcVadj_enable(int enable)
{
	for (u32 i = 0; i < PWR_FMC_COUNT; i++) {
		if (!gFmcVadj[i])
			return PWR_E_INIT;

		if (tpsm8286_set_enable(gFmcVadj[i], enable) != TPSM8286_OK)
			return PWR_E_DEVICE;
	}

	return PWR_OK;
}

/* --------------------------------------------------------------------------- */
/* Power monitors                                                               */
/* --------------------------------------------------------------------------- */

pwr_err_t pwr_mon_configure(void)
{
	const ina745a_adc_config_t adcCfg = {
		.mode   = PWR_MON_MODE,
		.vbusct = PWR_MON_VBUSCT,
		.vsenct = PWR_MON_VSENCT,
		.tct    = PWR_MON_TCT,
		.avg    = PWR_MON_AVG
	};
	ina745a_alert_cfg_t alertCfg = {
		.alatch        = PWR_MON_ALERT_LATCH,
		.conv_ready_en = PWR_MON_ALERT_CNVR,
		.slow_alert    = PWR_MON_ALERT_SLOW,
		.polarity      = PWR_MON_ALERT_POL
	};
	pwr_err_t status = PWR_OK;

	for (u32 i = 0; i < PWR_RAIL_COUNT; i++) {
		ina745a_diag_flags_t flags;

		if (!gMon[i])
			return PWR_E_INIT;

		if (ina745a_init(gMon[i], &adcCfg) != INA745A_OK) {
			bsp_printf("ERROR: Power-Monitor %s initialization failed\r\n",
			           gMonCfg[i].name);
			status = PWR_E_DEVICE;
			continue;
		}

		/* Protection limits; the under limits are programmed to values that
		 * can never trip (see PWR_MON_UV_DISABLED_V / PWR_MON_UI_DISABLED_A). */
		if (ina745a_set_bus_voltage_limits(gMon[i], gMonCfg[i].overVoltage_V,
		                                   PWR_MON_UV_DISABLED_V) != INA745A_OK ||
		    ina745a_set_current_limits(gMon[i], gMonCfg[i].overCurrent_A,
		                               PWR_MON_UI_DISABLED_A) != INA745A_OK ||
		    ina745a_set_temperature_limit(gMon[i], gMonCfg[i].overTemp_C) != INA745A_OK) {
			bsp_printf("ERROR: Power-Monitor %s limit setup failed\r\n",
			           gMonCfg[i].name);
			status = PWR_E_DEVICE;
			continue;
		}

		/* Arm the latched alert. Writing DIAG_ALRT also clears pending
		 * flags; the additional read drops anything latched in between so
		 * the power-good nets start from a clean state. */
		if (ina745a_configure_alert(gMon[i], &alertCfg) != INA745A_OK) {
			bsp_printf("ERROR: Power-Monitor %s alert setup failed\r\n",
			           gMonCfg[i].name);
			status = PWR_E_DEVICE;
			continue;
		}

		(void)ina745a_get_diag_flags(gMon[i], &flags);
		pwr_mon_clearFaults((pwrRail_t)i);
	}

	gMonConfigured = 1;

	return status;
}

pwr_err_t pwr_mon_read(pwrRail_t rail, pwrMon_values_t *values)
{
	ina745a_measurements_t m;

	if (!values || rail >= PWR_RAIL_COUNT)
		return PWR_E_PARAM;
	if (!gMon[rail] || !gMonConfigured)
		return PWR_E_INIT;

	if (ina745a_get_measurements(gMon[rail], &m) != INA745A_OK)
		return PWR_E_DEVICE;

	values->bus_voltage_V = m.bus_voltage_V;
	values->current_A     = m.current_A;
	values->die_temp_C    = m.die_temp_C;

	return PWR_OK;
}

pwr_err_t pwr_mon_poll(void)
{
	pwr_err_t status = PWR_OK;

	for (u32 i = 0; i < PWR_RAIL_COUNT; i++) {
		pwrMon_values_t v;
		pwr_err_t res = pwr_mon_read((pwrRail_t)i, &v);

		if (res != PWR_OK) {
			bsp_printf("ERROR: Power-Monitor %s read failed\r\n", gMonCfg[i].name);
			status = res;
			continue;
		}

		bsp_printf("Pmon %7s: U=%.dmV, I=%dmA, T=%.d.%d°C\r\n",
		           gMonCfg[i].name,
		           (int)(v.bus_voltage_V * 1000.0f),
		           (int)(v.current_A * 1000.0f),
		           (int)(v.die_temp_C),(int)((v.die_temp_C - (int)(v.die_temp_C)) * 100.f));
	}
	bsp_printf("\r\n");

	return status;
}

pwr_err_t pwr_checkAlerts(void)
{
	u32 pg = boardGpio_getPgStatus();
	pwr_err_t status = PWR_OK;

	if (!gMonConfigured)
		return PWR_E_INIT;

	for (u32 line = 0; line < PWR_PG_COUNT; line++) {
		int faults;

		if (pg & gPgMask[line])
			continue;	/* line high: supply good, no alert pending */

		faults = pwr_queryGroup((pwrPgLine_t)line);

		if (faults < 0) {
			bsp_printf("ERROR: %s low, monitor query failed\r\n", gPgName[line]);
			status = PWR_E_DEVICE;
		} else {
			bsp_printf("ERROR: %s low\r\n",gPgName[line]);
			if (status == PWR_OK)
				status = PWR_E_REGULATION;		
		}
	}

	return status;
}

pwr_err_t pwr_mon_getFaults(pwrRail_t rail, pwrMon_fault_t *faults)
{
	if (!faults || rail >= PWR_RAIL_COUNT)
		return PWR_E_PARAM;

	*faults = gFaultLatch[rail];

	return PWR_OK;
}

void pwr_mon_clearFaults(pwrRail_t rail)
{
	if (rail >= PWR_RAIL_COUNT)
		return;

	gFaultLatch[rail].over_voltage = 0;
	gFaultLatch[rail].over_current = 0;
	gFaultLatch[rail].over_temp    = 0;
	gFaultLatch[rail].over_power   = 0;
	gFaultLatch[rail].memory_fail  = 0;
}

void pwr_mon_reportFaults(void)
{
	for (u32 i = 0; i < PWR_RAIL_COUNT; i++) {
		const pwrMon_fault_t *f = &gFaultLatch[i];

		if (!f->over_voltage && !f->over_current && !f->over_temp &&
		    !f->over_power && !f->memory_fail)
			continue;

		bsp_printf("FAULT %s:%s%s%s%s%s\r\n", gMonCfg[i].name,
		           f->over_voltage ? " over-voltage" : "",
		           f->over_current ? " over-current" : "",
		           f->over_temp    ? " over-temperature" : "",
		           f->over_power   ? " over-power" : "",
		           f->memory_fail  ? " memory-error" : "");
	}
}

const char* pwr_railName(pwrRail_t rail)
{
	if (rail >= PWR_RAIL_COUNT)
		return "?";

	return gMonCfg[rail].name;
}
