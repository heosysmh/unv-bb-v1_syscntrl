

# File interrupt.h



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**interrupt.h**](interrupt_8h.md)

[Go to the source code of this file](interrupt_8h_source.md)

_PLIC-based interrupt controller abstraction for the application._ [More...](#detailed-description)

* `#include "plic.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef struct intrHandle\_t | [**intrHandle\_t**](#typedef-intrhandle_t)  <br> |
| typedef void(\* | [**intrHandler\_t**](#typedef-intrhandler_t)  <br>_Interrupt handler callback type._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**intr\_connect**](#function-intr_connect) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u8 intrId, [**intrHandler\_t**](interrupt_8h.md#typedef-intrhandler_t) handler, void \* pCallback) <br>_Registers a handler for an interrupt source._  |
|  [**intrHandle\_t**](structintrHandle__t.md) \* | [**intr\_initialize**](#function-intr_initialize) (u32 baseAddress) <br>_Initializes the interrupt controller and the RISC-V trap handling._  |
|  void | [**intr\_setEnable**](#function-intr_setenable) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u32 hartNo, u32 intrId) <br>_Enables an interrupt source for a hart._  |
|  void | [**intr\_setPriority**](#function-intr_setpriority) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u32 intrId, u32 prio) <br>_Sets the priority of an interrupt source._  |
|  void | [**intr\_setThreshold**](#function-intr_setthreshold) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u8 hartId, u8 thrshld) <br>_Sets the priority threshold of a hart._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**PLIC\_MAX\_NUM\_INTERRUPTS**](interrupt_8h.md#define-plic_max_num_interrupts)  `36`<br> |

## Detailed Description


Wraps the BSP PLIC driver (mcu/software/standalone/driver/plic.h) and the RISC-V trap handling. A single static instance manages the handler vector table; handlers are registered per interrupt ID with [**intr\_connect()**](interrupt_8h.md#function-intr_connect) and dispatched by the machine-external trap path. 


    
## Public Types Documentation




### typedef intrHandle\_t 

```C++
typedef struct intrHandle_t intrHandle_t;
```



Opaque interrupt controller handle (defined in [**interrupt.c**](interrupt_8c.md)). 


        

<hr>



### typedef intrHandler\_t 

_Interrupt handler callback type._ 
```C++
typedef void(* intrHandler_t) (void *data);
```





**Parameters:**


* `data` Callback reference registered with [**intr\_connect()**](interrupt_8h.md#function-intr_connect). 




        

<hr>
## Public Functions Documentation




### function intr\_connect 

_Registers a handler for an interrupt source._ 
```C++
void intr_connect (
    intrHandle_t * pInst,
    u8 intrId,
    intrHandler_t handler,
    void * pCallback
) 
```





**Parameters:**


* `pInst` Interrupt controller handle. 
* `intrId` Interrupt source ID (&lt; [**PLIC\_MAX\_NUM\_INTERRUPTS**](interrupt_8h.md#define-plic_max_num_interrupts)). 
* `handler` Handler function to invoke. 
* `pCallback` Callback reference passed back to `handler`. 




        

<hr>



### function intr\_initialize 

_Initializes the interrupt controller and the RISC-V trap handling._ 
```C++
intrHandle_t * intr_initialize (
    u32 baseAddress
) 
```



Clears the handler vector table, installs the machine trap vector (trap\_entry) and enables machine external interrupts.




**Parameters:**


* `baseAddress` Base address of the PLIC controller. 



**Returns:**

Pointer to the static interrupt controller instance. 





        

<hr>



### function intr\_setEnable 

_Enables an interrupt source for a hart._ 
```C++
void intr_setEnable (
    intrHandle_t * pInst,
    u32 hartNo,
    u32 intrId
) 
```





**Parameters:**


* `pInst` Interrupt controller handle. 
* `hartNo` Hart (CPU) index. 
* `intrId` Interrupt source ID. 




        

<hr>



### function intr\_setPriority 

_Sets the priority of an interrupt source._ 
```C++
void intr_setPriority (
    intrHandle_t * pInst,
    u32 intrId,
    u32 prio
) 
```





**Parameters:**


* `pInst` Interrupt controller handle. 
* `intrId` Interrupt source ID. 
* `prio` Priority (higher value = higher priority). 




        

<hr>



### function intr\_setThreshold 

_Sets the priority threshold of a hart._ 
```C++
void intr_setThreshold (
    intrHandle_t * pInst,
    u8 hartId,
    u8 thrshld
) 
```



Only interrupts with a priority above the threshold are delivered.




**Parameters:**


* `pInst` Interrupt controller handle. 
* `hartId` Hart (CPU) index. 
* `thrshld` Priority threshold (0 = all priorities enabled). 




        

<hr>
## Macro Definition Documentation





### define PLIC\_MAX\_NUM\_INTERRUPTS 

```C++
#define PLIC_MAX_NUM_INTERRUPTS `36`
```



Maximum number of PLIC interrupt sources supported by the vector table. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/interrupt.h`

