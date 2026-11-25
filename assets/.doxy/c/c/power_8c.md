

# File power.c



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**power.c**](power_8c.md)

[Go to the source code of this file](power_8c_source.md)

_Implementation of the power functions._ 

* `#include "ina745a.h"`
* `#include "tpsm8286x.h"`
* `#include "power.h"`
* `#include "bsp.h"`
* `#include "userDef.h"`
* `#include "boardGpio.h"`
* `#include "i2cBus.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**pwrFmcCfg\_t**](structpwrFmcCfg__t.md) <br> |
| struct | [**pwrMonCfg\_t**](structpwrMonCfg__t.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef char | [**pwr\_assert\_fmcCount**](#typedef-pwr_assert_fmccount)  <br> |
| typedef char | [**pwr\_assert\_railCount**](#typedef-pwr_assert_railcount)  <br> |






## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**pwrMon\_fault\_t**](structpwrMon__fault__t.md) | [**gFaultLatch**](#variable-gfaultlatch)  <br> |
|  const [**pwrFmcCfg\_t**](structpwrFmcCfg__t.md) | [**gFmcCfg**](#variable-gfmccfg)   = `/* multi line expression */`<br> |
|  [**tpsm8286\_t**](structtpsm8286__t.md) \* | [**gFmcVadj**](#variable-gfmcvadj)  <br> |
|  [**ina745a\_t**](structina745a__t.md) \* | [**gMon**](#variable-gmon)  <br> |
|  const [**pwrMonCfg\_t**](structpwrMonCfg__t.md) | [**gMonCfg**](#variable-gmoncfg)   = `/* multi line expression */`<br> |
|  u8 | [**gMonConfigured**](#variable-gmonconfigured)  <br> |
|  const u32 | [**gPgMask**](#variable-gpgmask)   = `/* multi line expression */`<br> |
|  const char \*const | [**gPgName**](#variable-gpgname)   = `/* multi line expression */`<br> |














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


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  int | [**pwr\_mergeFaults**](#function-pwr_mergefaults) ([**pwrRail\_t**](power_8h.md#enum-pwrrail_t) rail, const [**ina745a\_diag\_flags\_t**](structina745a__diag__flags__t.md) \* flags) <br>_Merges the device diagnostics into the sticky fault shadow._  |
|  int | [**pwr\_queryGroup**](#function-pwr_querygroup) ([**pwrPgLine\_t**](power_8h.md#enum-pwrpgline_t) line) <br>_Queries all monitors of one power-good group._  |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**PWR\_FMC\_VADJ\_CNTRL**](power_8c.md#define-pwr_fmc_vadj_cntrl)  `/* multi line expression */`<br> |
| define  | [**PWR\_MON\_ALERT\_CNVR**](power_8c.md#define-pwr_mon_alert_cnvr)  `0`<br> |
| define  | [**PWR\_MON\_ALERT\_LATCH**](power_8c.md#define-pwr_mon_alert_latch)  `1`<br> |
| define  | [**PWR\_MON\_ALERT\_POL**](power_8c.md#define-pwr_mon_alert_pol)  `INA745A\_ALERT\_ACTIVE\_LOW`<br> |
| define  | [**PWR\_MON\_ALERT\_SLOW**](power_8c.md#define-pwr_mon_alert_slow)  `0   /\* compare against the measured value \*/`<br> |
| define  | [**PWR\_MON\_AVG**](power_8c.md#define-pwr_mon_avg)  `INA745A\_AVG\_128`<br> |
| define  | [**PWR\_MON\_MODE**](power_8c.md#define-pwr_mon_mode)  `INA745A\_MODE\_CONT\_TEMP\_CURRENT\_BUSV`<br> |
| define  | [**PWR\_MON\_TCT**](power_8c.md#define-pwr_mon_tct)  `INA745A\_CT\_150US`<br> |
| define  | [**PWR\_MON\_UI\_DISABLED\_A**](power_8c.md#define-pwr_mon_ui_disabled_a)  `(-39.0f)`<br> |
| define  | [**PWR\_MON\_UV\_DISABLED\_V**](power_8c.md#define-pwr_mon_uv_disabled_v)  `0.0f`<br> |
| define  | [**PWR\_MON\_VBUSCT**](power_8c.md#define-pwr_mon_vbusct)  `INA745A\_CT\_150US`<br> |
| define  | [**PWR\_MON\_VSENCT**](power_8c.md#define-pwr_mon_vsenct)  `INA745A\_CT\_150US`<br> |
| define  | [**X**](power_8c.md#define-x) (name, addr, pg, vov, iov, tov) `{ #name, addr, pg, vov, iov, tov },`<br> |

## Public Types Documentation




### typedef pwr\_assert\_fmcCount 

```C++
typedef char pwr_assert_fmcCount[(PWR_FMC_COUNT==2) ? 1 :-1];
```




<hr>



### typedef pwr\_assert\_railCount 

```C++
typedef char pwr_assert_railCount[(PWR_RAIL_COUNT==6) ? 1 :-1];
```




<hr>
## Public Static Attributes Documentation




### variable gFaultLatch 

```C++
pwrMon_fault_t gFaultLatch[PWR_RAIL_COUNT];
```



Sticky fault shadow: DIAG\_ALRT clears on read, the application must not. 


        

<hr>



### variable gFmcCfg 

```C++
const pwrFmcCfg_t gFmcCfg[PWR_FMC_COUNT];
```




<hr>



### variable gFmcVadj 

```C++
tpsm8286_t* gFmcVadj[PWR_FMC_COUNT];
```




<hr>



### variable gMon 

```C++
ina745a_t* gMon[PWR_RAIL_COUNT];
```




<hr>



### variable gMonCfg 

```C++
const pwrMonCfg_t gMonCfg[PWR_RAIL_COUNT];
```




<hr>



### variable gMonConfigured 

```C++
u8 gMonConfigured;
```




<hr>



### variable gPgMask 

```C++
const u32 gPgMask[PWR_PG_COUNT];
```



Power-good / ALERT line masks, indexed by [**pwrPgLine\_t**](power_8h.md#enum-pwrpgline_t). 


        

<hr>



### variable gPgName 

```C++
const char* const gPgName[PWR_PG_COUNT];
```




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
## Public Static Functions Documentation




### function pwr\_mergeFaults 

_Merges the device diagnostics into the sticky fault shadow._ 
```C++
static int pwr_mergeFaults (
    pwrRail_t rail,
    const ina745a_diag_flags_t * flags
) 
```



Under-voltage and under-current flags are ignored because those limits are programmed to non-trippable values (see [**PWR\_MON\_UV\_DISABLED\_V**](power_8c.md#define-pwr_mon_uv_disabled_v) and [**PWR\_MON\_UI\_DISABLED\_A**](power_8c.md#define-pwr_mon_ui_disabled_a)).




**Parameters:**


* `rail` Rail the diagnostics belong to. 
* `flags` Decoded DIAG\_ALRT flags of the device. 



**Returns:**

Non-zero if at least one fault bit was set in `flags`. 





        

<hr>



### function pwr\_queryGroup 

_Queries all monitors of one power-good group._ 
```C++
static int pwr_queryGroup (
    pwrPgLine_t line
) 
```





**Parameters:**


* `line` Power-good / ALERT net whose monitors are queried. 



**Returns:**

Number of devices that reported a fault, or -1 on a device error. 





        

<hr>
## Macro Definition Documentation





### define PWR\_FMC\_VADJ\_CNTRL 

```C++
#define PWR_FMC_VADJ_CNTRL `( TPSM8286_FPWM_AT_VC_MASK | \ TPSM8286_FPWM_MASK | \ TPSM8286_OUTPUT_DISCHARGE_MASK | \ TPSM8286_HICCUP_EN_MASK | \ TPSM8286_VOLTAGE_RAMP_SPEED_1MV )`
```




<hr>



### define PWR\_MON\_ALERT\_CNVR 

```C++
#define PWR_MON_ALERT_CNVR `0`
```




<hr>



### define PWR\_MON\_ALERT\_LATCH 

```C++
#define PWR_MON_ALERT_LATCH `1`
```




<hr>



### define PWR\_MON\_ALERT\_POL 

```C++
#define PWR_MON_ALERT_POL `INA745A_ALERT_ACTIVE_LOW`
```




<hr>



### define PWR\_MON\_ALERT\_SLOW 

```C++
#define PWR_MON_ALERT_SLOW `0   /* compare against the measured value */`
```




<hr>



### define PWR\_MON\_AVG 

```C++
#define PWR_MON_AVG `INA745A_AVG_128`
```




<hr>



### define PWR\_MON\_MODE 

```C++
#define PWR_MON_MODE `INA745A_MODE_CONT_TEMP_CURRENT_BUSV`
```




<hr>



### define PWR\_MON\_TCT 

```C++
#define PWR_MON_TCT `INA745A_CT_150US`
```




<hr>



### define PWR\_MON\_UI\_DISABLED\_A 

```C++
#define PWR_MON_UI_DISABLED_A `(-39.0f)`
```




<hr>



### define PWR\_MON\_UV\_DISABLED\_V 

```C++
#define PWR_MON_UV_DISABLED_V `0.0f`
```




<hr>



### define PWR\_MON\_VBUSCT 

```C++
#define PWR_MON_VBUSCT `INA745A_CT_150US`
```




<hr>



### define PWR\_MON\_VSENCT 

```C++
#define PWR_MON_VSENCT `INA745A_CT_150US`
```




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
) `{ #name, addr, pg, vov, iov, tov },`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/power.c`

