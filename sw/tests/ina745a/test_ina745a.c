/*
 * Host test for the INA745A driver (mock device, no target needed).
 *
 * Build/Run: see tests/ina745a/Makefile  (make -C tests/ina745a run)
 *
 * Checked, among other things:
 *  - MANUFACTURER_ID verification in ina745a_init() (hard fail on mismatch)
 *  - ADC_CONFIG packing/unpacking and ina745a_set_mode() (RMW, preserves
 *    conversion times/averaging)
 *  - Measurement conversions against the worked example in datasheet
 *    Table 8-4 (current, bus voltage, power, energy, charge, temperature),
 *    including sign extension for negative current/temperature/charge
 *  - Threshold register encoding against the worked example in datasheet
 *    Section 8.2.2.2 (COL = 21A -> 445Ch, BOVL = 16V -> 1400h)
 *  - DIAG_ALRT configuration bit encoding and status flag decoding
 *  - CONFIG register handling (RST, RSTACC, CONVDLY)
 *  - Error paths (NULL parameters, uninitialized handle, manufacturer ID
 *    mismatch)
 */

#include <stdio.h>
#include <string.h>

/* Include the driver directly so static helpers/the struct definition can
 * also be used/tested (white-box, same approach as tests/si5332). */
#include "../../devices/ina745a/ina745a.c"

/* ------------------------------------------------------------------------- */
/* Mini framework (same style as tests/si5332)                               */
/* ------------------------------------------------------------------------- */
static int g_fail;
static int g_checks;

#define CHECK(c) do { g_checks++; if (!(c)) { printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); g_fail++; } } while (0)
#define CHECK_EQ(a, b) do { long long _a = (long long)(a), _b = (long long)(b); g_checks++; \
	if (_a != _b) { printf("  FAIL %s:%d: %s == %s  (%lld != %lld)\n", __FILE__, __LINE__, #a, #b, _a, _b); g_fail++; } } while (0)
#define CHECK_NEAR(a, b, eps) do { double _a = (double)(a), _b = (double)(b), _e = (double)(eps); g_checks++; \
	if ((_a - _b) > _e || (_b - _a) > _e) { printf("  FAIL %s:%d: %s ~= %s  (%g != %g)\n", __FILE__, __LINE__, #a, #b, _a, _b); g_fail++; } } while (0)

/* ------------------------------------------------------------------------- */
/* Mock device: one 5-byte-wide "register file", addressed like the INA745x  */
/* ------------------------------------------------------------------------- */
static u8 regs[128][5];

static void put_be(u8 *buf, u64 val, int nbytes)
{
	for (int i = 0; i < nbytes; i++)
		buf[nbytes - 1 - i] = (u8)(val >> (8 * i));
}

static u64 get_be(const u8 *buf, int nbytes)
{
	u64 v = 0;

	for (int i = 0; i < nbytes; i++)
		v = (v << 8) | buf[i];
	return v;
}

/* Convenience: write/read a register's raw content directly in the mock store */
static void mock_set_reg(u8 addr, u64 val, int nbytes) { put_be(regs[addr], val, nbytes); }
static u64  mock_get_reg(u8 addr, int nbytes)           { return get_be(regs[addr], nbytes); }

static ina745a_err_t mock_write(void *ctx, u8 i2c_addr, u8 reg, u8 *data, u32 len)
{
	(void)ctx; (void)i2c_addr;
	if (len > sizeof(regs[0]))
		return INA745A_E_PARAM;
	memcpy(regs[reg], data, len);
	return INA745A_OK;
}

static ina745a_err_t mock_read(void *ctx, u8 i2c_addr, u8 reg, u8 *data, u32 len)
{
	(void)ctx; (void)i2c_addr;
	if (len > sizeof(regs[0]))
		return INA745A_E_PARAM;
	memcpy(data, regs[reg], len);
	return INA745A_OK;
}

static void regs_reset(void)
{
	memset(regs, 0, sizeof(regs));
	/* Default: valid manufacturer ID ("TI") */
	mock_set_reg(REG_MANUFACTURER_ID, INA745A_MANUFACTURER_ID, 2);
}

/* Resets the static instance pool so repeated test runs don't exhaust it
 * (white-box access, same approach as tests/si5332 for si5332_t). */
static void pool_reset(void)
{
	memset(gInstances, 0, sizeof(gInstances));
}

static ina745a_t* fresh_device(void)
{
	regs_reset();
	pool_reset();
	return ina745a_initialize(0, INA745A_ADDR_MIN, mock_write, mock_read, 0);
}

static const ina745a_adc_config_t DEFAULT_CFG = {
	.mode   = INA745A_MODE_CONT_TEMP_CURRENT_BUSV,
	.vbusct = INA745A_CT_1052US,
	.vsenct = INA745A_CT_1052US,
	.tct    = INA745A_CT_1052US,
	.avg    = INA745A_AVG_16
};

static ina745a_t* init_device(void)
{
	ina745a_t *d = fresh_device();

	CHECK_EQ(ina745a_init(d, &DEFAULT_CFG), INA745A_OK);
	return d;
}

/* ------------------------------------------------------------------------- */
/* Tests                                                                     */
/* ------------------------------------------------------------------------- */

static void test_manufacturer_id(void)
{
	ina745a_t *d;
	u16 id;

	printf("test_manufacturer_id\n");

	/* Correct ID -> init succeeds */
	d = fresh_device();
	CHECK_EQ(ina745a_get_manufacturer_id(d, &id), INA745A_OK);
	CHECK_EQ(id, INA745A_MANUFACTURER_ID);
	CHECK_EQ(ina745a_init(d, &DEFAULT_CFG), INA745A_OK);

	/* Wrong ID -> hard failure, driver stays unconfigured */
	d = fresh_device();
	mock_set_reg(REG_MANUFACTURER_ID, 0x1234, 2);
	CHECK_EQ(ina745a_init(d, &DEFAULT_CFG), INA745A_E_MANUF_ID);
	{
		float v;
		CHECK_EQ(ina745a_get_bus_voltage(d, &v), INA745A_E_NOT_INIT);
	}
}

static void test_adc_config(void)
{
	ina745a_t *d = init_device();
	ina745a_adc_config_t cfg;

	printf("test_adc_config\n");

	CHECK_EQ(ina745a_get_adc_config(d, &cfg), INA745A_OK);
	CHECK_EQ(cfg.mode,   DEFAULT_CFG.mode);
	CHECK_EQ(cfg.vbusct, DEFAULT_CFG.vbusct);
	CHECK_EQ(cfg.vsenct, DEFAULT_CFG.vsenct);
	CHECK_EQ(cfg.tct,    DEFAULT_CFG.tct);
	CHECK_EQ(cfg.avg,    DEFAULT_CFG.avg);

	/* set_mode() must only change MODE, preserving conversion times/AVG */
	CHECK_EQ(ina745a_set_mode(d, INA745A_MODE_SHUTDOWN), INA745A_OK);
	CHECK_EQ(ina745a_get_adc_config(d, &cfg), INA745A_OK);
	CHECK_EQ(cfg.mode,   INA745A_MODE_SHUTDOWN);
	CHECK_EQ(cfg.vbusct, DEFAULT_CFG.vbusct);
	CHECK_EQ(cfg.vsenct, DEFAULT_CFG.vsenct);
	CHECK_EQ(cfg.tct,    DEFAULT_CFG.tct);
	CHECK_EQ(cfg.avg,    DEFAULT_CFG.avg);
}

/* Cross-checked against datasheet Table 8-4 (Section 8.2.2.3) */
static void test_measurements_positive(void)
{
	ina745a_t *d = init_device();
	ina745a_measurements_t m;

	printf("test_measurements_positive\n");

	mock_set_reg(REG_VBUS,    3840,     2);	/* 12 V */
	mock_set_reg(REG_DIETEMP, 680u << 4, 2);	/* 85 degC (12-bit field in bits 15-4) */
	mock_set_reg(REG_CURRENT, 15000,    2);	/* 18 A */
	mock_set_reg(REG_POWER,   900000,   3);	/* 216 W */
	mock_set_reg(REG_ENERGY,  50625000ULL, 5);	/* 194400 J */
	mock_set_reg(REG_CHARGE,  216000000ULL, 5);	/* 16200 C */

	CHECK_EQ(ina745a_get_measurements(d, &m), INA745A_OK);

	CHECK_NEAR(m.bus_voltage_V, 12.0f,    0.001f);
	CHECK_NEAR(m.die_temp_C,    85.0f,    0.001f);
	CHECK_NEAR(m.current_A,     18.0f,    0.001f);
	CHECK_NEAR(m.power_W,       216.0f,   0.01f);
	CHECK_NEAR(m.energy_J,      194400.0, 0.1);
	CHECK_NEAR(m.charge_C,      16200.0,  0.001);

	CHECK_EQ(m.vbus_raw,    3840);
	CHECK_EQ(m.dietemp_raw, 680);
	CHECK_EQ(m.current_raw, 15000);
	CHECK_EQ(m.power_raw,   900000u);
	CHECK_EQ(m.energy_raw,  50625000ULL);
	CHECK_EQ(m.charge_raw,  216000000LL);

	/* Individual getters must agree with the combined read */
	{
		float f; double dd;

		CHECK_EQ(ina745a_get_bus_voltage(d, &f), INA745A_OK);
		CHECK_NEAR(f, m.bus_voltage_V, 0.0001f);
		CHECK_EQ(ina745a_get_die_temperature(d, &f), INA745A_OK);
		CHECK_NEAR(f, m.die_temp_C, 0.0001f);
		CHECK_EQ(ina745a_get_current(d, &f), INA745A_OK);
		CHECK_NEAR(f, m.current_A, 0.0001f);
		CHECK_EQ(ina745a_get_power(d, &f), INA745A_OK);
		CHECK_NEAR(f, m.power_W, 0.0001f);
		CHECK_EQ(ina745a_get_energy(d, &dd), INA745A_OK);
		CHECK_NEAR(dd, m.energy_J, 0.0001);
		CHECK_EQ(ina745a_get_charge(d, &dd), INA745A_OK);
		CHECK_NEAR(dd, m.charge_C, 0.0001);
	}
}

/* Sign extension for negative current / temperature / charge */
static void test_measurements_negative(void)
{
	ina745a_t *d = init_device();
	ina745a_measurements_t m;

	printf("test_measurements_negative\n");

	/* -18 A: raw = two's complement of 15000 in 16 bit */
	mock_set_reg(REG_CURRENT, (u16)(-15000), 2);
	/* -40 degC: count = -320 (12-bit two's complement), field in bits 15-4 */
	mock_set_reg(REG_DIETEMP, ((u16)(-320) & 0xFFFu) << 4, 2);
	/* -1 C: 40-bit two's complement, all bits set */
	mock_set_reg(REG_CHARGE, (1ULL << 40) - 1ULL, 5);

	CHECK_EQ(ina745a_get_measurements(d, &m), INA745A_OK);

	CHECK_EQ(m.current_raw,  -15000);
	CHECK_EQ(m.dietemp_raw,  -320);
	CHECK_EQ(m.charge_raw,   -1);

	CHECK_NEAR(m.current_A,  -18.0f, 0.001f);
	CHECK_NEAR(m.die_temp_C, -40.0f, 0.001f);
	CHECK_NEAR(m.charge_C,   -0.000075, 0.0000001);
}

/* Cross-checked against the worked design example in Section 8.2.2.2 */
static void test_limits(void)
{
	ina745a_t *d = init_device();

	printf("test_limits\n");

	/* 21 A / 1.2 mA/LSB = 17500d = 445Ch */
	CHECK_EQ(ina745a_set_current_limits(d, 21.0f, 0.0f), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_COL, 2), 0x445Cu);
	CHECK_EQ(mock_get_reg(REG_COL, 2), 17500u);

	/* 16 V / 3.125 mV/LSB = 5120d = 1400h */
	CHECK_EQ(ina745a_set_bus_voltage_limits(d, 16.0f, 0.0f), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_BOVL, 2), 0x1400u);

	/* Negative current limit: -5 A */
	CHECK_EQ(ina745a_set_current_limits(d, 21.0f, -5.0f), INA745A_OK);
	CHECK_EQ((s16)mock_get_reg(REG_CUL, 2), (s16)round_f(-5.0f / INA745A_CURRENT_LSB_A));

	/* Temperature limit: 85 degC -> count 680, field in bits 15-4 */
	CHECK_EQ(ina745a_set_temperature_limit(d, 85.0f), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_TEMP_LIMIT, 2), 680u << 4);

	/* Power limit clamping: huge value clamps to 0xFFFF, negative to 0 */
	CHECK_EQ(ina745a_set_power_limit(d, 1.0e6f), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_PWR_LIMIT, 2), 0xFFFFu);
	CHECK_EQ(ina745a_set_power_limit(d, -10.0f), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_PWR_LIMIT, 2), 0u);

	/* Bus voltage limit clamping: bit 15 must never be set (15-bit field) */
	CHECK_EQ(ina745a_set_bus_voltage_limits(d, 1000.0f, -5.0f), INA745A_OK);
	CHECK((mock_get_reg(REG_BOVL, 2) & 0x8000u) == 0);
	CHECK_EQ(mock_get_reg(REG_BOVL, 2), 0x7FFFu);
	CHECK_EQ(mock_get_reg(REG_BUVL, 2), 0u);
}

static void test_diag_alert(void)
{
	ina745a_t *d = init_device();
	ina745a_diag_flags_t flags;
	ina745a_alert_cfg_t cfg;

	printf("test_diag_alert\n");

	/* All documented RO status bits (0-7) set, RO bits 8-11 clear */
	mock_set_reg(REG_DIAG_ALRT, 0x00FFu, 2);
	CHECK_EQ(ina745a_get_diag_flags(d, &flags), INA745A_OK);
	CHECK_EQ(flags.memory_ok,           1);
	CHECK_EQ(flags.conversion_ready,    1);
	CHECK_EQ(flags.power_over_limit,    1);
	CHECK_EQ(flags.bus_under_limit,     1);
	CHECK_EQ(flags.bus_over_limit,      1);
	CHECK_EQ(flags.current_under_limit, 1);
	CHECK_EQ(flags.current_over_limit,  1);
	CHECK_EQ(flags.temp_over_limit,     1);
	CHECK_EQ(flags.math_overflow,       0);
	CHECK_EQ(flags.charge_overflow,     0);
	CHECK_EQ(flags.energy_overflow,     0);

	cfg.alatch        = 1;
	cfg.conv_ready_en = 1;
	cfg.slow_alert     = 0;
	cfg.polarity       = INA745A_ALERT_ACTIVE_HIGH;
	CHECK_EQ(ina745a_configure_alert(d, &cfg), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_DIAG_ALRT, 2), (u16)((1u << 15) | (1u << 14) | (1u << 12)));
}

static void test_config_register(void)
{
	ina745a_t *d = init_device();

	printf("test_config_register\n");

	/* ina745a_reset(): CONFIG.RST set, driver becomes unconfigured */
	CHECK_EQ(ina745a_reset(d), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_CONFIG, 2), (u64)(1u << CONFIG_RST_BIT));
	{
		float v;
		CHECK_EQ(ina745a_get_bus_voltage(d, &v), INA745A_E_NOT_INIT);
	}

	/* ina745a_set_conversion_delay(): only CONVDLY (bits 13-6) is touched */
	d = init_device();
	CHECK_EQ(ina745a_set_conversion_delay(d, 100), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_CONFIG, 2), (u64)(100u << CONFIG_CONVDLY_SHIFT));

	/* ina745a_reset_accumulators(): RSTACC set, CONVDLY preserved, RST cleared */
	mock_set_reg(REG_CONFIG, (u64)(1u << CONFIG_RST_BIT) | (50u << CONFIG_CONVDLY_SHIFT), 2);
	CHECK_EQ(ina745a_reset_accumulators(d), INA745A_OK);
	CHECK_EQ(mock_get_reg(REG_CONFIG, 2), (u64)(1u << CONFIG_RSTACC_BIT) | (50u << CONFIG_CONVDLY_SHIFT));
}

static void test_param_errors(void)
{
	ina745a_t *d = init_device();
	ina745a_t uninit;
	float f;
	u16 id;

	printf("test_param_errors\n");

	CHECK_EQ(ina745a_get_bus_voltage(0, &f), INA745A_E_PARAM);
	CHECK_EQ(ina745a_get_bus_voltage(d, 0),  INA745A_E_PARAM);
	CHECK_EQ(ina745a_init(0, &DEFAULT_CFG),  INA745A_E_PARAM);
	CHECK_EQ(ina745a_init(d, 0),              INA745A_E_PARAM);

	/* Handle that was never returned by ina745a_initialize() (isReady = 0) */
	memset(&uninit, 0, sizeof(uninit));
	CHECK_EQ(ina745a_get_manufacturer_id(&uninit, &id), INA745A_E_NOT_INIT);

	/* Handle initialized but never ina745a_init()'ed (isConfigured = 0) */
	{
		ina745a_t *d2 = fresh_device();
		CHECK_EQ(ina745a_get_bus_voltage(d2, &f), INA745A_E_NOT_INIT);
	}
}

static void test_address_validation(void)
{
	printf("test_address_validation\n");

	regs_reset();
	pool_reset();

	/* Valid range: INA745A_ADDR_MIN..INA745A_ADDR_MAX (datasheet Table 6-2) */
	CHECK(ina745a_initialize(0, INA745A_ADDR_MIN, mock_write, mock_read, 0) != 0);
	pool_reset();
	CHECK(ina745a_initialize(0, INA745A_ADDR_MAX, mock_write, mock_read, 0) != 0);
	pool_reset();
	CHECK(ina745a_initialize(0, (u8)(INA745A_ADDR_MIN - 1), mock_write, mock_read, 0) == 0);
	CHECK(ina745a_initialize(0, (u8)(INA745A_ADDR_MAX + 1), mock_write, mock_read, 0) == 0);
	CHECK(ina745a_initialize(0, 0x00, mock_write, mock_read, 0) == 0);
	CHECK(ina745a_initialize(0, 0x7F, mock_write, mock_read, 0) == 0);

	/* NULL HAL callbacks are rejected regardless of a valid address */
	CHECK(ina745a_initialize(0, INA745A_ADDR_MIN, 0, mock_read, 0) == 0);
	CHECK(ina745a_initialize(0, INA745A_ADDR_MIN, mock_write, 0, 0) == 0);
}

/*
 * Verifies devId-based multi-instance support (analogous to
 * tpsm8286x_initialize()): devId directly indexes the static instance
 * pool, so several INA745A devices can be managed simultaneously, each
 * with an independent I2C address/state, and an out-of-range devId is
 * rejected.
 */
static void test_multi_instance(void)
{
	ina745a_t *d0, *d1;
	float v0, v1;

	printf("test_multi_instance\n");

	regs_reset();
	pool_reset();

	d0 = ina745a_initialize(0, INA745A_ADDR_MIN,     mock_write, mock_read, 0);
	d1 = ina745a_initialize(1, INA745A_ADDR_MIN + 1, mock_write, mock_read, 0);
	CHECK(d0 != 0);
	CHECK(d1 != 0);
	CHECK(d0 != d1);

	/* Both instances share the same mock register file (single I2C bus in
	 * this test harness), but independently track their own i2cAddr and
	 * isConfigured/isReady state. */
	CHECK_EQ(ina745a_init(d0, &DEFAULT_CFG), INA745A_OK);
	CHECK_EQ(ina745a_init(d1, &DEFAULT_CFG), INA745A_OK);

	mock_set_reg(REG_VBUS, 3840, 2);	/* 12 V */
	CHECK_EQ(ina745a_get_bus_voltage(d0, &v0), INA745A_OK);
	CHECK_EQ(ina745a_get_bus_voltage(d1, &v1), INA745A_OK);
	CHECK_NEAR(v0, 12.0f, 0.001f);
	CHECK_NEAR(v1, 12.0f, 0.001f);

	/* Re-initializing devId 0 must not disturb the independently created
	 * devId 1 handle. */
	CHECK(ina745a_initialize(0, INA745A_ADDR_MIN, mock_write, mock_read, 0) == d0);
	CHECK_EQ(ina745a_get_bus_voltage(d1, &v1), INA745A_OK);

	/* devId out of range (INA745A_NUM_INSTANCES, from userDef.h) is rejected */
	CHECK(ina745a_initialize(INA745A_NUM_INSTANCES, INA745A_ADDR_MIN, mock_write, mock_read, 0) == 0);
}

/* ------------------------------------------------------------------------- */
int main(void)
{
	test_manufacturer_id();
	test_adc_config();
	test_measurements_positive();
	test_measurements_negative();
	test_limits();
	test_diag_alert();
	test_config_register();
	test_param_errors();
	test_address_validation();
	test_multi_instance();

	printf("\n%d checks, %d failed\n", g_checks, g_fail);
	return g_fail ? 1 : 0;
}
