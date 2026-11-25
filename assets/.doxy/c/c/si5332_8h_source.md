

# File si5332.h

[**File List**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**si5332**](dir_f47f1daa29e3b569dba4f7284fbb0336.md) **>** [**si5332.h**](si5332_8h.md)

[Go to the documentation of this file](si5332_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


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
    SI5332_E_TIMEOUT          = -3, 
    SI5332_E_NO_INPUT         = -4, 
    SI5332_E_NO_FREE_MS       = -5, 
    SI5332_E_FREQ_UNSUPPORTED = -6, 
    SI5332_E_NOT_INIT         = -7  
} si5332_err_t;

/* ------------------------------------------------------------------------- */
/* Low-level HAL: provided by the BSP                                       */
/* ------------------------------------------------------------------------- */
typedef si5332_err_t (*si5332_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
typedef si5332_err_t (*si5332_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr,       u8 *data);

/* ------------------------------------------------------------------------- */
/* Types                                                                     */
/* ------------------------------------------------------------------------- */

typedef enum {
    SI5332_CLK_SRC_XTAL_27M = 0,    
    SI5332_CLK_SRC_CLKIN2_100M      
} si5332_clk_src_t;

typedef enum {
    SI5332_OUT_0 = 0,   
    SI5332_OUT_2 = 2,   
    SI5332_OUT_4 = 4,   
    SI5332_OUT_7 = 7    
} si5332_out_ch_t;

typedef enum {
    SI5332_FMT_LVDS          = 4,   
    SI5332_FMT_LVDS_FAST     = 6,   
    SI5332_FMT_HCSL_50_EXT   = 8,   
    SI5332_FMT_HCSL_50_INT   = 9,   
    SI5332_FMT_HCSL_42_5_EXT = 10,  
    SI5332_FMT_HCSL_42_5_INT = 11,  
    SI5332_FMT_LVPECL        = 12   
} si5332_out_format_t;

typedef enum {
    SI5332_DIV_SRC_NONE = 0,    
    SI5332_DIV_SRC_HSDIV0,      
    SI5332_DIV_SRC_HSDIV1,      
    SI5332_DIV_SRC_ID0,         
    SI5332_DIV_SRC_ID1          
} si5332_div_src_t;

typedef struct {
    u8 usys_stat;       
    u8 is_active;       
    u8 no_input;        
    u8 vdd_xtal_ok;     
    u8 vddo_ok_mask;    
} si5332_status_flags_t;

typedef struct si5332_t si5332_t;

/* ------------------------------------------------------------------------- */
/* Predefined output frequencies [Hz]                                       */
/* ------------------------------------------------------------------------- */
#define SI5332_25MHZ     25000000UL 
#define SI5332_26MHZ     26000000UL 
#define SI5332_27MHZ     27000000UL 
#define SI5332_50MHZ     50000000UL 
#define SI5332_52MHZ     52000000UL 
#define SI5332_100MHZ   100000000UL 
#define SI5332_108MHZ   108000000UL 
#define SI5332_125MHZ   125000000UL 
#define SI5332_135MHZ   135000000UL 
#define SI5332_150MHZ   150000000UL 

#define SI5332_FREQ_MIN_HZ    5000000UL 
#define SI5332_FREQ_MAX_HZ  250000000UL 

/* ------------------------------------------------------------------------- */
/* API                                                                       */
/* ------------------------------------------------------------------------- */

si5332_t* si5332_initialize(u8 i2c_addr, si5332_i2c_write_f w, si5332_i2c_read_f r, void *ctx);

si5332_err_t si5332_init(si5332_t *pDev, si5332_clk_src_t initial_src);

si5332_err_t si5332_set_clock_source(si5332_t *pDev, si5332_clk_src_t src);

si5332_err_t si5332_set_output_enable(si5332_t *pDev, si5332_out_ch_t ch, u8 enable);

si5332_err_t si5332_set_output_format(si5332_t *pDev, si5332_out_ch_t ch, si5332_out_format_t fmt);

si5332_err_t si5332_set_output_freq(si5332_t *pDev, si5332_out_ch_t ch, u32 freq_hz);

si5332_err_t si5332_commit(si5332_t *pDev);

si5332_err_t si5332_get_status(si5332_t *pDev, si5332_status_flags_t *flags);

si5332_err_t si5332_apply_config(si5332_t *pDev);

#endif /* SRC_SI5332_H_ */
```


