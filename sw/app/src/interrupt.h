
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file interrupt.h
* @brief PLIC-based interrupt controller abstraction for the application.
* @details
*        Wraps the BSP PLIC driver (mcu/software/standalone/driver/plic.h)
*        and the RISC-V trap handling. A single static instance manages the
*        handler vector table; handlers are registered per interrupt ID with
*        intr_connect() and dispatched by the machine-external trap path.
*
*/

#ifndef SRC_INTERRUPT_H_
#define SRC_INTERRUPT_H_

#include "plic.h"

/** Maximum number of PLIC interrupt sources supported by the vector table. */
#define PLIC_MAX_NUM_INTERRUPTS 36

/**
 * @brief Interrupt handler callback type.
 * @param data Callback reference registered with intr_connect().
 */
typedef void (*intrHandler_t)(void *data);

/** Opaque interrupt controller handle (defined in interrupt.c). */
typedef struct intrHandle_t intrHandle_t;

/**
 * @brief Initializes the interrupt controller and the RISC-V trap handling.
 *
 * Clears the handler vector table, installs the machine trap vector
 * (trap_entry) and enables machine external interrupts.
 *
 * @param baseAddress Base address of the PLIC controller.
 * @return Pointer to the static interrupt controller instance.
 */
intrHandle_t* intr_initialize(u32 baseAddress);

/**
 * @brief Sets the priority threshold of a hart.
 *
 * Only interrupts with a priority above the threshold are delivered.
 *
 * @param pInst   Interrupt controller handle.
 * @param hartId  Hart (CPU) index.
 * @param thrshld Priority threshold (0 = all priorities enabled).
 */
void intr_setThreshold(intrHandle_t *pInst, u8 hartId, u8 thrshld);

/**
 * @brief Enables an interrupt source for a hart.
 * @param pInst  Interrupt controller handle.
 * @param hartNo Hart (CPU) index.
 * @param intrId Interrupt source ID.
 */
void intr_setEnable(intrHandle_t *pInst, u32 hartNo, u32 intrId);

/**
 * @brief Sets the priority of an interrupt source.
 * @param pInst  Interrupt controller handle.
 * @param intrId Interrupt source ID.
 * @param prio   Priority (higher value = higher priority).
 */
void intr_setPriority(intrHandle_t *pInst, u32 intrId, u32 prio);

/**
 * @brief Registers a handler for an interrupt source.
 * @param pInst     Interrupt controller handle.
 * @param intrId    Interrupt source ID (< #PLIC_MAX_NUM_INTERRUPTS).
 * @param handler   Handler function to invoke.
 * @param pCallback Callback reference passed back to @p handler.
 */
void intr_connect(intrHandle_t *pInst, u8 intrId, intrHandler_t handler, void* pCallback);

#endif /* SRC_INTERRUPT_H_ */
