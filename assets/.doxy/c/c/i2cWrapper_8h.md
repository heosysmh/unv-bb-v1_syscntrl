

# File i2cWrapper.h



[**FileList**](files.md) **>** [**drivers**](dir_258f8fd52823f65304266634174f6293.md) **>** [**i2c**](dir_6bfd9f8c6bb0ff18ae4323cf33a90ad8.md) **>** [**i2cWrapper.h**](i2cWrapper_8h.md)

[Go to the source code of this file](i2cWrapper_8h_source.md)

_I2C master wrapper around the BSP I2C driver._ [More...](#detailed-description)

* `#include "i2c.h"`
* `#include "interrupt.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**i2cHandle\_config\_t**](structi2cHandle__config__t.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef struct i2cHandle\_t | [**i2cHandle\_t**](#typedef-i2chandle_t)  <br> |
| enum  | [**i2c\_wrapper\_err\_t**](#enum-i2c_wrapper_err_t)  <br> |
| typedef void(\* | [**intrHandler\_t**](#typedef-intrhandler_t)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**i2cHandle\_t**](structi2cHandle__t.md) \* | [**i2c\_initialize**](#function-i2c_initialize) ([**i2cHandle\_config\_t**](structi2cHandle__config__t.md) \* pDevCfg, [**intrHandle\_t**](structintrHandle__t.md) \* pIntr) <br>_Initializes one I2C controller instance._  |
|  int | [**i2c\_read**](#function-i2c_read) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst, u8 slaveAddr, u8 \* data, u32 length) <br>_Reads_ `length` _bytes without a register pointer (raw read)._ |
|  int | [**i2c\_readReg**](#function-i2c_readreg) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst, u8 slaveAddr, u8 regAddr, u8 \* data) <br>_Reads a single byte from an 8-bit register (1-byte special case of_ [_**i2c\_readRegMultipleBytes()**_](i2cWrapper_8h.md#function-i2c_readregmultiplebytes) _)._ |
|  int | [**i2c\_readRegMultipleBytes**](#function-i2c_readregmultiplebytes) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst, u8 slaveAddr, u8 regAddr, u8 \* data, u32 length) <br>_Reads_ `length` _bytes from a register, MSB first, using a repeated start._ |
|  int | [**i2c\_write**](#function-i2c_write) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst, u8 slaveAddr, u8 \* data, u32 length) <br>_Writes_ `length` _bytes without a register pointer (raw write)._ |
|  int | [**i2c\_writeReg**](#function-i2c_writereg) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst, u8 slaveAddr, u8 regAddr, u8 \* data) <br>_Writes a single byte to an 8-bit register (1-byte special case of_ [_**i2c\_writeRegMultipleBytes()**_](i2cWrapper_8h.md#function-i2c_writeregmultiplebytes) _)._ |
|  int | [**i2c\_writeRegMultipleBytes**](#function-i2c_writeregmultiplebytes) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst, u8 slaveAddr, u8 regAddr, u8 \* data, u32 length) <br>_Writes_ `length` _bytes to a register, MSB first._ |




























## Detailed Description







**Note:**

This file is deliberately not called i2c.h - that name is already used by the BSP driver and every src directory is on the include path. 





    
## Public Types Documentation




### typedef i2cHandle\_t 

```C++
typedef struct i2cHandle_t i2cHandle_t;
```



Opaque I2C controller handle (defined in [**i2cWrapper.c**](i2cWrapper_8c.md)). 


        

<hr>



### enum i2c\_wrapper\_err\_t 

```C++
enum i2c_wrapper_err_t {
    I2C_WRAPPER_OK =  0,
    I2C_WRAPPER_ERROR = -1,
    I2C_WRAPPER_TIMEOUT = -2,
    I2C_WRAPPER_ARB_LOSS = -3,
    I2C_WRAPPER_NACK = -4
};
```



Status codes returned by the I2C wrapper functions. 


        

<hr>



### typedef intrHandler\_t 

```C++
typedef void(* intrHandler_t) (void *data);
```



Interrupt handler callback type (see [**interrupt.h**](interrupt_8h.md)). 


        

<hr>
## Public Functions Documentation




### function i2c\_initialize 

_Initializes one I2C controller instance._ 
```C++
i2cHandle_t * i2c_initialize (
    i2cHandle_config_t * pDevCfg,
    intrHandle_t * pIntr
) 
```



Applies the timing configuration, connects the I2C ISR to the interrupt controller and enables the DROP (timeout) interrupt.




**Parameters:**


* `pDevCfg` Controller configuration; `deviceId` selects the instance. 
* `pIntr` Interrupt controller handle from [**intr\_initialize()**](interrupt_8c.md#function-intr_initialize). 



**Returns:**

Pointer to the controller instance. 





        

<hr>



### function i2c\_read 

_Reads_ `length` _bytes without a register pointer (raw read)._
```C++
int i2c_read (
    i2cHandle_t * pInst,
    u8 slaveAddr,
    u8 * data,
    u32 length
) 
```





**Parameters:**


* `pInst` Controller handle. 
* `slaveAddr` 7-bit I2C target address. 
* `data` Receives the data bytes on success. 
* `length` Number of data bytes (&gt;= 1). 



**Returns:**

I2C\_WRAPPER\_OK on success, negative [**i2c\_wrapper\_err\_t**](i2cWrapper_8h.md#enum-i2c_wrapper_err_t) code on error. 





        

<hr>



### function i2c\_readReg 

_Reads a single byte from an 8-bit register (1-byte special case of_ [_**i2c\_readRegMultipleBytes()**_](i2cWrapper_8h.md#function-i2c_readregmultiplebytes) _)._
```C++
int i2c_readReg (
    i2cHandle_t * pInst,
    u8 slaveAddr,
    u8 regAddr,
    u8 * data
) 
```





**Parameters:**


* `pInst` Controller handle. 
* `slaveAddr` 7-bit I2C target address. 
* `regAddr` Register address. 
* `data` Receives the register byte on success. 



**Returns:**

I2C\_WRAPPER\_OK on success, negative [**i2c\_wrapper\_err\_t**](i2cWrapper_8h.md#enum-i2c_wrapper_err_t) code on error. 





        

<hr>



### function i2c\_readRegMultipleBytes 

_Reads_ `length` _bytes from a register, MSB first, using a repeated start._
```C++
int i2c_readRegMultipleBytes (
    i2cHandle_t * pInst,
    u8 slaveAddr,
    u8 regAddr,
    u8 * data,
    u32 length
) 
```





**Parameters:**


* `pInst` Controller handle. 
* `slaveAddr` 7-bit I2C target address. 
* `regAddr` Register address. 
* `data` Receives the data bytes on success. 
* `length` Number of data bytes (&gt;= 1). 



**Returns:**

I2C\_WRAPPER\_OK on success, negative [**i2c\_wrapper\_err\_t**](i2cWrapper_8h.md#enum-i2c_wrapper_err_t) code on error. 





        

<hr>



### function i2c\_write 

_Writes_ `length` _bytes without a register pointer (raw write)._
```C++
int i2c_write (
    i2cHandle_t * pInst,
    u8 slaveAddr,
    u8 * data,
    u32 length
) 
```





**Parameters:**


* `pInst` Controller handle. 
* `slaveAddr` 7-bit I2C target address. 
* `data` Buffer with the bytes to write. 
* `length` Number of data bytes. 



**Returns:**

I2C\_WRAPPER\_OK on success, negative [**i2c\_wrapper\_err\_t**](i2cWrapper_8h.md#enum-i2c_wrapper_err_t) code on error. 





        

<hr>



### function i2c\_writeReg 

_Writes a single byte to an 8-bit register (1-byte special case of_ [_**i2c\_writeRegMultipleBytes()**_](i2cWrapper_8h.md#function-i2c_writeregmultiplebytes) _)._
```C++
int i2c_writeReg (
    i2cHandle_t * pInst,
    u8 slaveAddr,
    u8 regAddr,
    u8 * data
) 
```





**Parameters:**


* `pInst` Controller handle. 
* `slaveAddr` 7-bit I2C target address. 
* `regAddr` Register address. 
* `data` Pointer to the byte to write. 



**Returns:**

I2C\_WRAPPER\_OK on success, negative [**i2c\_wrapper\_err\_t**](i2cWrapper_8h.md#enum-i2c_wrapper_err_t) code on error. 





        

<hr>



### function i2c\_writeRegMultipleBytes 

_Writes_ `length` _bytes to a register, MSB first._
```C++
int i2c_writeRegMultipleBytes (
    i2cHandle_t * pInst,
    u8 slaveAddr,
    u8 regAddr,
    u8 * data,
    u32 length
) 
```





**Parameters:**


* `pInst` Controller handle. 
* `slaveAddr` 7-bit I2C target address. 
* `regAddr` Register address. 
* `data` Buffer with the bytes to write. 
* `length` Number of data bytes. 



**Returns:**

I2C\_WRAPPER\_OK on success, negative [**i2c\_wrapper\_err\_t**](i2cWrapper_8h.md#enum-i2c_wrapper_err_t) code on error. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/drivers/i2c/i2cWrapper.h`

