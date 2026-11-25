

# Struct si5332\_status\_flags\_t



[**ClassList**](annotated.md) **>** [**si5332\_status\_flags\_t**](structsi5332__status__flags__t.md)



[More...](#detailed-description)

* `#include <sw/devices/si5332/si5332.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u8 | [**is\_active**](#variable-is_active)  <br> |
|  u8 | [**no\_input**](#variable-no_input)  <br> |
|  u8 | [**usys\_stat**](#variable-usys_stat)  <br> |
|  u8 | [**vdd\_xtal\_ok**](#variable-vdd_xtal_ok)  <br> |
|  u8 | [**vddo\_ok\_mask**](#variable-vddo_ok_mask)  <br> |












































## Detailed Description


Decoded device status flags (see [**si5332\_get\_status()**](si5332_8c.md#function-si5332_get_status)). 


    
## Public Attributes Documentation




### variable is\_active 

```C++
u8 si5332_status_flags_t::is_active;
```



usys\_stat == 0x02. 


        

<hr>



### variable no\_input 

```C++
u8 si5332_status_flags_t::no_input;
```



usys\_stat == 0x89. 


        

<hr>



### variable usys\_stat 

```C++
u8 si5332_status_flags_t::usys_stat;
```



Raw value of register 0x07 (0x01 READY, 0x02 ACTIVE, 0x89 no input clock). 


        

<hr>



### variable vdd\_xtal\_ok 

```C++
u8 si5332_status_flags_t::vdd_xtal_ok;
```



0x05[7]: crystal supply OK. 


        

<hr>



### variable vddo\_ok\_mask 

```C++
u8 si5332_status_flags_t::vddo_ok_mask;
```



0x05[5:0], bit n = VDDOn &gt; 1.2 V. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/si5332/si5332.h`

