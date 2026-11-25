

# File ina745a.h



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**ina745a**](dir_dfd4869667a9d947f6105bade284a2f8.md) **>** [**ina745a.h**](ina745a_8h.md)

[Go to the source code of this file](ina745a_8h_source.md)

_Driver for the Texas Instruments INA745A/B 40V, 16-Bit, Precision I2C Output Digital Power Monitor with integrated 800µΩ shunt (EZShunt)._ [More...](#detailed-description)

* `#include <type.h>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**ina745a\_adc\_config\_t**](structina745a__adc__config__t.md) <br> |
| struct | [**ina745a\_alert\_cfg\_t**](structina745a__alert__cfg__t.md) <br> |
| struct | [**ina745a\_diag\_flags\_t**](structina745a__diag__flags__t.md) <br> |
| struct | [**ina745a\_measurements\_t**](structina745a__measurements__t.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ina745a\_alert\_pol\_t**](#enum-ina745a_alert_pol_t)  <br> |
| enum  | [**ina745a\_avg\_t**](#enum-ina745a_avg_t)  <br> |
| enum  | [**ina745a\_convtime\_t**](#enum-ina745a_convtime_t)  <br> |
| enum  | [**ina745a\_err\_t**](#enum-ina745a_err_t)  <br> |
| typedef [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t)(\* | [**ina745a\_i2c\_read\_f**](#typedef-ina745a_i2c_read_f)  <br> |
| typedef [**ina745a\_err\_t**](ina745a_8h.md#enum-ina745a_err_t)(\* | [**ina745a\_i2c\_write\_f**](#typedef-ina745a_i2c_write_f)  <br> |
| enum  | [**ina745a\_mode\_t**](#enum-ina745a_mode_t)  <br> |
| typedef struct ina745a\_t | [**ina745a\_t**](#typedef-ina745a_t)  <br> |




















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



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**INA745A\_ADDR\_MAX**](ina745a_8h.md#define-ina745a_addr_max)  `0x4Fu`<br> |
| define  | [**INA745A\_ADDR\_MIN**](ina745a_8h.md#define-ina745a_addr_min)  `0x40u`<br> |
| define  | [**INA745A\_BUS\_VOLTAGE\_LSB\_V**](ina745a_8h.md#define-ina745a_bus_voltage_lsb_v)  `0.003125f`<br> |
| define  | [**INA745A\_CHARGE\_LSB\_C**](ina745a_8h.md#define-ina745a_charge_lsb_c)  `0.000075`<br> |
| define  | [**INA745A\_CURRENT\_LSB\_A**](ina745a_8h.md#define-ina745a_current_lsb_a)  `0.0012f`<br> |
| define  | [**INA745A\_ENERGY\_LSB\_J**](ina745a_8h.md#define-ina745a_energy_lsb_j)  `0.00384`<br> |
| define  | [**INA745A\_MANUFACTURER\_ID**](ina745a_8h.md#define-ina745a_manufacturer_id)  `0x5449u`<br> |
| define  | [**INA745A\_POWER\_LSB\_W**](ina745a_8h.md#define-ina745a_power_lsb_w)  `0.00024f`<br> |
| define  | [**INA745A\_PWR\_LIMIT\_LSB\_W**](ina745a_8h.md#define-ina745a_pwr_limit_lsb_w)  `0.06144f`<br> |
| define  | [**INA745A\_TEMP\_LSB\_C**](ina745a_8h.md#define-ina745a_temp_lsb_c)  `0.125f`<br> |

## Detailed Description


Concept:
* The driver core is bus-agnostic: it only uses injected HAL callbacks ([**ina745a\_i2c\_write\_f**](ina745a_8h.md#typedef-ina745a_i2c_write_f) / [**ina745a\_i2c\_read\_f**](ina745a_8h.md#typedef-ina745a_i2c_read_f)), never a concrete I2C driver directly. This allows host-side unit testing with stubs (see tests/ina745a/) without any BSP/hardware dependency, exactly like the si5332 driver.
* The HAL callbacks operate on whole logical registers (1/2/3/5 bytes), MSB first, matching the INA745x register map. Their signature is intentionally identical to drivers/i2c/i2cWrapper's [**i2c\_writeRegMultipleBytes()**](i2cWrapper_8c.md#function-i2c_writeregmultiplebytes)/i2c\_readRegMultipleBytes(), so they can be passed directly to [**ina745a\_initialize()**](ina745a_8h.md#function-ina745a_initialize) via a function-pointer cast - no adapter module needed (same pattern as si5332 with i2c\_writeReg/i2c\_readReg). The core itself never includes [**i2cWrapper.h**](i2cWrapper_8h.md) or any BSP header. 




    
## Public Types Documentation




### enum ina745a\_alert\_pol\_t 

```C++
enum ina745a_alert_pol_t {
    INA745A_ALERT_ACTIVE_LOW = 0,
    INA745A_ALERT_ACTIVE_HIGH = 1
};
```



ALERT pin polarity (DIAG\_ALRT.APOL). 


        

<hr>



### enum ina745a\_avg\_t 

```C++
enum ina745a_avg_t {
    INA745A_AVG_1 = 0,
    INA745A_AVG_4 = 1,
    INA745A_AVG_16 = 2,
    INA745A_AVG_64 = 3,
    INA745A_AVG_128 = 4,
    INA745A_AVG_256 = 5,
    INA745A_AVG_512 = 6,
    INA745A_AVG_1024 = 7
};
```



ADC\_CONFIG.AVG averaging setting (3 bit), Table 7-4. 


        

<hr>



### enum ina745a\_convtime\_t 

```C++
enum ina745a_convtime_t {
    INA745A_CT_50US = 0,
    INA745A_CT_84US = 1,
    INA745A_CT_150US = 2,
    INA745A_CT_280US = 3,
    INA745A_CT_540US = 4,
    INA745A_CT_1052US = 5,
    INA745A_CT_2074US = 6,
    INA745A_CT_4120US = 7
};
```



ADC\_CONFIG.VBUSCT / VSENCT / TCT conversion times (3 bit each), Table 7-4. 


        

<hr>



### enum ina745a\_err\_t 

```C++
enum ina745a_err_t {
    INA745A_OK =  0,
    INA745A_E_PARAM = -1,
    INA745A_E_I2C = -2,
    INA745A_E_NOT_INIT = -3,
    INA745A_E_MANUF_ID = -4
};
```



Status codes of the power monitor driver. 


        

<hr>



### typedef ina745a\_i2c\_read\_f 

```C++
typedef ina745a_err_t(* ina745a_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);
```




<hr>



### typedef ina745a\_i2c\_write\_f 

```C++
typedef ina745a_err_t(* ina745a_i2c_write_f) (void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);
```




<hr>



### enum ina745a\_mode\_t 

```C++
enum ina745a_mode_t {
    INA745A_MODE_SHUTDOWN = 0x0,
    INA745A_MODE_TRIG_BUSV = 0x1,
    INA745A_MODE_TRIG_TEMP = 0x4,
    INA745A_MODE_TRIG_TEMP_BUSV = 0x5,
    INA745A_MODE_TRIG_TEMP_CURRENT = 0x6,
    INA745A_MODE_TRIG_TEMP_CURRENT_BUSV = 0x7,
    INA745A_MODE_CONT_BUSV = 0x9,
    INA745A_MODE_CONT_TEMP = 0xC,
    INA745A_MODE_CONT_TEMP_BUSV = 0xD,
    INA745A_MODE_CONT_TEMP_CURRENT = 0xE,
    INA745A_MODE_CONT_TEMP_CURRENT_BUSV = 0xF
};
```



ADC\_CONFIG.MODE (bits 15-12), Table 7-4. 


        

<hr>



### typedef ina745a\_t 

```C++
typedef struct ina745a_t ina745a_t;
```



Opaque device handle (defined in [**ina745a.c**](ina745a_8c.md)). 


        

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
## Macro Definition Documentation





### define INA745A\_ADDR\_MAX 

```C++
#define INA745A_ADDR_MAX `0x4Fu`
```



Highest valid 7-bit I2C address. 


        

<hr>



### define INA745A\_ADDR\_MIN 

```C++
#define INA745A_ADDR_MIN `0x40u`
```



Lowest valid 7-bit I2C address. 


        

<hr>



### define INA745A\_BUS\_VOLTAGE\_LSB\_V 

```C++
#define INA745A_BUS_VOLTAGE_LSB_V `0.003125f`
```



3.125 mV/LSB 


        

<hr>



### define INA745A\_CHARGE\_LSB\_C 

```C++
#define INA745A_CHARGE_LSB_C `0.000075`
```



75 µC/LSB (double) 


        

<hr>



### define INA745A\_CURRENT\_LSB\_A 

```C++
#define INA745A_CURRENT_LSB_A `0.0012f`
```



1.2 mA/LSB 


        

<hr>



### define INA745A\_ENERGY\_LSB\_J 

```C++
#define INA745A_ENERGY_LSB_J `0.00384`
```



3.84 mJ/LSB (double) 


        

<hr>



### define INA745A\_MANUFACTURER\_ID 

```C++
#define INA745A_MANUFACTURER_ID `0x5449u`
```



Expected content of the MANUFACTURER\_ID register ("TI" in ASCII). 


        

<hr>



### define INA745A\_POWER\_LSB\_W 

```C++
#define INA745A_POWER_LSB_W `0.00024f`
```



240 µW/LSB 


        

<hr>



### define INA745A\_PWR\_LIMIT\_LSB\_W 

```C++
#define INA745A_PWR_LIMIT_LSB_W `0.06144f`
```



61.44 mW/LSB (= 256 x Power-LSB) 


        

<hr>



### define INA745A\_TEMP\_LSB\_C 

```C++
#define INA745A_TEMP_LSB_C `0.125f`
```



125 m°C/LSB 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/ina745a/ina745a.h`

