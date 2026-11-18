/*
 * si5332.h
 *
 * Driver for the Skyworks Si5332A-D-GM2 (8 outputs, 40-QFN), fixed VCO at 2.5 GHz,
 * switchable input source (27 MHz crystal / 100 MHz CLKIN_2).
 *
 * Concept:
 *  - Almost all output registers (OUTx_MODE, OUTx_DIV, ID dividers, input/PLL
 *    registers, power-down bits) are only writable in READY mode. Changes are
 *    therefore collected in the driver (si5332_set_output_format / si5332_set_output_freq)
 *    and applied with si5332_commit() in a single READY->ACTIVE cycle.
 *    CAUTION: Every commit briefly interrupts ALL outputs, including OUT6 (25 MHz).
 *  - OUT6 (25 MHz CMOS system clock) is fixed and cannot be changed via the API.
 *  - HSDIV0 (100 MHz) and HSDIV1 (125 MHz) are fixed; additional frequencies are
 *    generated via the two MultiSynths (ID0/ID1, fractional).
 *
 *  Created on: 22.09.2026
 *      Author: matthias
 */

#ifndef SRC_SI5332_H_
#define SRC_SI5332_H_

#include <type.h>

/* ------------------------------------------------------------------------- */
/* Status codes                                                              */
/* ------------------------------------------------------------------------- */
typedef enum {
	SI5332_OK                 =  0,
	SI5332_E_PARAM            = -1,
	SI5332_E_I2C              = -2,
	SI5332_E_TIMEOUT          = -3,	/* USYS_STAT does not reach the desired state */
	SI5332_E_NO_INPUT         = -4,	/* USYS_STAT == 0x89: no input clock detected */
	SI5332_E_NO_FREE_MS       = -5,	/* no MultiSynth (ID0/ID1) free anymore */
	SI5332_E_FREQ_UNSUPPORTED = -6,	/* frequency outside the valid divider range */
	SI5332_E_NOT_INIT         = -7	/* si5332_init() has not yet been called successfully */
} si5332_err_t;

/* ------------------------------------------------------------------------- */
/* Low-level HAL: provided by the BSP                                       */
/* ------------------------------------------------------------------------- */
typedef si5332_err_t (*si5332_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
typedef si5332_err_t (*si5332_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr,       u8 *data);

/* ------------------------------------------------------------------------- */
/* Types                                                                     */
/* ------------------------------------------------------------------------- */

/* PLL input source */
typedef enum {
	SI5332_CLK_SRC_XTAL_27M = 0, 	/* XA/XB 27 MHz crystal */
	SI5332_CLK_SRC_CLKIN2_100M		/* CLKIN_2 100 MHz differential */
} si5332_clk_src_t;

/* Configurable outputs (OUT6 is fixed and intentionally not included) */
typedef enum {
	SI5332_OUT_0 = 0,
	SI5332_OUT_2 = 2,
	SI5332_OUT_4 = 4,
	SI5332_OUT_7 = 7
} si5332_out_ch_t;

/*
 * Signal formats (differential, 3.3 V). The enum value directly matches the
 * OUTx_MODE register value (RM table 7.8).
 * Not offered: 1.8 V LVDS (5/7) and CMOS (1..3).
 */
typedef enum {
	SI5332_FMT_LVDS          = 4,	/* 2.5/3.3 V LVDS */
	SI5332_FMT_LVDS_FAST     = 6,	/* 2.5/3.3 V LVDS fast (CBPro default) */
	SI5332_FMT_HCSL_50_EXT   = 8,	/* HCSL 50 Ohm, external termination */
	SI5332_FMT_HCSL_50_INT   = 9,	/* HCSL 50 Ohm, internal termination */
	SI5332_FMT_HCSL_42_5_EXT = 10,	/* HCSL 42.5 Ohm, external termination */
	SI5332_FMT_HCSL_42_5_INT = 11,	/* HCSL 42.5 Ohm, internal termination */
	SI5332_FMT_LVPECL        = 12	/* LVPECL (VDDO >= 2.25 V) */
} si5332_out_format_t;

/* Divider source of an output */
typedef enum {
	SI5332_DIV_SRC_NONE = 0,	/* not configured */
	SI5332_DIV_SRC_HSDIV0,		/* integer divider O0, 100 MHz */
	SI5332_DIV_SRC_HSDIV1,		/* integer divider O1, 125 MHz */
	SI5332_DIV_SRC_ID0,			/* MultiSynth N0 */
	SI5332_DIV_SRC_ID1			/* MultiSynth N1 */
} si5332_div_src_t;

typedef struct {
	u8 usys_stat;		/* raw value of register 0x07 (0x01 READY, 0x02 ACTIVE, 0x89 no input clock) */
	u8 is_active;		/* usys_stat == 0x02 */
	u8 no_input;		/* usys_stat == 0x89 */
	u8 vdd_xtal_ok;		/* 0x05[7] */
	u8 vddo_ok_mask;	/* 0x05[5:0], bit n = VDDOn > 1.2 V */
} si5332_status_flags_t;

typedef struct si5332_t si5332_t;

/* ------------------------------------------------------------------------- */
/* Predefined output frequencies [Hz]                                       */
/* ------------------------------------------------------------------------- */
#define SI5332_25MHZ	 25000000UL
#define SI5332_26MHZ	 26000000UL
#define SI5332_27MHZ	 27000000UL
#define SI5332_50MHZ	 50000000UL
#define SI5332_52MHZ	 52000000UL
#define SI5332_100MHZ	100000000UL
#define SI5332_108MHZ	108000000UL
#define SI5332_125MHZ	125000000UL
#define SI5332_135MHZ	135000000UL
#define SI5332_150MHZ	150000000UL

/* Valid range for si5332_set_output_freq() */
#define SI5332_FREQ_MIN_HZ	  5000000UL
#define SI5332_FREQ_MAX_HZ	250000000UL

/* ------------------------------------------------------------------------- */
/* API                                                                       */
/* ------------------------------------------------------------------------- */

/* Creates the device handle (no I2C access). */
si5332_t* si5332_initialize(u8 i2c_addr, si5332_i2c_write_f w, si5332_i2c_read_f r, void *ctx);

/*
 * Loads the base and source configuration, switches to ACTIVE and waits for the PLL.
 * Afterwards only OUT6 (25 MHz) is active; OUT0 (100 MHz) and OUT2 (125 MHz) are
 * preconfigured but switched off.
 */
si5332_err_t si5332_init(si5332_t *pDev, si5332_clk_src_t initial_src);

/* Switches the input source. Internally performs si5332_commit(). */
si5332_err_t si5332_set_clock_source(si5332_t *pDev, si5332_clk_src_t src);

/*
 * Output enable. Takes effect immediately if no changes are pending, otherwise
 * with the next si5332_commit(). An unconfigured output (OUT4/OUT7 without
 * set_output_freq) is only activated after configuration and commit.
 */
si5332_err_t si5332_set_output_enable(si5332_t *pDev, si5332_out_ch_t ch, u8 enable);

/* Schedules the signal format (takes effect after si5332_commit()). */
si5332_err_t si5332_set_output_format(si5332_t *pDev, si5332_out_ch_t ch, si5332_out_format_t fmt);

/*
 * Schedules the output frequency (takes effect after si5332_commit()).
 * Routing: HSDIV0 (100 MHz) -> HSDIV1 (125 MHz) -> already used MultiSynth -> free MultiSynth.
 * On error, the pending state remains unchanged.
 */
si5332_err_t si5332_set_output_freq(si5332_t *pDev, si5332_out_ch_t ch, u32 freq_hz);

/* Applies all pending changes (READY -> write registers -> ACTIVE). */
si5332_err_t si5332_commit(si5332_t *pDev);

si5332_err_t si5332_get_status(si5332_t *pDev, si5332_status_flags_t *flags);

/* Legacy: equivalent to si5332_init(pDev, SI5332_CLK_SRC_XTAL_27M). */
si5332_err_t si5332_apply_config(si5332_t *pDev);

#endif /* SRC_SI5332_H_ */
