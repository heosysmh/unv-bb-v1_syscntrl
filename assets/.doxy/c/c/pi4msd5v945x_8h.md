

# File pi4msd5v945x.h



[**FileList**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**pi4msd5v945x**](dir_519e93f1bdf9c64a9ee465d9f69baf9a.md) **>** [**pi4msd5v945x.h**](pi4msd5v945x_8h.md)

[Go to the source code of this file](pi4msd5v945x_8h_source.md)

_Driver for the Diodes PI4MSD5V945x 4-channel I2C bus switch._ [More...](#detailed-description)

* `#include <type.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**pi4msd5v945x\_err\_t**](#enum-pi4msd5v945x_err_t)  <br> |
| typedef [**pi4msd5v945x\_err\_t**](pi4msd5v945x_8h.md#enum-pi4msd5v945x_err_t)(\* | [**pi4msd5v945x\_i2c\_read\_f**](#typedef-pi4msd5v945x_i2c_read_f)  <br> |
| typedef [**pi4msd5v945x\_err\_t**](pi4msd5v945x_8h.md#enum-pi4msd5v945x_err_t)(\* | [**pi4msd5v945x\_i2c\_write\_f**](#typedef-pi4msd5v945x_i2c_write_f)  <br> |
| typedef struct pi4msd5v945x\_t | [**pi4msd5v945x\_t**](#typedef-pi4msd5v945x_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**pi4msd5v945x\_t**](structpi4msd5v945x__t.md) \* | [**pi4msd5v945x\_initialize**](#function-pi4msd5v945x_initialize) (u8 i2c\_addr, [**pi4msd5v945x\_i2c\_write\_f**](pi4msd5v945x_8h.md#typedef-pi4msd5v945x_i2c_write_f) w, [**pi4msd5v945x\_i2c\_read\_f**](pi4msd5v945x_8h.md#typedef-pi4msd5v945x_i2c_read_f) r, void \* ctx) <br>_Creates the device handle (no I2C access)._  |
|  [**pi4msd5v945x\_err\_t**](pi4msd5v945x_8h.md#enum-pi4msd5v945x_err_t) | [**pi4msd5v945x\_setChannels**](#function-pi4msd5v945x_setchannels) ([**pi4msd5v945x\_t**](structpi4msd5v945x__t.md) \* pDev, u8 channels) <br>_Selects the active downstream channels._  |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**PI4MSD5V945X\_CH0**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch0)  `0x1u`<br> |
| define  | [**PI4MSD5V945X\_CH1**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch1)  `0x2u`<br> |
| define  | [**PI4MSD5V945X\_CH2**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch2)  `0x4u`<br> |
| define  | [**PI4MSD5V945X\_CH3**](pi4msd5v945x_8h.md#define-pi4msd5v945x_ch3)  `0x8u`<br> |

## Detailed Description


 


    
## Public Types Documentation




### enum pi4msd5v945x\_err\_t 

```C++
enum pi4msd5v945x_err_t {
    PI4MSD5V945X_OK = 0,
    PI4MSD5V945X_E_PARAM = -1,
    PI4MSD5V945X_E_I2C = -2
};
```



Status codes of the bus switch driver. 


        

<hr>



### typedef pi4msd5v945x\_i2c\_read\_f 

```C++
typedef pi4msd5v945x_err_t(* pi4msd5v945x_i2c_read_f) (void *ctx, u8 i2c_addr, u8 *data, u32 length);
```




<hr>



### typedef pi4msd5v945x\_i2c\_write\_f 

```C++
typedef pi4msd5v945x_err_t(* pi4msd5v945x_i2c_write_f) (void *ctx, u8 i2c_addr, const u8 *data, u32 length);
```




<hr>



### typedef pi4msd5v945x\_t 

```C++
typedef struct pi4msd5v945x_t pi4msd5v945x_t;
```



Opaque device handle (defined in [**pi4msd5v945x.c**](pi4msd5v945x_8c.md)). 


        

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
## Macro Definition Documentation





### define PI4MSD5V945X\_CH0 

```C++
#define PI4MSD5V945X_CH0 `0x1u`
```



Downstream channel 0. 


        

<hr>



### define PI4MSD5V945X\_CH1 

```C++
#define PI4MSD5V945X_CH1 `0x2u`
```



Downstream channel 1. 


        

<hr>



### define PI4MSD5V945X\_CH2 

```C++
#define PI4MSD5V945X_CH2 `0x4u`
```



Downstream channel 2. 


        

<hr>



### define PI4MSD5V945X\_CH3 

```C++
#define PI4MSD5V945X_CH3 `0x8u`
```



Downstream channel 3. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/pi4msd5v945x/pi4msd5v945x.h`

