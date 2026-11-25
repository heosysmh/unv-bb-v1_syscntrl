

# Struct ch\_state\_t



[**ClassList**](annotated.md) **>** [**ch\_state\_t**](structch__state__t.md)



_Shadow state of one API-configurable output channel._ 






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u8 | [**configured**](#variable-configured)  <br> |
|  u8 | [**enabled**](#variable-enabled)  <br> |
|  u8 | [**fmt**](#variable-fmt)  <br> |
|  u32 | [**freq\_hz**](#variable-freq_hz)  <br> |
|  u8 | [**rdiv**](#variable-rdiv)  <br> |
|  u8 | [**src**](#variable-src)  <br> |












































## Public Attributes Documentation




### variable configured 

```C++
u8 ch_state_t::configured;
```



Non-zero once a divider has been set (block switched on). 


        

<hr>



### variable enabled 

```C++
u8 ch_state_t::enabled;
```



Desired output-enable (OE) state. 


        

<hr>



### variable fmt 

```C++
u8 ch_state_t::fmt;
```



Signal format, value of [**si5332\_out\_format\_t**](si5332_8h.md#enum-si5332_out_format_t) (OUTx\_MODE). 


        

<hr>



### variable freq\_hz 

```C++
u32 ch_state_t::freq_hz;
```



Currently configured output frequency in Hz. 


        

<hr>



### variable rdiv 

```C++
u8 ch_state_t::rdiv;
```



R divider, 1..63. 


        

<hr>



### variable src 

```C++
u8 ch_state_t::src;
```



Divider source, value of [**si5332\_div\_src\_t**](si5332_8h.md#enum-si5332_div_src_t). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/si5332/si5332.c`

