

# Struct i2cHandle\_t



[**ClassList**](annotated.md) **>** [**i2cHandle\_t**](structi2cHandle__t.md)



_I2C controller instance state (opaque handle, see_ [_**i2cWrapper.h**_](i2cWrapper_8h.md) _)._






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  volatile u8 | [**arbLoss**](#variable-arbloss)  <br> |
|  u32 | [**baseAddress**](#variable-baseaddress)  <br> |
|  I2c\_Config | [**cfg**](#variable-cfg)  <br> |
|  u8 | [**deviceId**](#variable-deviceid)  <br> |
|  u8 | [**intrHart**](#variable-intrhart)  <br> |
|  u8 | [**intrId**](#variable-intrid)  <br> |
|  u8 | [**intrPrio**](#variable-intrprio)  <br> |
|  volatile u8 | [**nackRecvd**](#variable-nackrecvd)  <br> |
|  volatile u8 | [**timeout**](#variable-timeout)  <br> |












































## Public Attributes Documentation




### variable arbLoss 

```C++
volatile u8 i2cHandle_t::arbLoss;
```



Flag for arbitration loss. 


        

<hr>



### variable baseAddress 

```C++
u32 i2cHandle_t::baseAddress;
```



Base address of the I2C controller. 


        

<hr>



### variable cfg 

```C++
I2c_Config i2cHandle_t::cfg;
```



Controller timing configuration. 


        

<hr>



### variable deviceId 

```C++
u8 i2cHandle_t::deviceId;
```



Instance index into [**gI2cInstances**](i2cWrapper_8c.md#variable-gi2cinstances). 


        

<hr>



### variable intrHart 

```C++
u8 i2cHandle_t::intrHart;
```



Hart (CPU) the interrupt is routed to. 


        

<hr>



### variable intrId 

```C++
u8 i2cHandle_t::intrId;
```



PLIC interrupt source ID. 


        

<hr>



### variable intrPrio 

```C++
u8 i2cHandle_t::intrPrio;
```



PLIC interrupt priority. 


        

<hr>



### variable nackRecvd 

```C++
volatile u8 i2cHandle_t::nackRecvd;
```



Flag for NACK reception. 


        

<hr>



### variable timeout 

```C++
volatile u8 i2cHandle_t::timeout;
```



Flag set by ISR when timeout detected. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/drivers/i2c/i2cWrapper.c`

