
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file si5332_cfg.h
* @brief Static configuration tables for the Si5332-GM2 Rev D (VCO 2.5 GHz).
* @details
*        Source: ClockBuilder Pro exports (Design ID UNVBB1)
*          - docs/si5332/Si5332-GM2-RevD-UNVBB1-27MHz_XTAL-Registers.h
*          - docs/si5332/Si5332-GM2-RevD-UNVBB1-100MHz_ClkIn2-Registers.h
*
*        Structure: base table (identical in both exports) + one delta per
*        clock source. The pre-/postamble (register 0x06) is NOT included;
*        it is controlled by the driver (READY / ACTIVE).
*
*/

#ifndef SI5332_CFG_H_
#define SI5332_CFG_H_

#include <type.h>

/** One (address, value) pair of a static register configuration table. */
typedef struct {
	u8 addr;	/**< 8-bit register address. */
	u8 val;		/**< 8-bit register value. */
} si5332_reg_t;

/**
 * @name Common base configuration
 * VCO 2.5 GHz, OUT0 100 MHz, OUT2 125 MHz, OUT6 25 MHz CMOS.
 * @{
 */
/**
 * @brief Base configuration: all registers identical in both CBPro exports.
 *
 * Power-up state: only OUT6 active (OE), OUT0/OUT2 preconfigured but with
 * OE=0, OUT4/OUT7 and ID0/ID1 switched off.
 */
extern const si5332_reg_t si5332_base_regs[];
extern const u32          si5332_base_regs_len;	/**< Number of entries in #si5332_base_regs. */
/** @} */

/**
 * @name Delta for 27 MHz crystal on XA/XB
 * @{
 */
/** @brief Delta: 27 MHz crystal on XA/XB. Fpfd = 27 MHz, P = 1, M = 92 + 16/27. */
extern const si5332_reg_t si5332_src_xtal_27m_regs[];
extern const u32          si5332_src_xtal_27m_regs_len;	/**< Number of entries in #si5332_src_xtal_27m_regs. */
/** @} */

/**
 * @name Delta for 100 MHz differential clock on CLKIN_2
 * @{
 */
/** @brief Delta: 100 MHz differential clock on CLKIN_2. Fpfd = 50 MHz, P = 2, M = 50. */
extern const si5332_reg_t si5332_src_clkin2_100m_regs[];
extern const u32          si5332_src_clkin2_100m_regs_len;	/**< Number of entries in #si5332_src_clkin2_100m_regs. */
/** @} */

#endif /* SI5332_CFG_H_ */
