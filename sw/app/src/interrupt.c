
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys

/**
* @file interrupt.c
* @brief Implementation of the PLIC interrupt controller abstraction.
* @details
*        Contains the machine trap dispatcher (trap()), the machine-external
*        interrupt dispatcher (intr_external()) and the crash handler.
*
*/

#include "interrupt.h"
#include "bsp.h"




/* The following data type defines each entry in an interrupt vector table.
 * The callback reference is the base address of the interrupting device
 * for the low level driver and an instance pointer for the high level driver.
 */
typedef struct
{
       intrHandler_t handler; /**< Interrupt Handler */
       void *callBackRef; /**< CallBackRef is the callback reference passed in
                               by the upper layer when setting the Interrupt
                               handler for specific interrupt ID, and it will
                               passed back to Interrupt handler when it is
                               invoked. */
} intr_vectorTableEntry;

/**
 * @brief Interrupt controller instance state (opaque handle, see interrupt.h).
 */
struct intrHandle_t
{
	u32 baseAddress;	/**< Base address of the PLIC controller. */
	u8 	threshold;	/**< Currently configured priority threshold. */
	intr_vectorTableEntry HandlerTable[PLIC_MAX_NUM_INTERRUPTS];/**<
					 Vector table of interrupt handlers */

};

/** Single static interrupt controller instance backing intr_initialize(). */
static intrHandle_t gIntrInstance;

static void intr_external(void);

void crash();
void trap_entry();



/* Public API: documented in interrupt.h only. */
intrHandle_t* intr_initialize(u32 baseAddress)
{
	intrHandle_t *pInst = &gIntrInstance;
	u8 i;

	pInst->baseAddress = baseAddress;

	for(i=0;i<PLIC_MAX_NUM_INTERRUPTS;i++)
		pInst->HandlerTable[i].handler = NULL;

	//configure RISC-V interrupt CSR
	//Set the machine trap vector (trap.S)
	csr_write(mtvec, trap_entry);
	//Enable machine external interrupts
	csr_write(mie, MIE_MEIE);
	//Enable interrupts
	csr_write(mstatus, csr_read(mstatus) | MSTATUS_MPP | MSTATUS_MIE);


	return pInst;
}

void intr_setThreshold(intrHandle_t *pInst, u8 hartId, u8 thrshld)
{
	pInst->threshold = thrshld;
	plic_set_threshold(pInst->baseAddress, hartId, pInst->threshold);
}

void intr_setEnable(intrHandle_t *pInst, u32 hartNo, u32 intrId)
{
	plic_set_enable(pInst->baseAddress, hartNo, intrId, 1);
}

void intr_setPriority(intrHandle_t *pInst, u32 intrId, u32 prio)
{
    plic_set_priority(pInst->baseAddress, intrId, prio);
}

void intr_connect(intrHandle_t *pInst, u8 intrId, intrHandler_t handler, void* pCallback)
{
	pInst->HandlerTable[intrId].handler = handler;
	pInst->HandlerTable[intrId].callBackRef = pCallback;
}

/******************************************************************************
*
* @brief This function handles exceptions and interrupts in the system.
*
* @note It is called by the trap_entry function on both exceptions and interrupts
* 		events. If the cause of the trap is an interrupt, it checks the cause of
* 		the interrupt and calls corresponding interrupt handler functions. If
* 		the cause is an exception or an unhandled interrupt, it calls a
*		crash function to handle the error.
*
******************************************************************************/
void trap(){
    int32_t mcause = csr_read(mcause);
    int32_t interrupt = mcause < 0;
    int32_t cause     = mcause & 0xF;

    if(interrupt){
        switch(cause){
        case CAUSE_MACHINE_EXTERNAL: intr_external(); break;
        default: crash(); break;
        }
    }
    else
    {
      crash();
    }
}

/******************************************************************************
*
* @brief This function handles the system crash scenario by printing a crash message
* 		 and entering an infinite loop.
*
******************************************************************************/
void crash(){
    bsp_printf("\r\n*** CRASH ***\r\n");
    while(1);
}

/******************************************************************************
*
* @brief This function handles I2C interrupts by claiming pending interrupts
* 		 and processing them through i2c_intc().
*
******************************************************************************/
static void intr_external(){
    uint32_t claim;
    //While there is pending interrupts
    while((claim = plic_claim(gIntrInstance.baseAddress, BSP_PLIC_CPU_0)))
    {
    	if(claim >= PLIC_MAX_NUM_INTERRUPTS) crash();
    	if(gIntrInstance.HandlerTable[claim].handler == NULL) crash();

    	gIntrInstance.HandlerTable[claim].handler(gIntrInstance.HandlerTable[claim].callBackRef);
    	//unmask the claimed interrupt
    	plic_release(gIntrInstance.baseAddress, BSP_PLIC_CPU_0, claim);
    }
}


