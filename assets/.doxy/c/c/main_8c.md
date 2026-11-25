

# File main.c



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**main.c**](main_8c.md)

[Go to the source code of this file](main_8c_source.md)

_Board bring-up and supervision of the carrier board._ [More...](#detailed-description)

* `#include "bsp.h"`
* `#include "riscv.h"`
* `#include "boardConfig.h"`
* `#include "boardGpio.h"`
* `#include "clkGen.h"`
* `#include "i2cBus.h"`
* `#include "power.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**init\_err\_t**](#enum-init_err_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**main**](#function-main) (void) <br> |
|  void | [**trap\_entry**](#function-trap_entry) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  int | [**board\_bringUp**](#function-board_bringup) (void) <br>_Brings the board up in the required order._  |
|  void | [**cpu\_enableInterrupts**](#function-cpu_enableinterrupts) (void) <br>_Enables machine external interrupts and installs the trap vector._  |


























## Detailed Description


: system controller application


This file only contains the CPU-level startup (trap vector, interrupt enable) and the bring-up sequence. All device handling lives in the function groups:


[**i2cBus.c**](i2cBus_8c.md) I2C controllers, interrupt controller, bus switch [**boardGpio.c**](boardGpio_8c.md) named GPIO access (enables, resets, power-good lines) [**power.c**](power_8c.md) FMC Vadj regulators and power monitors [**clkGen.c**](clkGen_8c.md) clock generator


The startup configuration is in [**boardConfig.h**](boardConfig_8h.md). 


    
## Public Types Documentation




### enum init\_err\_t 

```C++
enum init_err_t {
    INIT_OK =  0,
    INIT_E_CLKGEN = -1,
    INIT_E_PWRMON = -2,
    INIT_E_FMCADJ = -3
};
```



Status codes of the board bring-up sequence. 


        

<hr>
## Public Functions Documentation




### function main 

```C++
void main (
    void
) 
```




<hr>



### function trap\_entry 

```C++
void trap_entry () 
```




<hr>
## Public Static Functions Documentation




### function board\_bringUp 

_Brings the board up in the required order._ 
```C++
static int board_bringUp (
    void
) 
```



Ordering constraints:
* The power monitors must be armed after the SoM power-good wait: their ALERT outputs are wired onto the power-good nets, so a latched alert would keep SYS\_PG low and stall the wait loop.
* They are armed before the FMC supply is switched on so that the FMC rails are already protected while ramping up.






**Returns:**

INIT\_OK / 0 if the whole sequence succeeded, a negative error code otherwise. 





        

<hr>



### function cpu\_enableInterrupts 

_Enables machine external interrupts and installs the trap vector._ 
```C++
static void cpu_enableInterrupts (
    void
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/main.c`

