

# File si5332.c

[**File List**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**si5332**](dir_f47f1daa29e3b569dba4f7284fbb0336.md) **>** [**si5332.c**](si5332_8c.md)

[Go to the documentation of this file](si5332_8c.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#include <string.h>

#include "si5332.h"
#include "si5332_cfg.h"
#include "bsp.h"        

/* ------------------------------------------------------------------------- */
/* Registers / constants                                                    */
/* ------------------------------------------------------------------------- */

#define REG_STATUS          0x05    
#define REG_USYS_CTRL           0x06    
#define REG_USYS_STAT           0x07    
#define REG_OE_0_5          0xB6    
#define REG_OE_6_7          0xB7    
#define REG_PD_ID           0xBA    
#define REG_PD_OMUX         0xBB    

#define USYS_READY          0x01    
#define USYS_ACTIVE         0x02    
#define USYS_NO_INPUT           0x89    

#define OE6_BIT             (1u << 1)   

#define TIMEOUT_READY_MS        20u 
#define TIMEOUT_ACTIVE_MS       500u    

#define VCO_HZ              2500000000ULL   
#define HSDIV0_HZ           100000000UL 
#define HSDIV1_HZ           125000000UL 
#define R_MAX               63u     
#define ID_N_MIN            10u     
#define ID_N_MAX            255u        
#define ID_FREQ_MAX_HZ          250000000UL 
#define ID_DEN_MAX          32767u      

#define NUM_CH              4   
#define NUM_ID              2   

#define CHK(x)  do { si5332_err_t _e = (x); if (_e != SI5332_OK) return _e; } while (0)

/* ------------------------------------------------------------------------- */
/* Data types                                                                */
/* ------------------------------------------------------------------------- */

typedef struct {
    u32 num;    
    u32 den;    
    u8  refcnt; 
} id_state_t;

typedef struct {
    u8  enabled;    
    u8  configured; 
    u8  fmt;    
    u8  src;    
    u8  rdiv;   
    u32 freq_hz;    
} ch_state_t;

typedef struct {
    u8 reg_mode;        
    u8 reg_div;     
    u8 reg_omux;        
    u8 oe_reg;      
    u8 oe_bit;      
    u8 omux_dis_bit;    
    u8 out_dis_reg;     
    u8 out_dis_bit;     
} ch_desc_t;

struct si5332_t {
    u8 isReady;     
    u8 isConfigured;    
    u8 i2cAddr;     
    void *hal_ctx;      
    si5332_i2c_write_f i2c_write;   
    si5332_i2c_read_f  i2c_read;    

    si5332_clk_src_t clk_src;   
    si5332_clk_src_t clk_src_req;   

    ch_state_t ch[NUM_CH];  
    id_state_t id[NUM_ID];  

    u8 dirty_ch;    
    u8 dirty_id;    
};

static struct si5332_t gDevice;

static const ch_desc_t gChDesc[NUM_CH] = {
    /* mode  div   omux  oe_reg      oe_bit omux_dis out_dis_reg out_dis_bit */
    { 0x7A, 0x7B, 0x25, REG_OE_0_5, 0,     0,        0xBC,       0 },   /* OUT0 / OMUX0 */
    { 0x89, 0x8A, 0x27, REG_OE_0_5, 3,     2,        0xBC,       3 },   /* OUT2 / OMUX2 */
    { 0x98, 0x99, 0x28, REG_OE_0_5, 6,     3,        0xBC,       6 },   /* OUT4 / OMUX3 */
    { 0xAC, 0xAD, 0x2A, REG_OE_6_7, 2,     5,        0xBD,       2 },   /* OUT7 / OMUX5 */
};

static const u8 gIdBase[NUM_ID] = { 0x36, 0x4E };

/* ------------------------------------------------------------------------- */
/* Low-level                                                                 */
/* ------------------------------------------------------------------------- */

static si5332_err_t reg_write(si5332_t *d, u8 addr, u8 val)
{
    u8 v = val;

    if (d->i2c_write(d->hal_ctx, d->i2cAddr, addr, &v) != SI5332_OK)
        return SI5332_E_I2C;
    return SI5332_OK;
}

static si5332_err_t reg_read(si5332_t *d, u8 addr, u8 *val)
{
    if (d->i2c_read(d->hal_ctx, d->i2cAddr, addr, val) != SI5332_OK)
        return SI5332_E_I2C;
    return SI5332_OK;
}

static si5332_err_t reg_rmw(si5332_t *d, u8 addr, u8 mask, u8 val)
{
    u8 v;

    CHK(reg_read(d, addr, &v));
    v = (u8)((v & ~mask) | (val & mask));
    return reg_write(d, addr, v);
}

static si5332_err_t reg_write16(si5332_t *d, u8 addr, u16 val)
{
    CHK(reg_write(d, addr,     (u8)((val >> 8) & 0x7F)));
    return reg_write(d, (u8)(addr + 1), (u8)(val & 0xFF));
}

static si5332_err_t write_table(si5332_t *d, const si5332_reg_t *t, u32 n)
{
    for (u32 i = 0; i < n; i++)
        CHK(reg_write(d, t[i].addr, t[i].val));
    return SI5332_OK;
}

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

static void rational_approx(u64 n, u64 d, u32 max_den, u32 *out_n, u32 *out_d)
{
    u64 h2 = 0, h1 = 1; /* numerators  h(-2), h(-1) */
    u64 k2 = 1, k1 = 0; /* denominators k(-2), k(-1) */

    while (d != 0) {
        u64 a = n / d;
        u64 r = n % d;
        u64 h = a * h1 + h2;
        u64 k = a * k1 + k2;

        if (k > max_den) {
            /* k1 >= 1, since k(0) = 1 <= max_den */
            u64 as = ((u64)max_den - k2) / k1;

            if (2 * as > a) {   /* semiconvergent is better than the last convergent */
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

static void id_encode(u32 num, u32 den, u16 *intg, u16 *res, u16 *den_reg)
{
    u64 x = 128ULL * num;

    *intg    = (u16)(x / den);
    *res     = (u16)(x % den);
    *den_reg = (u16)den;
}

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

static si5332_err_t plan_id(u32 f, u32 *num, u32 *den, u8 *r)
{
    u64 denom = 255ULL * f;
    u64 rr = (VCO_HZ + denom - 1) / denom;  /* smallest R with N <= 255 */
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

static int has_pending(const si5332_t *d)
{
    return d->dirty_ch || d->dirty_id || (d->clk_src_req != d->clk_src);
}

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

static si5332_err_t write_id(si5332_t *d, int i)
{
    const u8 base = gIdBase[i];
    const u8 bit  = (u8)(1u << (5 + i));    /* ID0_DIS = 0xBA[5], ID1_DIS = 0xBA[6] */

    if (d->id[i].refcnt > 0) {
        u16 intg, res, den;

        id_encode(d->id[i].num, d->id[i].den, &intg, &res, &den);
        CHK(reg_write16(d, base,                 intg));
        CHK(reg_write16(d, (u8)(base + 2), res));
        CHK(reg_write16(d, (u8)(base + 4), den));
        return reg_rmw(d, REG_PD_ID, bit, 0);       /* switch ID on */
    }
    return reg_rmw(d, REG_PD_ID, bit, bit);         /* switch ID off */
}

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
    CHK(reg_write(d, c->reg_omux, (u8)(omux_sel1(s->src) << 4)));   /* SEL0 = 0 */
    CHK(reg_rmw(d, REG_PD_OMUX,    omux_bit, 0));
    return reg_rmw(d, c->out_dis_reg, out_bit, 0);
}

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

/* Public API: documented in si5332.h only. */
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

/*
 * Commit sequence (implementation detail, API contract in si5332.h):
 *  1. Disable all outputs (REG_OE_0_5 / REG_OE_6_7 = 0).
 *  2. Switch the device to READY.
 *  3. If the clock source changed, write the new source's delta table.
 *  4. Write dirty MultiSynths (write_id()), then dirty channels (write_channel()).
 *  5. Switch the device to ACTIVE (PLL calibration/lock).
 *  6. Restore the output-enable state from the shadow (oe_apply()).
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
        pDev->clk_src = pDev->clk_src_req;  /* device now carries the new configuration */
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

/*
 * Implementation detail (API contract and routing order in si5332.h):
 * operates on a local working copy of the MultiSynth shadow state (tid) so
 * that the device shadow state remains unchanged if routing fails. Only on
 * success are the working copy and the new channel state written back and
 * the affected MultiSynths/channel marked dirty.
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

si5332_err_t si5332_apply_config(si5332_t *pDev)
{
    return si5332_init(pDev, SI5332_CLK_SRC_XTAL_27M);
}
```


