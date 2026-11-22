
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file clkGen.h
* @brief Clock generator function group (si5332).
* @details
*        Owns the device handle and applies the configuration defined by the
*        BOARD_CLKGEN_* macros in boardConfig.h. OUT6 (25 MHz CMOS system
*        clock) is fixed in hardware and always enabled.
*
*/

#ifndef SRC_CLKGEN_H_
#define SRC_CLKGEN_H_

#include <type.h>

#include "si5332.h"

/** Status codes of the clock generator function group. */
typedef enum {
	CLKGEN_OK       =  0,	/**< Success. */
	CLKGEN_E_PARAM  = -1,	/**< Invalid argument. */
	CLKGEN_E_INIT   = -2,	/**< Handle could not be created / not initialized. */
	CLKGEN_E_DEVICE = -3,	/**< Device reported an error. */
	CLKGEN_E_BUS    = -4	/**< Bus switch could not be set to the clock generator. */
} clkGen_err_t;

/**
 * @brief Creates the device handle (no bus traffic).
 *
 * Requires i2cBus_init() to have succeeded.
 *
 * @return #CLKGEN_OK on success, #CLKGEN_E_INIT if the handle could not be
 *         created.
 */
clkGen_err_t clkGen_init(void);

/**
 * @brief Applies the board configuration to the clock generator.
 *
 * Selects the bus segment, loads the base configuration with
 * #BOARD_CLKGEN_SRC and applies frequency, format and enable state of
 * OUT0/OUT2/OUT4/OUT7 in a single commit.
 *
 * @return #CLKGEN_OK on success, #CLKGEN_E_INIT if not initialized,
 *         #CLKGEN_E_BUS on a bus-switch error, #CLKGEN_E_DEVICE on a device
 *         error (e.g. unsupported frequency).
 */
clkGen_err_t clkGen_configure(void);

/**
 * @brief Enables/disables a single output at runtime.
 * @param ch     Output channel.
 * @param enable Non-zero to enable, zero to disable.
 * @return #CLKGEN_OK on success, #CLKGEN_E_INIT if not initialized,
 *         #CLKGEN_E_BUS on a bus-switch error, #CLKGEN_E_DEVICE on a device
 *         error.
 */
clkGen_err_t clkGen_setOutputEnable(si5332_out_ch_t ch, int enable);

/**
 * @brief Reads the device status flags (PLL state, input detection, VDDO).
 * @param[out] flags Receives the decoded status flags on success.
 * @return #CLKGEN_OK on success, #CLKGEN_E_PARAM if @p flags is NULL,
 *         #CLKGEN_E_INIT if not initialized, #CLKGEN_E_BUS on a bus-switch
 *         error, #CLKGEN_E_DEVICE on a device error.
 */
clkGen_err_t clkGen_getStatus(si5332_status_flags_t *flags);

#endif /* SRC_CLKGEN_H_ */
