

# Struct tpsm8286\_t



[**ClassList**](annotated.md) **>** [**tpsm8286\_t**](structtpsm8286__t.md)



_Opaque device handle (definition), see_ [_**tpsm8286\_t**_](structtpsm8286__t.md) _in_[_**tpsm8286x.h**_](tpsm8286x_8h.md) _._






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u8 | [**control**](#variable-control)  <br> |
|  void \* | [**hal\_ctx**](#variable-hal_ctx)  <br> |
|  u8 | [**i2cAddr**](#variable-i2caddr)  <br> |
|  [**tpsm8286\_i2c\_read\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_read_f) | [**i2c\_read**](#variable-i2c_read)  <br> |
|  [**tpsm8286\_i2c\_write\_f**](tpsm8286x_8h.md#typedef-tpsm8286_i2c_write_f) | [**i2c\_write**](#variable-i2c_write)  <br> |
|  u8 | [**isEnabled**](#variable-isenabled)  <br> |
|  u8 | [**isReady**](#variable-isready)  <br> |
|  u8 | [**status**](#variable-status)  <br> |
|  float | [**voltage**](#variable-voltage)  <br> |
|  u8 | [**voltage\_raw**](#variable-voltage_raw)  <br> |












































## Public Attributes Documentation




### variable control 

```C++
u8 tpsm8286_t::control;
```



Shadow of the CNTRL register. 


        

<hr>



### variable hal\_ctx 

```C++
void* tpsm8286_t::hal_ctx;
```



Opaque context pointer passed through to the HAL callbacks. 


        

<hr>



### variable i2cAddr 

```C++
u8 tpsm8286_t::i2cAddr;
```



7-bit I2C address of the device. 


        

<hr>



### variable i2c\_read 

```C++
tpsm8286_i2c_read_f tpsm8286_t::i2c_read;
```



HAL register read callback. 


        

<hr>



### variable i2c\_write 

```C++
tpsm8286_i2c_write_f tpsm8286_t::i2c_write;
```



HAL register write callback. 


        

<hr>



### variable isEnabled 

```C++
u8 tpsm8286_t::isEnabled;
```



Cached output enable state. 


        

<hr>



### variable isReady 

```C++
u8 tpsm8286_t::isReady;
```



Non-zero once [**tpsm8286\_initialize()**](tpsm8286x_8c.md#function-tpsm8286_initialize) has set up the HAL callbacks. 


        

<hr>



### variable status 

```C++
u8 tpsm8286_t::status;
```



Shadow of the STATUS register. 


        

<hr>



### variable voltage 

```C++
float tpsm8286_t::voltage;
```



Last programmed output voltage [V]. 


        

<hr>



### variable voltage\_raw 

```C++
u8 tpsm8286_t::voltage_raw;
```



Last programmed VOUT1 register value. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/tpsm8286x/tpsm8286x.c`

