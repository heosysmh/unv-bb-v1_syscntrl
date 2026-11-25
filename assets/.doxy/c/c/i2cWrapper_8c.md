

# File i2cWrapper.c



[**FileList**](files.md) **>** [**drivers**](dir_258f8fd52823f65304266634174f6293.md) **>** [**i2c**](dir_6bfd9f8c6bb0ff18ae4323cf33a90ad8.md) **>** [**i2cWrapper.c**](i2cWrapper_8c.md)

[Go to the source code of this file](i2cWrapper_8c_source.md)

_Implementation of the I2C master wrapper._ [More...](#detailed-description)

* `#include "i2cWrapper.h"`
* `#include "userDef.h"`
* `#include "bsp.h"`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**i2cHandle\_t**](structi2cHandle__t.md) <br>_I2C controller instance state (opaque handle, see_ [_**i2cWrapper.h**_](i2cWrapper_8h.md) _)._ |








## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**i2cHandle\_t**](structi2cHandle__t.md) | [**gI2cInstances**](#variable-gi2cinstances)  <br> |














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


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  int | [**TX\_AND\_CHECK\_WITH\_EC**](#function-tx_and_check_with_ec) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst, u8 byte) <br>_Transmits one byte and waits for the target acknowledge._  |
|  void | [**i2c\_isr**](#function-i2c_isr) () <br> |
|  void | [**i2c\_isr**](#function-i2c_isr) (void \* callBackRef) <br>_I2C interrupt service routine._  |
|  int | [**i2c\_masterStartWithEC**](#function-i2c_masterstartwithec) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst) <br>_Issues a (repeated) START and waits for completion._  |
|  int | [**i2c\_masterStopWithEC**](#function-i2c_masterstopwithec) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst) <br>_Issues a STOP and waits for completion._  |
|  int | [**i2c\_rxAckWaitWithEC**](#function-i2c_rxackwaitwithec) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst) <br>_Waits until the received ACK/NACK bit is valid._  |
|  int | [**i2c\_txAckWithEC**](#function-i2c_txackwithec) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst) <br>_Transmits an ACK and waits for completion._  |
|  int | [**i2c\_txNackWithEC**](#function-i2c_txnackwithec) ([**i2cHandle\_t**](structi2cHandle__t.md) \* pInst) <br>_Transmits a NACK and waits for completion._  |

























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**DEFAULT\_TIMEOUT**](i2cWrapper_8c.md#define-default_timeout)  `100000`<br> |

## Detailed Description


Transfers are polled; the ISR only sets error flags (timeout, arbitration loss) which the polling loops check, so a stuck bus aborts the transfer instead of hanging forever. 


    
## Public Static Attributes Documentation




### variable gI2cInstances 

```C++
i2cHandle_t gI2cInstances[I2C_NUM_OF_INSTANCES];
```



Static pool of controller instances (size from [**userDef.h**](userDef_8h.md)). 


        

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
## Public Static Functions Documentation




### function TX\_AND\_CHECK\_WITH\_EC 

_Transmits one byte and waits for the target acknowledge._ 
```C++
static int TX_AND_CHECK_WITH_EC (
    i2cHandle_t * pInst,
    u8 byte
) 
```





**Parameters:**


* `pInst` Controller handle. 
* `byte` Byte to transmit. 



**Returns:**

I2C\_WRAPPER\_OK, I2C\_WRAPPER\_TIMEOUT or I2C\_WRAPPER\_ARB\_LOSS. 





        

<hr>



### function i2c\_isr 

```C++
static void i2c_isr () 
```




<hr>



### function i2c\_isr 

_I2C interrupt service routine._ 
```C++
static void i2c_isr (
    void * callBackRef
) 
```



Sets the timeout / arbitration-loss error flags evaluated by the polling helpers and clears all interrupt flags.




**Parameters:**


* `callBackRef` Controller handle registered via [**intr\_connect()**](interrupt_8c.md#function-intr_connect). 




        

<hr>



### function i2c\_masterStartWithEC 

_Issues a (repeated) START and waits for completion._ 
```C++
static int i2c_masterStartWithEC (
    i2cHandle_t * pInst
) 
```



Resets the timeout/arbitration-loss flags before polling.




**Parameters:**


* `pInst` Controller handle. 



**Returns:**

I2C\_WRAPPER\_OK, I2C\_WRAPPER\_TIMEOUT or I2C\_WRAPPER\_ARB\_LOSS. 





        

<hr>



### function i2c\_masterStopWithEC 

_Issues a STOP and waits for completion._ 
```C++
static int i2c_masterStopWithEC (
    i2cHandle_t * pInst
) 
```



Resets the timeout/arbitration-loss flags before polling.




**Parameters:**


* `pInst` Controller handle. 



**Returns:**

I2C\_WRAPPER\_OK, I2C\_WRAPPER\_TIMEOUT or I2C\_WRAPPER\_ARB\_LOSS. 





        

<hr>



### function i2c\_rxAckWaitWithEC 

_Waits until the received ACK/NACK bit is valid._ 
```C++
static int i2c_rxAckWaitWithEC (
    i2cHandle_t * pInst
) 
```





**Parameters:**


* `pInst` Controller handle. 



**Returns:**

I2C\_WRAPPER\_OK, I2C\_WRAPPER\_TIMEOUT or I2C\_WRAPPER\_ARB\_LOSS. 





        

<hr>



### function i2c\_txAckWithEC 

_Transmits an ACK and waits for completion._ 
```C++
static int i2c_txAckWithEC (
    i2cHandle_t * pInst
) 
```





**Parameters:**


* `pInst` Controller handle. 



**Returns:**

I2C\_WRAPPER\_OK, I2C\_WRAPPER\_TIMEOUT or I2C\_WRAPPER\_ARB\_LOSS. 





        

<hr>



### function i2c\_txNackWithEC 

_Transmits a NACK and waits for completion._ 
```C++
static int i2c_txNackWithEC (
    i2cHandle_t * pInst
) 
```





**Parameters:**


* `pInst` Controller handle. 



**Returns:**

I2C\_WRAPPER\_OK, I2C\_WRAPPER\_TIMEOUT or I2C\_WRAPPER\_ARB\_LOSS. 





        

<hr>
## Macro Definition Documentation





### define DEFAULT\_TIMEOUT 

```C++
#define DEFAULT_TIMEOUT `100000`
```



Default timeout in ticks. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/drivers/i2c/i2cWrapper.c`

