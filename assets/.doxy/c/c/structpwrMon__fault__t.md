

# Struct pwrMon\_fault\_t



[**ClassList**](annotated.md) **>** [**pwrMon\_fault\_t**](structpwrMon__fault__t.md)



[More...](#detailed-description)

* `#include <sw/app/src/power.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u8 | [**memory\_fail**](#variable-memory_fail)  <br> |
|  u8 | [**over\_current**](#variable-over_current)  <br> |
|  u8 | [**over\_power**](#variable-over_power)  <br> |
|  u8 | [**over\_temp**](#variable-over_temp)  <br> |
|  u8 | [**over\_voltage**](#variable-over_voltage)  <br> |












































## Detailed Description


Sticky fault flags of one rail (accumulated since the last clear). 


    
## Public Attributes Documentation




### variable memory\_fail 

```C++
u8 pwrMon_fault_t::memory_fail;
```



MEMSTAT reported a checksum error. 


        

<hr>



### variable over\_current 

```C++
u8 pwrMon_fault_t::over_current;
```



CURRENTOL: over-current. 


        

<hr>



### variable over\_power 

```C++
u8 pwrMon_fault_t::over_power;
```



POL: over-power. 


        

<hr>



### variable over\_temp 

```C++
u8 pwrMon_fault_t::over_temp;
```



TMPOL: over-temperature. 


        

<hr>



### variable over\_voltage 

```C++
u8 pwrMon_fault_t::over_voltage;
```



BUSOL: bus over-voltage. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/power.h`

