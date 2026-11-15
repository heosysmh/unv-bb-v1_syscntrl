/**
 * @file si5332.c
 * @brief Driver implementation for the Skyworks Si5332A-D-GM2 clock generator.
 *
 * @details
 * Driver for the Skyworks Si5332A-D-GM2 (8 outputs, 40-QFN), fixed VCO at
 * 2.5 GHz, switchable input source (27 MHz crystal / 100 MHz CLKIN_2).
 *
 * Concept:
 *  - Almost all output registers (`OUTx_MODE`, `OUTx_DIV`, ID dividers,
 *    input/PLL registers, power-down bits) are only writable in `READY`
 *    mode. Changes are therefore collected in the driver
 *    (si5332_set_output_format() / si5332_set_output_freq()) and applied
 *    with si5332_commit() in a single `READY` -> `ACTIVE` cycle.
 *    @warning Every commit briefly interrupts ALL outputs, including OUT6
 *    (25 MHz).
 *  - OUT6 (25 MHz clock for Ethernet PHY) is fixed and cannot be changed 
 *    via the API.
 *  - HSDIV0 (100 MHz) and HSDIV1 (125 MHz) are fixed; additional
 *    frequencies are generated via the two MultiSynths (ID0/ID1,
 *    fractional).
 *
 * @see si5332.h
 * @see plans/si5332_architecture.md
 *
 * @date Created on: 22.09.2026
 * @author matthias
 */

#include <string.h>

#include "si5332.h"
#include "si5332_cfg.h"
#include "bsp.h"		/**< bsp_uDelay() */

/* ------------------------------------------------------------------------- */
/* Registers / constants                                                    */
/* ------------------------------------------------------------------------- */

/**
 * @name Register addresses
 * Fixed 8-bit register addresses of the Si5332 used directly by the driver.
 * @{
 */
#define REG_STATUS			0x05	/**< Status register: [7] VDD_XTAL_OK, [5:0] VDDO_OK. */
#define REG_USYS_CTRL			0x06	/**< System control register: 0x01 = READY, 0x02 = ACTIVE. */
#define REG_USYS_STAT			0x07	/**< System status register, mirrors the state reached by #REG_USYS_CTRL. */
#define REG_OE_0_5			0xB6	/**< Output enable register for OUT0..OUT5. */
#define REG_OE_6_7			0xB7	/**< Output enable register for OUT6..OUT7. */
#define REG_PD_ID			0xBA	/**< Power-down register for the MultiSynths: [5] ID0_DIS, [6] ID1_DIS. */
#define REG_PD_OMUX			0xBB	/**< Power-down register for the output muxes (OMUXx_DIS). */
/** @} */

/**
 * @name USYS_STAT values
 * Possible values read back from #REG_USYS_STAT.
 * @{
 */
#define USYS_READY			0x01	/**< Device is in READY state (register access for output/PLL config). */
#define USYS_ACTIVE			0x02	/**< Device is in ACTIVE state (outputs running). */
#define USYS_NO_INPUT			0x89	/**< No input clock detected on the currently selected source. */
/** @} */

#define OE6_BIT				(1u << 1)	/**< OUT6_OE bit position in #REG_OE_6_7; OUT6 is always kept enabled. */

/**
 * @name Timeouts
 * Maximum time to wait for #REG_USYS_STAT to reach the requested state, in milliseconds.
 * @{
 */
#define TIMEOUT_READY_MS		20u	/**< Timeout for the READY transition. */
#define TIMEOUT_ACTIVE_MS		500u	/**< Timeout for the ACTIVE transition (includes PLL lock). */
/** @} */

/**
 * @name PLL / divider framework
 * Constants describing the fixed VCO frequency and the valid divider ranges
 * of the integer (HSDIV) and fractional (MultiSynth / ID) dividers.
 * @{
 */
#define VCO_HZ				2500000000ULL	/**< Fixed VCO frequency, 2.5 GHz. */
#define HSDIV0_HZ			100000000UL	/**< Fixed HSDIV0 output frequency: VCO / 25. */
#define HSDIV1_HZ			125000000UL	/**< Fixed HSDIV1 output frequency: VCO / 20. */
#define R_MAX				63u		/**< Maximum value of an output R divider (1..63). */
#define ID_N_MIN			10u		/**< Smallest allowed ID (MultiSynth) divider N. */
#define ID_N_MAX			255u		/**< Largest allowed ID (MultiSynth) divider N. */
#define ID_FREQ_MAX_HZ			250000000UL	/**< Maximum ID output frequency (limited by #ID_N_MIN). */
#define ID_DEN_MAX			32767u		/**< Maximum denominator of an ID divider fraction (15 bit). */
/** @} */

#define NUM_CH				4	/**< Number of API-configurable output channels (OUT0, OUT2, OUT4, OUT7). */
#define NUM_ID				2	/**< Number of fractional MultiSynth dividers (ID0, ID1). */

/**
 * @brief Executes @p x and returns its error code if it is not #SI5332_OK.
 * @param x Expression of type si5332_err_t.
 */
#define CHK(x)	do { si5332_err_t _e = (x); if (_e != SI5332_OK) return _e; } while (0)

/* ------------------------------------------------------------------------- */
/* Data types                                                                */
/* ------------------------------------------------------------------------- */

/**
 * @brief Shadow state of one fractional MultiSynth divider (ID0 or ID1).
 *
 * The ID output frequency is `f_id = VCO_HZ / (num/den)`. Several output
 * channels may share one ID as long as the resulting R divider stays
 * within 1..63 (see try_share_id()).
 */
typedef struct {
	u32 num;	/**< Divider numerator; N = num / den (VCO / N = ID output frequency). */
	u32 den;	/**< Divider denominator. */
	u8  refcnt;	/**< Number of output channels currently using this ID. */
} id_state_t;

/**
 * @brief Shadow state of one API-configurable output channel.
 */
typedef struct {
	u8  enabled;	/**< Desired output-enable (OE) state. */
	u8  configured;	/**< Non-zero once a divider has been set (block switched on). */
	u8  fmt;	/**< Signal format, value of #si5332_out_format_t (OUTx_MODE). */
	u8  src;	/**< Divider source, value of #si5332_div_src_t. */
	u8  rdiv;	/**< R divider, 1..63. */
	u32 freq_hz;	/**< Currently configured output frequency in Hz. */
} ch_state_t;

/**
 * @brief Static register/bit layout descriptor for one output channel.
 *
 * One entry per entry in #gChDesc; holds all register addresses and bit
 * positions needed to configure and enable/disable a given output.
 */
typedef struct {
	u8 reg_mode;		/**< Address of the OUTx_MODE register. */
	u8 reg_div;		/**< Address of the OUTx_DIV (R divider) register. */
	u8 reg_omux;		/**< Address of the OMUXx_SEL0/SEL1 register. */
	u8 oe_reg;		/**< Address of the output-enable register (#REG_OE_0_5 or #REG_OE_6_7). */
	u8 oe_bit;		/**< Bit position of this output's OE bit within @ref oe_reg. */
	u8 omux_dis_bit;	/**< Bit position of the OMUXx_DIS bit in #REG_PD_OMUX. */
	u8 out_dis_reg;		/**< Address of the OUTx_DIS power-down register (0xBC or 0xBD). */
	u8 out_dis_bit;		/**< Bit position of the OUTx_DIS bit within @ref out_dis_reg. */
} ch_desc_t;

/**
 * @brief Opaque device handle (definition), see si5332_t in si5332.h.
 *
 * Holds the HAL callbacks, the currently/requested clock source and the
 * complete shadow state (channels + MultiSynths) together with dirty
 * bitmasks that track which parts still need to be written to the device
 * by si5332_commit().
 */
struct si5332_t {
	u8 isReady;		/**< Non-zero once HAL function pointers have been set by si5332_initialize(). */
	u8 isConfigured;	/**< Non-zero once si5332_init() has completed successfully. */
	u8 i2cAddr;		/**< 7-bit I2C address of the device. */
	void *hal_ctx;		/**< Opaque context pointer passed through to the HAL callbacks. */
	si5332_i2c_write_f i2c_write;	/**< HAL register write callback. */
	si5332_i2c_read_f  i2c_read;	/**< HAL register read callback. */

	si5332_clk_src_t clk_src;	/**< Clock source currently active on the device. */
	si5332_clk_src_t clk_src_req;	/**< Clock source requested by the application. */

	ch_state_t ch[NUM_CH];	/**< Shadow state of the configurable output channels. */
	id_state_t id[NUM_ID];	/**< Shadow state of the MultiSynth dividers. */

	u8 dirty_ch;	/**< Bitmask over channel indices with pending (uncommitted) changes. */
	u8 dirty_id;	/**< Bitmask over MultiSynth indices with pending (uncommitted) changes. */
};

/** @brief Single static device instance backing si5332_initialize(). */
static struct si5332_t gDevice;

/**
 * @brief Static channel descriptor table.
 *
 * Index order matches ch_index(): OUT0, OUT2, OUT4, OUT7.
 */
static const ch_desc_t gChDesc[NUM_CH] = {
	/* mode  div   omux  oe_reg      oe_bit omux_dis out_dis_reg out_dis_bit */
	{ 0x7A, 0x7B, 0x25, REG_OE_0_5, 0,     0,        0xBC,       0 },	/* OUT0 / OMUX0 */
	{ 0x89, 0x8A, 0x27, REG_OE_0_5, 3,     2,        0xBC,       3 },	/* OUT2 / OMUX2 */
	{ 0x98, 0x99, 0x28, REG_OE_0_5, 6,     3,        0xBC,       6 },	/* OUT4 / OMUX3 */
	{ 0xAC, 0xAD, 0x2A, REG_OE_6_7, 2,     5,        0xBD,       2 },	/* OUT7 / OMUX5 */
};

/**
 * @brief Base register addresses of the ID0A / ID1A divider triples.
 *
 * For index @c i, `INTG` is at `gIdBase[i]`, `RES` at `gIdBase[i] + 2` and
 * `DEN` at `gIdBase[i] + 4`; each value is 15 bit, big-endian.
 */
static const u8 gIdBase[NUM_ID] = { 0x36, 0x4E };

/* ------------------------------------------------------------------------- */
/* Low-level                                                                 */
/* ------------------------------------------------------------------------- */

/**
 * @brief Writes a single 8-bit register via the HAL write callback.
 * @param d    Device handle.
 * @param addr Register address.
 * @param val  Value to write.
 * @return #SI5332_OK on success, #SI5332_E_I2C on a HAL/bus error.
 */
static si5332_err_t reg_write(si5332_t *d, u8 addr, u8 val)
{
	u8 v = val;

	if (d->i2c_write(d->hal_ctx, d->i2cAddr, addr, &v) != SI5332_OK)
		return SI5332_E_I2C;
	return SI5332_OK;
}

/**
 * @brief Reads a single 8-bit register via the HAL read callback.
 * @param d    Device handle.
 * @param addr Register address.
 * @param[out] val Receives the register value on success.
 * @return #SI5332_OK on success, #SI5332_E_I2C on a HAL/bus error.
 */
static si5332_err_t reg_read(si5332_t *d, u8 addr, u8 *val)
{
	if (d->i2c_read(d->hal_ctx, d->i2cAddr, addr, val) != SI5332_OK)
		return SI5332_E_I2C;
	return SI5332_OK;
}

/**
 * @brief Read-modify-write of selected bits of a register.
 * @param d    Device handle.
 * @param addr Register address.
 * @param mask Bitmask of the bits to modify.
 * @param val  New value for the masked bits (unmasked bits are ignored).
 * @return #SI5332_OK on success, #SI5332_E_I2C on a HAL/bus error.
 */
static si5332_err_t reg_rmw(si5332_t *d, u8 addr, u8 mask, u8 val)
{
	u8 v;

	CHK(reg_read(d, addr, &v));
	v = (u8)((v & ~mask) | (val & mask));
	return reg_write(d, addr, v);
}

/**
 * @brief Writes a 15-bit value across two registers, big-endian.
 *
 * The high byte (bits 14:8) is written to @p addr, the low byte (bits 7:0)
 * to `addr + 1`. Used for the ID divider triples (INTG/RES/DEN) and the PLL
 * feedback divider.
 *
 * @param d    Device handle.
 * @param addr Address of the high byte; the low byte is written to `addr + 1`.
 * @param val  15-bit value to write.
 * @return #SI5332_OK on success, #SI5332_E_I2C on a HAL/bus error.
 */
static si5332_err_t reg_write16(si5332_t *d, u8 addr, u16 val)
{
	CHK(reg_write(d, addr,     (u8)((val >> 8) & 0x7F)));
	return reg_write(d, (u8)(addr + 1), (u8)(val & 0xFF));
}

/**
 * @brief Writes a whole table of (address, value) pairs to the device.
 * @param d Device handle.
 * @param t Pointer to the table of register entries.
 * @param n Number of entries in @p t.
 * @return #SI5332_OK on success, #SI5332_E_I2C on the first HAL/bus error.
 */
static si5332_err_t write_table(si5332_t *d, const si5332_reg_t *t, u32 n)
{
	for (u32 i = 0; i < n; i++)
		CHK(reg_write(d, t[i].addr, t[i].val));
	return SI5332_OK;
}

/**
 * @brief Switches the device state (READY/ACTIVE) and waits for confirmation.
 *
 * Writes @p state to #REG_USYS_CTRL and then polls #REG_USYS_STAT until it
 * reports the same value, up to @p timeout_ms milliseconds (one poll per
 * millisecond via bsp_uDelay()).
 *
 * @param d          Device handle.
 * @param state      Requested state (#USYS_READY or #USYS_ACTIVE).
 * @param timeout_ms Maximum time to wait, in milliseconds.
 * @return #SI5332_OK if @p state was reached; #SI5332_E_NO_INPUT if the
 *         device reports #USYS_NO_INPUT instead; #SI5332_E_TIMEOUT if the
 *         timeout expired without reaching @p state; #SI5332_E_I2C on a
 *         HAL/bus error.
 */
static si5332_err_t goto_state(si5332_t *d, u8 state, u32 timeout_ms)
{
	u8 stat = 0;

	CHK(reg_write(d, REG_USYS_CTRL, state));

	for (u32 t = 0; ; t++) {
		CHK(reg_read(d, REG_USYS_STAT, &stat));
		if (stat == state)
			return SI5332_OK;
		if (t >= timeout_ms)
			break;
		bsp_uDelay(1000);
	}
	return (stat == USYS_NO_INPUT) ? SI5332_E_NO_INPUT : SI5332_E_TIMEOUT;
}

/* ------------------------------------------------------------------------- */
/* Math helper functions                                                     */
/* ------------------------------------------------------------------------- */

/**
 * @brief Computes the best rational approximation of n/d with a bounded denominator.
 *
 * Uses a continued-fraction expansion with semiconvergents. If n/d is
 * exactly representable with a denominator `<= max_den`, the reduced
 * fraction is returned.
 *
 * @param n       Numerator of the target ratio.
 * @param d       Denominator of the target ratio.
 * @param max_den Maximum allowed denominator of the result.
 * @param[out] out_n Numerator of the approximation.
 * @param[out] out_d Denominator of the approximation (`1 <= out_d <= max_den`).
 */
static void rational_approx(u64 n, u64 d, u32 max_den, u32 *out_n, u32 *out_d)
{
	u64 h2 = 0, h1 = 1;	/* numerators  h(-2), h(-1) */
	u64 k2 = 1, k1 = 0;	/* denominators k(-2), k(-1) */

	while (d != 0) {
		u64 a = n / d;
		u64 r = n % d;
		u64 h = a * h1 + h2;
		u64 k = a * k1 + k2;

		if (k > max_den) {
			/* k1 >= 1, since k(0) = 1 <= max_den */
			u64 as = ((u64)max_den - k2) / k1;

			if (2 * as > a) {	/* semiconvergent is better than the last convergent */
				h1 = as * h1 + h2;
				k1 = as * k1 + k2;
			}
			break;
		}
		h2 = h1;  h1 = h;
		k2 = k1;  k1 = k;
		n = d;    d = r;
	}

	*out_n = (u32)h1;
	*out_d = (u32)k1;
}

/**
 * @brief Encodes a fractional divider N = num/den into the 15-bit register triple.
 *
 * Formula verified against the ID_TEST reference export (no `-512` offset,
 * as stated in the Si5332 reference manual):
 * @code
 *   INTG = floor(128 * num / den)
 *   RES  = (128 * num) mod den
 *   DEN  = den
 * @endcode
 *
 * @param num Divider numerator.
 * @param den Divider denominator.
 * @param[out] intg    INTG register value.
 * @param[out] res     RES register value.
 * @param[out] den_reg DEN register value (equals @p den).
 */
static void id_encode(u32 num, u32 den, u16 *intg, u16 *res, u16 *den_reg)
{
	u64 x = 128ULL * num;

	*intg    = (u16)(x / den);
	*res     = (u16)(x % den);
	*den_reg = (u16)den;
}

/**
 * @brief Checks whether @p base can be integer-divided down to @p f.
 * @param f    Desired output frequency in Hz.
 * @param base Fixed source frequency (#HSDIV0_HZ or #HSDIV1_HZ).
 * @param[out] r Resulting R divider (1..63) on success.
 * @return Non-zero if @p base is evenly divisible by @p f with `1 <= R <= 63`,
 *         zero otherwise (@p r is left unchanged).
 */
static int try_hsdiv(u32 f, u32 base, u8 *r)
{
	if (base % f != 0)
		return 0;
	u32 q = base / f;
	if (q < 1 || q > R_MAX)
		return 0;
	*r = (u8)q;
	return 1;
}

/**
 * @brief Checks whether an already-used MultiSynth can also deliver frequency @p f.
 *
 * Two outputs can share one ID divider as long as `f_id / f` is an integer
 * R divider within 1..63, where `f_id = VCO_HZ * id->den / id->num`.
 *
 * @param id Shadow state of the candidate MultiSynth (must have `refcnt > 0`).
 * @param f  Desired output frequency in Hz.
 * @param[out] r Resulting R divider (1..63) on success.
 * @return Non-zero if @p id can serve @p f, zero otherwise (@p r is left unchanged).
 */
static int try_share_id(const id_state_t *id, u32 f, u8 *r)
{
	u64 a = VCO_HZ * id->den;
	u64 b = (u64)f * id->num;

	if (b == 0 || (a % b) != 0)
		return 0;
	u64 q = a / b;
	if (q < 1 || q > R_MAX)
		return 0;
	*r = (u8)q;
	return 1;
}

/**
 * @brief Plans a fresh (currently unused) MultiSynth divider for frequency @p f.
 *
 * Selects the smallest R divider for which the resulting N = VCO_HZ / (f*R)
 * fits into the 15-bit fractional divider (`#ID_N_MIN <= N < 256`), then
 * approximates N with rational_approx() using a denominator up to
 * #ID_DEN_MAX.
 *
 * @param f       Desired output frequency in Hz.
 * @param[out] num Numerator of the resulting divider N = num/den.
 * @param[out] den Denominator of the resulting divider N = num/den.
 * @param[out] r   Resulting R divider (1..63).
 * @return #SI5332_OK on success, #SI5332_E_FREQ_UNSUPPORTED if no valid
 *         (R, N) combination exists for @p f.
 */
static si5332_err_t plan_id(u32 f, u32 *num, u32 *den, u8 *r)
{
	u64 denom = 255ULL * f;
	u64 rr = (VCO_HZ + denom - 1) / denom;	/* smallest R with N <= 255 */
	u32 n, dd;

	if (rr < 1)
		rr = 1;
	if (rr > R_MAX || (u64)f * rr > ID_FREQ_MAX_HZ)
		return SI5332_E_FREQ_UNSUPPORTED;

	rational_approx(VCO_HZ, (u64)f * rr, ID_DEN_MAX, &n, &dd);
	if (dd == 0 || (u64)n < (u64)ID_N_MIN * dd || (u64)n >= 256ULL * dd)
		return SI5332_E_FREQ_UNSUPPORTED;

	*num = n;
	*den = dd;
	*r   = (u8)rr;
	return SI5332_OK;
}

/**
 * @brief Maps a public channel enum to the internal shadow/descriptor array index.
 * @param ch Output channel.
 * @return Index into #gChDesc / si5332_t::ch (0..#NUM_CH-1), or -1 if @p ch
 *         is not a valid configurable channel.
 */
static int ch_index(si5332_out_ch_t ch)
{
	switch (ch) {
	case SI5332_OUT_0: return 0;
	case SI5332_OUT_2: return 1;
	case SI5332_OUT_4: return 2;
	case SI5332_OUT_7: return 3;
	default:           return -1;
	}
}

/**
 * @brief Checks whether @p fmt is one of the signal formats offered by the API.
 * @param fmt Signal format to validate.
 * @return Non-zero if @p fmt is a supported differential 3.3 V format, zero otherwise.
 */
static int fmt_valid(si5332_out_format_t fmt)
{
	switch (fmt) {
	case SI5332_FMT_LVDS:
	case SI5332_FMT_LVDS_FAST:
	case SI5332_FMT_HCSL_50_EXT:
	case SI5332_FMT_HCSL_50_INT:
	case SI5332_FMT_HCSL_42_5_EXT:
	case SI5332_FMT_HCSL_42_5_INT:
	case SI5332_FMT_LVPECL:
		return 1;
	default:
		return 0;
	}
}

/**
 * @brief Maps a divider source to the OMUXx_SEL1 register field value.
 * @param src Divider source, value of #si5332_div_src_t.
 * @return OMUXx_SEL1 field value (0 = HSDIV0, 1 = HSDIV1, 5 = ID0, 6 = ID1);
 *         returns 0 for #SI5332_DIV_SRC_NONE or unknown values.
 */
static u8 omux_sel1(u8 src)
{
	switch (src) {
	case SI5332_DIV_SRC_HSDIV0: return 0;
	case SI5332_DIV_SRC_HSDIV1: return 1;
	case SI5332_DIV_SRC_ID0:    return 5;
	case SI5332_DIV_SRC_ID1:    return 6;
	default:                    return 0;
	}
}

/**
 * @brief Checks whether the device has any uncommitted shadow changes.
 * @param d Device handle.
 * @return Non-zero if a clock-source change, channel change or MultiSynth
 *         change is pending (i.e. si5332_commit() would do work).
 */
static int has_pending(const si5332_t *d)
{
	return d->dirty_ch || d->dirty_id || (d->clk_src_req != d->clk_src);
}

/**
 * @brief Resets the shadow state to match the base configuration.
 *
 * Called after si5332_init() has successfully written the base and source
 * tables to the device. Sets up the default channel formats, marks OUT0
 * (HSDIV0, 100 MHz) and OUT2 (HSDIV1, 125 MHz) as configured, and clears all
 * dirty flags.
 *
 * @param d   Device handle.
 * @param src Clock source now active on the device.
 */
static void shadow_reset(si5332_t *d, si5332_clk_src_t src)
{
	memset(d->ch, 0, sizeof(d->ch));
	memset(d->id, 0, sizeof(d->id));

	for (int i = 0; i < NUM_CH; i++) {
		d->ch[i].fmt  = SI5332_FMT_LVDS_FAST;
		d->ch[i].rdiv = 1;
		d->ch[i].src  = SI5332_DIV_SRC_NONE;
	}
	/* OUT0: 100 MHz (HSDIV0), OUT2: 125 MHz (HSDIV1), OUT4/OUT7 switched off */
	d->ch[0].configured = 1;
	d->ch[0].src        = SI5332_DIV_SRC_HSDIV0;
	d->ch[0].freq_hz    = HSDIV0_HZ;
	d->ch[1].configured = 1;
	d->ch[1].src        = SI5332_DIV_SRC_HSDIV1;
	d->ch[1].freq_hz    = HSDIV1_HZ;

	d->clk_src     = src;
	d->clk_src_req = src;
	d->dirty_ch    = 0;
	d->dirty_id    = 0;
}

/* ------------------------------------------------------------------------- */
/* Commit helpers (only call in READY mode)                                 */
/* ------------------------------------------------------------------------- */

/**
 * @brief Writes (or powers down) one MultiSynth divider from the shadow state.
 *
 * If `d->id[i].refcnt > 0`, encodes and writes the INTG/RES/DEN register
 * triple via id_encode() and reg_write16(), then clears the corresponding
 * `IDx_DIS` bit in #REG_PD_ID. Otherwise only sets the `IDx_DIS` bit.
 *
 * @param d Device handle.
 * @param i MultiSynth index (0 = ID0, 1 = ID1).
 * @return #SI5332_OK on success, #SI5332_E_I2C on a HAL/bus error.
 * @note Must only be called while the device is in the READY state.
 */
static si5332_err_t write_id(si5332_t *d, int i)
{
	const u8 base = gIdBase[i];
	const u8 bit  = (u8)(1u << (5 + i));	/* ID0_DIS = 0xBA[5], ID1_DIS = 0xBA[6] */

	if (d->id[i].refcnt > 0) {
		u16 intg, res, den;

		id_encode(d->id[i].num, d->id[i].den, &intg, &res, &den);
		CHK(reg_write16(d, base,                 intg));
		CHK(reg_write16(d, (u8)(base + 2), res));
		CHK(reg_write16(d, (u8)(base + 4), den));
		return reg_rmw(d, REG_PD_ID, bit, 0);		/* switch ID on */
	}
	return reg_rmw(d, REG_PD_ID, bit, bit);			/* switch ID off */
}

/**
 * @brief Writes (or powers down) one output channel from the shadow state.
 *
 * Always writes `OUTx_MODE`. If the channel is configured, writes the
 * complete register set (`OUTx_DIV`, OMUX byte, power-down bits) because the
 * CBPro export omits registers of blocks that were unused at export time.
 * If the channel is not configured, only the OMUX and OUTx power-down bits
 * are set to disable the block.
 *
 * @param d Device handle.
 * @param i Channel index (see ch_index()).
 * @return #SI5332_OK on success, #SI5332_E_I2C on a HAL/bus error.
 * @note Must only be called while the device is in the READY state.
 */
static si5332_err_t write_channel(si5332_t *d, int i)
{
	const ch_desc_t  *c = &gChDesc[i];
	const ch_state_t *s = &d->ch[i];
	const u8 omux_bit = (u8)(1u << c->omux_dis_bit);
	const u8 out_bit  = (u8)(1u << c->out_dis_bit);

	CHK(reg_write(d, c->reg_mode, s->fmt));

	if (!s->configured) {
		/* switch block off */
		CHK(reg_rmw(d, REG_PD_OMUX,    omux_bit, omux_bit));
		return reg_rmw(d, c->out_dis_reg, out_bit, out_bit);
	}

	/* Complete register set (the CBPro export omits unused blocks) */
	CHK(reg_write(d, c->reg_div,  s->rdiv));
	CHK(reg_write(d, c->reg_omux, (u8)(omux_sel1(s->src) << 4)));	/* SEL0 = 0 */
	CHK(reg_rmw(d, REG_PD_OMUX,    omux_bit, 0));
	return reg_rmw(d, c->out_dis_reg, out_bit, 0);
}

/**
 * @brief Writes the output-enable registers from the shadow state.
 *
 * OUT6 is always kept enabled (#OE6_BIT); all other channels are enabled
 * according to `ch[i].enabled && ch[i].configured`.
 *
 * @param d Device handle.
 * @return #SI5332_OK on success, #SI5332_E_I2C on a HAL/bus error.
 */
static si5332_err_t oe_apply(si5332_t *d)
{
	u8 b0_5 = 0;
	u8 b6_7 = OE6_BIT;

	for (int i = 0; i < NUM_CH; i++) {
		if (!(d->ch[i].enabled && d->ch[i].configured))
			continue;
		if (gChDesc[i].oe_reg == REG_OE_0_5)
			b0_5 |= (u8)(1u << gChDesc[i].oe_bit);
		else
			b6_7 |= (u8)(1u << gChDesc[i].oe_bit);
	}
	CHK(reg_write(d, REG_OE_0_5, b0_5));
	return reg_write(d, REG_OE_6_7, b6_7);
}

/* ------------------------------------------------------------------------- */
/* API                                                                       */
/* ------------------------------------------------------------------------- */

/**
 * @brief Creates the device handle (no I2C access).
 *
 * See si5332.h for the full API contract.
 *
 * @param i2c_addr 7-bit I2C address of the device.
 * @param w        HAL register write callback, must not be NULL.
 * @param r        HAL register read callback, must not be NULL.
 * @param ctx      Opaque context pointer passed through to @p w and @p r.
 * @return Pointer to the device handle, or NULL if @p w or @p r is NULL.
 */
si5332_t* si5332_initialize(u8 i2c_addr, si5332_i2c_write_f w, si5332_i2c_read_f r, void *ctx)
{
	si5332_t *pDev = &gDevice;

	if (!w || !r)
		return 0;

	memset(pDev, 0, sizeof(*pDev));
	pDev->i2cAddr   = i2c_addr;
	pDev->i2c_write = w;
	pDev->i2c_read  = r;
	pDev->hal_ctx   = ctx;
	pDev->isReady   = 1;

	return pDev;
}

/**
 * @brief Loads the base/source configuration and brings the device to ACTIVE.
 *
 * See si5332.h for the full API contract.
 *
 * @param pDev        Device handle from si5332_initialize().
 * @param initial_src Input clock source to configure.
 * @return #SI5332_OK on success. #SI5332_E_PARAM if @p pDev is NULL or
 *         @p initial_src is invalid. #SI5332_E_NOT_INIT if @p pDev was not
 *         created via si5332_initialize(). #SI5332_E_NO_INPUT if no clock is
 *         detected on @p initial_src. #SI5332_E_TIMEOUT if the device does
 *         not reach READY/ACTIVE in time. #SI5332_E_I2C on a HAL/bus error.
 */
si5332_err_t si5332_init(si5332_t *pDev, si5332_clk_src_t initial_src)
{
	const si5332_reg_t *delta;
	u32 delta_len;

	if (!pDev)
		return SI5332_E_PARAM;
	if (!pDev->isReady)
		return SI5332_E_NOT_INIT;

	switch (initial_src) {
	case SI5332_CLK_SRC_XTAL_27M:
		delta = si5332_src_xtal_27m_regs;
		delta_len = si5332_src_xtal_27m_regs_len;
		break;
	case SI5332_CLK_SRC_CLKIN2_100M:
		delta = si5332_src_clkin2_100m_regs;
		delta_len = si5332_src_clkin2_100m_regs_len;
		break;
	default:
		return SI5332_E_PARAM;
	}

	pDev->isConfigured = 0;

	CHK(goto_state(pDev, USYS_READY, TIMEOUT_READY_MS));
	CHK(write_table(pDev, si5332_base_regs, si5332_base_regs_len));
	CHK(write_table(pDev, delta, delta_len));
	CHK(goto_state(pDev, USYS_ACTIVE, TIMEOUT_ACTIVE_MS));

	shadow_reset(pDev, initial_src);
	pDev->isConfigured = 1;

	return SI5332_OK;
}

/**
 * @brief Applies all pending shadow changes to the device.
 *
 * See si5332.h for the full API contract and plans/si5332_architecture.md
 * (chapter 4.6) for the detailed commit sequence:
 * -# Disable all outputs (#REG_OE_0_5 / #REG_OE_6_7 = 0).
 * -# Switch the device to READY.
 * -# If the clock source changed, write the new source's delta table.
 * -# Write dirty MultiSynths (write_id()), then dirty channels (write_channel()).
 * -# Switch the device to ACTIVE (PLL calibration/lock).
 * -# Restore the output-enable state from the shadow (oe_apply()).
 *
 * @param pDev Device handle.
 * @return #SI5332_OK on success or if nothing was pending. #SI5332_E_PARAM
 *         if @p pDev is NULL. #SI5332_E_NOT_INIT if si5332_init() has not
 *         completed successfully. #SI5332_E_NO_INPUT / #SI5332_E_TIMEOUT if
 *         the READY/ACTIVE transition fails. #SI5332_E_I2C on a HAL/bus error.
 */
si5332_err_t si5332_commit(si5332_t *pDev)
{
	if (!pDev)
		return SI5332_E_PARAM;
	if (!pDev->isConfigured)
		return SI5332_E_NOT_INIT;
	if (!has_pending(pDev))
		return SI5332_OK;

	/* 1. outputs off (clean stop before reprogramming) */
	CHK(reg_write(pDev, REG_OE_0_5, 0x00));
	CHK(reg_write(pDev, REG_OE_6_7, 0x00));

	/* 2. READY: input, divider and driver registers are only writable here */
	CHK(goto_state(pDev, USYS_READY, TIMEOUT_READY_MS));

	/* 3. clock source */
	if (pDev->clk_src_req != pDev->clk_src) {
		if (pDev->clk_src_req == SI5332_CLK_SRC_XTAL_27M)
			CHK(write_table(pDev, si5332_src_xtal_27m_regs, si5332_src_xtal_27m_regs_len));
		else
			CHK(write_table(pDev, si5332_src_clkin2_100m_regs, si5332_src_clkin2_100m_regs_len));
		pDev->clk_src = pDev->clk_src_req;	/* device now carries the new configuration */
	}

	/* 4. MultiSynths, then channels */
	for (int i = 0; i < NUM_ID; i++)
		if (pDev->dirty_id & (1u << i))
			CHK(write_id(pDev, i));
	for (int i = 0; i < NUM_CH; i++)
		if (pDev->dirty_ch & (1u << i))
			CHK(write_channel(pDev, i));

	/* 5. ACTIVE: PLL calibration / lock */
	CHK(goto_state(pDev, USYS_ACTIVE, TIMEOUT_ACTIVE_MS));

	/* 6. restore output enable according to shadow */
	CHK(oe_apply(pDev));

	pDev->dirty_ch = 0;
	pDev->dirty_id = 0;

	return SI5332_OK;
}

/**
 * @brief Switches the PLL input clock source.
 *
 * See si5332.h for the full API contract. Internally performs
 * si5332_commit(). If the new source does not provide a detectable clock
 * (or the transition times out), the driver reverts to the previously
 * active source on a best-effort basis.
 *
 * @param pDev Device handle.
 * @param src  Desired input clock source.
 * @return #SI5332_OK on success. #SI5332_E_PARAM if @p pDev is NULL or
 *         @p src is invalid. #SI5332_E_NOT_INIT if si5332_init() has not
 *         completed successfully. #SI5332_E_NO_INPUT / #SI5332_E_TIMEOUT if
 *         @p src does not provide a usable clock (rollback attempted).
 *         #SI5332_E_I2C on a HAL/bus error.
 */
si5332_err_t si5332_set_clock_source(si5332_t *pDev, si5332_clk_src_t src)
{
	si5332_clk_src_t old;
	si5332_err_t err;

	if (!pDev)
		return SI5332_E_PARAM;
	if (!pDev->isConfigured)
		return SI5332_E_NOT_INIT;
	if (src != SI5332_CLK_SRC_XTAL_27M && src != SI5332_CLK_SRC_CLKIN2_100M)
		return SI5332_E_PARAM;

	old = pDev->clk_src;
	pDev->clk_src_req = src;

	err = si5332_commit(pDev);
	if (err == SI5332_E_NO_INPUT || err == SI5332_E_TIMEOUT) {
		/* new source does not provide a clock: revert to the old source (best effort) */
		pDev->clk_src_req = old;
		(void)si5332_commit(pDev);
	}
	return err;
}

/**
 * @brief Enables or disables one output channel.
 *
 * See si5332.h for the full API contract. Takes effect immediately via a
 * direct register read-modify-write if no other change is pending and the
 * channel is already configured; otherwise the request is only recorded in
 * the shadow state and applied by the next si5332_commit().
 *
 * @param pDev   Device handle.
 * @param ch     Output channel.
 * @param enable Non-zero to enable, zero to disable.
 * @return #SI5332_OK on success. #SI5332_E_PARAM if @p pDev is NULL or
 *         @p ch is not a configurable channel. #SI5332_E_NOT_INIT if
 *         si5332_init() has not completed successfully. #SI5332_E_I2C on a
 *         HAL/bus error during the immediate path.
 */
si5332_err_t si5332_set_output_enable(si5332_t *pDev, si5332_out_ch_t ch, u8 enable)
{
	int i;

	if (!pDev)
		return SI5332_E_PARAM;
	if (!pDev->isConfigured)
		return SI5332_E_NOT_INIT;
	i = ch_index(ch);
	if (i < 0)
		return SI5332_E_PARAM;

	pDev->ch[i].enabled = enable ? 1 : 0;

	/* Takes effect immediately only if nothing is pending and the block is configured */
	if (!has_pending(pDev) && pDev->ch[i].configured) {
		const u8 bit = (u8)(1u << gChDesc[i].oe_bit);
		return reg_rmw(pDev, gChDesc[i].oe_reg, bit, enable ? bit : 0);
	}
	return SI5332_OK;
}

/**
 * @brief Schedules the output signal format of one channel.
 *
 * See si5332.h for the full API contract. Only updates the shadow state and
 * marks the channel dirty; the new format is written to the device by the
 * next si5332_commit().
 *
 * @param pDev Device handle.
 * @param ch   Output channel.
 * @param fmt  Desired signal format.
 * @return #SI5332_OK on success. #SI5332_E_PARAM if @p pDev is NULL,
 *         @p ch is not a configurable channel, or @p fmt is not a supported
 *         format (see fmt_valid()). #SI5332_E_NOT_INIT if si5332_init() has
 *         not completed successfully.
 */
si5332_err_t si5332_set_output_format(si5332_t *pDev, si5332_out_ch_t ch, si5332_out_format_t fmt)
{
	int i;

	if (!pDev)
		return SI5332_E_PARAM;
	if (!pDev->isConfigured)
		return SI5332_E_NOT_INIT;
	i = ch_index(ch);
	if (i < 0 || !fmt_valid(fmt))
		return SI5332_E_PARAM;

	pDev->ch[i].fmt = (u8)fmt;
	pDev->dirty_ch |= (u8)(1u << i);
	return SI5332_OK;
}

/**
 * @brief Schedules the output frequency of one channel.
 *
 * See si5332.h and plans/si5332_architecture.md (chapter 4.5) for the full
 * API contract and the routing algorithm. Operates on a local working copy
 * of the MultiSynth shadow state (@c tid) so that the device shadow state
 * (si5332_t::id, si5332_t::ch) remains unchanged if routing fails.
 *
 * Routing order:
 * -# Release the MultiSynth previously used by @p ch, if any (working copy only).
 * -# Try #HSDIV0_HZ via try_hsdiv().
 * -# Try #HSDIV1_HZ via try_hsdiv().
 * -# Try to share an already-used MultiSynth via try_share_id().
 * -# Try to allocate a free MultiSynth via plan_id().
 *
 * Only on success are the working copy and the new channel state written
 * back into @p pDev, and the affected MultiSynths/channel marked dirty.
 *
 * @param pDev    Device handle.
 * @param ch      Output channel.
 * @param freq_hz Desired output frequency in Hz
 *                (#SI5332_FREQ_MIN_HZ .. #SI5332_FREQ_MAX_HZ).
 * @return #SI5332_OK on success. #SI5332_E_PARAM if @p pDev is NULL or
 *         @p ch is not a configurable channel. #SI5332_E_NOT_INIT if
 *         si5332_init() has not completed successfully.
 *         #SI5332_E_FREQ_UNSUPPORTED if @p freq_hz is out of range or no
 *         valid divider combination exists. #SI5332_E_NO_FREE_MS if a
 *         fractional divider would be required but both MultiSynths are
 *         already in use by other channels.
 */
si5332_err_t si5332_set_output_freq(si5332_t *pDev, si5332_out_ch_t ch, u32 freq_hz)
{
	id_state_t tid[NUM_ID];
	ch_state_t *cur;
	u8 src, r = 0;
	int i;

	if (!pDev)
		return SI5332_E_PARAM;
	if (!pDev->isConfigured)
		return SI5332_E_NOT_INIT;
	i = ch_index(ch);
	if (i < 0)
		return SI5332_E_PARAM;
	if (freq_hz < SI5332_FREQ_MIN_HZ || freq_hz > SI5332_FREQ_MAX_HZ)
		return SI5332_E_FREQ_UNSUPPORTED;

	cur = &pDev->ch[i];
	memcpy(tid, pDev->id, sizeof(tid));

	/* Release the previously used MultiSynth (only in the working copy) */
	if (cur->configured && (cur->src == SI5332_DIV_SRC_ID0 || cur->src == SI5332_DIV_SRC_ID1)) {
		id_state_t *old = &tid[cur->src == SI5332_DIV_SRC_ID0 ? 0 : 1];

		if (old->refcnt > 0 && --old->refcnt == 0) {
			old->num = 0;
			old->den = 0;
		}
	}

	if (try_hsdiv(freq_hz, HSDIV0_HZ, &r)) {
		src = SI5332_DIV_SRC_HSDIV0;
	} else if (try_hsdiv(freq_hz, HSDIV1_HZ, &r)) {
		src = SI5332_DIV_SRC_HSDIV1;
	} else {
		int sel = -1;

		/* 1. reuse an already used MultiSynth */
		for (int k = 0; k < NUM_ID; k++) {
			if (tid[k].refcnt > 0 && try_share_id(&tid[k], freq_hz, &r)) {
				sel = k;
				tid[k].refcnt++;
				break;
			}
		}
		/* 2. use a free MultiSynth */
		if (sel < 0) {
			u32 num, den;

			for (int k = 0; k < NUM_ID; k++) {
				if (tid[k].refcnt == 0) {
					sel = k;
					break;
				}
			}
			if (sel < 0)
				return SI5332_E_NO_FREE_MS;
			{
				si5332_err_t e = plan_id(freq_hz, &num, &den, &r);
				if (e != SI5332_OK)
					return e;
			}
			tid[sel].num    = num;
			tid[sel].den    = den;
			tid[sel].refcnt = 1;
		}
		src = (sel == 0) ? SI5332_DIV_SRC_ID0 : SI5332_DIV_SRC_ID1;
	}

	/* Success: adopt the working copy and mark changed IDs */
	for (int k = 0; k < NUM_ID; k++) {
		const id_state_t *o = &pDev->id[k];
		const id_state_t *n = &tid[k];

		if ((o->refcnt > 0) != (n->refcnt > 0) || o->num != n->num || o->den != n->den)
			pDev->dirty_id |= (u8)(1u << k);
	}
	memcpy(pDev->id, tid, sizeof(tid));

	cur->src        = src;
	cur->rdiv       = r;
	cur->freq_hz    = freq_hz;
	cur->configured = 1;
	pDev->dirty_ch |= (u8)(1u << i);

	return SI5332_OK;
}

/**
 * @brief Reads the current device status flags.
 *
 * See si5332.h for the full API contract. Performs two register reads
 * (#REG_USYS_STAT and #REG_STATUS); does not require si5332_init() to have
 * been called, only a valid handle from si5332_initialize().
 *
 * @param pDev Device handle.
 * @param[out] flags Receives the decoded status flags on success.
 * @return #SI5332_OK on success. #SI5332_E_PARAM if @p pDev or @p flags is
 *         NULL. #SI5332_E_NOT_INIT if @p pDev was not created via
 *         si5332_initialize(). #SI5332_E_I2C on a HAL/bus error.
 */
si5332_err_t si5332_get_status(si5332_t *pDev, si5332_status_flags_t *flags)
{
	u8 stat, sts;

	if (!pDev || !flags)
		return SI5332_E_PARAM;
	if (!pDev->isReady)
		return SI5332_E_NOT_INIT;

	CHK(reg_read(pDev, REG_USYS_STAT, &stat));
	CHK(reg_read(pDev, REG_STATUS, &sts));

	flags->usys_stat    = stat;
	flags->is_active    = (stat == USYS_ACTIVE);
	flags->no_input     = (stat == USYS_NO_INPUT);
	flags->vdd_xtal_ok  = (sts >> 7) & 0x01;
	flags->vddo_ok_mask = sts & 0x3F;

	return SI5332_OK;
}

/**
 * @brief Legacy helper, equivalent to `si5332_init(pDev, SI5332_CLK_SRC_XTAL_27M)`.
 * @param pDev Device handle.
 * @return See si5332_init().
 */
si5332_err_t si5332_apply_config(si5332_t *pDev)
{
	return si5332_init(pDev, SI5332_CLK_SRC_XTAL_27M);
}
