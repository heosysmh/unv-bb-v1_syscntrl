

# File power.h



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**power.h**](power_8h.md)

[Go to the source code of this file](power_8h_source.md)

_Power functions of the board: FMC Vadj regulators (tpsm8286x) and the power monitors (ina745a)._ [More...](#detailed-description)

* `#include <type.h>`
* `#include "boardConfig.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**pwrMon\_fault\_t**](structpwrMon__fault__t.md) <br> |
| struct | [**pwrMon\_values\_t**](structpwrMon__values__t.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**pwrFmc\_t**](#enum-pwrfmc_t)  <br> |
| enum  | [**pwrPgLine\_t**](#enum-pwrpgline_t)  <br> |
| enum  | [**pwrRail\_t**](#enum-pwrrail_t)  <br> |
| enum  | [**pwr\_err\_t**](#enum-pwr_err_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_checkAlerts**](#function-pwr_checkalerts) (void) <br>_Checks the power-good / ALERT lines._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_fmcVadj\_configure**](#function-pwr_fmcvadj_configure) (void) <br>_Writes control register and voltage setpoint of both FMC Vadj regulators._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_fmcVadj\_enable**](#function-pwr_fmcvadj_enable) (int enable) <br>_Enables/disables both FMC Vadj regulator outputs._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_fmcVadj\_isRegulating**](#function-pwr_fmcvadj_isregulating) ([**pwrFmc\_t**](power_8h.md#enum-pwrfmc_t) fmc) <br>_Checks whether the regulator reports a valid output voltage setting._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_init**](#function-pwr_init) (void) <br>_Creates all device handles (regulators and monitors)._  |
|  void | [**pwr\_mon\_clearFaults**](#function-pwr_mon_clearfaults) ([**pwrRail\_t**](power_8h.md#enum-pwrrail_t) rail) <br>_Clears the sticky fault shadow of a rail._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_mon\_configure**](#function-pwr_mon_configure) (void) <br>_Configures and arms all monitors._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_mon\_getFaults**](#function-pwr_mon_getfaults) ([**pwrRail\_t**](power_8h.md#enum-pwrrail_t) rail, [**pwrMon\_fault\_t**](structpwrMon__fault__t.md) \* faults) <br>_Reads the sticky fault shadow of a rail._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_mon\_poll**](#function-pwr_mon_poll) (void) <br>_Polls all rails and prints the measured values._  |
|  [**pwr\_err\_t**](power_8h.md#enum-pwr_err_t) | [**pwr\_mon\_read**](#function-pwr_mon_read) ([**pwrRail\_t**](power_8h.md#enum-pwrrail_t) rail, [**pwrMon\_values\_t**](structpwrMon__values__t.md) \* values) <br>_Polls bus voltage, current and die temperature of a single rail._  |
|  void | [**pwr\_mon\_reportFaults**](#function-pwr_mon_reportfaults) (void) <br>_Prints all accumulated faults of all rails._  |
|  const char \* | [**pwr\_railName**](#function-pwr_railname) ([**pwrRail\_t**](power_8h.md#enum-pwrrail_t) rail) <br>_Returns the display name of a rail, e.g. "VCCADJ\_FMC1"._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**PWR\_MON\_TABLE**](power_8h.md#define-pwr_mon_table)  `/* multi line expression */`<br>_Rail configuration table (X-macro)._  |
| define  | [**X**](power_8h.md#define-x) (name, addr, pg, vov, iov, tov) `PWR\_RAIL\_##name,`<br> |

## Detailed Description


Rails, I2C addresses, power-good groups and protection limits are defined by [**PWR\_MON\_TABLE**](power_8h.md#define-pwr_mon_table) below; the rail enum is generated from it.


Protection concept:
* Over-voltage, over-current and over-temperature limits are armed with a latched alert (ALATCH = 1). Under-voltage/under-current protection is not used on this board.
* The ALERT pins are wired-OR onto the power-good inputs, so a fault appears as a power-good line going low. [**pwr\_checkAlerts()**](power_8h.md#function-pwr_checkalerts) reads those lines first (no bus traffic) and only queries the devices of an affected group over I2C.
* Reading DIAG\_ALRT clears the latch in the device, therefore the module keeps a sticky fault shadow (see [**pwr\_mon\_getFaults()**](power_8h.md#function-pwr_mon_getfaults)). 




    
## Public Types Documentation




### enum pwrFmc\_t 

```C++
enum pwrFmc_t {
    PWR_FMC1 = 0,
    PWR_FMC2,
    PWR_FMC_COUNT
};
```



FMC Vadj regulator identifiers. 


        

<hr>



### enum pwrPgLine\_t 

```C++
enum pwrPgLine_t {
    PWR_PG_SYS = 0,
    PWR_PG_FMC1_SUP,
    PWR_PG_FMC2_SUP,
    PWR_PG_COUNT
};
```



Power-good / ALERT nets the monitors are wired-OR onto. 


        

<hr>



### enum pwrRail\_t 

```C++
enum pwrRail_t {
    PWR_RAIL_12V,
    PWR_RAIL_3V3,
    PWR_RAIL_5V0_SOM,
    PWR_RAIL_1V8,
    PWR_RAIL_VCCADJ_FMC1,
    PWR_RAIL_VCCADJ_FMC2,
    PWR_RAIL_COUNT
};
```




<hr>



### enum pwr\_err\_t 

```C++
enum pwr_err_t {
    PWR_OK =  0,
    PWR_E_PARAM = -1,
    PWR_E_INIT = -2,
    PWR_E_DEVICE = -3,
    PWR_E_RANGE = -4,
    PWR_E_REGULATION = -5
};
```



Status codes of the power function group. 


        

<hr>
## Public Functions Documentation




### function pwr\_checkAlerts 

_Checks the power-good / ALERT lines._ 
```C++
pwr_err_t pwr_checkAlerts (
    void
) 
```



Only if a line is low, the devices of that group are queried over I2C; faults found are merged into the sticky fault shadow. If a line is low but no device reports a fault, the supply itself failed and PWR\_E\_REGULATION is returned.




**Returns:**

PWR\_OK if all lines are high, PWR\_E\_INIT if the monitors are not configured, PWR\_E\_REGULATION if a line is low, PWR\_E\_DEVICE if a monitor query failed. 





        

<hr>



### function pwr\_fmcVadj\_configure 

_Writes control register and voltage setpoint of both FMC Vadj regulators._ 
```C++
pwr_err_t pwr_fmcVadj_configure (
    void
) 
```



Outputs stay disabled. The setpoints are taken from [**BOARD\_FMC1\_VADJ\_V**](boardConfig_8h.md#define-board_fmc1_vadj_v) / [**BOARD\_FMC2\_VADJ\_V**](boardConfig_8h.md#define-board_fmc2_vadj_v).




**Returns:**

PWR\_OK on success, PWR\_E\_INIT if not initialized, PWR\_E\_RANGE if a configured setpoint exceeds [**BOARD\_FMC\_VADJ\_MAX\_V**](boardConfig_8h.md#define-board_fmc_vadj_max_v), PWR\_E\_DEVICE on a device error. 





        

<hr>



### function pwr\_fmcVadj\_enable 

_Enables/disables both FMC Vadj regulator outputs._ 
```C++
pwr_err_t pwr_fmcVadj_enable (
    int enable
) 
```





**Parameters:**


* `enable` Non-zero to enable, zero to disable. 



**Returns:**

PWR\_OK on success, PWR\_E\_INIT if not initialized, PWR\_E\_DEVICE on a device error. 





        

<hr>



### function pwr\_fmcVadj\_isRegulating 

_Checks whether the regulator reports a valid output voltage setting._ 
```C++
pwr_err_t pwr_fmcVadj_isRegulating (
    pwrFmc_t fmc
) 
```





**Parameters:**


* `fmc` Regulator to check. 



**Returns:**

PWR\_OK if the voltage register reads back the programmed value, PWR\_E\_PARAM if `fmc` is invalid, PWR\_E\_INIT if not initialized, PWR\_E\_DEVICE on a device error, PWR\_E\_REGULATION on a mismatch. 





        

<hr>



### function pwr\_init 

_Creates all device handles (regulators and monitors)._ 
```C++
pwr_err_t pwr_init (
    void
) 
```



No bus traffic, no device configuration. Requires [**i2cBus\_init()**](i2cBus_8c.md#function-i2cbus_init) to have succeeded.




**Returns:**

PWR\_OK on success, PWR\_E\_INIT if a handle could not be created. 





        

<hr>



### function pwr\_mon\_clearFaults 

_Clears the sticky fault shadow of a rail._ 
```C++
void pwr_mon_clearFaults (
    pwrRail_t rail
) 
```





**Parameters:**


* `rail` Rail to clear (invalid values are ignored). 




        

<hr>



### function pwr\_mon\_configure 

_Configures and arms all monitors._ 
```C++
pwr_err_t pwr_mon_configure (
    void
) 
```



ADC configuration, over-voltage, over-current and over-temperature limits, latched alert, and a clearing read of DIAG\_ALRT.




**Note:**

Call order matters: the ALERT outputs sit on the power-good nets, so this must run after [**boardGpio\_waitSomReady()**](boardGpio_8c.md#function-boardgpio_waitsomready) (a latched alert would otherwise keep SYS\_PG low) and before the FMC supply is switched on.




**Returns:**

PWR\_OK if all monitors were configured, PWR\_E\_INIT if not initialized, PWR\_E\_DEVICE if at least one monitor failed (the remaining monitors are still configured). 





        

<hr>



### function pwr\_mon\_getFaults 

_Reads the sticky fault shadow of a rail._ 
```C++
pwr_err_t pwr_mon_getFaults (
    pwrRail_t rail,
    pwrMon_fault_t * faults
) 
```



Accumulated over all queries since the last clear.




**Parameters:**


* `rail` Rail to query. 
* `faults` Receives the accumulated fault flags. 



**Returns:**

PWR\_OK on success, PWR\_E\_PARAM if `faults` is NULL or `rail` is invalid. 





        

<hr>



### function pwr\_mon\_poll 

_Polls all rails and prints the measured values._ 
```C++
pwr_err_t pwr_mon_poll (
    void
) 
```





**Returns:**

PWR\_OK if all rails were read, otherwise the last error code. 





        

<hr>



### function pwr\_mon\_read 

_Polls bus voltage, current and die temperature of a single rail._ 
```C++
pwr_err_t pwr_mon_read (
    pwrRail_t rail,
    pwrMon_values_t * values
) 
```





**Parameters:**


* `rail` Rail to read. 
* `values` Receives the measurement snapshot on success. 



**Returns:**

PWR\_OK on success, PWR\_E\_PARAM if `values` is NULL or `rail` is invalid, PWR\_E\_INIT if not initialized/configured, PWR\_E\_DEVICE on a device error. 





        

<hr>



### function pwr\_mon\_reportFaults 

_Prints all accumulated faults of all rails._ 
```C++
void pwr_mon_reportFaults (
    void
) 
```




<hr>



### function pwr\_railName 

_Returns the display name of a rail, e.g. "VCCADJ\_FMC1"._ 
```C++
const char * pwr_railName (
    pwrRail_t rail
) 
```





**Parameters:**


* `rail` Rail to name. 



**Returns:**

Name string, or "?" if `rail` is invalid. 





        

<hr>
## Macro Definition Documentation





### define PWR\_MON\_TABLE 

_Rail configuration table (X-macro)._ 
```C++
#define PWR_MON_TABLE `X (12V, I2CBUS_PWRMON_12V_ADDR , PWR_PG_SYS ,                            13.20f,  5.0f, 95.0f) \ X (3V3, I2CBUS_PWRMON_3V3_ADDR , PWR_PG_SYS ,                             3.60f, 12.0f, 95.0f) \ X (5V0_SOM, I2CBUS_PWRMON_5V0_ADDR , PWR_PG_SYS ,                             5.50f,  5.0f, 95.0f) \ X (1V8, I2CBUS_PWRMON_1V8_ADDR , PWR_PG_SYS ,                             1.98f,  5.0f, 95.0f) \ X (VCCADJ_FMC1, I2CBUS_PWRMON_FMC1_ADDR , PWR_PG_FMC1_SUP ,  ( BOARD_FMC_VADJ_MAX_V +0.2f),  4.5f, 95.0f) \ X (VCCADJ_FMC2, I2CBUS_PWRMON_FMC2_ADDR , PWR_PG_FMC2_SUP ,  ( BOARD_FMC_VADJ_MAX_V +0.2f),  4.5f, 95.0f)`
```



One row per monitored rail. This table is the only place where rails are defined; [**power.h**](power_8h.md)/power.c derive the rail enum, the name strings, the I2C addresses, the power-good group and the protection limits from it.


Columns:
* name: identifier suffix, also used as display name
* addr: 7-bit I2C address on the PMBus (A0/A1 strapping)
* pg: power-good / ALERT net the device is wired to ([**pwrPgLine\_t**](power_8h.md#enum-pwrpgline_t))
* vov: bus over-voltage limit [V]
* iov: over-current limit [A]
* tov: over-temperature limit [degC]






**Note:**

The number of rows must match [**INA745A\_NUM\_INSTANCES**](userDef_8h.md#define-ina745a_num_instances) in [**userDef.h**](userDef_8h.md). 





        

<hr>



### define X 

```C++
#define X (
    name,
    addr,
    pg,
    vov,
    iov,
    tov
) `PWR_RAIL_##name,`
```



Monitored rails, generated from [**PWR\_MON\_TABLE**](power_8h.md#define-pwr_mon_table) (one PWR\_RAIL\_&lt;name&gt; per row). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/power.h`

