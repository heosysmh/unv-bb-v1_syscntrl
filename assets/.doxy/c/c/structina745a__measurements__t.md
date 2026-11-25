

# Struct ina745a\_measurements\_t



[**ClassList**](annotated.md) **>** [**ina745a\_measurements\_t**](structina745a__measurements__t.md)



[More...](#detailed-description)

* `#include <sw/devices/ina745a/ina745a.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  float | [**bus\_voltage\_V**](#variable-bus_voltage_v)  <br> |
|  double | [**charge\_C**](#variable-charge_c)  <br> |
|  s64 | [**charge\_raw**](#variable-charge_raw)  <br> |
|  float | [**current\_A**](#variable-current_a)  <br> |
|  s16 | [**current\_raw**](#variable-current_raw)  <br> |
|  float | [**die\_temp\_C**](#variable-die_temp_c)  <br> |
|  s16 | [**dietemp\_raw**](#variable-dietemp_raw)  <br> |
|  double | [**energy\_J**](#variable-energy_j)  <br> |
|  u64 | [**energy\_raw**](#variable-energy_raw)  <br> |
|  float | [**power\_W**](#variable-power_w)  <br> |
|  u32 | [**power\_raw**](#variable-power_raw)  <br> |
|  u16 | [**vbus\_raw**](#variable-vbus_raw)  <br> |












































## Detailed Description


Complete measurement snapshot (converted values and raw registers). 


    
## Public Attributes Documentation




### variable bus\_voltage\_V 

```C++
float ina745a_measurements_t::bus_voltage_V;
```



Bus voltage [V]. 


        

<hr>



### variable charge\_C 

```C++
double ina745a_measurements_t::charge_C;
```



Accumulated charge [C]. 


        

<hr>



### variable charge\_raw 

```C++
s64 ina745a_measurements_t::charge_raw;
```



CHARGE, 40 bit, two's complement, sign-extended to 64 bit 


        

<hr>



### variable current\_A 

```C++
float ina745a_measurements_t::current_A;
```



Current [A]. 


        

<hr>



### variable current\_raw 

```C++
s16 ina745a_measurements_t::current_raw;
```



CURRENT, 16 bit, two's complement 


        

<hr>



### variable die\_temp\_C 

```C++
float ina745a_measurements_t::die_temp_C;
```



Die temperature [degC]. 


        

<hr>



### variable dietemp\_raw 

```C++
s16 ina745a_measurements_t::dietemp_raw;
```



DIETEMP, 12-bit field, sign-extended to 16 bit 


        

<hr>



### variable energy\_J 

```C++
double ina745a_measurements_t::energy_J;
```



Accumulated energy [J]. 


        

<hr>



### variable energy\_raw 

```C++
u64 ina745a_measurements_t::energy_raw;
```



ENERGY, 40 bit, unsigned (in u64) 


        

<hr>



### variable power\_W 

```C++
float ina745a_measurements_t::power_W;
```



Power [W]. 


        

<hr>



### variable power\_raw 

```C++
u32 ina745a_measurements_t::power_raw;
```



POWER, 24 bit, unsigned (in u32) 


        

<hr>



### variable vbus\_raw 

```C++
u16 ina745a_measurements_t::vbus_raw;
```



VBUS, 16 bit, unsigned 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/ina745a/ina745a.h`

