

# Struct ina745a\_alert\_cfg\_t



[**ClassList**](annotated.md) **>** [**ina745a\_alert\_cfg\_t**](structina745a__alert__cfg__t.md)



[More...](#detailed-description)

* `#include <sw/devices/ina745a/ina745a.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u8 | [**alatch**](#variable-alatch)  <br> |
|  u8 | [**conv\_ready\_en**](#variable-conv_ready_en)  <br> |
|  [**ina745a\_alert\_pol\_t**](ina745a_8h.md#enum-ina745a_alert_pol_t) | [**polarity**](#variable-polarity)  <br> |
|  u8 | [**slow\_alert**](#variable-slow_alert)  <br> |












































## Detailed Description


Writable DIAG\_ALRT configuration bits (15-12). 


    
## Public Attributes Documentation




### variable alatch 

```C++
u8 ina745a_alert_cfg_t::alatch;
```



1 = latched alert, 0 = transparent. 


        

<hr>



### variable conv\_ready\_en 

```C++
u8 ina745a_alert_cfg_t::conv_ready_en;
```



CNVR: assert ALERT on conversion-ready. 


        

<hr>



### variable polarity 

```C++
ina745a_alert_pol_t ina745a_alert_cfg_t::polarity;
```



ALERT pin polarity (APOL). 


        

<hr>



### variable slow\_alert 

```C++
u8 ina745a_alert_cfg_t::slow_alert;
```



1 = compare thresholds against the averaged value. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/ina745a/ina745a.h`

