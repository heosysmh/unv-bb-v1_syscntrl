

# File boardConfig.h



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**boardConfig.h**](boardConfig_8h.md)

[Go to the source code of this file](boardConfig_8h_source.md)

_Single configuration point of the system controller application. Set your FMC voltages and clock config here._ [More...](#detailed-description)

* `#include "si5332.h"`
































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**BOARD\_CLKGEN\_OUT0\_EN**](boardConfig_8h.md#define-board_clkgen_out0_en)  `0`<br> |
| define  | [**BOARD\_CLKGEN\_OUT0\_FORMAT**](boardConfig_8h.md#define-board_clkgen_out0_format)  `SI5332\_FMT\_LVDS\_FAST`<br> |
| define  | [**BOARD\_CLKGEN\_OUT0\_FREQ\_HZ**](boardConfig_8h.md#define-board_clkgen_out0_freq_hz)  `[**SI5332\_100MHZ**](si5332_8h.md#define-si5332_100mhz)`<br> |
| define  | [**BOARD\_CLKGEN\_OUT2\_EN**](boardConfig_8h.md#define-board_clkgen_out2_en)  `0`<br> |
| define  | [**BOARD\_CLKGEN\_OUT2\_FORMAT**](boardConfig_8h.md#define-board_clkgen_out2_format)  `SI5332\_FMT\_LVDS\_FAST`<br> |
| define  | [**BOARD\_CLKGEN\_OUT2\_FREQ\_HZ**](boardConfig_8h.md#define-board_clkgen_out2_freq_hz)  `[**SI5332\_125MHZ**](si5332_8h.md#define-si5332_125mhz)`<br> |
| define  | [**BOARD\_CLKGEN\_OUT4\_EN**](boardConfig_8h.md#define-board_clkgen_out4_en)  `0`<br> |
| define  | [**BOARD\_CLKGEN\_OUT4\_FORMAT**](boardConfig_8h.md#define-board_clkgen_out4_format)  `SI5332\_FMT\_LVDS\_FAST`<br> |
| define  | [**BOARD\_CLKGEN\_OUT4\_FREQ\_HZ**](boardConfig_8h.md#define-board_clkgen_out4_freq_hz)  `[**SI5332\_100MHZ**](si5332_8h.md#define-si5332_100mhz)`<br> |
| define  | [**BOARD\_CLKGEN\_OUT7\_EN**](boardConfig_8h.md#define-board_clkgen_out7_en)  `0`<br> |
| define  | [**BOARD\_CLKGEN\_OUT7\_FORMAT**](boardConfig_8h.md#define-board_clkgen_out7_format)  `SI5332\_FMT\_LVDS\_FAST`<br> |
| define  | [**BOARD\_CLKGEN\_OUT7\_FREQ\_HZ**](boardConfig_8h.md#define-board_clkgen_out7_freq_hz)  `[**SI5332\_125MHZ**](si5332_8h.md#define-si5332_125mhz)`<br> |
| define  | [**BOARD\_CLKGEN\_SRC**](boardConfig_8h.md#define-board_clkgen_src)  `SI5332\_CLK\_SRC\_XTAL\_27M`<br> |
| define  | [**BOARD\_CLK\_SETTLE\_US**](boardConfig_8h.md#define-board_clk_settle_us)  `1000u`<br> |
| define  | [**BOARD\_ETH\_RESET\_PULSE\_US**](boardConfig_8h.md#define-board_eth_reset_pulse_us)  `10u`<br> |
| define  | [**BOARD\_FMC1\_VADJ\_V**](boardConfig_8h.md#define-board_fmc1_vadj_v)  `1.80f`<br> |
| define  | [**BOARD\_FMC2\_VADJ\_V**](boardConfig_8h.md#define-board_fmc2_vadj_v)  `1.80f`<br> |
| define  | [**BOARD\_FMC\_VADJ\_MAX\_V**](boardConfig_8h.md#define-board_fmc_vadj_max_v)  `1.80f`<br> |
| define  | [**BOARD\_MONITOR\_PERIOD\_US**](boardConfig_8h.md#define-board_monitor_period_us)  `1000000`<br> |
| define  | [**BOARD\_POWER\_SETTLE\_US**](boardConfig_8h.md#define-board_power_settle_us)  `1000u`<br> |
| define  | [**BOARD\_VCCIO\_SELECT\_HDB**](boardConfig_8h.md#define-board_vccio_select_hdb)  `0`<br> |
| define  | [**BOARD\_VCCIO\_SELECT\_HPC**](boardConfig_8h.md#define-board_vccio_select_hpc)  `0`<br> |

## Detailed Description


Sections:
* Startup Timing
* VCCIO selection (HPC/HDB)
* FMC Vadj (tpsm8286x)
* Clock generator (si5332) 




    
## Macro Definition Documentation





### define BOARD\_CLKGEN\_OUT0\_EN 

```C++
#define BOARD_CLKGEN_OUT0_EN `0`
```



OUT0 enable at startup. 


        

<hr>



### define BOARD\_CLKGEN\_OUT0\_FORMAT 

```C++
#define BOARD_CLKGEN_OUT0_FORMAT `SI5332_FMT_LVDS_FAST`
```



OUT0 signal format. 


        

<hr>



### define BOARD\_CLKGEN\_OUT0\_FREQ\_HZ 

```C++
#define BOARD_CLKGEN_OUT0_FREQ_HZ `SI5332_100MHZ`
```



OUT0 frequency [Hz]. 


        

<hr>



### define BOARD\_CLKGEN\_OUT2\_EN 

```C++
#define BOARD_CLKGEN_OUT2_EN `0`
```



OUT2 enable at startup. 


        

<hr>



### define BOARD\_CLKGEN\_OUT2\_FORMAT 

```C++
#define BOARD_CLKGEN_OUT2_FORMAT `SI5332_FMT_LVDS_FAST`
```



OUT2 signal format. 


        

<hr>



### define BOARD\_CLKGEN\_OUT2\_FREQ\_HZ 

```C++
#define BOARD_CLKGEN_OUT2_FREQ_HZ `SI5332_125MHZ`
```



OUT2 frequency [Hz]. 


        

<hr>



### define BOARD\_CLKGEN\_OUT4\_EN 

```C++
#define BOARD_CLKGEN_OUT4_EN `0`
```



OUT4 enable at startup. 


        

<hr>



### define BOARD\_CLKGEN\_OUT4\_FORMAT 

```C++
#define BOARD_CLKGEN_OUT4_FORMAT `SI5332_FMT_LVDS_FAST`
```



OUT4 signal format. 


        

<hr>



### define BOARD\_CLKGEN\_OUT4\_FREQ\_HZ 

```C++
#define BOARD_CLKGEN_OUT4_FREQ_HZ `SI5332_100MHZ`
```



OUT4 frequency [Hz]. 


        

<hr>



### define BOARD\_CLKGEN\_OUT7\_EN 

```C++
#define BOARD_CLKGEN_OUT7_EN `0`
```



OUT7 enable at startup. 


        

<hr>



### define BOARD\_CLKGEN\_OUT7\_FORMAT 

```C++
#define BOARD_CLKGEN_OUT7_FORMAT `SI5332_FMT_LVDS_FAST`
```



OUT7 signal format. 


        

<hr>



### define BOARD\_CLKGEN\_OUT7\_FREQ\_HZ 

```C++
#define BOARD_CLKGEN_OUT7_FREQ_HZ `SI5332_125MHZ`
```



OUT7 frequency [Hz]. 


        

<hr>



### define BOARD\_CLKGEN\_SRC 

```C++
#define BOARD_CLKGEN_SRC `SI5332_CLK_SRC_XTAL_27M`
```



PLL input clock source. 


        

<hr>



### define BOARD\_CLK\_SETTLE\_US 

```C++
#define BOARD_CLK_SETTLE_US `1000u`
```



Wait for clock outputs [us]. 


        

<hr>



### define BOARD\_ETH\_RESET\_PULSE\_US 

```C++
#define BOARD_ETH_RESET_PULSE_US `10u`
```



ETH PHY reset pulse width [us]. 


        

<hr>



### define BOARD\_FMC1\_VADJ\_V 

```C++
#define BOARD_FMC1_VADJ_V `1.80f`
```



FMC1 Vadj setpoint [V]. 


        

<hr>



### define BOARD\_FMC2\_VADJ\_V 

```C++
#define BOARD_FMC2_VADJ_V `1.80f`
```



FMC2 Vadj setpoint [V]. 


        

<hr>



### define BOARD\_FMC\_VADJ\_MAX\_V 

```C++
#define BOARD_FMC_VADJ_MAX_V `1.80f`
```



Set according to the FPGAs VCCIO limits, checked at startup and used to define power monitor limits. Hard upper bound for both FMC Vadj rails [V]. 


        

<hr>



### define BOARD\_MONITOR\_PERIOD\_US 

```C++
#define BOARD_MONITOR_PERIOD_US `1000000`
```



Period of the monitoring loop [us]. 


        

<hr>



### define BOARD\_POWER\_SETTLE\_US 

```C++
#define BOARD_POWER_SETTLE_US `1000u`
```



Wait for supplies to stabilize [us]. 


        

<hr>



### define BOARD\_VCCIO\_SELECT\_HDB 

```C++
#define BOARD_VCCIO_SELECT_HDB `0`
```



HDB VCCIO source: 0 = FMC2-Vadj, 1 = 2.5V. 


        

<hr>



### define BOARD\_VCCIO\_SELECT\_HPC 

```C++
#define BOARD_VCCIO_SELECT_HPC `0`
```



HPC VCCIO source: 0 = FMC2-Vadj, 1 = FMC1-Vadj. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/boardConfig.h`

