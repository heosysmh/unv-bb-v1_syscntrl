

# File si5332\_cfg.c

[**File List**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**si5332**](dir_f47f1daa29e3b569dba4f7284fbb0336.md) **>** [**si5332\_cfg.c**](si5332__cfg_8c.md)

[Go to the documentation of this file](si5332__cfg_8c.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#include "si5332_cfg.h"

#define ARRAY_LEN(a)    ((u32)(sizeof(a) / sizeof((a)[0])))

/* Public tables: documented in si5332_cfg.h only. */
const si5332_reg_t si5332_base_regs[] =
{
    /* Design ID "UNVBB1" */
    { 0x17, 0x55 },
    { 0x18, 0x4E },
    { 0x19, 0x56 },
    { 0x1A, 0x42 },
    { 0x1B, 0x42 },
    { 0x1C, 0x31 },

    /* Output muxes / HSDIV (O0 = 25 -> 100 MHz, O1 = 20 -> 125 MHz) */
    { 0x25, 0x00 }, /* OMUX0: HSDIV0 */
    { 0x27, 0x10 }, /* OMUX2: HSDIV1 */
    { 0x29, 0x00 }, /* OMUX4: HSDIV0 (OUT6) */
    { 0x2B, 0x19 }, /* HSDIV0A_DIV = 25 */
    { 0x2D, 0x14 }, /* HSDIV1A_DIV = 20 */

    /* Spread spectrum: off */
    { 0x3C, 0x00 },
    { 0x48, 0x00 },
    { 0x54, 0x00 },
    { 0x60, 0x00 },

    { 0x74, 0x00 }, /* CLKIN_3_CLK_SEL: off */

    /* OUT0: 100 MHz LVDS fast */
    { 0x7A, 0x06 },
    { 0x7B, 0x01 },
    { 0x7C, 0x00 },
    { 0x7D, 0x00 },
    /* OUT1: unused */
    { 0x7F, 0x07 },
    { 0x81, 0x00 },
    { 0x82, 0x00 },
    /* OUT2: 125 MHz LVDS fast */
    { 0x89, 0x06 },
    { 0x8A, 0x01 },
    { 0x8B, 0x00 },
    { 0x8C, 0x00 },
    /* OUT3: unused */
    { 0x8E, 0x07 },
    { 0x90, 0x00 },
    { 0x91, 0x00 },
    /* OUT4: unused (mode LVDS fast preset) */
    { 0x98, 0x06 },
    { 0x9A, 0x00 },
    { 0x9B, 0x00 },
    /* OUT5: unused */
    { 0x9D, 0x07 },
    { 0x9F, 0x00 },
    { 0xA0, 0x00 },
    /* OUT6: 25 MHz LVCMOS (HSDIV0 / R6 = 4), fixed */
    { 0xA7, 0x03 },
    { 0xA8, 0x04 },
    { 0xA9, 0x00 },
    { 0xAA, 0x00 },
    { 0xAB, 0x00 },
    /* OUT7: unused (mode LVDS fast preset) */
    { 0xAC, 0x06 },
    { 0xAE, 0x00 },
    { 0xAF, 0x00 },

    /* Output enable: OUT6 only */
    { 0xB6, 0x00 },
    { 0xB7, 0x02 },

    /* Power-down bits (OUT4, OUT7, OMUX3, OMUX5, ID0, ID1, HSDIV2..4 off) */
    { 0xBA, 0x7C },
    { 0xBB, 0x2A },
    { 0xBC, 0xD2 },
    { 0xBD, 0x04 },

    { 0xBF, 0x00 }, /* XOSC_CINT_ENA */
};
const u32 si5332_base_regs_len = ARRAY_LEN(si5332_base_regs);

const si5332_reg_t si5332_src_xtal_27m_regs[] =
{
    { 0x24, 0x01 }, /* IMUX_SEL = XOSC */
    /* IDPA: feedback divider M = 2500/27, big-endian 15 bit */
    { 0x67, 0x2E }, { 0x68, 0x4B }, /* IDPA_INTG = 11851 */
    { 0x69, 0x00 }, { 0x6A, 0x17 }, /* IDPA_RES  = 23 */
    { 0x6B, 0x00 }, { 0x6C, 0x1B }, /* IDPA_DEN  = 27 */
    { 0x73, 0x00 }, /* CLKIN_2_CLK_SEL = off */
    { 0x75, 0x01 }, /* P_VAL = 1 */
    { 0xB9, 0x06 }, /* XOSC on, IBUF0/IBUF1 off */
    { 0xBE, 0x10 }, /* PLL_MODE (loop BW for Fpfd 27 MHz) */
    { 0xC0, 0x1B }, /* XOSC_CTRIM_XIN */
    { 0xC1, 0x1B }, /* XOSC_CTRIM_XOUT */
};
const u32 si5332_src_xtal_27m_regs_len = ARRAY_LEN(si5332_src_xtal_27m_regs);

const si5332_reg_t si5332_src_clkin2_100m_regs[] =
{
    { 0x24, 0x02 }, /* IMUX_SEL = CLKIN_2 */
    /* IDPA: feedback divider M = 50, big-endian 15 bit */
    { 0x67, 0x19 }, { 0x68, 0x00 }, /* IDPA_INTG = 6400 */
    { 0x69, 0x00 }, { 0x6A, 0x00 }, /* IDPA_RES  = 0 */
    { 0x6B, 0x00 }, { 0x6C, 0x01 }, /* IDPA_DEN  = 1 */
    { 0x73, 0x01 }, /* CLKIN_2_CLK_SEL = differential */
    { 0x75, 0x02 }, /* P_VAL = 2 */
    { 0xB9, 0x05 }, /* XOSC off, IBUF0 on, IBUF1 off */
    { 0xBE, 0x20 }, /* PLL_MODE (loop BW for Fpfd 50 MHz) */
    { 0xC0, 0x00 }, /* XOSC_CTRIM_XIN */
    { 0xC1, 0x00 }, /* XOSC_CTRIM_XOUT */
};
const u32 si5332_src_clkin2_100m_regs_len = ARRAY_LEN(si5332_src_clkin2_100m_regs);
```


