

# Struct clkGenOutCfg\_t



[**ClassList**](annotated.md) **>** [**clkGenOutCfg\_t**](structclkGenOutCfg__t.md)



[More...](#detailed-description)






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**si5332\_out\_ch\_t**](si5332_8h.md#enum-si5332_out_ch_t) | [**ch**](#variable-ch)  <br> |
|  u8 | [**enable**](#variable-enable)  <br> |
|  [**si5332\_out\_format\_t**](si5332_8h.md#enum-si5332_out_format_t) | [**format**](#variable-format)  <br> |
|  u32 | [**freq\_hz**](#variable-freq_hz)  <br> |












































## Detailed Description


Per-output configuration, taken from [**boardConfig.h**](boardConfig_8h.md). 


    
## Public Attributes Documentation




### variable ch 

```C++
si5332_out_ch_t clkGenOutCfg_t::ch;
```



Output channel. 


        

<hr>



### variable enable 

```C++
u8 clkGenOutCfg_t::enable;
```



Enable state. 


        

<hr>



### variable format 

```C++
si5332_out_format_t clkGenOutCfg_t::format;
```



Signal format. 


        

<hr>



### variable freq\_hz 

```C++
u32 clkGenOutCfg_t::freq_hz;
```



Output frequency [Hz]. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/clkGen.c`

