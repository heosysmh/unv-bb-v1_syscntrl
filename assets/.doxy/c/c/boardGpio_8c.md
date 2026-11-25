

# File boardGpio.c



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**boardGpio.c**](boardGpio_8c.md)

[Go to the source code of this file](boardGpio_8c_source.md)

_Implementation of GPIO access (interface to FPGA)._ 

* `#include "boardGpio.h"`
* `#include "bsp.h"`
* `#include "gpio.h"`
* `#include "boardConfig.h"`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  u32 | [**gOutShadow**](#variable-goutshadow)   = `0`<br> |














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


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**boardGpio\_modifyOutput**](#function-boardgpio_modifyoutput) (u32 mask, u32 value) <br>_Applies_ `value` _to the masked output bits via the shadow register._ |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**GPIO\_ETH\_RSTN\_MASK**](boardGpio_8c.md#define-gpio_eth_rstn_mask)  `(1u &lt;&lt; [**GPIO\_OUT\_ETH\_RSTN\_BIT**](boardGpio_8h.md#define-gpio_out_eth_rstn_bit))`<br> |
| define  | [**GPIO\_FMC\_MOD\_EN\_MASK**](boardGpio_8c.md#define-gpio_fmc_mod_en_mask)  `/* multi line expression */`<br> |
| define  | [**GPIO\_FMC\_PG\_RLS\_MASK**](boardGpio_8c.md#define-gpio_fmc_pg_rls_mask)  `/* multi line expression */`<br> |
| define  | [**GPIO\_FMC\_SUP\_EN\_MASK**](boardGpio_8c.md#define-gpio_fmc_sup_en_mask)  `/* multi line expression */`<br> |
| define  | [**GPIO\_HDB\_VCCIO\_SELECT\_MASK**](boardGpio_8c.md#define-gpio_hdb_vccio_select_mask)  `(1u &lt;&lt; [**GPIO\_OUT\_HDB\_VCCIO\_SELECT\_BIT**](boardGpio_8h.md#define-gpio_out_hdb_vccio_select_bit))`<br> |
| define  | [**GPIO\_HPC\_VCCIO\_SELECT\_MASK**](boardGpio_8c.md#define-gpio_hpc_vccio_select_mask)  `(1u &lt;&lt; [**GPIO\_OUT\_HPC\_VCCIO\_SELECT\_BIT**](boardGpio_8h.md#define-gpio_out_hpc_vccio_select_bit))`<br> |
| define  | [**GPIO\_IN**](boardGpio_8c.md#define-gpio_in)  `SYSTEM\_GPIO\_0\_IO\_CTRL`<br> |
| define  | [**GPIO\_OUT**](boardGpio_8c.md#define-gpio_out)  `SYSTEM\_GPIO\_1\_IO\_CTRL`<br> |
| define  | [**GPIO\_PG\_MASK**](boardGpio_8c.md#define-gpio_pg_mask)  `/* multi line expression */`<br> |
| define  | [**GPIO\_POR\_B\_MASK**](boardGpio_8c.md#define-gpio_por_b_mask)  `(1u &lt;&lt; [**GPIO\_OUT\_POR\_B\_BIT**](boardGpio_8h.md#define-gpio_out_por_b_bit))`<br> |

## Public Static Attributes Documentation




### variable gOutShadow 

```C++
u32 gOutShadow;
```



Shadow of the output register; the hardware register is write-only safe. 


        

<hr>
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
## Public Static Functions Documentation




### function boardGpio\_modifyOutput 

_Applies_ `value` _to the masked output bits via the shadow register._
```C++
static void boardGpio_modifyOutput (
    u32 mask,
    u32 value
) 
```




<hr>
## Macro Definition Documentation





### define GPIO\_ETH\_RSTN\_MASK 

```C++
#define GPIO_ETH_RSTN_MASK `(1u << GPIO_OUT_ETH_RSTN_BIT )`
```




<hr>



### define GPIO\_FMC\_MOD\_EN\_MASK 

```C++
#define GPIO_FMC_MOD_EN_MASK `/* multi line expression */`
```




<hr>



### define GPIO\_FMC\_PG\_RLS\_MASK 

```C++
#define GPIO_FMC_PG_RLS_MASK `/* multi line expression */`
```




<hr>



### define GPIO\_FMC\_SUP\_EN\_MASK 

```C++
#define GPIO_FMC_SUP_EN_MASK `/* multi line expression */`
```




<hr>



### define GPIO\_HDB\_VCCIO\_SELECT\_MASK 

```C++
#define GPIO_HDB_VCCIO_SELECT_MASK `(1u << GPIO_OUT_HDB_VCCIO_SELECT_BIT )`
```




<hr>



### define GPIO\_HPC\_VCCIO\_SELECT\_MASK 

```C++
#define GPIO_HPC_VCCIO_SELECT_MASK `(1u << GPIO_OUT_HPC_VCCIO_SELECT_BIT )`
```




<hr>



### define GPIO\_IN 

```C++
#define GPIO_IN `SYSTEM_GPIO_0_IO_CTRL`
```




<hr>



### define GPIO\_OUT 

```C++
#define GPIO_OUT `SYSTEM_GPIO_1_IO_CTRL`
```




<hr>



### define GPIO\_PG\_MASK 

```C++
#define GPIO_PG_MASK `( GPIO_PG_SYS_MASK | \ GPIO_PG_FMC1_SUP_MASK | \ GPIO_PG_FMC2_SUP_MASK )`
```




<hr>



### define GPIO\_POR\_B\_MASK 

```C++
#define GPIO_POR_B_MASK `(1u << GPIO_OUT_POR_B_BIT )`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/boardGpio.c`

