/*******************************************************************************
*
* @file clkGen.h
*
* @brief Clock generator function group (si5332).
*
*        Owns the device handle and applies the configuration defined by the
*        BOARD_CLKGEN_* macros in boardConfig.h. OUT6 (25 MHz CMOS system
*        clock) is fixed in hardware and always enabled.
*
*******************************************************************************/

#ifndef SRC_CLKGEN_H_
#define SRC_CLKGEN_H_

#include <type.h>

#include "si5332.h"

typedef enum {
	CLKGEN_OK       =  0,
	CLKGEN_E_PARAM  = -1,
	CLKGEN_E_INIT   = -2,	/* handle could not be created / not initialized */
	CLKGEN_E_DEVICE = -3,	/* device reported an error */
	CLKGEN_E_BUS    = -4	/* bus switch could not be set to the clock generator */
} clkGen_err_t;

/* Creates the device handle (no bus traffic). Requires i2cBus_init(). */
clkGen_err_t clkGen_init(void);

/*
 * Selects the bus segment, loads the base configuration with
 * BOARD_CLKGEN_SRC and applies frequency, format and enable state of
 * OUT0/OUT2/OUT4/OUT7 in a single commit.
 */
clkGen_err_t clkGen_configure(void);

/* Enables/disables a single output at runtime. */
clkGen_err_t clkGen_setOutputEnable(si5332_out_ch_t ch, int enable);

/* Reads the device status flags (PLL state, input detection, VDDO). */
clkGen_err_t clkGen_getStatus(si5332_status_flags_t *flags);

#endif /* SRC_CLKGEN_H_ */
