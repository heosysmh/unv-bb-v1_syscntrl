

# File si5332\_cfg.h

[**File List**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**si5332**](dir_f47f1daa29e3b569dba4f7284fbb0336.md) **>** [**si5332\_cfg.h**](si5332__cfg_8h.md)

[Go to the documentation of this file](si5332__cfg_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SI5332_CFG_H_
#define SI5332_CFG_H_

#include <type.h>

typedef struct {
    u8 addr;    
    u8 val;     
} si5332_reg_t;

extern const si5332_reg_t si5332_base_regs[];
extern const u32          si5332_base_regs_len; 

extern const si5332_reg_t si5332_src_xtal_27m_regs[];
extern const u32          si5332_src_xtal_27m_regs_len; 

extern const si5332_reg_t si5332_src_clkin2_100m_regs[];
extern const u32          si5332_src_clkin2_100m_regs_len;  

#endif /* SI5332_CFG_H_ */
```


