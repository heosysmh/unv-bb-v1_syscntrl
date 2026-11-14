/*
 * interrupt.h
 *
 *  Created on: 19.09.2026
 *      Author: matthias
 */

#ifndef SRC_INTERRUPT_H_
#define SRC_INTERRUPT_H_

#include "plic.h"
#include "riscv.h"


#define PLIC_MAX_NUM_INTERRUPTS 36

typedef void (*intrHandler_t)(void *data);

typedef struct intrHandle_t intrHandle_t;

intrHandle_t* intr_initialize(u32 baseAddress);

void intr_setThreshold(intrHandle_t *pInst, u8 hartId, u8 thrshld);
void intr_setEnable(intrHandle_t *pInst, u32 hartNo, u32 intrId);
void intr_serPriority(intrHandle_t *pInst, u32 intrId, u32 prio);
void intr_connect(intrHandle_t *pInst, u8 intrId, intrHandler_t handler, void* pCallback);

#endif /* SRC_INTERRUPT_H_ */
