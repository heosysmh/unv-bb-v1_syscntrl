

# File clkGen.h

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**clkGen.h**](clkGen_8h.md)

[Go to the documentation of this file](clkGen_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_CLKGEN_H_
#define SRC_CLKGEN_H_

#include <type.h>

#include "si5332.h"

typedef enum {
    CLKGEN_OK       =  0,   
    CLKGEN_E_PARAM  = -1,   
    CLKGEN_E_INIT   = -2,   
    CLKGEN_E_DEVICE = -3,   
    CLKGEN_E_BUS    = -4    
} clkGen_err_t;

clkGen_err_t clkGen_init(void);

clkGen_err_t clkGen_configure(void);

clkGen_err_t clkGen_setOutputEnable(si5332_out_ch_t ch, int enable);

clkGen_err_t clkGen_getStatus(si5332_status_flags_t *flags);

#endif /* SRC_CLKGEN_H_ */
```


