/*******************************************************************************
*
* @file si5332_cfg.h
*
* @brief Static configuration tables for the Si5332-GM2 Rev D (VCO 2.5 GHz).
*
*        Source: ClockBuilder Pro exports (Design ID UNVBB1)
*          - docs/si5332/Si5332-GM2-RevD-UNVBB1-27MHz_XTAL-Registers.h
*          - docs/si5332/Si5332-GM2-RevD-UNVBB1-100MHz_ClkIn2-Registers.h
*
*        Structure: base table (identical in both exports) + one delta per
*        clock source. The pre-/postamble (register 0x06) is NOT included;
*        it is controlled by the driver (READY / ACTIVE).
*
*******************************************************************************/

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
extern const si5332_reg_t si5332_base_regs[];
extern const u32          si5332_base_regs_len;
/** @} */

/**
 * @name Delta for 27 MHz crystal on XA/XB
 * @{
 */
extern const si5332_reg_t si5332_src_xtal_27m_regs[];
extern const u32          si5332_src_xtal_27m_regs_len;
/** @} */

/**
 * @name Delta for 100 MHz differential clock on CLKIN_2
 * @{
 */
extern const si5332_reg_t si5332_src_clkin2_100m_regs[];
extern const u32          si5332_src_clkin2_100m_regs_len;
/** @} */

#endif /* SI5332_CFG_H_ */
