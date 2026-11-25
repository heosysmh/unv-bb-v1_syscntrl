

# File interrupt.h

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**interrupt.h**](interrupt_8h.md)

[Go to the documentation of this file](interrupt_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_INTERRUPT_H_
#define SRC_INTERRUPT_H_

#include "plic.h"

#define PLIC_MAX_NUM_INTERRUPTS 36

typedef void (*intrHandler_t)(void *data);

typedef struct intrHandle_t intrHandle_t;

intrHandle_t* intr_initialize(u32 baseAddress);

void intr_setThreshold(intrHandle_t *pInst, u8 hartId, u8 thrshld);

void intr_setEnable(intrHandle_t *pInst, u32 hartNo, u32 intrId);

void intr_setPriority(intrHandle_t *pInst, u32 intrId, u32 prio);

void intr_connect(intrHandle_t *pInst, u8 intrId, intrHandler_t handler, void* pCallback);

#endif /* SRC_INTERRUPT_H_ */
```


