

# File tpsm8286x.h



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**tpsm8286x**](dir_d611db536405966ae215d07169916749.md) **>** [**tpsm8286x.h**](tpsm8286x_8h.md)

[Go to the source code of this file](tpsm8286x_8h_source.md)

_Driver for the TI TPSM8286x step-down power module with I2C (PMBus-style) output voltage control._ [More...](#detailed-description)

* `#include <type.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**tpsm8286\_err\_t**](#enum-tpsm8286_err_t)  <br> |
| typedef [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t)(\* | [**tpsm8286\_i2c\_read\_f**](#typedef-tpsm8286_i2c_read_f)  <br> |
| typedef [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t)(\* | [**tpsm8286\_i2c\_write\_f**](#typedef-tpsm8286_i2c_write_f)  <br> |
| typedef struct tpsm8286\_t | [**tpsm8286\_t**](#typedef-tpsm8286_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_check\_voltageReg**](#function-tpsm8286_check_voltagereg) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, int \* regOk) <br>_Checks whether the VOUT1 register reads back the programmed value._  |
|  [**tpsm8286\_t**](structtpsm8286__t.md) \* | [**tpsm8286\_initialize**](#function-tpsm8286_initialize) (u32 devId, u8 i2c\_addr, [**tpsm8286\_i2c\_write\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_write_f) w, [**tpsm8286\_i2c\_read\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_read_f) r, void \* ctx) <br>_Creates the device handle for instance_ `devId` _(no I2C access)._ |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_set\_cntrl**](#function-tpsm8286_set_cntrl) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, u8 regVal) <br>_Writes the complete CNTRL register._  |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_set\_enable**](#function-tpsm8286_set_enable) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, int enable) <br>_Enables/disables the output (CNTRL.DEVICE\_ENABLE, read-modify-write)._  |
|  [**tpsm8286\_err\_t**](tpsm8286x_8h.md#enum-tpsm8286_err_t) | [**tpsm8286\_set\_voltage**](#function-tpsm8286_set_voltage) ([**tpsm8286\_t**](structtpsm8286__t.md) \* pDev, float voltage) <br>_Sets the output voltage setpoint (VOUT1 register)._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**TPSM8286\_CNTRL\_REG\_ADDR**](tpsm8286x_8h.md#define-tpsm8286_cntrl_reg_addr)  `0x03`<br> |
| define  | [**TPSM8286\_DEVICE\_ENABLE\_MASK**](tpsm8286x_8h.md#define-tpsm8286_device_enable_mask)  `0x20`<br> |
| define  | [**TPSM8286\_FPWM\_AT\_VC\_MASK**](tpsm8286x_8h.md#define-tpsm8286_fpwm_at_vc_mask)  `0x40`<br> |
| define  | [**TPSM8286\_FPWM\_MASK**](tpsm8286x_8h.md#define-tpsm8286_fpwm_mask)  `0x10`<br> |
| define  | [**TPSM8286\_HICCUP\_EN\_MASK**](tpsm8286x_8h.md#define-tpsm8286_hiccup_en_mask)  `0x04`<br> |
| define  | [**TPSM8286\_OUTPUT\_DISCHARGE\_MASK**](tpsm8286x_8h.md#define-tpsm8286_output_discharge_mask)  `0x08`<br> |
| define  | [**TPSM8286\_RESET\_REGS\_MASK**](tpsm8286x_8h.md#define-tpsm8286_reset_regs_mask)  `0x80`<br> |
| define  | [**TPSM8286\_STATUS\_REG\_ADDR**](tpsm8286x_8h.md#define-tpsm8286_status_reg_addr)  `0x05`<br> |
| define  | [**TPSM8286\_STAT\_HICCUP\_MASK**](tpsm8286x_8h.md#define-tpsm8286_stat_hiccup_mask)  `0x04`<br> |
| define  | [**TPSM8286\_STAT\_TEMPWARM\_MASK**](tpsm8286x_8h.md#define-tpsm8286_stat_tempwarm_mask)  `0x10`<br> |
| define  | [**TPSM8286\_STAT\_UVLO\_MASK**](tpsm8286x_8h.md#define-tpsm8286_stat_uvlo_mask)  `0x01`<br> |
| define  | [**TPSM8286\_VOLTAGE\_RAMP\_SPEED\_10MV**](tpsm8286x_8h.md#define-tpsm8286_voltage_ramp_speed_10mv)  `0x01`<br> |
| define  | [**TPSM8286\_VOLTAGE\_RAMP\_SPEED\_1MV**](tpsm8286x_8h.md#define-tpsm8286_voltage_ramp_speed_1mv)  `0x11`<br> |
| define  | [**TPSM8286\_VOLTAGE\_RAMP\_SPEED\_20MV**](tpsm8286x_8h.md#define-tpsm8286_voltage_ramp_speed_20mv)  `0x00`<br> |
| define  | [**TPSM8286\_VOLTAGE\_RAMP\_SPEED\_5MV**](tpsm8286x_8h.md#define-tpsm8286_voltage_ramp_speed_5mv)  `0x10`<br> |
| define  | [**TPSM8286\_VOLTAGE\_RAMP\_SPEED\_MASK**](tpsm8286x_8h.md#define-tpsm8286_voltage_ramp_speed_mask)  `0x03`<br> |
| define  | [**TPSM8286\_VOUT1\_REG\_ADDR**](tpsm8286x_8h.md#define-tpsm8286_vout1_reg_addr)  `0x01`<br> |
| define  | [**TPSM8286\_VOUT2\_REG\_ADDR**](tpsm8286x_8h.md#define-tpsm8286_vout2_reg_addr)  `0x02`<br> |
| define  | [**TPSM8286\_XXX3\_V\_TO\_REG**](tpsm8286x_8h.md#define-tpsm8286_xxx3_v_to_reg)  <br> |

## Detailed Description


Bus-agnostic core driver: it only uses injected HAL callbacks ([**tpsm8286\_i2c\_write\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_write_f) / [**tpsm8286\_i2c\_read\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_read_f)), never a concrete I2C driver directly. The callback signatures match [**i2c\_writeReg()**](i2cWrapper_8c.md#function-i2c_writereg)/i2c\_readReg() in [**drivers/i2c/i2cWrapper.h**](i2cWrapper_8h.md), so they can be passed directly via a function-pointer cast.


Multiple simultaneous instances are supported: [**tpsm8286\_initialize()**](tpsm8286x_8h.md#function-tpsm8286_initialize) takes an explicit `devId` that indexes the static instance pool (size [**TPSM8286\_NUM\_OF\_INSTANCES**](userDef_8h.md#define-tpsm8286_num_of_instances), defined in [**userDef.h**](userDef_8h.md)). 


    
## Public Types Documentation




### enum tpsm8286\_err\_t 

```C++
enum tpsm8286_err_t {
    TPSM8286_OK = 0,
    TPSM8286_E_PARAM = -1,
    TPSM8286_E_I2C = -2
};
```



Status codes of the regulator driver. 


        

<hr>



### typedef tpsm8286\_i2c\_read\_f 

```C++
typedef tpsm8286_err_t(* tpsm8286_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data);
```




<hr>



### typedef tpsm8286\_i2c\_write\_f 

```C++
typedef tpsm8286_err_t(* tpsm8286_i2c_write_f) (void *ctx, u8 i2c_addr, u8 reg_addr, const u8 *data);
```




<hr>



### typedef tpsm8286\_t 

```C++
typedef struct tpsm8286_t tpsm8286_t;
```



Opaque device handle (defined in [**tpsm8286x.c**](tpsm8286x_8c.md)). 


        

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
## Macro Definition Documentation





### define TPSM8286\_CNTRL\_REG\_ADDR 

```C++
#define TPSM8286_CNTRL_REG_ADDR `0x03`
```



Control register. 


        

<hr>



### define TPSM8286\_DEVICE\_ENABLE\_MASK 

```C++
#define TPSM8286_DEVICE_ENABLE_MASK `0x20`
```



Device/output enable. 


        

<hr>



### define TPSM8286\_FPWM\_AT\_VC\_MASK 

```C++
#define TPSM8286_FPWM_AT_VC_MASK `0x40`
```



Force PWM at voltage change. 


        

<hr>



### define TPSM8286\_FPWM\_MASK 

```C++
#define TPSM8286_FPWM_MASK `0x10`
```



Force PWM mode (no pulse skipping). 


        

<hr>



### define TPSM8286\_HICCUP\_EN\_MASK 

```C++
#define TPSM8286_HICCUP_EN_MASK `0x04`
```



Enable hiccup mode on over-current. 


        

<hr>



### define TPSM8286\_OUTPUT\_DISCHARGE\_MASK 

```C++
#define TPSM8286_OUTPUT_DISCHARGE_MASK `0x08`
```



Enable output discharge when disabled. 


        

<hr>



### define TPSM8286\_RESET\_REGS\_MASK 

```C++
#define TPSM8286_RESET_REGS_MASK `0x80`
```



Reset registers to defaults. 


        

<hr>



### define TPSM8286\_STATUS\_REG\_ADDR 

```C++
#define TPSM8286_STATUS_REG_ADDR `0x05`
```



Status register. 


        

<hr>



### define TPSM8286\_STAT\_HICCUP\_MASK 

```C++
#define TPSM8286_STAT_HICCUP_MASK `0x04`
```



Hiccup mode active. 


        

<hr>



### define TPSM8286\_STAT\_TEMPWARM\_MASK 

```C++
#define TPSM8286_STAT_TEMPWARM_MASK `0x10`
```



Temperature warning. 


        

<hr>



### define TPSM8286\_STAT\_UVLO\_MASK 

```C++
#define TPSM8286_STAT_UVLO_MASK `0x01`
```



Under-voltage lockout active. 


        

<hr>



### define TPSM8286\_VOLTAGE\_RAMP\_SPEED\_10MV 

```C++
#define TPSM8286_VOLTAGE_RAMP_SPEED_10MV `0x01`
```



10 mV/step ramp speed. 


        

<hr>



### define TPSM8286\_VOLTAGE\_RAMP\_SPEED\_1MV 

```C++
#define TPSM8286_VOLTAGE_RAMP_SPEED_1MV `0x11`
```



1 mV/step ramp speed. 


        

<hr>



### define TPSM8286\_VOLTAGE\_RAMP\_SPEED\_20MV 

```C++
#define TPSM8286_VOLTAGE_RAMP_SPEED_20MV `0x00`
```



20 mV/step ramp speed. 


        

<hr>



### define TPSM8286\_VOLTAGE\_RAMP\_SPEED\_5MV 

```C++
#define TPSM8286_VOLTAGE_RAMP_SPEED_5MV `0x10`
```



5 mV/step ramp speed. 


        

<hr>



### define TPSM8286\_VOLTAGE\_RAMP\_SPEED\_MASK 

```C++
#define TPSM8286_VOLTAGE_RAMP_SPEED_MASK `0x03`
```



Mask of the ramp-speed field. 


        

<hr>



### define TPSM8286\_VOUT1\_REG\_ADDR 

```C++
#define TPSM8286_VOUT1_REG_ADDR `0x01`
```



Output voltage setpoint register 1. 


        

<hr>



### define TPSM8286\_VOUT2\_REG\_ADDR 

```C++
#define TPSM8286_VOUT2_REG_ADDR `0x02`
```



Output voltage setpoint register 2. 


        

<hr>



### define TPSM8286\_XXX3\_V\_TO\_REG 

```C++
#define TPSM8286_XXX3_V_TO_REG 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/tpsm8286x/tpsm8286x.h`

