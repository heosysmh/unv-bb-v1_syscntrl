

# File i2cBus.c



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**i2cBus.c**](i2cBus_8c.md)

[Go to the source code of this file](i2cBus_8c_source.md)

_Implementation of the I2C infrastructure ownership._ 

* `#include "i2cBus.h"`
* `#include "bsp.h"`
* `#include "boardConfig.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**pi4msd5v945x\_t**](structpi4msd5v945x__t.md) \* | [**gBusSwitch**](#variable-gbusswitch)   = `0`<br> |
|  [**intrHandle\_t**](structintrHandle__t.md) \* | [**gIntr**](#variable-gintr)   = `0`<br> |
|  [**i2cHandle\_t**](structi2cHandle__t.md) \* | [**gPmbus**](#variable-gpmbus)   = `0`<br> |
|  [**i2cHandle\_t**](structi2cHandle__t.md) \* | [**gSysCfg**](#variable-gsyscfg)   = `0`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**i2cBus\_err\_t**](i2cBus_8h.md#enum-i2cbus_err_t) | [**i2cBus\_init**](#function-i2cbus_init) (void) <br>_Creates the I2C infrastructure handles._  |
|  [**intrHandle\_t**](structintrHandle__t.md) \* | [**i2cBus\_intr**](#function-i2cbus_intr) (void) <br>_Returns the interrupt controller handle._  |
|  [**i2cHandle\_t**](structi2cHandle__t.md) \* | [**i2cBus\_pmbus**](#function-i2cbus_pmbus) (void) <br>_Returns the PMBus controller handle (power devices)._  |
|  [**i2cBus\_err\_t**](i2cBus_8h.md#enum-i2cbus_err_t) | [**i2cBus\_select**](#function-i2cbus_select) (u8 channelMask) <br>_Selects one or more channels of the SysCfg bus switch._  |
|  [**i2cHandle\_t**](structi2cHandle__t.md) \* | [**i2cBus\_sysCfg**](#function-i2cbus_syscfg) (void) <br>_Returns the SysCfg controller handle (clock generator, add-on modules)._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**i2cBus\_buildConfig**](#function-i2cbus_buildconfig) ([**i2cHandle\_config\_t**](structi2cHandle__config__t.md) \* cfg, u32 base, u8 devId, u8 intrId, u8 intrPrio, u8 intrHart, u32 coreFreq, u32 ifFreq) <br>_Fills a controller configuration from the board definition._  |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BOARD\_PLIC\_BASE**](i2cBus_8c.md#define-board_plic_base)  `SYSTEM\_PLIC\_CTRL`<br> |
| define  | [**I2C\_PMBUS\_BASE**](i2cBus_8c.md#define-i2c_pmbus_base)  `SYSTEM\_I2C\_1\_IO\_CTRL`<br> |
| define  | [**I2C\_PMBUS\_CORE\_FREQ**](i2cBus_8c.md#define-i2c_pmbus_core_freq)  `SYSTEM\_CLINT\_HZ`<br> |
| define  | [**I2C\_PMBUS\_DEV\_ID**](i2cBus_8c.md#define-i2c_pmbus_dev_id)  `0x1`<br> |
| define  | [**I2C\_PMBUS\_IF\_FREQ**](i2cBus_8c.md#define-i2c_pmbus_if_freq)  `100000U`<br> |
| define  | [**I2C\_PMBUS\_INTR\_HART**](i2cBus_8c.md#define-i2c_pmbus_intr_hart)  `0U`<br> |
| define  | [**I2C\_PMBUS\_INTR\_ID**](i2cBus_8c.md#define-i2c_pmbus_intr_id)  `SYSTEM\_PLIC\_SYSTEM\_I2C\_1\_IO\_INTERRUPT`<br> |
| define  | [**I2C\_PMBUS\_INTR\_PRIO**](i2cBus_8c.md#define-i2c_pmbus_intr_prio)  `1U`<br> |
| define  | [**I2C\_SAMPLING\_DIVIDER**](i2cBus_8c.md#define-i2c_sampling_divider)  `3`<br> |
| define  | [**I2C\_SYSCFG\_BASE**](i2cBus_8c.md#define-i2c_syscfg_base)  `SYSTEM\_I2C\_0\_IO\_CTRL`<br> |
| define  | [**I2C\_SYSCFG\_CORE\_FREQ**](i2cBus_8c.md#define-i2c_syscfg_core_freq)  `SYSTEM\_CLINT\_HZ`<br> |
| define  | [**I2C\_SYSCFG\_DEV\_ID**](i2cBus_8c.md#define-i2c_syscfg_dev_id)  `0x0`<br> |
| define  | [**I2C\_SYSCFG\_IF\_FREQ**](i2cBus_8c.md#define-i2c_syscfg_if_freq)  `100000U`<br> |
| define  | [**I2C\_SYSCFG\_INTR\_HART**](i2cBus_8c.md#define-i2c_syscfg_intr_hart)  `0U`<br> |
| define  | [**I2C\_SYSCFG\_INTR\_ID**](i2cBus_8c.md#define-i2c_syscfg_intr_id)  `SYSTEM\_PLIC\_SYSTEM\_I2C\_0\_IO\_INTERRUPT`<br> |
| define  | [**I2C\_SYSCFG\_INTR\_PRIO**](i2cBus_8c.md#define-i2c_syscfg_intr_prio)  `2U`<br> |
| define  | [**I2C\_TIMEOUT\_DIVIDER**](i2cBus_8c.md#define-i2c_timeout_divider)  `10  /\* timeout = coreFreq / 10 = 0.1 s \*/`<br> |

## Public Static Attributes Documentation




### variable gBusSwitch 

```C++
pi4msd5v945x_t* gBusSwitch;
```



SysCfg bus switch handle. 


        

<hr>



### variable gIntr 

```C++
intrHandle_t* gIntr;
```



Interrupt controller handle. 


        

<hr>



### variable gPmbus 

```C++
i2cHandle_t* gPmbus;
```



PMBus controller handle (power devices). 


        

<hr>



### variable gSysCfg 

```C++
i2cHandle_t* gSysCfg;
```



SysCfg controller handle (clock generator, add-on modules). 


        

<hr>
## Public Functions Documentation




### function i2cBus\_init 

_Creates the I2C infrastructure handles._ 
```C++
i2cBus_err_t i2cBus_init (
    void
) 
```



Creates the interrupt controller, both I2C controllers and the bus switch handle. Must be called before any device module is initialized.




**Returns:**

I2CBUS\_OK on success, I2CBUS\_E\_INIT if a handle could not be created. 





        

<hr>



### function i2cBus\_intr 

_Returns the interrupt controller handle._ 
```C++
intrHandle_t * i2cBus_intr (
    void
) 
```



For future users besides the I2C controllers.




**Returns:**

Handle, or NULL before [**i2cBus\_init()**](i2cBus_8h.md#function-i2cbus_init). 





        

<hr>



### function i2cBus\_pmbus 

_Returns the PMBus controller handle (power devices)._ 
```C++
i2cHandle_t * i2cBus_pmbus (
    void
) 
```





**Returns:**

Handle, or NULL before [**i2cBus\_init()**](i2cBus_8h.md#function-i2cbus_init). 





        

<hr>



### function i2cBus\_select 

_Selects one or more channels of the SysCfg bus switch._ 
```C++
i2cBus_err_t i2cBus_select (
    u8 channelMask
) 
```



The channel mask is passed through unchanged, so several channels may be combined (e.g. [**I2CBUS\_CH\_MBRD**](i2cBus_8h.md#define-i2cbus_ch_mbrd)).




**Parameters:**


* `channelMask` Bitmask of PI4MSD5V945X\_CHx channels. 



**Returns:**

I2CBUS\_OK on success, I2CBUS\_E\_INIT if not initialized, I2CBUS\_E\_SELECT if the bus switch rejected the selection. 





        

<hr>



### function i2cBus\_sysCfg 

_Returns the SysCfg controller handle (clock generator, add-on modules)._ 
```C++
i2cHandle_t * i2cBus_sysCfg (
    void
) 
```





**Returns:**

Handle, or NULL before [**i2cBus\_init()**](i2cBus_8h.md#function-i2cbus_init). 





        

<hr>
## Public Static Functions Documentation




### function i2cBus\_buildConfig 

_Fills a controller configuration from the board definition._ 
```C++
static void i2cBus_buildConfig (
    i2cHandle_config_t * cfg,
    u32 base,
    u8 devId,
    u8 intrId,
    u8 intrPrio,
    u8 intrHart,
    u32 coreFreq,
    u32 ifFreq
) 
```





**Parameters:**


* `cfg` Configuration to fill. 
* `base` Base address of the I2C controller. 
* `devId` Instance index into the wrapper's static pool. 
* `intrId` PLIC interrupt source ID. 
* `intrPrio` PLIC interrupt priority. 
* `intrHart` Hart (CPU) the interrupt is routed to. 
* `coreFreq` Controller input clock frequency [Hz]. 
* `ifFreq` Desired I2C bus frequency [Hz]. 




        

<hr>
## Macro Definition Documentation





### define BOARD\_PLIC\_BASE 

```C++
#define BOARD_PLIC_BASE `SYSTEM_PLIC_CTRL`
```




<hr>



### define I2C\_PMBUS\_BASE 

```C++
#define I2C_PMBUS_BASE `SYSTEM_I2C_1_IO_CTRL`
```




<hr>



### define I2C\_PMBUS\_CORE\_FREQ 

```C++
#define I2C_PMBUS_CORE_FREQ `SYSTEM_CLINT_HZ`
```




<hr>



### define I2C\_PMBUS\_DEV\_ID 

```C++
#define I2C_PMBUS_DEV_ID `0x1`
```




<hr>



### define I2C\_PMBUS\_IF\_FREQ 

```C++
#define I2C_PMBUS_IF_FREQ `100000U`
```




<hr>



### define I2C\_PMBUS\_INTR\_HART 

```C++
#define I2C_PMBUS_INTR_HART `0U`
```




<hr>



### define I2C\_PMBUS\_INTR\_ID 

```C++
#define I2C_PMBUS_INTR_ID `SYSTEM_PLIC_SYSTEM_I2C_1_IO_INTERRUPT`
```




<hr>



### define I2C\_PMBUS\_INTR\_PRIO 

```C++
#define I2C_PMBUS_INTR_PRIO `1U`
```




<hr>



### define I2C\_SAMPLING\_DIVIDER 

```C++
#define I2C_SAMPLING_DIVIDER `3`
```




<hr>



### define I2C\_SYSCFG\_BASE 

```C++
#define I2C_SYSCFG_BASE `SYSTEM_I2C_0_IO_CTRL`
```




<hr>



### define I2C\_SYSCFG\_CORE\_FREQ 

```C++
#define I2C_SYSCFG_CORE_FREQ `SYSTEM_CLINT_HZ`
```




<hr>



### define I2C\_SYSCFG\_DEV\_ID 

```C++
#define I2C_SYSCFG_DEV_ID `0x0`
```




<hr>



### define I2C\_SYSCFG\_IF\_FREQ 

```C++
#define I2C_SYSCFG_IF_FREQ `100000U`
```




<hr>



### define I2C\_SYSCFG\_INTR\_HART 

```C++
#define I2C_SYSCFG_INTR_HART `0U`
```




<hr>



### define I2C\_SYSCFG\_INTR\_ID 

```C++
#define I2C_SYSCFG_INTR_ID `SYSTEM_PLIC_SYSTEM_I2C_0_IO_INTERRUPT`
```




<hr>



### define I2C\_SYSCFG\_INTR\_PRIO 

```C++
#define I2C_SYSCFG_INTR_PRIO `2U`
```




<hr>



### define I2C\_TIMEOUT\_DIVIDER 

```C++
#define I2C_TIMEOUT_DIVIDER `10  /* timeout = coreFreq / 10 = 0.1 s */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/i2cBus.c`

