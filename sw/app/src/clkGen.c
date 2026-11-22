
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


/**
* @file clkGen.c
* @brief Implementation of the clock generator function group (SI5332).
*/

#include "clkGen.h"

#include "bsp.h"
#include "boardConfig.h"
#include "i2cBus.h"

/** Per-output configuration, taken from boardConfig.h. */
typedef struct {
	si5332_out_ch_t     ch;		/**< Output channel. */
	u8                  enable;	/**< Enable state. */
	u32                 freq_hz;	/**< Output frequency [Hz]. */
	si5332_out_format_t format;	/**< Signal format. */
} clkGenOutCfg_t;

static const clkGenOutCfg_t gOutCfg[] = {
	{ SI5332_OUT_0, BOARD_CLKGEN_OUT0_EN, BOARD_CLKGEN_OUT0_FREQ_HZ, BOARD_CLKGEN_OUT0_FORMAT },
	{ SI5332_OUT_2, BOARD_CLKGEN_OUT2_EN, BOARD_CLKGEN_OUT2_FREQ_HZ, BOARD_CLKGEN_OUT2_FORMAT },
	{ SI5332_OUT_4, BOARD_CLKGEN_OUT4_EN, BOARD_CLKGEN_OUT4_FREQ_HZ, BOARD_CLKGEN_OUT4_FORMAT },
	{ SI5332_OUT_7, BOARD_CLKGEN_OUT7_EN, BOARD_CLKGEN_OUT7_FREQ_HZ, BOARD_CLKGEN_OUT7_FORMAT }
};

/** Number of configurable outputs in #gOutCfg. */
#define CLKGEN_NUM_OUT	(sizeof(gOutCfg) / sizeof(gOutCfg[0]))

/** Clock generator device handle, created by clkGen_init(). */
static si5332_t *gClkGen = 0;

clkGen_err_t clkGen_init(void)
{
	i2cHandle_t *sysCfg = i2cBus_sysCfg();

	if (!sysCfg)
		return CLKGEN_E_INIT;

	gClkGen = si5332_initialize(I2CBUS_CLKGEN_ADDR,
	                            (si5332_i2c_write_f)i2c_writeReg,
	                            (si5332_i2c_read_f)i2c_readReg,
	                            (void*)sysCfg);
	if (!gClkGen)
		return CLKGEN_E_INIT;

	return CLKGEN_OK;
}

clkGen_err_t clkGen_configure(void)
{
	if (!gClkGen)
		return CLKGEN_E_INIT;

	/* The clock generator sits behind the bus switch on the SysCfg bus. */
	if (i2cBus_select(I2CBUS_CH_MBRD) != I2CBUS_OK)
		return CLKGEN_E_BUS;

	/*
	 * Loads the base configuration and the selected input source. Afterwards
	 * only OUT6 (25 MHz) is running; OUT0/OUT2 are preconfigured but off.
	 */
	if (si5332_init(gClkGen, BOARD_CLKGEN_SRC) != SI5332_OK)
		return CLKGEN_E_DEVICE;

	/*
	 * Schedule format and frequency of all configurable outputs, including
	 * the ones that stay off, so that clkGen_setOutputEnable() can switch
	 * them on later without another commit cycle.
	 */
	for (u32 i = 0; i < CLKGEN_NUM_OUT; i++) {
		if (si5332_set_output_format(gClkGen, gOutCfg[i].ch, gOutCfg[i].format) != SI5332_OK)
			return CLKGEN_E_DEVICE;

		if (si5332_set_output_freq(gClkGen, gOutCfg[i].ch, gOutCfg[i].freq_hz) != SI5332_OK) {
			bsp_printf("ERROR: Clock-Generator OUT%d frequency unsupported\r\n",
			           (int)gOutCfg[i].ch);
			return CLKGEN_E_DEVICE;
		}

		if (si5332_set_output_enable(gClkGen, gOutCfg[i].ch, gOutCfg[i].enable) != SI5332_OK)
			return CLKGEN_E_DEVICE;
	}

	/* Apply everything in one READY -> ACTIVE cycle. */
	if (si5332_commit(gClkGen) != SI5332_OK)
		return CLKGEN_E_DEVICE;

	return CLKGEN_OK;
}

clkGen_err_t clkGen_setOutputEnable(si5332_out_ch_t ch, int enable)
{
	if (!gClkGen)
		return CLKGEN_E_INIT;

	if (i2cBus_select(I2CBUS_CH_MBRD) != I2CBUS_OK)
		return CLKGEN_E_BUS;

	if (si5332_set_output_enable(gClkGen, ch, (u8)(enable ? 1 : 0)) != SI5332_OK)
		return CLKGEN_E_DEVICE;

	return CLKGEN_OK;
}

clkGen_err_t clkGen_getStatus(si5332_status_flags_t *flags)
{
	if (!flags)
		return CLKGEN_E_PARAM;
	if (!gClkGen)
		return CLKGEN_E_INIT;

	if (i2cBus_select(I2CBUS_CH_MBRD) != I2CBUS_OK)
		return CLKGEN_E_BUS;

	if (si5332_get_status(gClkGen, flags) != SI5332_OK)
		return CLKGEN_E_DEVICE;

	return CLKGEN_OK;
}
