

# File si5332\_cfg.c



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**si5332**](dir_f47f1daa29e3b569dba4f7284fbb0336.md) **>** [**si5332\_cfg.c**](si5332__cfg_8c.md)

[Go to the source code of this file](si5332__cfg_8c_source.md)

_Static configuration tables._ [More...](#detailed-description)

* `#include "si5332_cfg.h"`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  const [**si5332\_reg\_t**](structsi5332__reg__t.md) | [**si5332\_base\_regs**](#variable-si5332_base_regs)  <br>_Base configuration: all registers identical in both CBPro exports._  |
|  const u32 | [**si5332\_base\_regs\_len**](#variable-si5332_base_regs_len)   = `((u32)(sizeof([**si5332\_base\_regs**](si5332__cfg_8c.md#variable-si5332_base_regs)) / sizeof(([**si5332\_base\_regs**](si5332__cfg_8c.md#variable-si5332_base_regs))[0])))`<br> |
|  const [**si5332\_reg\_t**](structsi5332__reg__t.md) | [**si5332\_src\_clkin2\_100m\_regs**](#variable-si5332_src_clkin2_100m_regs)   = `/* multi line expression */`<br>_Delta: 100 MHz differential clock on CLKIN\_2. Fpfd = 50 MHz, P = 2, M = 50._  |
|  const u32 | [**si5332\_src\_clkin2\_100m\_regs\_len**](#variable-si5332_src_clkin2_100m_regs_len)   = `((u32)(sizeof([**si5332\_src\_clkin2\_100m\_regs**](si5332__cfg_8c.md#variable-si5332_src_clkin2_100m_regs)) / sizeof(([**si5332\_src\_clkin2\_100m\_regs**](si5332__cfg_8c.md#variable-si5332_src_clkin2_100m_regs))[0])))`<br> |
|  const [**si5332\_reg\_t**](structsi5332__reg__t.md) | [**si5332\_src\_xtal\_27m\_regs**](#variable-si5332_src_xtal_27m_regs)   = `/* multi line expression */`<br>_Delta: 27 MHz crystal on XA/XB. Fpfd = 27 MHz, P = 1, M = 92 + 16/27._  |
|  const u32 | [**si5332\_src\_xtal\_27m\_regs\_len**](#variable-si5332_src_xtal_27m_regs_len)   = `((u32)(sizeof([**si5332\_src\_xtal\_27m\_regs**](si5332__cfg_8c.md#variable-si5332_src_xtal_27m_regs)) / sizeof(([**si5332\_src\_xtal\_27m\_regs**](si5332__cfg_8c.md#variable-si5332_src_xtal_27m_regs))[0])))`<br> |











































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ARRAY\_LEN**](si5332__cfg_8c.md#define-array_len) (a) `((u32)(sizeof(a) / sizeof((a)[0])))`<br>_Computes the number of elements of a static array._  |

## Detailed Description




**Note:**

The CBPro export omits registers of unused blocks (e.g. 0x28 OMUX3, 0x2A OMUX5, 0x36..0x3B ID0A, 0x99 OUT4\_DIV, 0xAD OUT7\_DIV). These registers are written completely by the driver when a block is activated (see [**si5332\_commit()**](si5332_8c.md#function-si5332_commit)). 





    
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
## Macro Definition Documentation





### define ARRAY\_LEN 

_Computes the number of elements of a static array._ 
```C++
#define ARRAY_LEN (
    a
) `((u32)(sizeof(a) / sizeof((a)[0])))`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/si5332/si5332_cfg.c`

