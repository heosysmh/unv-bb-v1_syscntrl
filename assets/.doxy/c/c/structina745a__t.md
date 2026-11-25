

# Struct ina745a\_t



[**ClassList**](annotated.md) **>** [**ina745a\_t**](structina745a__t.md)



_Opaque device handle (definition), see_ [_**ina745a\_t**_](structina745a__t.md) _in_[_**ina745a.h**_](ina745a_8h.md) _._






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  void \* | [**hal\_ctx**](#variable-hal_ctx)  <br> |
|  u8 | [**i2cAddr**](#variable-i2caddr)  <br> |
|  [**ina745a\_i2c\_read\_f**](ina745a_8h.md#typedef-ina745a_i2c_read_f) | [**i2c\_read**](#variable-i2c_read)  <br> |
|  [**ina745a\_i2c\_write\_f**](ina745a_8h.md#typedef-ina745a_i2c_write_f) | [**i2c\_write**](#variable-i2c_write)  <br> |
|  u8 | [**isConfigured**](#variable-isconfigured)  <br> |
|  u8 | [**isReady**](#variable-isready)  <br> |












































## Public Attributes Documentation




### variable hal\_ctx 

```C++
void* ina745a_t::hal_ctx;
```



Opaque context pointer passed through to the HAL callbacks. 


        

<hr>



### variable i2cAddr 

```C++
u8 ina745a_t::i2cAddr;
```



7-bit I2C address of the device. 


        

<hr>



### variable i2c\_read 

```C++
ina745a_i2c_read_f ina745a_t::i2c_read;
```



HAL register read callback. 


        

<hr>



### variable i2c\_write 

```C++
ina745a_i2c_write_f ina745a_t::i2c_write;
```



HAL register write callback. 


        

<hr>



### variable isConfigured 

```C++
u8 ina745a_t::isConfigured;
```



Non-zero once [**ina745a\_init()**](ina745a_8c.md#function-ina745a_init) has completed successfully (MANUFACTURER\_ID verified). 


        

<hr>



### variable isReady 

```C++
u8 ina745a_t::isReady;
```



Non-zero once [**ina745a\_initialize()**](ina745a_8c.md#function-ina745a_initialize) has set up the HAL callbacks. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/ina745a/ina745a.c`

