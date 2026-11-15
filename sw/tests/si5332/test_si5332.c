/*
 * Host test for the Si5332 driver (mock device, no target needed).
 *
 * Build/Run: see tests/si5332/Makefile  (make -C tests/si5332 run)
 *
 * Checked, among other things:
 *  - Register values after si5332_init() against the original CBPro exports (XTAL and CLKIN2)
 *  - ID encoding and register set for OUT7 = 26 MHz against ID_TEST.pdf (architecture chapter 1.6)
 *  - Routing (HSDIV0/HSDIV1/MultiSynth sharing/no free MultiSynth)
 *  - Commit order (OE off -> READY -> registers -> ACTIVE -> OE on)
 *  - Clock source switching including rollback on missing clock
 */

#include <stdio.h>
#include <string.h>

/* Include the driver directly so static functions can also be tested */
#include "../../devices/si5332/si5332.c"

/* Original CBPro exports (same symbol names -> rename) */
#define si5332_gm2_revd_registers xtal_regs
#define si5332_gm2_revd_register_t export_reg_a_t
#include "../../docs/si5332/Si5332-GM2-RevD-UNVBB1-27MHz_XTAL-Registers.h"
#undef si5332_gm2_revd_registers
#undef si5332_gm2_revd_register_t
#undef SI5332_GM2_REVD_REG_CONFIG_HEADER
#define si5332_gm2_revd_registers clkin2_regs
#define si5332_gm2_revd_register_t export_reg_b_t
#include "../../docs/si5332/Si5332-GM2-RevD-UNVBB1-100MHz_ClkIn2-Registers.h"
#undef si5332_gm2_revd_registers
#undef si5332_gm2_revd_register_t

unsigned long g_delay_us;

/* ------------------------------------------------------------------------- */
/* Mini framework                                                            */
/* ------------------------------------------------------------------------- */
static int g_fail;
static int g_checks;

#define CHECK(c) do { g_checks++; if (!(c)) { printf("  FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); g_fail++; } } while (0)
#define CHECK_EQ(a, b) do { unsigned long _a = (unsigned long)(a), _b = (unsigned long)(b); g_checks++; \
	if (_a != _b) { printf("  FAIL %s:%d: %s == %s  (0x%lX != 0x%lX)\n", __FILE__, __LINE__, #a, #b, _a, _b); g_fail++; } } while (0)

/* ------------------------------------------------------------------------- */
/* Mock device                                                               */
/* ------------------------------------------------------------------------- */
typedef struct { u8 addr; u8 val; } wlog_t;

static u8      regs[256];
static wlog_t  wlog[8192];
static int     wlog_n;
static int     sim_no_clkin2;	/* 1: CLKIN_2 provides no clock (USYS_STAT = 0x89 when IMUX_SEL = 2) */

static si5332_err_t mock_write(void *ctx, u8 i2c_addr, u8 reg, const u8 *data)
{
	(void)ctx; (void)i2c_addr;
	if (wlog_n < (int)(sizeof(wlog) / sizeof(wlog[0]))) {
		wlog[wlog_n].addr = reg;
		wlog[wlog_n].val  = *data;
		wlog_n++;
	}
	regs[reg] = *data;
	if (reg == 0x06) {
		if (*data == 0x01)
			regs[0x07] = 0x01;
		else if (*data == 0x02)
			regs[0x07] = (sim_no_clkin2 && regs[0x24] == 0x02) ? 0x89 : 0x02;
	}
	return SI5332_OK;
}

static si5332_err_t mock_read(void *ctx, u8 i2c_addr, u8 reg, u8 *data)
{
	(void)ctx; (void)i2c_addr;
	*data = regs[reg];
	return SI5332_OK;
}

static si5332_t* fresh_device(void)
{
	memset(regs, 0, sizeof(regs));
	wlog_n = 0;
	sim_no_clkin2 = 0;
	return si5332_initialize(0x6A, mock_write, mock_read, 0);
}

static si5332_t* init_device(si5332_clk_src_t src)
{
	si5332_t *d = fresh_device();
	CHECK_EQ(si5332_init(d, src), SI5332_OK);
	return d;
}

/* Index of the first log entry from 'start', -1 if not present */
static int find_log(u8 addr, int val, int start)
{
	for (int i = start; i < wlog_n; i++)
		if (wlog[i].addr == addr && (val < 0 || wlog[i].val == (u8)val))
			return i;
	return -1;
}

/* Both export types have an identical layout */
static int cmp_export(const void *table, int n)
{
	const export_reg_a_t *t = (const export_reg_a_t *)table;
	int bad = 0;

	for (int i = 0; i < n; i++) {
		if (t[i].address == 0x06)
			continue;
		if (regs[t[i].address] != t[i].value) {
			printf("  MISMATCH reg 0x%02X: is 0x%02X, expected 0x%02X\n",
			       t[i].address, regs[t[i].address], t[i].value);
			bad++;
		}
	}
	return bad;
}

/* ------------------------------------------------------------------------- */
/* Tests                                                                     */
/* ------------------------------------------------------------------------- */
static void test_math(void)
{
	u16 intg, res, den;
	u32 n, d;

	printf("test_math\n");

	/* ID_TEST.pdf: N0 = 2500/26 = 1250/13 */
	id_encode(1250, 13, &intg, &res, &den);
	CHECK_EQ(intg, 0x3013); CHECK_EQ(res, 9); CHECK_EQ(den, 13);

	/* PLL feedback XTAL: M = 2500/27 */
	id_encode(2500, 27, &intg, &res, &den);
	CHECK_EQ(intg, 0x2E4B); CHECK_EQ(res, 23); CHECK_EQ(den, 27);

	/* PLL feedback CLKIN2: M = 50 */
	id_encode(50, 1, &intg, &res, &den);
	CHECK_EQ(intg, 0x1900); CHECK_EQ(res, 0); CHECK_EQ(den, 1);

	rational_approx(2500000000ULL, 26000000ULL, ID_DEN_MAX, &n, &d);
	CHECK_EQ(n, 1250); CHECK_EQ(d, 13);

	/* Not exactly representable: denominator limited. Here 75/1 is the best
	 * approximation with denominator <= 32767 (error 10 ppb, the semiconvergent
	 * 75 + 1/32766 would be worse). */
	rational_approx(2500000000ULL, 33333333ULL, ID_DEN_MAX, &n, &d);
	{
		double ref = 2500000000.0 / 33333333.0;
		double got = (double)n / (double)d;
		double err = (got > ref ? got - ref : ref - got) / ref;
		CHECK(d >= 1 && d <= ID_DEN_MAX);
		CHECK(n == 75 && d == 1);
		CHECK(err < 2e-8);
	}

	/* try_hsdiv / try_share_id */
	{
		u8 r = 0;
		id_state_t id = { 625, 13, 1 };	/* 52 MHz */
		CHECK(try_hsdiv(50000000, HSDIV0_HZ, &r) && r == 2);
		CHECK(try_hsdiv(25000000, HSDIV0_HZ, &r) && r == 4);
		CHECK(!try_hsdiv(26000000, HSDIV0_HZ, &r));
		CHECK(!try_hsdiv(1000000, HSDIV0_HZ, &r));	/* R = 100 > 63 */
		CHECK(try_share_id(&id, 26000000, &r) && r == 2);
		CHECK(try_share_id(&id, 52000000, &r) && r == 1);
		CHECK(!try_share_id(&id, 104000000, &r));	/* higher frequency than the ID */
		CHECK(!try_share_id(&id, 27000000, &r));
	}
}

static void test_init_matches_cbpro_exports(void)
{
	si5332_t *d;
	si5332_status_flags_t st;

	printf("test_init_matches_cbpro_exports\n");

	d = init_device(SI5332_CLK_SRC_XTAL_27M);
	CHECK_EQ(cmp_export(xtal_regs, SI5332_GM2_REVD_REG_CONFIG_NUM_REGS), 0);
	CHECK_EQ(regs[0x07], 0x02);

	d = init_device(SI5332_CLK_SRC_CLKIN2_100M);
	CHECK_EQ(cmp_export(clkin2_regs, SI5332_GM2_REVD_REG_CONFIG_NUM_REGS), 0);

	regs[0x05] = 0xBF;
	CHECK_EQ(si5332_get_status(d, &st), SI5332_OK);
	CHECK(st.is_active && !st.no_input);
	CHECK_EQ(st.vdd_xtal_ok, 1);
	CHECK_EQ(st.vddo_ok_mask, 0x3F);

	/* No configuration without init */
	d = fresh_device();
	CHECK_EQ(si5332_commit(d), SI5332_E_NOT_INIT);
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_7, SI5332_26MHZ), SI5332_E_NOT_INIT);
}

static void test_out7_26mhz_vs_id_test(void)
{
	si5332_t *d;
	int ready, active, first;

	printf("test_out7_26mhz_vs_id_test\n");

	d = init_device(SI5332_CLK_SRC_XTAL_27M);

	CHECK_EQ(si5332_set_output_format(d, SI5332_OUT_7, SI5332_FMT_LVDS_FAST), SI5332_OK);
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_7, SI5332_26MHZ), SI5332_OK);
	CHECK_EQ(si5332_set_output_enable(d, SI5332_OUT_7, 1), SI5332_OK);	/* pending -> shadow only */
	CHECK_EQ(regs[0xB7], 0x02);

	wlog_n = 0;
	CHECK_EQ(si5332_commit(d), SI5332_OK);

	/* Expected register values per ID_TEST (chapter 1.6), OUT7_OE = 1 */
	CHECK_EQ(regs[0x2A], 0x50);
	CHECK_EQ(regs[0x36], 0x30); CHECK_EQ(regs[0x37], 0x13);
	CHECK_EQ(regs[0x38], 0x00); CHECK_EQ(regs[0x39], 0x09);
	CHECK_EQ(regs[0x3A], 0x00); CHECK_EQ(regs[0x3B], 0x0D);
	CHECK_EQ(regs[0xAC], 0x06);
	CHECK_EQ(regs[0xAD], 0x01);
	CHECK_EQ(regs[0xB6], 0x00);
	CHECK_EQ(regs[0xB7], 0x06);
	CHECK_EQ(regs[0xBA], 0x5C);
	CHECK_EQ(regs[0xBB], 0x0A);
	CHECK_EQ(regs[0xBC], 0xD2);
	CHECK_EQ(regs[0xBD], 0x00);
	CHECK_EQ(regs[0x07], 0x02);

	/* ID1 stays off, HSDIV unchanged */
	CHECK_EQ(regs[0xBA] & 0x40, 0x40);
	CHECK_EQ(regs[0x2B], 0x19);
	CHECK_EQ(regs[0x2D], 0x14);

	/* Sequence: OE off -> READY -> registers -> ACTIVE -> OE on */
	first  = find_log(0xB6, 0x00, 0);
	ready  = find_log(0x06, 0x01, 0);
	active = find_log(0x06, 0x02, 0);
	CHECK(first == 0);
	CHECK(find_log(0xB7, 0x00, 0) == 1);
	CHECK(ready == 2);
	CHECK(active > ready);
	CHECK(find_log(0x2A, 0x50, 0) > ready && find_log(0x2A, 0x50, 0) < active);
	CHECK(find_log(0x36, 0x30, 0) > ready && find_log(0x36, 0x30, 0) < active);
	CHECK(find_log(0xBA, 0x5C, 0) > ready && find_log(0xBA, 0x5C, 0) < active);
	CHECK(find_log(0xB7, 0x06, 0) > active);
	/* OUT0/OUT2 registers not touched */
	CHECK(find_log(0x7A, -1, 0) < 0);
	CHECK(find_log(0x89, -1, 0) < 0);

	/* Repeated commit without changes: no accesses */
	wlog_n = 0;
	CHECK_EQ(si5332_commit(d), SI5332_OK);
	CHECK_EQ(wlog_n, 0);
}

static void test_hsdiv_routing(void)
{
	si5332_t *d;

	printf("test_hsdiv_routing\n");
	d = init_device(SI5332_CLK_SRC_XTAL_27M);

	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_0, SI5332_50MHZ), SI5332_OK);	/* HSDIV0 / 2 */
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_4, SI5332_125MHZ), SI5332_OK);	/* HSDIV1 / 1 */
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_2, SI5332_25MHZ), SI5332_OK);	/* HSDIV0 / 4 */
	CHECK_EQ(si5332_commit(d), SI5332_OK);

	CHECK_EQ(regs[0x25], 0x00); CHECK_EQ(regs[0x7B], 2);
	CHECK_EQ(regs[0x28], 0x10); CHECK_EQ(regs[0x99], 1);
	CHECK_EQ(regs[0x27], 0x00); CHECK_EQ(regs[0x8A], 4);
	CHECK_EQ(regs[0xBB], 0x22);	/* OMUX3 switched on */
	CHECK_EQ(regs[0xBC], 0x92);	/* OUT4 switched on */
	CHECK_EQ(regs[0xBA], 0x7C);	/* no ID in use */
}

static void test_multisynth_sharing_and_exhaustion(void)
{
	si5332_t *d;

	printf("test_multisynth_sharing_and_exhaustion\n");
	d = init_device(SI5332_CLK_SRC_XTAL_27M);

	/* OUT7 = 52 MHz -> ID0 (N = 625/13), OUT4 = 26 MHz shares ID0 with R = 2 */
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_7, SI5332_52MHZ), SI5332_OK);
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_4, SI5332_26MHZ), SI5332_OK);
	CHECK_EQ(d->id[0].refcnt, 2);
	CHECK_EQ(d->id[1].refcnt, 0);
	CHECK_EQ(si5332_commit(d), SI5332_OK);
	CHECK_EQ(regs[0x2A], 0x50); CHECK_EQ(regs[0xAD], 1);
	CHECK_EQ(regs[0x28], 0x50); CHECK_EQ(regs[0x99], 2);
	CHECK_EQ(regs[0xBA] & 0x60, 0x40);	/* ID0 on, ID1 off */
	{
		u16 intg, res, den;
		id_encode(625, 13, &intg, &res, &den);
		CHECK_EQ(regs[0x36], intg >> 8); CHECK_EQ(regs[0x37], intg & 0xFF);
		CHECK_EQ(regs[0x38], res >> 8);  CHECK_EQ(regs[0x39], res & 0xFF);
		CHECK_EQ(regs[0x3A], den >> 8);  CHECK_EQ(regs[0x3B], den & 0xFF);
	}

	/* OUT4 to 27 MHz -> own ID1 */
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_4, SI5332_27MHZ), SI5332_OK);
	CHECK_EQ(d->id[0].refcnt, 1);
	CHECK_EQ(d->id[1].refcnt, 1);
	CHECK_EQ(si5332_commit(d), SI5332_OK);
	CHECK_EQ(regs[0x28], 0x60);
	CHECK_EQ(regs[0xBA] & 0x60, 0x00);	/* both IDs on */
	CHECK_EQ(regs[0x4E], 0x2E); CHECK_EQ(regs[0x4F], 0x4B);	/* N = 2500/27 -> 11851 */
	CHECK_EQ(regs[0x50], 0x00); CHECK_EQ(regs[0x51], 0x17);
	CHECK_EQ(regs[0x52], 0x00); CHECK_EQ(regs[0x53], 0x1B);

	/* Third independent fractional: no MultiSynth free, state remains unchanged */
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_2, 33000000UL), SI5332_E_NO_FREE_MS);
	CHECK_EQ(d->ch[1].src, SI5332_DIV_SRC_HSDIV1);
	CHECK_EQ(d->ch[1].freq_hz, SI5332_125MHZ);
	CHECK_EQ(d->dirty_ch, 0);
	CHECK_EQ(d->dirty_id, 0);

	/* OUT4 back to HSDIV0: ID1 is released and switched off */
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_4, SI5332_100MHZ), SI5332_OK);
	CHECK_EQ(d->id[1].refcnt, 0);
	CHECK_EQ(si5332_commit(d), SI5332_OK);
	CHECK_EQ(regs[0x28], 0x00);
	CHECK_EQ(regs[0xBA] & 0x60, 0x40);
}

static void test_param_checks(void)
{
	si5332_t *d;

	printf("test_param_checks\n");
	d = init_device(SI5332_CLK_SRC_XTAL_27M);

	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_7, 4000000UL), SI5332_E_FREQ_UNSUPPORTED);
	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_7, 300000000UL), SI5332_E_FREQ_UNSUPPORTED);
	CHECK_EQ(si5332_set_output_freq(d, (si5332_out_ch_t)6, SI5332_26MHZ), SI5332_E_PARAM);	/* OUT6 locked */
	CHECK_EQ(si5332_set_output_freq(d, (si5332_out_ch_t)1, SI5332_26MHZ), SI5332_E_PARAM);
	CHECK_EQ(si5332_set_output_enable(d, (si5332_out_ch_t)6, 0), SI5332_E_PARAM);
	CHECK_EQ(si5332_set_output_format(d, SI5332_OUT_0, (si5332_out_format_t)5), SI5332_E_PARAM);	/* 1.8 V */
	CHECK_EQ(si5332_set_output_format(d, SI5332_OUT_0, (si5332_out_format_t)3), SI5332_E_PARAM);	/* CMOS */
	CHECK_EQ(si5332_set_clock_source(d, (si5332_clk_src_t)2), SI5332_E_PARAM);
	CHECK_EQ(si5332_init(0, SI5332_CLK_SRC_XTAL_27M), SI5332_E_PARAM);
	CHECK(si5332_initialize(0x6A, 0, mock_read, 0) == 0);
	CHECK_EQ(d->dirty_ch, 0);
	CHECK_EQ(d->dirty_id, 0);
}

static void test_format_and_enable(void)
{
	si5332_t *d;

	printf("test_format_and_enable\n");
	d = init_device(SI5332_CLK_SRC_XTAL_27M);

	/* Immediate OE (nothing pending, channel configured) */
	CHECK_EQ(si5332_set_output_enable(d, SI5332_OUT_0, 1), SI5332_OK);
	CHECK_EQ(regs[0xB6], 0x01);
	CHECK_EQ(si5332_set_output_enable(d, SI5332_OUT_2, 1), SI5332_OK);
	CHECK_EQ(regs[0xB6], 0x09);
	CHECK_EQ(regs[0xB7], 0x02);	/* OUT6 unchanged */

	/* Unconfigured output: only after configuration + commit */
	CHECK_EQ(si5332_set_output_enable(d, SI5332_OUT_4, 1), SI5332_OK);
	CHECK_EQ(regs[0xB6], 0x09);

	/* All formats */
	{
		static const si5332_out_format_t f[] = {
			SI5332_FMT_LVDS, SI5332_FMT_LVDS_FAST, SI5332_FMT_HCSL_50_EXT, SI5332_FMT_HCSL_50_INT,
			SI5332_FMT_HCSL_42_5_EXT, SI5332_FMT_HCSL_42_5_INT, SI5332_FMT_LVPECL
		};
		for (unsigned i = 0; i < sizeof(f) / sizeof(f[0]); i++) {
			CHECK_EQ(si5332_set_output_format(d, SI5332_OUT_0, f[i]), SI5332_OK);
			CHECK_EQ(si5332_commit(d), SI5332_OK);
			CHECK_EQ(regs[0x7A], f[i]);
			CHECK_EQ(regs[0xB6] & 0x01, 0x01);	/* OE restored after commit */
		}
	}

	CHECK_EQ(si5332_set_output_freq(d, SI5332_OUT_4, SI5332_100MHZ), SI5332_OK);
	CHECK_EQ(si5332_commit(d), SI5332_OK);
	CHECK_EQ(regs[0xB6], 0x49);	/* OUT0, OUT2, OUT4 */
	CHECK_EQ(regs[0xB7], 0x02);	/* OUT6 always on, OUT7 off */

	CHECK_EQ(si5332_set_output_enable(d, SI5332_OUT_0, 0), SI5332_OK);
	CHECK_EQ(regs[0xB6], 0x48);
}

static void test_clock_source_switch(void)
{
	si5332_t *d;

	printf("test_clock_source_switch\n");
	d = init_device(SI5332_CLK_SRC_XTAL_27M);

	CHECK_EQ(si5332_set_clock_source(d, SI5332_CLK_SRC_CLKIN2_100M), SI5332_OK);
	CHECK_EQ(cmp_export(clkin2_regs, SI5332_GM2_REVD_REG_CONFIG_NUM_REGS), 0);
	CHECK_EQ(regs[0x07], 0x02);

	CHECK_EQ(si5332_set_clock_source(d, SI5332_CLK_SRC_XTAL_27M), SI5332_OK);
	CHECK_EQ(cmp_export(xtal_regs, SI5332_GM2_REVD_REG_CONFIG_NUM_REGS), 0);

	/* CLKIN_2 without clock: error and rollback to XTAL */
	sim_no_clkin2 = 1;
	CHECK_EQ(si5332_set_clock_source(d, SI5332_CLK_SRC_CLKIN2_100M), SI5332_E_NO_INPUT);
	CHECK_EQ(regs[0x24], 0x01);
	CHECK_EQ(regs[0x75], 0x01);
	CHECK_EQ(regs[0x07], 0x02);
	CHECK_EQ(cmp_export(xtal_regs, SI5332_GM2_REVD_REG_CONFIG_NUM_REGS), 0);

	/* Init directly with missing clock */
	d = fresh_device();
	sim_no_clkin2 = 1;
	CHECK_EQ(si5332_init(d, SI5332_CLK_SRC_CLKIN2_100M), SI5332_E_NO_INPUT);
	CHECK_EQ(si5332_commit(d), SI5332_E_NOT_INIT);
}

int main(void)
{
	test_math();
	test_init_matches_cbpro_exports();
	test_out7_26mhz_vs_id_test();
	test_hsdiv_routing();
	test_multisynth_sharing_and_exhaustion();
	test_param_checks();
	test_format_and_enable();
	test_clock_source_switch();

	printf("\n%d checks, %d failed\n", g_checks, g_fail);
	return g_fail ? 1 : 0;
}
