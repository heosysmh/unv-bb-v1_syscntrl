

# File pi4msd5v945x.c



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**pi4msd5v945x**](dir_519e93f1bdf9c64a9ee465d9f69baf9a.md) **>** [**pi4msd5v945x.c**](pi4msd5v945x_8c.md)

[Go to the source code of this file](pi4msd5v945x_8c_source.md)

_Driver implementation for the Diodes PI4MSD5V945x I2C bus switch._ 

* `#include "pi4msd5v945x.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**pi4msd5v945x\_t**](structpi4msd5v945x__t.md) <br>_Opaque device handle (definition), see_ [_**pi4msd5v945x\_t**_](structpi4msd5v945x__t.md) _in_[_**pi4msd5v945x.h**_](pi4msd5v945x_8h.md) _._ |








## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**pi4msd5v945x\_t**](structpi4msd5v945x__t.md) | [**gDevice**](#variable-gdevice)  <br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**pi4msd5v945x\_t**](structpi4msd5v945x__t.md) \* | [**pi4msd5v945x\_initialize**](#function-pi4msd5v945x_initialize) (u8 i2c\_addr, [**pi4msd5v945x\_i2c\_write\_f**](pi4msd5v945x_8h.md#typedef-pi4msd5v945x_i2c_write_f) w, [**pi4msd5v945x\_i2c\_read\_f**](pi4msd5v945x_8h.md#typedef-pi4msd5v945x_i2c_read_f) r, void \* ctx) <br>_Creates the device handle (no I2C access)._  |
|  [**pi4msd5v945x\_err\_t**](pi4msd5v945x_8h.md#enum-pi4msd5v945x_err_t) | [**pi4msd5v945x\_setChannels**](#function-pi4msd5v945x_setchannels) ([**pi4msd5v945x\_t**](structpi4msd5v945x__t.md) \* pDev, u8 channels) <br>_Selects the active downstream channels._  |




























## Public Static Attributes Documentation




### variable gDevice 

```C++
pi4msd5v945x_t gDevice;
```



Single static device instance backing [**pi4msd5v945x\_initialize()**](pi4msd5v945x_8c.md#function-pi4msd5v945x_initialize). 


        

<hr>
## Public Functions Documentation




### function pi4msd5v945x\_initialize 

_Creates the device handle (no I2C access)._ 
```C++
pi4msd5v945x_t * pi4msd5v945x_initialize (
    u8 i2c_addr,
    pi4msd5v945x_i2c_write_f w,
    pi4msd5v945x_i2c_read_f r,
    void * ctx
) 
```



The driver manages a single static instance.




**Parameters:**


* `i2c_addr` 7-bit I2C address of the bus switch. 
* `w` HAL write callback. 
* `r` HAL read callback. 
* `ctx` Opaque context pointer passed through to `w` and `r`. 



**Returns:**

Pointer to the device handle. 





        

<hr>



### function pi4msd5v945x\_setChannels 

_Selects the active downstream channels._ 
```C++
pi4msd5v945x_err_t pi4msd5v945x_setChannels (
    pi4msd5v945x_t * pDev,
    u8 channels
) 
```



Writes the channel mask as the single control byte; previously selected channels are replaced, not added.




**Parameters:**


* `pDev` Device handle from [**pi4msd5v945x\_initialize()**](pi4msd5v945x_8h.md#function-pi4msd5v945x_initialize). 
* `channels` Bitmask of PI4MSD5V945X\_CHx bits (0 = all channels off). 



**Returns:**

PI4MSD5V945X\_OK on success, PI4MSD5V945X\_E\_PARAM if `pDev` is NULL. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/pi4msd5v945x/pi4msd5v945x.c`

