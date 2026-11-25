

# Struct ina745a\_adc\_config\_t



[**ClassList**](annotated.md) **>** [**ina745a\_adc\_config\_t**](structina745a__adc__config__t.md)



[More...](#detailed-description)

* `#include <sw/devices/ina745a/ina745a.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**ina745a\_avg\_t**](ina745a_8h.md#enum-ina745a_avg_t) | [**avg**](#variable-avg)  <br> |
|  [**ina745a\_mode\_t**](ina745a_8h.md#enum-ina745a_mode_t) | [**mode**](#variable-mode)  <br> |
|  [**ina745a\_convtime\_t**](ina745a_8h.md#enum-ina745a_convtime_t) | [**tct**](#variable-tct)  <br> |
|  [**ina745a\_convtime\_t**](ina745a_8h.md#enum-ina745a_convtime_t) | [**vbusct**](#variable-vbusct)  <br> |
|  [**ina745a\_convtime\_t**](ina745a_8h.md#enum-ina745a_convtime_t) | [**vsenct**](#variable-vsenct)  <br> |












































## Detailed Description


Complete ADC\_CONFIG register content. 


    
## Public Attributes Documentation




### variable avg 

```C++
ina745a_avg_t ina745a_adc_config_t::avg;
```



Averaging setting (AVG field). 


        

<hr>



### variable mode 

```C++
ina745a_mode_t ina745a_adc_config_t::mode;
```



Conversion mode (MODE field). 


        

<hr>



### variable tct 

```C++
ina745a_convtime_t ina745a_adc_config_t::tct;
```



Temperature conversion time (TCT field). 


        

<hr>



### variable vbusct 

```C++
ina745a_convtime_t ina745a_adc_config_t::vbusct;
```



Bus voltage conversion time (VBUSCT field). 


        

<hr>



### variable vsenct 

```C++
ina745a_convtime_t ina745a_adc_config_t::vsenct;
```



Shunt current conversion time (VSENCT field). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/ina745a/ina745a.h`

