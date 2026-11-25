

# Struct pi4msd5v945x\_t



[**ClassList**](annotated.md) **>** [**pi4msd5v945x\_t**](structpi4msd5v945x__t.md)



_Opaque device handle (definition), see_ [_**pi4msd5v945x\_t**_](structpi4msd5v945x__t.md) _in_[_**pi4msd5v945x.h**_](pi4msd5v945x_8h.md) _._






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  void \* | [**hal\_ctx**](#variable-hal_ctx)  <br> |
|  u8 | [**i2cAddr**](#variable-i2caddr)  <br> |
|  [**pi4msd5v945x\_i2c\_read\_f**](pi4msd5v945x_8h.md#typedef-pi4msd5v945x_i2c_read_f) | [**i2c\_read**](#variable-i2c_read)  <br> |
|  [**pi4msd5v945x\_i2c\_write\_f**](pi4msd5v945x_8h.md#typedef-pi4msd5v945x_i2c_write_f) | [**i2c\_write**](#variable-i2c_write)  <br> |
|  int | [**isReady**](#variable-isready)  <br> |












































## Public Attributes Documentation




### variable hal\_ctx 

```C++
void* pi4msd5v945x_t::hal_ctx;
```



Opaque context pointer passed through to the HAL callbacks. 


        

<hr>



### variable i2cAddr 

```C++
u8 pi4msd5v945x_t::i2cAddr;
```



7-bit I2C address of the device. 


        

<hr>



### variable i2c\_read 

```C++
pi4msd5v945x_i2c_read_f pi4msd5v945x_t::i2c_read;
```



HAL read callback. 


        

<hr>



### variable i2c\_write 

```C++
pi4msd5v945x_i2c_write_f pi4msd5v945x_t::i2c_write;
```



HAL write callback. 


        

<hr>



### variable isReady 

```C++
int pi4msd5v945x_t::isReady;
```



Non-zero once [**pi4msd5v945x\_initialize()**](pi4msd5v945x_8c.md#function-pi4msd5v945x_initialize) has set up the HAL callbacks. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/pi4msd5v945x/pi4msd5v945x.c`

