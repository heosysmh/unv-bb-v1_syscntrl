

# File ina745a.c



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**ina745a**](dir_dfd4869667a9d947f6105bade284a2f8.md) **>** [**ina745a.c**](ina745a_8c.md)

[Go to the source code of this file](ina745a_8c_source.md)

_Driver implementation for the TI INA745A/B digital power monitor._ [More...](#detailed-description)

* `#include <string.h>`
* `#include "ina745a.h"`
* `#include "userDef.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**ina745a\_t**](structina745a__t.md) <br>_Opaque device handle (definition), see_ [_**ina745a\_t**_](structina745a__t.md) _in_[_**ina745a.h**_](ina745a_8h.md) _._ |








## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  struct [**ina745a\_t**](structina745a__t.md) | [**gInstances**](#variable-ginstances)  <br>_Static pool of device instances backing_ [_**ina745a\_initialize()**_](ina745a_8c.md#function-ina745a_initialize) _._ |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_configure\_alert**](#function-ina745a_configure_alert) ([**ina745a\_t**](structina745a__t.md) \* pDev, const [**ina745a\_alert\_cfg\_t**](structina745a__alert__cfg__t.md) \* cfg) <br>_Writes the DIAG\_ALRT configuration bits (ALATCH/CNVR/SLOWALERT/APOL)._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_adc\_config**](#function-ina745a_get_adc_config) ([**ina745a\_t**](structina745a__t.md) \* pDev, [**ina745a\_adc\_config\_t**](structina745a__adc__config__t.md) \* cfg) <br>_Reads back the complete ADC\_CONFIG register._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_bus\_voltage**](#function-ina745a_get_bus_voltage) ([**ina745a\_t**](structina745a__t.md) \* pDev, float \* volts) <br>_Reads the bus voltage._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_charge**](#function-ina745a_get_charge) ([**ina745a\_t**](structina745a__t.md) \* pDev, double \* coulombs) <br>_Reads the accumulated charge._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_current**](#function-ina745a_get_current) ([**ina745a\_t**](structina745a__t.md) \* pDev, float \* amps) <br>_Reads the current._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_diag\_flags**](#function-ina745a_get_diag_flags) ([**ina745a\_t**](structina745a__t.md) \* pDev, [**ina745a\_diag\_flags\_t**](structina745a__diag__flags__t.md) \* out) <br>_Reads and decodes the DIAG\_ALRT register._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_die\_temperature**](#function-ina745a_get_die_temperature) ([**ina745a\_t**](structina745a__t.md) \* pDev, float \* celsius) <br>_Reads the die temperature._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_energy**](#function-ina745a_get_energy) ([**ina745a\_t**](structina745a__t.md) \* pDev, double \* joules) <br>_Reads the accumulated energy._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_manufacturer\_id**](#function-ina745a_get_manufacturer_id) ([**ina745a\_t**](structina745a__t.md) \* pDev, u16 \* id) <br>_Reads the MANUFACTURER\_ID register._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_measurements**](#function-ina745a_get_measurements) ([**ina745a\_t**](structina745a__t.md) \* pDev, [**ina745a\_measurements\_t**](structina745a__measurements__t.md) \* out) <br>_Reads all measurement registers (VBUS, DIETEMP, CURRENT, POWER, ENERGY, CHARGE)._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_get\_power**](#function-ina745a_get_power) ([**ina745a\_t**](structina745a__t.md) \* pDev, float \* watts) <br>_Reads the power._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_init**](#function-ina745a_init) ([**ina745a\_t**](structina745a__t.md) \* pDev, const [**ina745a\_adc\_config\_t**](structina745a__adc__config__t.md) \* cfg) <br>_Verifies MANUFACTURER\_ID and applies the ADC configuration._  |
|  [**ina745a\_t**](structina745a__t.md) \* | [**ina745a\_initialize**](#function-ina745a_initialize) (u32 devId, u8 i2c\_addr, [**ina745a\_i2c\_write\_f**](ina745a_8h.md#typedef-ina745a_i2c_write_f) w, [**ina745a\_i2c\_read\_f**](ina745a_8h.md#typedef-ina745a_i2c_read_f) r, void \* ctx) <br>_Creates the device handle for instance_ `devId` _(no I2C access)._ |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_reset**](#function-ina745a_reset) ([**ina745a\_t**](structina745a__t.md) \* pDev) <br>_Software reset (CONFIG.RST), equivalent to a power-on reset._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_reset\_accumulators**](#function-ina745a_reset_accumulators) ([**ina745a\_t**](structina745a__t.md) \* pDev) <br>_Clears the ENERGY and CHARGE accumulator registers (CONFIG.RSTACC)._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_set\_adc\_config**](#function-ina745a_set_adc_config) ([**ina745a\_t**](structina745a__t.md) \* pDev, const [**ina745a\_adc\_config\_t**](structina745a__adc__config__t.md) \* cfg) <br>_Writes the complete ADC\_CONFIG register._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_set\_bus\_voltage\_limits**](#function-ina745a_set_bus_voltage_limits) ([**ina745a\_t**](structina745a__t.md) \* pDev, float overvoltage\_V, float undervoltage\_V) <br>_Writes the bus over-/under-voltage thresholds (BOVL/BUVL)._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_set\_conversion\_delay**](#function-ina745a_set_conversion_delay) ([**ina745a\_t**](structina745a__t.md) \* pDev, u8 steps\_2ms) <br>_Sets the initial-conversion delay in steps of 2 ms._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_set\_current\_limits**](#function-ina745a_set_current_limits) ([**ina745a\_t**](structina745a__t.md) \* pDev, float over\_limit\_A, float under\_limit\_A) <br>_Writes the current over-/under-limit thresholds (COL/CUL)._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_set\_mode**](#function-ina745a_set_mode) ([**ina745a\_t**](structina745a__t.md) \* pDev, [**ina745a\_mode\_t**](ina745a_8h.md#enum-ina745a_mode_t) mode) <br>_Changes only ADC\_CONFIG.MODE (read-modify-write)._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_set\_power\_limit**](#function-ina745a_set_power_limit) ([**ina745a\_t**](structina745a__t.md) \* pDev, float overpower\_W) <br>_Writes the power over-limit threshold (PWR\_LIMIT)._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**ina745a\_set\_temperature\_limit**](#function-ina745a_set_temperature_limit) ([**ina745a\_t**](structina745a__t.md) \* pDev, float overtemp\_C) <br>_Writes the temperature over-limit threshold (TEMP\_LIMIT)._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**adc\_config\_pack**](#function-adc_config_pack) (const [**ina745a\_adc\_config\_t**](structina745a__adc__config__t.md) \* cfg, u16 \* raw) <br>_Packs an_ [_**ina745a\_adc\_config\_t**_](structina745a__adc__config__t.md) _into the raw 16-bit ADC\_CONFIG value._ |
|  void | [**adc\_config\_unpack**](#function-adc_config_unpack) (u16 raw, [**ina745a\_adc\_config\_t**](structina745a__adc__config__t.md) \* cfg) <br>_Unpacks the raw 16-bit ADC\_CONFIG value into an_ [_**ina745a\_adc\_config\_t**_](structina745a__adc__config__t.md) _._ |
|  s32 | [**clampi**](#function-clampi) (s32 x, s32 lo, s32 hi) <br>_Clamps a signed 32-bit integer between two bounds._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**reg\_read16**](#function-reg_read16) ([**ina745a\_t**](structina745a__t.md) \* d, u8 addr, u16 \* val) <br>_Reads a 16-bit register, MSB first._  |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**reg\_read24**](#function-reg_read24) ([**ina745a\_t**](structina745a__t.md) \* d, u8 addr, u32 \* val) <br>_Reads a 24-bit register, MSB first, into the low 24 bit of_ `val` _._ |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**reg\_read40**](#function-reg_read40) ([**ina745a\_t**](structina745a__t.md) \* d, u8 addr, u64 \* val) <br>_Reads a 40-bit register, MSB first, into the low 40 bit of_ `val` _._ |
|  [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) | [**reg\_write16**](#function-reg_write16) ([**ina745a\_t**](structina745a__t.md) \* d, u8 addr, u16 val) <br>_Writes a 16-bit register, MSB first._  |
|  s32 | [**round\_f**](#function-round_f) (float x) <br>_Rounds a float to the nearest integer (away from zero on .5)._  |
|  s32 | [**sign\_extend32**](#function-sign_extend32) (u32 value, u8 bits) <br>_Sign-extends the lowest_ `bits` _bits of_`value` _to a 32-bit signed integer._ |
|  s64 | [**sign\_extend64**](#function-sign_extend64) (u64 value, u8 bits) <br>_Sign-extends the lowest_ `bits` _bits of_`value` _to a 64-bit signed integer._ |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**CHK**](ina745a_8c.md#define-chk) (x) `do { [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) \_e = (x); if (\_e != INA745A\_OK) return \_e; } while (0)`<br>_Executes_ `x` _and returns its error code if it is not_INA745A\_OK _._ |
| define  | [**CONFIG\_CONVDLY\_MASK**](ina745a_8c.md#define-config_convdly_mask)  `(0xFFu &lt;&lt; [**CONFIG\_CONVDLY\_SHIFT**](ina745a_8c.md#define-config_convdly_shift))`<br> |
| define  | [**CONFIG\_CONVDLY\_SHIFT**](ina745a_8c.md#define-config_convdly_shift)  `6u`<br> |
| define  | [**CONFIG\_RSTACC\_BIT**](ina745a_8c.md#define-config_rstacc_bit)  `14u`<br> |
| define  | [**CONFIG\_RST\_BIT**](ina745a_8c.md#define-config_rst_bit)  `15u`<br> |
| define  | [**DIAG\_ALATCH\_BIT**](ina745a_8c.md#define-diag_alatch_bit)  `15u`<br> |
| define  | [**DIAG\_APOL\_BIT**](ina745a_8c.md#define-diag_apol_bit)  `12u`<br> |
| define  | [**DIAG\_BUSOL\_BIT**](ina745a_8c.md#define-diag_busol_bit)  `4u`<br> |
| define  | [**DIAG\_BUSUL\_BIT**](ina745a_8c.md#define-diag_busul_bit)  `3u`<br> |
| define  | [**DIAG\_CHARGEOF\_BIT**](ina745a_8c.md#define-diag_chargeof_bit)  `10u`<br> |
| define  | [**DIAG\_CNVRF\_BIT**](ina745a_8c.md#define-diag_cnvrf_bit)  `1u`<br> |
| define  | [**DIAG\_CNVR\_BIT**](ina745a_8c.md#define-diag_cnvr_bit)  `14u`<br> |
| define  | [**DIAG\_CURRENTOL\_BIT**](ina745a_8c.md#define-diag_currentol_bit)  `6u`<br> |
| define  | [**DIAG\_CURRENTUL\_BIT**](ina745a_8c.md#define-diag_currentul_bit)  `5u`<br> |
| define  | [**DIAG\_ENERGYOF\_BIT**](ina745a_8c.md#define-diag_energyof_bit)  `11u`<br> |
| define  | [**DIAG\_MATHOF\_BIT**](ina745a_8c.md#define-diag_mathof_bit)  `9u`<br> |
| define  | [**DIAG\_MEMSTAT\_BIT**](ina745a_8c.md#define-diag_memstat_bit)  `0u`<br> |
| define  | [**DIAG\_POL\_BIT**](ina745a_8c.md#define-diag_pol_bit)  `2u`<br> |
| define  | [**DIAG\_SLOWALERT\_BIT**](ina745a_8c.md#define-diag_slowalert_bit)  `13u`<br> |
| define  | [**DIAG\_TMPOL\_BIT**](ina745a_8c.md#define-diag_tmpol_bit)  `7u`<br> |
| define  | [**REG\_ADC\_CONFIG**](ina745a_8c.md#define-reg_adc_config)  `0x01u`<br> |
| define  | [**REG\_BOVL**](ina745a_8c.md#define-reg_bovl)  `0x0Eu`<br> |
| define  | [**REG\_BUVL**](ina745a_8c.md#define-reg_buvl)  `0x0Fu`<br> |
| define  | [**REG\_CHARGE**](ina745a_8c.md#define-reg_charge)  `0x0Au`<br> |
| define  | [**REG\_COL**](ina745a_8c.md#define-reg_col)  `0x0Cu`<br> |
| define  | [**REG\_CONFIG**](ina745a_8c.md#define-reg_config)  `0x00u`<br> |
| define  | [**REG\_CUL**](ina745a_8c.md#define-reg_cul)  `0x0Du`<br> |
| define  | [**REG\_CURRENT**](ina745a_8c.md#define-reg_current)  `0x07u`<br> |
| define  | [**REG\_DIAG\_ALRT**](ina745a_8c.md#define-reg_diag_alrt)  `0x0Bu`<br> |
| define  | [**REG\_DIETEMP**](ina745a_8c.md#define-reg_dietemp)  `0x06u`<br> |
| define  | [**REG\_ENERGY**](ina745a_8c.md#define-reg_energy)  `0x09u`<br> |
| define  | [**REG\_MANUFACTURER\_ID**](ina745a_8c.md#define-reg_manufacturer_id)  `0x3Eu`<br> |
| define  | [**REG\_POWER**](ina745a_8c.md#define-reg_power)  `0x08u`<br> |
| define  | [**REG\_PWR\_LIMIT**](ina745a_8c.md#define-reg_pwr_limit)  `0x11u`<br> |
| define  | [**REG\_TEMP\_LIMIT**](ina745a_8c.md#define-reg_temp_limit)  `0x10u`<br> |
| define  | [**REG\_VBUS**](ina745a_8c.md#define-reg_vbus)  `0x05u`<br> |

## Detailed Description


Bus-agnostic core driver for the INA745A/B (40V, 16-bit, precision I2C digital power monitor with integrated 800µΩ shunt). The core only uses injected HAL callbacks ([**ina745a\_i2c\_write\_f**](ina745a_8h.md#typedef-ina745a_i2c_write_f) / [**ina745a\_i2c\_read\_f**](ina745a_8h.md#typedef-ina745a_i2c_read_f)) and never includes [**drivers/i2c/i2cWrapper.h**](i2cWrapper_8h.md) or any BSP header directly. Their signature matches [**i2c\_writeRegMultipleBytes()**](i2cWrapper_8c.md#function-i2c_writeregmultiplebytes)/i2c\_readRegMultipleBytes() in [**drivers/i2c/i2cWrapper.h**](i2cWrapper_8h.md), so no adapter module is needed - callers pass them in directly via a function-pointer cast (see [**ina745a.h**](ina745a_8h.md)).


There is no calibration register (unlike INA226/INA228): the integrated shunt is factory-trimmed, so all LSB sizes are fixed constants (see [**ina745a.h**](ina745a_8h.md) and datasheet Table 8-1).


Multiple simultaneous instances are supported analogous to tpsm8286x: [**ina745a\_initialize()**](ina745a_8c.md#function-ina745a_initialize) takes an explicit 'devId' that directly indexes the static instance pool (size [**INA745A\_NUM\_INSTANCES**](userDef_8h.md#define-ina745a_num_instances), defined in [**userDef.h**](userDef_8h.md)).




**See also:** [**ina745a.h**](ina745a_8h.md) 


**See also:** plans/ina745a\_driver\_architecture.md


**Date:**

Created on: 15.11.2026 




**Author:**

matthias 





    
## Public Static Attributes Documentation




### variable gInstances 

_Static pool of device instances backing_ [_**ina745a\_initialize()**_](ina745a_8c.md#function-ina745a_initialize) _._
```C++
struct ina745a_t gInstances[INA745A_NUM_INSTANCES];
```




<hr>
## Public Functions Documentation




### function ina745a\_configure\_alert 

_Writes the DIAG\_ALRT configuration bits (ALATCH/CNVR/SLOWALERT/APOL)._ 
```C++
ina745a_err_t ina745a_configure_alert (
    ina745a_t * pDev,
    const ina745a_alert_cfg_t * cfg
) 
```





**Note:**

Writing this register also clears any latched alert flags (same documented side effect as reading it while ALATCH = 1).




**Parameters:**


* `pDev` Device handle. 
* `cfg` Desired alert configuration. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_adc\_config 

_Reads back the complete ADC\_CONFIG register._ 
```C++
ina745a_err_t ina745a_get_adc_config (
    ina745a_t * pDev,
    ina745a_adc_config_t * cfg
) 
```





**Parameters:**


* `pDev` Device handle. 
* `cfg` Receives the decoded configuration on success. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_bus\_voltage 

_Reads the bus voltage._ 
```C++
ina745a_err_t ina745a_get_bus_voltage (
    ina745a_t * pDev,
    float * volts
) 
```





**Parameters:**


* `pDev` Device handle. 
* `volts` Receives the bus voltage in volts. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_charge 

_Reads the accumulated charge._ 
```C++
ina745a_err_t ina745a_get_charge (
    ina745a_t * pDev,
    double * coulombs
) 
```





**Parameters:**


* `pDev` Device handle. 
* `coulombs` Receives the charge in coulombs. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_current 

_Reads the current._ 
```C++
ina745a_err_t ina745a_get_current (
    ina745a_t * pDev,
    float * amps
) 
```





**Parameters:**


* `pDev` Device handle. 
* `amps` Receives the current in amperes. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_diag\_flags 

_Reads and decodes the DIAG\_ALRT register._ 
```C++
ina745a_err_t ina745a_get_diag_flags (
    ina745a_t * pDev,
    ina745a_diag_flags_t * out
) 
```





**Note:**

If ALATCH = 1, reading this register clears the latched alert flags (documented device behaviour).




**Parameters:**


* `pDev` Device handle. 
* `out` Receives the decoded diagnostic flags on success. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_die\_temperature 

_Reads the die temperature._ 
```C++
ina745a_err_t ina745a_get_die_temperature (
    ina745a_t * pDev,
    float * celsius
) 
```





**Parameters:**


* `pDev` Device handle. 
* `celsius` Receives the die temperature in °C. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_energy 

_Reads the accumulated energy._ 
```C++
ina745a_err_t ina745a_get_energy (
    ina745a_t * pDev,
    double * joules
) 
```





**Parameters:**


* `pDev` Device handle. 
* `joules` Receives the energy in joules. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_manufacturer\_id 

_Reads the MANUFACTURER\_ID register._ 
```C++
ina745a_err_t ina745a_get_manufacturer_id (
    ina745a_t * pDev,
    u16 * id
) 
```



Only requires a valid handle, not a prior successful [**ina745a\_init()**](ina745a_8h.md#function-ina745a_init).




**Parameters:**


* `pDev` Device handle. 
* `id` Receives the ID (expected: [**INA745A\_MANUFACTURER\_ID**](ina745a_8h.md#define-ina745a_manufacturer_id)). 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_measurements 

_Reads all measurement registers (VBUS, DIETEMP, CURRENT, POWER, ENERGY, CHARGE)._ 
```C++
ina745a_err_t ina745a_get_measurements (
    ina745a_t * pDev,
    ina745a_measurements_t * out
) 
```





**Parameters:**


* `pDev` Device handle. 
* `out` Receives all measurement values (converted and raw). 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_get\_power 

_Reads the power._ 
```C++
ina745a_err_t ina745a_get_power (
    ina745a_t * pDev,
    float * watts
) 
```





**Parameters:**


* `pDev` Device handle. 
* `watts` Receives the power in watts. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_init 

_Verifies MANUFACTURER\_ID and applies the ADC configuration._ 
```C++
ina745a_err_t ina745a_init (
    ina745a_t * pDev,
    const ina745a_adc_config_t * cfg
) 
```



Fails hard with INA745A\_E\_MANUF\_ID if the ID does not read 0x5449 ("TI") (wrong address / wrong device) - the driver stays unconfigured in that case.




**Parameters:**


* `pDev` Device handle from [**ina745a\_initialize()**](ina745a_8h.md#function-ina745a_initialize). 
* `cfg` Desired ADC configuration. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_initialize 

_Creates the device handle for instance_ `devId` _(no I2C access)._
```C++
ina745a_t * ina745a_initialize (
    u32 devId,
    u8 i2c_addr,
    ina745a_i2c_write_f w,
    ina745a_i2c_read_f r,
    void * ctx
) 
```



Analogous to [**tpsm8286\_initialize()**](tpsm8286x_8c.md#function-tpsm8286_initialize): `devId` directly indexes the static instance pool (size [**INA745A\_NUM\_INSTANCES**](userDef_8h.md#define-ina745a_num_instances), defined in [**userDef.h**](userDef_8h.md)), allowing multiple INA745A devices to be managed simultaneously.




**Parameters:**


* `devId` Instance index, 0 .. ([**INA745A\_NUM\_INSTANCES**](userDef_8h.md#define-ina745a_num_instances) - 1). 
* `i2c_addr` 7-bit I2C address determined by the A0/A1 pin strapping on the board (datasheet Table 6-2), validated against [**INA745A\_ADDR\_MIN**](ina745a_8h.md#define-ina745a_addr_min) .. [**INA745A\_ADDR\_MAX**](ina745a_8h.md#define-ina745a_addr_max). 
* `w` HAL register write callback, must not be NULL. 
* `r` HAL register read callback, must not be NULL. 
* `ctx` Opaque context pointer passed through to `w` and `r`. 



**Returns:**

Pointer to the device handle, or NULL if `devId` or `i2c_addr` is out of range, or `w/ r is` NULL. 





        

<hr>



### function ina745a\_reset 

_Software reset (CONFIG.RST), equivalent to a power-on reset._ 
```C++
ina745a_err_t ina745a_reset (
    ina745a_t * pDev
) 
```



Only requires a valid handle (not a prior successful [**ina745a\_init()**](ina745a_8h.md#function-ina745a_init)), since this is also used as a recovery mechanism. Marks the driver as unconfigured afterwards, since all registers revert to their defaults.




**Parameters:**


* `pDev` Device handle. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_reset\_accumulators 

_Clears the ENERGY and CHARGE accumulator registers (CONFIG.RSTACC)._ 
```C++
ina745a_err_t ina745a_reset_accumulators (
    ina745a_t * pDev
) 
```





**Parameters:**


* `pDev` Device handle. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_set\_adc\_config 

_Writes the complete ADC\_CONFIG register._ 
```C++
ina745a_err_t ina745a_set_adc_config (
    ina745a_t * pDev,
    const ina745a_adc_config_t * cfg
) 
```





**Parameters:**


* `pDev` Device handle. 
* `cfg` Desired ADC configuration. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_set\_bus\_voltage\_limits 

_Writes the bus over-/under-voltage thresholds (BOVL/BUVL)._ 
```C++
ina745a_err_t ina745a_set_bus_voltage_limits (
    ina745a_t * pDev,
    float overvoltage_V,
    float undervoltage_V
) 
```





**Parameters:**


* `pDev` Device handle. 
* `overvoltage_V` Overvoltage threshold in volts. 
* `undervoltage_V` Undervoltage threshold in volts. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 




**Note:**

Both registers are unsigned, positive-only, 15 bit wide (bit 15 reserved = 0); values are clamped to 0 .. 0x7FFF LSBs. 





        

<hr>



### function ina745a\_set\_conversion\_delay 

_Sets the initial-conversion delay in steps of 2 ms._ 
```C++
ina745a_err_t ina745a_set_conversion_delay (
    ina745a_t * pDev,
    u8 steps_2ms
) 
```





**Parameters:**


* `pDev` Device handle. 
* `steps_2ms` Delay steps (0 = no delay, 255 = 510 ms). 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_set\_current\_limits 

_Writes the current over-/under-limit thresholds (COL/CUL)._ 
```C++
ina745a_err_t ina745a_set_current_limits (
    ina745a_t * pDev,
    float over_limit_A,
    float under_limit_A
) 
```





**Parameters:**


* `pDev` Device handle. 
* `over_limit_A` Overcurrent threshold in amperes. 
* `under_limit_A` Undercurrent threshold in amperes. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 




**Note:**

Values are rounded to the nearest LSB and clamped to the 16-bit two's complement range; see datasheet Section 8.2.2.2 on choosing COL &gt;= CUL when both are negative. 





        

<hr>



### function ina745a\_set\_mode 

_Changes only ADC\_CONFIG.MODE (read-modify-write)._ 
```C++
ina745a_err_t ina745a_set_mode (
    ina745a_t * pDev,
    ina745a_mode_t mode
) 
```



Conversion times and AVG are preserved.




**Parameters:**


* `pDev` Device handle. 
* `mode` Desired conversion mode. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 





        

<hr>



### function ina745a\_set\_power\_limit 

_Writes the power over-limit threshold (PWR\_LIMIT)._ 
```C++
ina745a_err_t ina745a_set_power_limit (
    ina745a_t * pDev,
    float overpower_W
) 
```





**Parameters:**


* `pDev` Device handle. 
* `overpower_W` Over-power threshold in watts. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 




**Note:**

PWR\_LIMIT uses an LSB that is 256x the POWER register LSB ([**INA745A\_PWR\_LIMIT\_LSB\_W**](ina745a_8h.md#define-ina745a_pwr_limit_lsb_w) = 61.44 mW/LSB), since POWER is 24 bit while PWR\_LIMIT is only 16 bit. 





        

<hr>



### function ina745a\_set\_temperature\_limit 

_Writes the temperature over-limit threshold (TEMP\_LIMIT)._ 
```C++
ina745a_err_t ina745a_set_temperature_limit (
    ina745a_t * pDev,
    float overtemp_C
) 
```





**Parameters:**


* `pDev` Device handle. 
* `overtemp_C` Over-temperature threshold in °C. 



**Returns:**

INA745A\_OK on success, negative [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t) code on error. 




**Note:**

The value occupies the 12-bit field in bits 15-4; bits 3-0 are read-only reserved (always 0) and are written as 0. 





        

<hr>
## Public Static Functions Documentation




### function adc\_config\_pack 

_Packs an_ [_**ina745a\_adc\_config\_t**_](structina745a__adc__config__t.md) _into the raw 16-bit ADC\_CONFIG value._
```C++
static ina745a_err_t adc_config_pack (
    const ina745a_adc_config_t * cfg,
    u16 * raw
) 
```





**Parameters:**


* `cfg` Configuration to encode. 
* `raw` Receives the packed register value. 



**Returns:**

INA745A\_OK on success, INA745A\_E\_PARAM if any field is out of range. 





        

<hr>



### function adc\_config\_unpack 

_Unpacks the raw 16-bit ADC\_CONFIG value into an_ [_**ina745a\_adc\_config\_t**_](structina745a__adc__config__t.md) _._
```C++
static void adc_config_unpack (
    u16 raw,
    ina745a_adc_config_t * cfg
) 
```





**Parameters:**


* `raw` Raw register value. 
* `cfg` Receives the decoded configuration. 




        

<hr>



### function clampi 

_Clamps a signed 32-bit integer between two bounds._ 
```C++
static s32 clampi (
    s32 x,
    s32 lo,
    s32 hi
) 
```




<hr>



### function reg\_read16 

_Reads a 16-bit register, MSB first._ 
```C++
static ina745a_err_t reg_read16 (
    ina745a_t * d,
    u8 addr,
    u16 * val
) 
```





**Parameters:**


* `d` Device handle. 
* `addr` Register address. 
* `val` Receives the 16-bit value on success. 



**Returns:**

INA745A\_OK on success, INA745A\_E\_I2C on a HAL/bus error. 





        

<hr>



### function reg\_read24 

_Reads a 24-bit register, MSB first, into the low 24 bit of_ `val` _._
```C++
static ina745a_err_t reg_read24 (
    ina745a_t * d,
    u8 addr,
    u32 * val
) 
```





**Parameters:**


* `d` Device handle. 
* `addr` Register address. 
* `val` Receives the 24-bit value on success. 



**Returns:**

INA745A\_OK on success, INA745A\_E\_I2C on a HAL/bus error. 





        

<hr>



### function reg\_read40 

_Reads a 40-bit register, MSB first, into the low 40 bit of_ `val` _._
```C++
static ina745a_err_t reg_read40 (
    ina745a_t * d,
    u8 addr,
    u64 * val
) 
```





**Parameters:**


* `d` Device handle. 
* `addr` Register address. 
* `val` Receives the 40-bit value on success. 



**Returns:**

INA745A\_OK on success, INA745A\_E\_I2C on a HAL/bus error. 





        

<hr>



### function reg\_write16 

_Writes a 16-bit register, MSB first._ 
```C++
static ina745a_err_t reg_write16 (
    ina745a_t * d,
    u8 addr,
    u16 val
) 
```





**Parameters:**


* `d` Device handle. 
* `addr` Register address. 
* `val` 16-bit value to write. 



**Returns:**

INA745A\_OK on success, INA745A\_E\_I2C on a HAL/bus error. 





        

<hr>



### function round\_f 

_Rounds a float to the nearest integer (away from zero on .5)._ 
```C++
static s32 round_f (
    float x
) 
```




<hr>



### function sign\_extend32 

_Sign-extends the lowest_ `bits` _bits of_`value` _to a 32-bit signed integer._
```C++
static s32 sign_extend32 (
    u32 value,
    u8 bits
) 
```





**Parameters:**


* `value` Unsigned value whose lowest `bits` bits hold a two's complement number. 
* `bits` Width of the two's complement field (1..32). 



**Returns:**

Sign-extended signed value. 





        

<hr>



### function sign\_extend64 

_Sign-extends the lowest_ `bits` _bits of_`value` _to a 64-bit signed integer._
```C++
static s64 sign_extend64 (
    u64 value,
    u8 bits
) 
```





**Parameters:**


* `value` Unsigned value whose lowest `bits` bits hold a two's complement number. 
* `bits` Width of the two's complement field (1..64). 



**Returns:**

Sign-extended signed value. 





        

<hr>
## Macro Definition Documentation





### define CHK 

_Executes_ `x` _and returns its error code if it is not_INA745A\_OK _._
```C++
#define CHK (
    x
) `do { ina745a_err_t _e = (x); if (_e != INA745A_OK ) return _e; } while (0)`
```





**Parameters:**


* `x` Expression of type [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t). 




        

<hr>



### define CONFIG\_CONVDLY\_MASK 

```C++
#define CONFIG_CONVDLY_MASK `(0xFFu << CONFIG_CONVDLY_SHIFT )`
```




<hr>



### define CONFIG\_CONVDLY\_SHIFT 

```C++
#define CONFIG_CONVDLY_SHIFT `6u`
```



Start of the 8-bit CONVDLY field (bits 13-6). 


        

<hr>



### define CONFIG\_RSTACC\_BIT 

```C++
#define CONFIG_RSTACC_BIT `14u`
```



Resets ENERGY/CHARGE accumulators. 


        

<hr>



### define CONFIG\_RST\_BIT 

```C++
#define CONFIG_RST_BIT `15u`
```



Software reset, self-clearing. 


        

<hr>



### define DIAG\_ALATCH\_BIT 

```C++
#define DIAG_ALATCH_BIT `15u`
```




<hr>



### define DIAG\_APOL\_BIT 

```C++
#define DIAG_APOL_BIT `12u`
```




<hr>



### define DIAG\_BUSOL\_BIT 

```C++
#define DIAG_BUSOL_BIT `4u`
```




<hr>



### define DIAG\_BUSUL\_BIT 

```C++
#define DIAG_BUSUL_BIT `3u`
```




<hr>



### define DIAG\_CHARGEOF\_BIT 

```C++
#define DIAG_CHARGEOF_BIT `10u`
```




<hr>



### define DIAG\_CNVRF\_BIT 

```C++
#define DIAG_CNVRF_BIT `1u`
```




<hr>



### define DIAG\_CNVR\_BIT 

```C++
#define DIAG_CNVR_BIT `14u`
```




<hr>



### define DIAG\_CURRENTOL\_BIT 

```C++
#define DIAG_CURRENTOL_BIT `6u`
```




<hr>



### define DIAG\_CURRENTUL\_BIT 

```C++
#define DIAG_CURRENTUL_BIT `5u`
```




<hr>



### define DIAG\_ENERGYOF\_BIT 

```C++
#define DIAG_ENERGYOF_BIT `11u`
```




<hr>



### define DIAG\_MATHOF\_BIT 

```C++
#define DIAG_MATHOF_BIT `9u`
```




<hr>



### define DIAG\_MEMSTAT\_BIT 

```C++
#define DIAG_MEMSTAT_BIT `0u`
```




<hr>



### define DIAG\_POL\_BIT 

```C++
#define DIAG_POL_BIT `2u`
```




<hr>



### define DIAG\_SLOWALERT\_BIT 

```C++
#define DIAG_SLOWALERT_BIT `13u`
```




<hr>



### define DIAG\_TMPOL\_BIT 

```C++
#define DIAG_TMPOL_BIT `7u`
```




<hr>



### define REG\_ADC\_CONFIG 

```C++
#define REG_ADC_CONFIG `0x01u`
```



ADC configuration register (16 bit). 


        

<hr>



### define REG\_BOVL 

```C++
#define REG_BOVL `0x0Eu`
```



Bus overvoltage threshold (15 bit, unsigned). 


        

<hr>



### define REG\_BUVL 

```C++
#define REG_BUVL `0x0Fu`
```



Bus undervoltage threshold (15 bit, unsigned). 


        

<hr>



### define REG\_CHARGE 

```C++
#define REG_CHARGE `0x0Au`
```



Charge result (40 bit, two's complement). 


        

<hr>



### define REG\_COL 

```C++
#define REG_COL `0x0Cu`
```



Current over-limit threshold (16 bit, two's complement). 


        

<hr>



### define REG\_CONFIG 

```C++
#define REG_CONFIG `0x00u`
```



Configuration register (16 bit). 


        

<hr>



### define REG\_CUL 

```C++
#define REG_CUL `0x0Du`
```



Current under-limit threshold (16 bit, two's complement). 


        

<hr>



### define REG\_CURRENT 

```C++
#define REG_CURRENT `0x07u`
```



Current result (16 bit, two's complement). 


        

<hr>



### define REG\_DIAG\_ALRT 

```C++
#define REG_DIAG_ALRT `0x0Bu`
```



Diagnostic flags and alert register (16 bit). 


        

<hr>



### define REG\_DIETEMP 

```C++
#define REG_DIETEMP `0x06u`
```



Die temperature measurement (12-bit field in bits 15-4). 


        

<hr>



### define REG\_ENERGY 

```C++
#define REG_ENERGY `0x09u`
```



Energy result (40 bit, unsigned). 


        

<hr>



### define REG\_MANUFACTURER\_ID 

```C++
#define REG_MANUFACTURER_ID `0x3Eu`
```



Manufacturer ID register (16 bit, constant 0x5449 = "TI"). 


        

<hr>



### define REG\_POWER 

```C++
#define REG_POWER `0x08u`
```



Power result (24 bit, unsigned). 


        

<hr>



### define REG\_PWR\_LIMIT 

```C++
#define REG_PWR_LIMIT `0x11u`
```



Power over-limit threshold (16 bit, unsigned). 


        

<hr>



### define REG\_TEMP\_LIMIT 

```C++
#define REG_TEMP_LIMIT `0x10u`
```



Temperature over-limit threshold (12-bit field in bits 15-4). 


        

<hr>



### define REG\_VBUS 

```C++
#define REG_VBUS `0x05u`
```



Bus voltage measurement (16 bit, unsigned). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/ina745a/ina745a.c`

