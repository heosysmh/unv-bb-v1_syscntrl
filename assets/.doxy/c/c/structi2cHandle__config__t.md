

# Struct i2cHandle\_config\_t



[**ClassList**](annotated.md) **>** [**i2cHandle\_config\_t**](structi2cHandle__config__t.md)



[More...](#detailed-description)

* `#include <sw/drivers/i2c/i2cWrapper.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u32 | [**baseAddress**](#variable-baseaddress)  <br> |
|  I2c\_Config | [**cfg**](#variable-cfg)  <br> |
|  u8 | [**deviceId**](#variable-deviceid)  <br> |
|  u8 | [**intrHart**](#variable-intrhart)  <br> |
|  u8 | [**intrId**](#variable-intrid)  <br> |
|  u8 | [**intrPrio**](#variable-intrprio)  <br> |












































## Detailed Description


Configuration of one I2C controller instance, passed to [**i2c\_initialize()**](i2cWrapper_8c.md#function-i2c_initialize). 


    
## Public Attributes Documentation




### variable baseAddress 

```C++
u32 i2cHandle_config_t::baseAddress;
```



Base address of the I2C controller. 


        

<hr>



### variable cfg 

```C++
I2c_Config i2cHandle_config_t::cfg;
```



Controller timing configuration (BSP driver). 


        

<hr>



### variable deviceId 

```C++
u8 i2cHandle_config_t::deviceId;
```



Instance index into the static pool (0 .. I2C\_NUM\_OF\_INSTANCES-1). 


        

<hr>



### variable intrHart 

```C++
u8 i2cHandle_config_t::intrHart;
```



Hart (CPU) the interrupt is routed to. 


        

<hr>



### variable intrId 

```C++
u8 i2cHandle_config_t::intrId;
```



PLIC interrupt source ID of the controller. 


        

<hr>



### variable intrPrio 

```C++
u8 i2cHandle_config_t::intrPrio;
```



PLIC interrupt priority. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/drivers/i2c/i2cWrapper.h`

