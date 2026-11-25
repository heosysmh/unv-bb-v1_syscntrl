

# Struct tpsm8286\_voltage\_map\_t



[**ClassList**](annotated.md) **>** [**tpsm8286\_voltage\_map\_t**](structtpsm8286__voltage__map__t.md)



_Linear voltage-to-register mapping descriptor._ 






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  float | [**max\_volt**](#variable-max_volt)  <br> |
|  float | [**min\_volt**](#variable-min_volt)  <br> |
|  u8 | [**reg\_max**](#variable-reg_max)  <br> |
|  u8 | [**reg\_min**](#variable-reg_min)  <br> |
|  float | [**step\_volt**](#variable-step_volt)  <br> |












































## Public Attributes Documentation




### variable max\_volt 

```C++
float tpsm8286_voltage_map_t::max_volt;
```



Largest supported voltage [V]. 


        

<hr>



### variable min\_volt 

```C++
float tpsm8286_voltage_map_t::min_volt;
```



Smallest supported voltage [V]. 


        

<hr>



### variable reg\_max 

```C++
u8 tpsm8286_voltage_map_t::reg_max;
```



Register value at [**max\_volt**](structtpsm8286__voltage__map__t.md#variable-max_volt). 


        

<hr>



### variable reg\_min 

```C++
u8 tpsm8286_voltage_map_t::reg_min;
```



Register value at [**min\_volt**](structtpsm8286__voltage__map__t.md#variable-min_volt). 


        

<hr>



### variable step\_volt 

```C++
float tpsm8286_voltage_map_t::step_volt;
```



Volts per LSB (e.g. 0.01). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/tpsm8286x/tpsm8286x.c`

