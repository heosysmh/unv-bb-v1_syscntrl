

# File i2cBus.h



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**i2cBus.h**](i2cBus_8h.md)

[Go to the source code of this file](i2cBus_8h_source.md)

_Ownership of the I2C infrastructure of the board._ [More...](#detailed-description)

* `#include "pi4msd5v945x.h"`
* `#include "i2cWrapper.h"`
* `#include "interrupt.h"`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**i2cBus\_err\_t**](#enum-i2cbus_err_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**i2cBus\_err\_t**](i2cBus_8h.md#enum-i2cbus_err_t) | [**i2cBus\_init**](#function-i2cbus_init) (void) <br>_Creates the I2C infrastructure handles._  |
|  [**intrHandle\_t**](structintrHandle__t.md) \* | [**i2cBus\_intr**](#function-i2cbus_intr) (void) <br>_Returns the interrupt controller handle._  |
|  [**i2cHandle\_t**](structi2cHandle__t.md) \* | [**i2cBus\_pmbus**](#function-i2cbus_pmbus) (void) <br>_Returns the PMBus controller handle (power devices)._  |
|  [**i2cBus\_err\_t**](i2cBus_8h.md#enum-i2cbus_err_t) | [**i2cBus\_select**](#function-i2cbus_select) (u8 channelMask) <br>_Selects one or more channels of the SysCfg bus switch._  |
|  [**i2cHandle\_t**](structi2cHandle__t.md) \* | [**i2cBus\_sysCfg**](#function-i2cbus_syscfg) (void) <br>_Returns the SysCfg controller handle (clock generator, add-on modules)._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**I2CBUS\_BUS\_SWITCH\_ADDR**](i2cBus_8h.md#define-i2cbus_bus_switch_addr)  `0x77`<br> |
| define  | [**I2CBUS\_CH\_FMC1**](i2cBus_8h.md#define-i2cbus_ch_fmc1)  `[**PI4MSD5V945X\_CH2**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch2)`<br> |
| define  | [**I2CBUS\_CH\_FMC2**](i2cBus_8h.md#define-i2cbus_ch_fmc2)  `[**PI4MSD5V945X\_CH1**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch1)`<br> |
| define  | [**I2CBUS\_CH\_MBRD**](i2cBus_8h.md#define-i2cbus_ch_mbrd)  `[**PI4MSD5V945X\_CH3**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch3)`<br> |
| define  | [**I2CBUS\_CH\_SYZYGY**](i2cBus_8h.md#define-i2cbus_ch_syzygy)  `[**PI4MSD5V945X\_CH0**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch0)`<br> |
| define  | [**I2CBUS\_CLKGEN\_ADDR**](i2cBus_8h.md#define-i2cbus_clkgen_addr)  `0x6A`<br> |
| define  | [**I2CBUS\_FMC1\_VADJ\_ADDR**](i2cBus_8h.md#define-i2cbus_fmc1_vadj_addr)  `0x4E`<br> |
| define  | [**I2CBUS\_FMC2\_VADJ\_ADDR**](i2cBus_8h.md#define-i2cbus_fmc2_vadj_addr)  `0x4F`<br> |
| define  | [**I2CBUS\_PWRMON\_12V\_ADDR**](i2cBus_8h.md#define-i2cbus_pwrmon_12v_addr)  `0x40`<br> |
| define  | [**I2CBUS\_PWRMON\_1V8\_ADDR**](i2cBus_8h.md#define-i2cbus_pwrmon_1v8_addr)  `0x44`<br> |
| define  | [**I2CBUS\_PWRMON\_3V3\_ADDR**](i2cBus_8h.md#define-i2cbus_pwrmon_3v3_addr)  `0x41`<br> |
| define  | [**I2CBUS\_PWRMON\_5V0\_ADDR**](i2cBus_8h.md#define-i2cbus_pwrmon_5v0_addr)  `0x43`<br> |
| define  | [**I2CBUS\_PWRMON\_FMC1\_ADDR**](i2cBus_8h.md#define-i2cbus_pwrmon_fmc1_addr)  `0x4C`<br> |
| define  | [**I2CBUS\_PWRMON\_FMC2\_ADDR**](i2cBus_8h.md#define-i2cbus_pwrmon_fmc2_addr)  `0x4D`<br> |

## Detailed Description


Owns the interrupt controller handle, both I2C controller instances (PMBus and SysCfg) and the PI4MSD5V945X bus switch that segments the SysCfg bus. Addresses and channels are assigned here.




**Note:**

This file is deliberately not called i2c.h - that name is already used by the BSP driver (mcu/software/standalone/driver/i2c.h) and every src directory is on the include path. 





    
## Public Types Documentation




### enum i2cBus\_err\_t 

```C++
enum i2cBus_err_t {
    I2CBUS_OK =  0,
    I2CBUS_E_INIT = -1,
    I2CBUS_E_SELECT = -2
};
```



Status codes of the I2C infrastructure module. 


        

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
## Macro Definition Documentation





### define I2CBUS\_BUS\_SWITCH\_ADDR 

```C++
#define I2CBUS_BUS_SWITCH_ADDR `0x77`
```



PI4MSD5V945X bus switch. 


        

<hr>



### define I2CBUS\_CH\_FMC1 

```C++
#define I2CBUS_CH_FMC1 `PI4MSD5V945X_CH2`
```



FMC1 segment. 


        

<hr>



### define I2CBUS\_CH\_FMC2 

```C++
#define I2CBUS_CH_FMC2 `PI4MSD5V945X_CH1`
```



FMC2 segment. 


        

<hr>



### define I2CBUS\_CH\_MBRD 

```C++
#define I2CBUS_CH_MBRD `PI4MSD5V945X_CH3`
```



Motherboard segment (clock generator). 


        

<hr>



### define I2CBUS\_CH\_SYZYGY 

```C++
#define I2CBUS_CH_SYZYGY `PI4MSD5V945X_CH0`
```



SYZYGY segment. 


        

<hr>



### define I2CBUS\_CLKGEN\_ADDR 

```C++
#define I2CBUS_CLKGEN_ADDR `0x6A`
```



Si5332 clock generator (behind channel [**I2CBUS\_CH\_MBRD**](i2cBus_8h.md#define-i2cbus_ch_mbrd)). 


        

<hr>



### define I2CBUS\_FMC1\_VADJ\_ADDR 

```C++
#define I2CBUS_FMC1_VADJ_ADDR `0x4E`
```



TPSM8286x FMC1 Vadj regulator. 


        

<hr>



### define I2CBUS\_FMC2\_VADJ\_ADDR 

```C++
#define I2CBUS_FMC2_VADJ_ADDR `0x4F`
```



TPSM8286x FMC2 Vadj regulator. 


        

<hr>



### define I2CBUS\_PWRMON\_12V\_ADDR 

```C++
#define I2CBUS_PWRMON_12V_ADDR `0x40`
```



INA745A on the 12V rail. 


        

<hr>



### define I2CBUS\_PWRMON\_1V8\_ADDR 

```C++
#define I2CBUS_PWRMON_1V8_ADDR `0x44`
```



INA745A on the 1.8V rail. 


        

<hr>



### define I2CBUS\_PWRMON\_3V3\_ADDR 

```C++
#define I2CBUS_PWRMON_3V3_ADDR `0x41`
```



INA745A on the 3.3V rail. 


        

<hr>



### define I2CBUS\_PWRMON\_5V0\_ADDR 

```C++
#define I2CBUS_PWRMON_5V0_ADDR `0x43`
```



INA745A on the 5.0V SoM rail. 


        

<hr>



### define I2CBUS\_PWRMON\_FMC1\_ADDR 

```C++
#define I2CBUS_PWRMON_FMC1_ADDR `0x4C`
```



INA745A on FMC1 Vadj. 


        

<hr>



### define I2CBUS\_PWRMON\_FMC2\_ADDR 

```C++
#define I2CBUS_PWRMON_FMC2_ADDR `0x4D`
```



INA745A on FMC2 Vadj. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/i2cBus.h`

