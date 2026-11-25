

# File interrupt.c



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**interrupt.c**](interrupt_8c.md)

[Go to the source code of this file](interrupt_8c_source.md)

_Implementation of the PLIC interrupt controller abstraction._ [More...](#detailed-description)

* `#include "interrupt.h"`
* `#include "bsp.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**intrHandle\_t**](structintrHandle__t.md) <br>_Interrupt controller instance state (opaque handle, see_ [_**interrupt.h**_](interrupt_8h.md) _)._ |
| struct | [**intr\_vectorTableEntry**](structintr__vectorTableEntry.md) <br> |








## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**intrHandle\_t**](structintrHandle__t.md) | [**gIntrInstance**](#variable-gintrinstance)  <br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**crash**](#function-crash) () <br> |
|  void | [**intr\_connect**](#function-intr_connect) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u8 intrId, [**intrHandler\_t**](interrupt_8h.md#typedef-intrhandler_t) handler, void \* pCallback) <br>_Registers a handler for an interrupt source._  |
|  [**intrHandle\_t**](structintrHandle__t.md) \* | [**intr\_initialize**](#function-intr_initialize) (u32 baseAddress) <br>_Initializes the interrupt controller and the RISC-V trap handling._  |
|  void | [**intr\_setEnable**](#function-intr_setenable) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u32 hartNo, u32 intrId) <br>_Enables an interrupt source for a hart._  |
|  void | [**intr\_setPriority**](#function-intr_setpriority) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u32 intrId, u32 prio) <br>_Sets the priority of an interrupt source._  |
|  void | [**intr\_setThreshold**](#function-intr_setthreshold) ([**intrHandle\_t**](structintrHandle__t.md) \* pInst, u8 hartId, u8 thrshld) <br>_Sets the priority threshold of a hart._  |
|  void | [**trap**](#function-trap) () <br> |
|  void | [**trap\_entry**](#function-trap_entry) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**intr\_external**](#function-intr_external) (void) <br> |


























## Detailed Description


Contains the machine trap dispatcher ([**trap()**](interrupt_8c.md#function-trap)), the machine-external interrupt dispatcher ([**intr\_external()**](interrupt_8c.md#function-intr_external)) and the crash handler. 


    
## Public Static Attributes Documentation




### variable gIntrInstance 

```C++
intrHandle_t gIntrInstance;
```



Single static interrupt controller instance backing [**intr\_initialize()**](interrupt_8c.md#function-intr_initialize). 


        

<hr>
## Public Functions Documentation




### function crash 

```C++
void crash () 
```




<hr>



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



### function trap 

```C++
void trap () 
```




<hr>



### function trap\_entry 

```C++
void trap_entry () 
```




<hr>
## Public Static Functions Documentation




### function intr\_external 

```C++
static void intr_external (
    void
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/interrupt.c`

