

# File si5332\_cfg.h



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**si5332**](dir_f47f1daa29e3b569dba4f7284fbb0336.md) **>** [**si5332\_cfg.h**](si5332__cfg_8h.md)

[Go to the source code of this file](si5332__cfg_8h_source.md)

_Static configuration tables for the Si5332-GM2 Rev D (VCO 2.5 GHz)._ [More...](#detailed-description)

* `#include <type.h>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**si5332\_reg\_t**](structsi5332__reg__t.md) <br> |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  const [**si5332\_reg\_t**](structsi5332__reg__t.md) | [**si5332\_base\_regs**](#variable-si5332_base_regs)  <br>_Base configuration: all registers identical in both CBPro exports._  |
|  const u32 | [**si5332\_base\_regs\_len**](#variable-si5332_base_regs_len)  <br> |
|  const [**si5332\_reg\_t**](structsi5332__reg__t.md) | [**si5332\_src\_clkin2\_100m\_regs**](#variable-si5332_src_clkin2_100m_regs)  <br>_Delta: 100 MHz differential clock on CLKIN\_2. Fpfd = 50 MHz, P = 2, M = 50._  |
|  const u32 | [**si5332\_src\_clkin2\_100m\_regs\_len**](#variable-si5332_src_clkin2_100m_regs_len)  <br> |
|  const [**si5332\_reg\_t**](structsi5332__reg__t.md) | [**si5332\_src\_xtal\_27m\_regs**](#variable-si5332_src_xtal_27m_regs)  <br>_Delta: 27 MHz crystal on XA/XB. Fpfd = 27 MHz, P = 1, M = 92 + 16/27._  |
|  const u32 | [**si5332\_src\_xtal\_27m\_regs\_len**](#variable-si5332_src_xtal_27m_regs_len)  <br> |












































## Detailed Description


Source: ClockBuilder Pro exports (Design ID UNVBB1)
* docs/si5332/Si5332-GM2-RevD-UNVBB1-27MHz\_XTAL-Registers.h
* docs/si5332/Si5332-GM2-RevD-UNVBB1-100MHz\_ClkIn2-Registers.h




Structure: base table (identical in both exports) + one delta per clock source. The pre-/postamble (register 0x06) is NOT included; it is controlled by the driver (READY / ACTIVE). 


    
## Public Attributes Documentation




### variable si5332\_base\_regs 

_Base configuration: all registers identical in both CBPro exports._ 
```C++
const si5332_reg_t si5332_base_regs[];
```



Power-up state: only OUT6 active (OE), OUT0/OUT2 preconfigured but with OE=0, OUT4/OUT7 and ID0/ID1 switched off. 


        

<hr>



### variable si5332\_base\_regs\_len 

```C++
const u32 si5332_base_regs_len;
```



Number of entries in [**si5332\_base\_regs**](si5332__cfg_8h.md#variable-si5332_base_regs). 


        

<hr>



### variable si5332\_src\_clkin2\_100m\_regs 

_Delta: 100 MHz differential clock on CLKIN\_2. Fpfd = 50 MHz, P = 2, M = 50._ 
```C++
const si5332_reg_t si5332_src_clkin2_100m_regs[];
```




<hr>



### variable si5332\_src\_clkin2\_100m\_regs\_len 

```C++
const u32 si5332_src_clkin2_100m_regs_len;
```



Number of entries in [**si5332\_src\_clkin2\_100m\_regs**](si5332__cfg_8h.md#variable-si5332_src_clkin2_100m_regs). 


        

<hr>



### variable si5332\_src\_xtal\_27m\_regs 

_Delta: 27 MHz crystal on XA/XB. Fpfd = 27 MHz, P = 1, M = 92 + 16/27._ 
```C++
const si5332_reg_t si5332_src_xtal_27m_regs[];
```




<hr>



### variable si5332\_src\_xtal\_27m\_regs\_len 

```C++
const u32 si5332_src_xtal_27m_regs_len;
```



Number of entries in [**si5332\_src\_xtal\_27m\_regs**](si5332__cfg_8h.md#variable-si5332_src_xtal_27m_regs). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/si5332/si5332_cfg.h`

