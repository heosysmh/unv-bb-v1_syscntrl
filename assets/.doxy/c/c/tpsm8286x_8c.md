

# File tpsm8286x.c



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**tpsm8286x**](dir_d611db536405966ae215d07169916749.md) **>** [**tpsm8286x.c**](tpsm8286x_8c.md)

[Go to the source code of this file](tpsm8286x_8c_source.md)

_Driver implementation for the TI TPSM8286x step-down power module._ 

* `#include "tpsm8286x.h"`
* `#include "userDef.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**tpsm8286\_t**](structtpsm8286__t.md) <br>_Opaque device handle (definition), see_ [_**tpsm8286\_t**_](structtpsm8286__t.md) _in_[_**tpsm8286x.h**_](tpsm8286x_8h.md) _._ |
| struct | [**tpsm8286\_voltage\_map\_t**](structtpsm8286__voltage__map__t.md) <br>_Linear voltage-to-register mapping descriptor._  |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  const [**tpsm8286\_voltage\_map\_t**](structtpsm8286__voltage__map__t.md) | [**TPSM8286\_VOLTAGEMAP\_10MV**](#variable-tpsm8286_voltagemap_10mv)   = `/* multi line expression */`<br> |


## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**tpsm8286\_t**](structtpsm8286__t.md) | [**gInstances**](#variable-ginstances)  <br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_check\_voltageReg**](#function-tpsm8286_check_voltagereg) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, int \* regOk) <br>_Checks whether the VOUT1 register reads back the programmed value._  |
|  [**tpsm8286\_t**](structtpsm8286__t.md) \* | [**tpsm8286\_initialize**](#function-tpsm8286_initialize) (u32 devId, u8 i2c\_addr, [**tpsm8286\_i2c\_write\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_write_f) w, [**tpsm8286\_i2c\_read\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_read_f) r, void \* ctx) <br>_Creates the device handle for instance_ `devId` _(no I2C access)._ |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_set\_cntrl**](#function-tpsm8286_set_cntrl) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, u8 regVal) <br>_Writes the complete CNTRL register._  |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_set\_enable**](#function-tpsm8286_set_enable) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, int enable) <br>_Enables/disables the output (CNTRL.DEVICE\_ENABLE, read-modify-write)._  |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_set\_voltage**](#function-tpsm8286_set_voltage) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, float voltage) <br>_Sets the output voltage setpoint (VOUT1 register)._  |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_voltage\_to\_reg**](#function-tpsm8286_voltage_to_reg) (float voltage, u8 \* reg\_out) <br>_Converts a voltage to the VOUT1 register value._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  float | [**clampf**](#function-clampf) (float x, float a, float b) <br>_Clamps a float value between two bounds._  |


























## Public Attributes Documentation




### variable TPSM8286\_VOLTAGEMAP\_10MV 

```C++
const tpsm8286_voltage_map_t TPSM8286_VOLTAGEMAP_10MV;
```



Voltage mapping of the 10 mV/LSB device variant: 0.8 V .. 3.35 V. 


        

<hr>
## Public Static Attributes Documentation




### variable gInstances 

```C++
tpsm8286_t gInstances[TPSM8286_NUM_OF_INSTANCES];
```



Static pool of device instances (size from [**userDef.h**](userDef_8h.md)). 


        

<hr>
## Public Functions Documentation




### function tpsm8286\_check\_voltageReg 

_Checks whether the VOUT1 register reads back the programmed value._ 
```C++
tpsm8286_err_t tpsm8286_check_voltageReg (
    tpsm8286_t * pDev,
    int * regOk
) 
```





**Parameters:**


* `pDev` Device handle. 
* `regOk` Receives 1 if the register matches the last setpoint, 0 otherwise. 



**Returns:**

TPSM8286\_OK on success, TPSM8286\_E\_PARAM if `pDev` is NULL, TPSM8286\_E\_I2C on a HAL/bus error. 





        

<hr>



### function tpsm8286\_initialize 

_Creates the device handle for instance_ `devId` _(no I2C access)._
```C++
tpsm8286_t * tpsm8286_initialize (
    u32 devId,
    u8 i2c_addr,
    tpsm8286_i2c_write_f w,
    tpsm8286_i2c_read_f r,
    void * ctx
) 
```



`devId` directly indexes the static instance pool (size [**TPSM8286\_NUM\_OF\_INSTANCES**](userDef_8h.md#define-tpsm8286_num_of_instances), defined in [**userDef.h**](userDef_8h.md)).




**Parameters:**


* `devId` Instance index. 
* `i2c_addr` 7-bit I2C address of the device. 
* `w` HAL register write callback. 
* `r` HAL register read callback. 
* `ctx` Opaque context pointer passed through to `w` and `r`. 



**Returns:**

Pointer to the device handle. 





        

<hr>



### function tpsm8286\_set\_cntrl 

_Writes the complete CNTRL register._ 
```C++
tpsm8286_err_t tpsm8286_set_cntrl (
    tpsm8286_t * pDev,
    u8 regVal
) 
```





**Parameters:**


* `pDev` Device handle. 
* `regVal` Raw control register value (OR of TPSM8286\_\*\_MASK bits). 



**Returns:**

TPSM8286\_OK on success, TPSM8286\_E\_PARAM if `pDev` is NULL, otherwise the HAL error code. 





        

<hr>



### function tpsm8286\_set\_enable 

_Enables/disables the output (CNTRL.DEVICE\_ENABLE, read-modify-write)._ 
```C++
tpsm8286_err_t tpsm8286_set_enable (
    tpsm8286_t * pDev,
    int enable
) 
```





**Parameters:**


* `pDev` Device handle. 
* `enable` Non-zero to enable, zero to disable. 



**Returns:**

TPSM8286\_OK on success, TPSM8286\_E\_PARAM if `pDev` is NULL, TPSM8286\_E\_I2C on a HAL/bus error. 





        

<hr>



### function tpsm8286\_set\_voltage 

_Sets the output voltage setpoint (VOUT1 register)._ 
```C++
tpsm8286_err_t tpsm8286_set_voltage (
    tpsm8286_t * pDev,
    float voltage
) 
```



The voltage is mapped to the 10 mV/LSB register scale and clamped to the supported range (0.8 V .. 3.35 V). Takes effect according to the configured ramp speed; the output enable state is not changed.




**Parameters:**


* `pDev` Device handle. 
* `voltage` Desired output voltage [V]. 



**Returns:**

TPSM8286\_OK on success, TPSM8286\_E\_PARAM if `pDev` is NULL, TPSM8286\_E\_I2C on a HAL/bus error. 





        

<hr>



### function tpsm8286\_voltage\_to\_reg 

_Converts a voltage to the VOUT1 register value._ 
```C++
tpsm8286_err_t tpsm8286_voltage_to_reg (
    float voltage,
    u8 * reg_out
) 
```



The voltage is clamped to the range of [**TPSM8286\_VOLTAGEMAP\_10MV**](tpsm8286x_8c.md#variable-tpsm8286_voltagemap_10mv) and mapped linearly onto the register range with rounding.




**Parameters:**


* `voltage` Desired output voltage [V]. 
* `reg_out` Receives the register value. 



**Returns:**

TPSM8286\_OK on success, TPSM8286\_E\_PARAM if `reg_out` is NULL. 





        

<hr>
## Public Static Functions Documentation




### function clampf 

_Clamps a float value between two bounds._ 
```C++
static inline float clampf (
    float x,
    float a,
    float b
) 
```





**Parameters:**


* `x` Value to clamp. 
* `a` Lower bound. 
* `b` Upper bound. 



**Returns:**

`x` limited to [a, b]. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/tpsm8286x/tpsm8286x.c`

