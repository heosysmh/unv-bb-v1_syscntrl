

# File clkGen.c



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**clkGen.c**](clkGen_8c.md)

[Go to the source code of this file](clkGen_8c_source.md)

_Implementation of the clock generator function group (SI5332)._ 

* `#include "clkGen.h"`
* `#include "bsp.h"`
* `#include "boardConfig.h"`
* `#include "i2cBus.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**clkGenOutCfg\_t**](structclkGenOutCfg__t.md) <br> |








## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**si5332\_t**](structsi5332__t.md) \* | [**gClkGen**](#variable-gclkgen)   = `0`<br> |
|  const [**clkGenOutCfg\_t**](structclkGenOutCfg__t.md) | [**gOutCfg**](#variable-goutcfg)   = `/* multi line expression */`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**clkGen\_err\_t**](clkGen_8h.md#enum-clkgen_err_t) | [**clkGen\_configure**](#function-clkgen_configure) (void) <br>_Applies the board configuration to the clock generator._  |
|  [**clkGen\_err\_t**](clkGen_8h.md#enum-clkgen_err_t) | [**clkGen\_getStatus**](#function-clkgen_getstatus) ([**si5332\_status\_flags\_t**](structsi5332__status__flags__t.md) \* flags) <br>_Reads the device status flags (PLL state, input detection, VDDO)._  |
|  [**clkGen\_err\_t**](clkGen_8h.md#enum-clkgen_err_t) | [**clkGen\_init**](#function-clkgen_init) (void) <br>_Creates the device handle (no bus traffic)._  |
|  [**clkGen\_err\_t**](clkGen_8h.md#enum-clkgen_err_t) | [**clkGen\_setOutputEnable**](#function-clkgen_setoutputenable) ([**si5332\_out\_ch\_t**](si5332_8h.md#enum-si5332_out_ch_t) ch, int enable) <br>_Enables/disables a single output at runtime._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**CLKGEN\_NUM\_OUT**](clkGen_8c.md#define-clkgen_num_out)  `(sizeof([**gOutCfg**](clkGen_8c.md#variable-goutcfg)) / sizeof([**gOutCfg**](clkGen_8c.md#variable-goutcfg)[0]))`<br> |

## Public Static Attributes Documentation




### variable gClkGen 

```C++
si5332_t* gClkGen;
```



Clock generator device handle, created by [**clkGen\_init()**](clkGen_8c.md#function-clkgen_init). 


        

<hr>



### variable gOutCfg 

```C++
const clkGenOutCfg_t gOutCfg[];
```




<hr>
## Public Functions Documentation




### function clkGen\_configure 

_Applies the board configuration to the clock generator._ 
```C++
clkGen_err_t clkGen_configure (
    void
) 
```



Selects the bus segment, loads the base configuration with [**BOARD\_CLKGEN\_SRC**](boardConfig_8h.md#define-board_clkgen_src) and applies frequency, format and enable state of OUT0/OUT2/OUT4/OUT7 in a single commit.




**Returns:**

CLKGEN\_OK on success, CLKGEN\_E\_INIT if not initialized, CLKGEN\_E\_BUS on a bus-switch error, CLKGEN\_E\_DEVICE on a device error (e.g. unsupported frequency). 





        

<hr>



### function clkGen\_getStatus 

_Reads the device status flags (PLL state, input detection, VDDO)._ 
```C++
clkGen_err_t clkGen_getStatus (
    si5332_status_flags_t * flags
) 
```





**Parameters:**


* `flags` Receives the decoded status flags on success. 



**Returns:**

CLKGEN\_OK on success, CLKGEN\_E\_PARAM if `flags` is NULL, CLKGEN\_E\_INIT if not initialized, CLKGEN\_E\_BUS on a bus-switch error, CLKGEN\_E\_DEVICE on a device error. 





        

<hr>



### function clkGen\_init 

_Creates the device handle (no bus traffic)._ 
```C++
clkGen_err_t clkGen_init (
    void
) 
```



Requires [**i2cBus\_init()**](i2cBus_8c.md#function-i2cbus_init) to have succeeded.




**Returns:**

CLKGEN\_OK on success, CLKGEN\_E\_INIT if the handle could not be created. 





        

<hr>



### function clkGen\_setOutputEnable 

_Enables/disables a single output at runtime._ 
```C++
clkGen_err_t clkGen_setOutputEnable (
    si5332_out_ch_t ch,
    int enable
) 
```





**Parameters:**


* `ch` Output channel. 
* `enable` Non-zero to enable, zero to disable. 



**Returns:**

CLKGEN\_OK on success, CLKGEN\_E\_INIT if not initialized, CLKGEN\_E\_BUS on a bus-switch error, CLKGEN\_E\_DEVICE on a device error. 





        

<hr>
## Macro Definition Documentation





### define CLKGEN\_NUM\_OUT 

```C++
#define CLKGEN_NUM_OUT `(sizeof( gOutCfg ) / sizeof( gOutCfg [0]))`
```



Number of configurable outputs in [**gOutCfg**](clkGen_8c.md#variable-goutcfg). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/clkGen.c`

