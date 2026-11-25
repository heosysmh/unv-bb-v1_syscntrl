

# Struct ina745a\_diag\_flags\_t



[**ClassList**](annotated.md) **>** [**ina745a\_diag\_flags\_t**](structina745a__diag__flags__t.md)



[More...](#detailed-description)

* `#include <sw/devices/ina745a/ina745a.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u8 | [**bus\_over\_limit**](#variable-bus_over_limit)  <br> |
|  u8 | [**bus\_under\_limit**](#variable-bus_under_limit)  <br> |
|  u8 | [**charge\_overflow**](#variable-charge_overflow)  <br> |
|  u8 | [**conversion\_ready**](#variable-conversion_ready)  <br> |
|  u8 | [**current\_over\_limit**](#variable-current_over_limit)  <br> |
|  u8 | [**current\_under\_limit**](#variable-current_under_limit)  <br> |
|  u8 | [**energy\_overflow**](#variable-energy_overflow)  <br> |
|  u8 | [**math\_overflow**](#variable-math_overflow)  <br> |
|  u8 | [**memory\_ok**](#variable-memory_ok)  <br> |
|  u8 | [**power\_over\_limit**](#variable-power_over_limit)  <br> |
|  u8 | [**temp\_over\_limit**](#variable-temp_over_limit)  <br> |












































## Detailed Description


Read-only DIAG\_ALRT status/diagnostic bits (11-0). 


    
## Public Attributes Documentation




### variable bus\_over\_limit 

```C++
u8 ina745a_diag_flags_t::bus_over_limit;
```



BUSOL: bus voltage over limit. 


        

<hr>



### variable bus\_under\_limit 

```C++
u8 ina745a_diag_flags_t::bus_under_limit;
```



BUSUL: bus voltage under limit. 


        

<hr>



### variable charge\_overflow 

```C++
u8 ina745a_diag_flags_t::charge_overflow;
```



CHARGEOF: charge accumulator overflow. 


        

<hr>



### variable conversion\_ready 

```C++
u8 ina745a_diag_flags_t::conversion_ready;
```



CNVRF: conversion ready. 


        

<hr>



### variable current\_over\_limit 

```C++
u8 ina745a_diag_flags_t::current_over_limit;
```



CURRENTOL: current over limit. 


        

<hr>



### variable current\_under\_limit 

```C++
u8 ina745a_diag_flags_t::current_under_limit;
```



CURRENTUL: current under limit. 


        

<hr>



### variable energy\_overflow 

```C++
u8 ina745a_diag_flags_t::energy_overflow;
```



ENERGYOF: energy accumulator overflow. 


        

<hr>



### variable math\_overflow 

```C++
u8 ina745a_diag_flags_t::math_overflow;
```



MATHOF: internal math overflow. 


        

<hr>



### variable memory\_ok 

```C++
u8 ina745a_diag_flags_t::memory_ok;
```



MEMSTAT, 1 = normal operation. 


        

<hr>



### variable power\_over\_limit 

```C++
u8 ina745a_diag_flags_t::power_over_limit;
```



POL: power over limit. 


        

<hr>



### variable temp\_over\_limit 

```C++
u8 ina745a_diag_flags_t::temp_over_limit;
```



TMPOL: temperature over limit. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/ina745a/ina745a.h`

