

# File boardGpio.h



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**boardGpio.h**](boardGpio_8h.md)

[Go to the source code of this file](boardGpio_8h_source.md)

_Implementation of GPIO access (interface to FPGA)._ [More...](#detailed-description)

* `#include <type.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  u32 | [**boardGpio\_getInput**](#function-boardgpio_getinput) (void) <br>_Reads the raw input register of the board._  |
|  u32 | [**boardGpio\_getPgStatus**](#function-boardgpio_getpgstatus) (void) <br>_Reads the power-good / ALERT nets._  |
|  void | [**boardGpio\_init**](#function-boardgpio_init) (void) <br>_Drives all outputs to their reset state._  |
|  void | [**boardGpio\_pulseEthPhyReset**](#function-boardgpio_pulseethphyreset) (void) <br>_Generates a reset pulse on the ETH PHY reset line._  |
|  void | [**boardGpio\_releaseEthPhyReset**](#function-boardgpio_releaseethphyreset) (void) <br>_Releases the ETH PHY reset (ETH\_RSTN high)._  |
|  void | [**boardGpio\_releaseSomPor**](#function-boardgpio_releasesompor) (void) <br>_Releases POR\_B of the SoM._  |
|  void | [**boardGpio\_setFmcModuleEnable**](#function-boardgpio_setfmcmoduleenable) (int enable) <br>_Enables/disables the FMC modules (both FMC slots)._  |
|  void | [**boardGpio\_setFmcPgoodRelease**](#function-boardgpio_setfmcpgoodrelease) (int release) <br>_Releases/locks the FMC power-good (both FMC slots)._  |
|  void | [**boardGpio\_setFmcSupplyEnable**](#function-boardgpio_setfmcsupplyenable) (int enable) <br>_Enables/disables the FMC supply (both FMC slots)._  |
|  void | [**boardGpio\_setHdbVccio**](#function-boardgpio_sethdbvccio) (int source2V5) <br>_Selects the VCCIO source of the HDB bank._  |
|  void | [**boardGpio\_setHpcVccio**](#function-boardgpio_sethpcvccio) (int sourceFmc1) <br>_Selects the VCCIO source of the HPC bank._  |
|  void | [**boardGpio\_setStatusLed**](#function-boardgpio_setstatusled) (int state) <br>_Sets the state of the status LED._  |
|  void | [**boardGpio\_waitSomReady**](#function-boardgpio_waitsomready) (void) <br>_Blocks until the SoM has switched on its PL/PS supplies._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**GPIO\_IN\_2V5\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_2v5_pg_bit)  `10`<br> |
| define  | [**GPIO\_IN\_ETH\_PME\_BIT**](boardGpio_8h.md#define-gpio_in_eth_pme_bit)  `14`<br> |
| define  | [**GPIO\_IN\_FMC1\_MOD\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_fmc1_mod_pg_bit)  `8`<br> |
| define  | [**GPIO\_IN\_FMC1\_SUP\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_fmc1_sup_pg_bit)  `6`<br> |
| define  | [**GPIO\_IN\_FMC2\_MOD\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_fmc2_mod_pg_bit)  `9`<br> |
| define  | [**GPIO\_IN\_FMC2\_SUP\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_fmc2_sup_pg_bit)  `7`<br> |
| define  | [**GPIO\_IN\_HDA14N\_BIT**](boardGpio_8h.md#define-gpio_in_hda14n_bit)  `4`<br> |
| define  | [**GPIO\_IN\_HDA14P\_BIT**](boardGpio_8h.md#define-gpio_in_hda14p_bit)  `3`<br> |
| define  | [**GPIO\_IN\_LMK\_INTR\_BIT**](boardGpio_8h.md#define-gpio_in_lmk_intr_bit)  `13`<br> |
| define  | [**GPIO\_IN\_MIO35\_BIT**](boardGpio_8h.md#define-gpio_in_mio35_bit)  `5`<br> |
| define  | [**GPIO\_IN\_SDPWR\_EN\_BIT**](boardGpio_8h.md#define-gpio_in_sdpwr_en_bit)  `2`<br> |
| define  | [**GPIO\_IN\_SYS\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_sys_pg_bit)  `11`<br> |
| define  | [**GPIO\_IN\_SZG\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_szg_pg_bit)  `12`<br> |
| define  | [**GPIO\_IN\_VCCO\_PL\_EN\_BIT**](boardGpio_8h.md#define-gpio_in_vcco_pl_en_bit)  `1`<br> |
| define  | [**GPIO\_IN\_VCCO\_PS\_EN\_BIT**](boardGpio_8h.md#define-gpio_in_vcco_ps_en_bit)  `0`<br> |
| define  | [**GPIO\_OUT\_ETH\_RSTN\_BIT**](boardGpio_8h.md#define-gpio_out_eth_rstn_bit)  `9`<br> |
| define  | [**GPIO\_OUT\_FMC1\_MOD\_EN\_BIT**](boardGpio_8h.md#define-gpio_out_fmc1_mod_en_bit)  `6`<br> |
| define  | [**GPIO\_OUT\_FMC1\_PG\_RLS\_BIT**](boardGpio_8h.md#define-gpio_out_fmc1_pg_rls_bit)  `4`<br> |
| define  | [**GPIO\_OUT\_FMC1\_SUP\_EN\_BIT**](boardGpio_8h.md#define-gpio_out_fmc1_sup_en_bit)  `2`<br> |
| define  | [**GPIO\_OUT\_FMC2\_MOD\_EN\_BIT**](boardGpio_8h.md#define-gpio_out_fmc2_mod_en_bit)  `7`<br> |
| define  | [**GPIO\_OUT\_FMC2\_PG\_RLS\_BIT**](boardGpio_8h.md#define-gpio_out_fmc2_pg_rls_bit)  `5`<br> |
| define  | [**GPIO\_OUT\_FMC2\_SUP\_EN\_BIT**](boardGpio_8h.md#define-gpio_out_fmc2_sup_en_bit)  `3`<br> |
| define  | [**GPIO\_OUT\_HDB\_VCCIO\_SELECT\_BIT**](boardGpio_8h.md#define-gpio_out_hdb_vccio_select_bit)  `1`<br> |
| define  | [**GPIO\_OUT\_HPC\_VCCIO\_SELECT\_BIT**](boardGpio_8h.md#define-gpio_out_hpc_vccio_select_bit)  `0`<br> |
| define  | [**GPIO\_OUT\_POR\_B\_BIT**](boardGpio_8h.md#define-gpio_out_por_b_bit)  `8`<br> |
| define  | [**GPIO\_OUT\_STATUS\_LED\_BIT**](boardGpio_8h.md#define-gpio_out_status_led_bit)  `31`<br> |
| define  | [**GPIO\_PG\_FMC1\_SUP\_MASK**](boardGpio_8h.md#define-gpio_pg_fmc1_sup_mask)  `(1u &lt;&lt; [**GPIO\_IN\_FMC1\_SUP\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_fmc1_sup_pg_bit))`<br> |
| define  | [**GPIO\_PG\_FMC2\_SUP\_MASK**](boardGpio_8h.md#define-gpio_pg_fmc2_sup_mask)  `(1u &lt;&lt; [**GPIO\_IN\_FMC2\_SUP\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_fmc2_sup_pg_bit))`<br> |
| define  | [**GPIO\_PG\_SYS\_MASK**](boardGpio_8h.md#define-gpio_pg_sys_mask)  `(1u &lt;&lt; [**GPIO\_IN\_SYS\_PG\_BIT**](boardGpio_8h.md#define-gpio_in_sys_pg_bit))`<br> |
| define  | [**GPIO\_SOM\_READY\_MASK**](boardGpio_8h.md#define-gpio_som_ready_mask)  `/* multi line expression */`<br> |
| define  | [**GPIO\_STATUS\_LED\_MASK**](boardGpio_8h.md#define-gpio_status_led_mask)  `(1u &lt;&lt; [**GPIO\_OUT\_STATUS\_LED\_BIT**](boardGpio_8h.md#define-gpio_out_status_led_bit))`<br> |

## Detailed Description


This is the only place where GPIO bit positions are evaluated; the input/output bit positions are defined below. The output register is shadowed so that read-modify-write sequences cannot lose bits.




**Note:**

This file is deliberately not called gpio.h - that name is already used by the BSP driver (mcu/software/standalone/driver/gpio.h) and every src directory is on the include path. 





    
## Public Functions Documentation




### function boardGpio\_getInput 

_Reads the raw input register of the board._ 
```C++
u32 boardGpio_getInput (
    void
) 
```





**Returns:**

Input register value (bit positions: GPIO\_IN\_\*\_BIT). 





        

<hr>



### function boardGpio\_getPgStatus 

_Reads the power-good / ALERT nets._ 
```C++
u32 boardGpio_getPgStatus (
    void
) 
```



Masked to [**GPIO\_PG\_SYS\_MASK**](boardGpio_8h.md#define-gpio_pg_sys_mask), [**GPIO\_PG\_FMC1\_SUP\_MASK**](boardGpio_8h.md#define-gpio_pg_fmc1_sup_mask) and [**GPIO\_PG\_FMC2\_SUP\_MASK**](boardGpio_8h.md#define-gpio_pg_fmc2_sup_mask). A bit reads 0 either because the supply is not good or because an INA745A on that net has asserted its (active low, open-drain) ALERT output.




**Returns:**

Masked power-good status bits. 





        

<hr>



### function boardGpio\_init 

_Drives all outputs to their reset state._ 
```C++
void boardGpio_init (
    void
) 
```



Everything off: FMC supplies and modules disabled, POR\_B asserted, ETH PHY in reset. 


        

<hr>



### function boardGpio\_pulseEthPhyReset 

_Generates a reset pulse on the ETH PHY reset line._ 
```C++
void boardGpio_pulseEthPhyReset (
    void
) 
```



Pulse width is [**BOARD\_ETH\_RESET\_PULSE\_US**](boardConfig_8h.md#define-board_eth_reset_pulse_us); the line is high afterwards. 


        

<hr>



### function boardGpio\_releaseEthPhyReset 

_Releases the ETH PHY reset (ETH\_RSTN high)._ 
```C++
void boardGpio_releaseEthPhyReset (
    void
) 
```




<hr>



### function boardGpio\_releaseSomPor 

_Releases POR\_B of the SoM._ 
```C++
void boardGpio_releaseSomPor (
    void
) 
```



Cannot be re-asserted by this API on purpose. 


        

<hr>



### function boardGpio\_setFmcModuleEnable 

_Enables/disables the FMC modules (both FMC slots)._ 
```C++
void boardGpio_setFmcModuleEnable (
    int enable
) 
```





**Parameters:**


* `enable` Non-zero to enable, zero to disable. 




        

<hr>



### function boardGpio\_setFmcPgoodRelease 

_Releases/locks the FMC power-good (both FMC slots)._ 
```C++
void boardGpio_setFmcPgoodRelease (
    int release
) 
```



Power-good is forced low if locked.




**Parameters:**


* `release` Non-zero to release, zero to force low. 




        

<hr>



### function boardGpio\_setFmcSupplyEnable 

_Enables/disables the FMC supply (both FMC slots)._ 
```C++
void boardGpio_setFmcSupplyEnable (
    int enable
) 
```





**Parameters:**


* `enable` Non-zero to enable, zero to disable. 




        

<hr>



### function boardGpio\_setHdbVccio 

_Selects the VCCIO source of the HDB bank._ 
```C++
void boardGpio_setHdbVccio (
    int source2V5
) 
```





**Parameters:**


* `source2V5` Non-zero: 2.5V, zero: FMC2-Vadj. 




        

<hr>



### function boardGpio\_setHpcVccio 

_Selects the VCCIO source of the HPC bank._ 
```C++
void boardGpio_setHpcVccio (
    int sourceFmc1
) 
```





**Parameters:**


* `sourceFmc1` Non-zero: FMC1-Vadj, zero: FMC2-Vadj. 




        

<hr>



### function boardGpio\_setStatusLed 

_Sets the state of the status LED._ 
```C++
void boardGpio_setStatusLed (
    int state
) 
```





**Parameters:**


* `state` Non-zero: LED on, zero: LED off. 




        

<hr>



### function boardGpio\_waitSomReady 

_Blocks until the SoM has switched on its PL/PS supplies._ 
```C++
void boardGpio_waitSomReady (
    void
) 
```



Waits until VCCO\_PL\_EN, VCCO\_PS\_EN and SYS\_PG are all asserted (see [**GPIO\_SOM\_READY\_MASK**](boardGpio_8h.md#define-gpio_som_ready_mask)). 


        

<hr>
## Macro Definition Documentation





### define GPIO\_IN\_2V5\_PG\_BIT 

```C++
#define GPIO_IN_2V5_PG_BIT `10`
```



2.5V rail power-good. 


        

<hr>



### define GPIO\_IN\_ETH\_PME\_BIT 

```C++
#define GPIO_IN_ETH_PME_BIT `14`
```



ETH PHY power-management event. 


        

<hr>



### define GPIO\_IN\_FMC1\_MOD\_PG\_BIT 

```C++
#define GPIO_IN_FMC1_MOD_PG_BIT `8`
```



FMC1 module power-good. 


        

<hr>



### define GPIO\_IN\_FMC1\_SUP\_PG\_BIT 

```C++
#define GPIO_IN_FMC1_SUP_PG_BIT `6`
```



FMC1 supply power-good / ALERT net. 


        

<hr>



### define GPIO\_IN\_FMC2\_MOD\_PG\_BIT 

```C++
#define GPIO_IN_FMC2_MOD_PG_BIT `9`
```



FMC2 module power-good. 


        

<hr>



### define GPIO\_IN\_FMC2\_SUP\_PG\_BIT 

```C++
#define GPIO_IN_FMC2_SUP_PG_BIT `7`
```



FMC2 supply power-good / ALERT net. 


        

<hr>



### define GPIO\_IN\_HDA14N\_BIT 

```C++
#define GPIO_IN_HDA14N_BIT `4`
```



HDA14N input. 


        

<hr>



### define GPIO\_IN\_HDA14P\_BIT 

```C++
#define GPIO_IN_HDA14P_BIT `3`
```



HDA14P input. 


        

<hr>



### define GPIO\_IN\_LMK\_INTR\_BIT 

```C++
#define GPIO_IN_LMK_INTR_BIT `13`
```



LMK interrupt input. 


        

<hr>



### define GPIO\_IN\_MIO35\_BIT 

```C++
#define GPIO_IN_MIO35_BIT `5`
```



MIO35 input. 


        

<hr>



### define GPIO\_IN\_SDPWR\_EN\_BIT 

```C++
#define GPIO_IN_SDPWR_EN_BIT `2`
```



SD card power enable indication. 


        

<hr>



### define GPIO\_IN\_SYS\_PG\_BIT 

```C++
#define GPIO_IN_SYS_PG_BIT `11`
```



System power-good / ALERT net. 


        

<hr>



### define GPIO\_IN\_SZG\_PG\_BIT 

```C++
#define GPIO_IN_SZG_PG_BIT `12`
```



SYZYGY power-good. 


        

<hr>



### define GPIO\_IN\_VCCO\_PL\_EN\_BIT 

```C++
#define GPIO_IN_VCCO_PL_EN_BIT `1`
```



SoM PL supply enable indication. 


        

<hr>



### define GPIO\_IN\_VCCO\_PS\_EN\_BIT 

```C++
#define GPIO_IN_VCCO_PS_EN_BIT `0`
```



SoM PS supply enable indication. 


        

<hr>



### define GPIO\_OUT\_ETH\_RSTN\_BIT 

```C++
#define GPIO_OUT_ETH_RSTN_BIT `9`
```



ETH PHY reset (active low, 0 = in reset). 


        

<hr>



### define GPIO\_OUT\_FMC1\_MOD\_EN\_BIT 

```C++
#define GPIO_OUT_FMC1_MOD_EN_BIT `6`
```



FMC1 module enable. 


        

<hr>



### define GPIO\_OUT\_FMC1\_PG\_RLS\_BIT 

```C++
#define GPIO_OUT_FMC1_PG_RLS_BIT `4`
```



FMC1 power-good release. 


        

<hr>



### define GPIO\_OUT\_FMC1\_SUP\_EN\_BIT 

```C++
#define GPIO_OUT_FMC1_SUP_EN_BIT `2`
```



FMC1 supply enable. 


        

<hr>



### define GPIO\_OUT\_FMC2\_MOD\_EN\_BIT 

```C++
#define GPIO_OUT_FMC2_MOD_EN_BIT `7`
```



FMC2 module enable. 


        

<hr>



### define GPIO\_OUT\_FMC2\_PG\_RLS\_BIT 

```C++
#define GPIO_OUT_FMC2_PG_RLS_BIT `5`
```



FMC2 power-good release. 


        

<hr>



### define GPIO\_OUT\_FMC2\_SUP\_EN\_BIT 

```C++
#define GPIO_OUT_FMC2_SUP_EN_BIT `3`
```



FMC2 supply enable. 


        

<hr>



### define GPIO\_OUT\_HDB\_VCCIO\_SELECT\_BIT 

```C++
#define GPIO_OUT_HDB_VCCIO_SELECT_BIT `1`
```



HDB VCCIO source select (0 = FMC2-Vadj, 1 = 2.5V). 


        

<hr>



### define GPIO\_OUT\_HPC\_VCCIO\_SELECT\_BIT 

```C++
#define GPIO_OUT_HPC_VCCIO_SELECT_BIT `0`
```



HPC VCCIO source select (0 = FMC2, 1 = FMC1). 


        

<hr>



### define GPIO\_OUT\_POR\_B\_BIT 

```C++
#define GPIO_OUT_POR_B_BIT `8`
```



SoM POR\_B (active low, 0 = reset asserted). 


        

<hr>



### define GPIO\_OUT\_STATUS\_LED\_BIT 

```C++
#define GPIO_OUT_STATUS_LED_BIT `31`
```



Status LED. 


        

<hr>



### define GPIO\_PG\_FMC1\_SUP\_MASK 

```C++
#define GPIO_PG_FMC1_SUP_MASK `(1u << GPIO_IN_FMC1_SUP_PG_BIT )`
```



FMC1\_SUP\_PG net mask. 


        

<hr>



### define GPIO\_PG\_FMC2\_SUP\_MASK 

```C++
#define GPIO_PG_FMC2_SUP_MASK `(1u << GPIO_IN_FMC2_SUP_PG_BIT )`
```



FMC2\_SUP\_PG net mask. 


        

<hr>



### define GPIO\_PG\_SYS\_MASK 

```C++
#define GPIO_PG_SYS_MASK `(1u << GPIO_IN_SYS_PG_BIT )`
```



SYS\_PG net mask. 


        

<hr>



### define GPIO\_SOM\_READY\_MASK 

```C++
#define GPIO_SOM_READY_MASK `/* multi line expression */`
```



Signals that indicate that the SoM has switched on its PL/PS supplies. 


        

<hr>



### define GPIO\_STATUS\_LED\_MASK 

```C++
#define GPIO_STATUS_LED_MASK `(1u << GPIO_OUT_STATUS_LED_BIT )`
```



Status LED output mask. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/boardGpio.h`

