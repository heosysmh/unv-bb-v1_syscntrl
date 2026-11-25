

# Struct si5332\_t



[**ClassList**](annotated.md) **>** [**si5332\_t**](structsi5332__t.md)



_Opaque device handle (definition), see_ [_**si5332\_t**_](structsi5332__t.md) _in_[_**si5332.h**_](si5332_8h.md) _._[More...](#detailed-description)






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**ch\_state\_t**](structch__state__t.md) | [**ch**](#variable-ch)  <br> |
|  [**si5332\_clk\_src\_t**](si5332_8h.md#enum-si5332_clk_src_t) | [**clk\_src**](#variable-clk_src)  <br> |
|  [**si5332\_clk\_src\_t**](si5332_8h.md#enum-si5332_clk_src_t) | [**clk\_src\_req**](#variable-clk_src_req)  <br> |
|  u8 | [**dirty\_ch**](#variable-dirty_ch)  <br> |
|  u8 | [**dirty\_id**](#variable-dirty_id)  <br> |
|  void \* | [**hal\_ctx**](#variable-hal_ctx)  <br> |
|  u8 | [**i2cAddr**](#variable-i2caddr)  <br> |
|  [**si5332\_i2c\_read\_f**](si5332_8h.md#typedef-si5332_i2c_read_f) | [**i2c\_read**](#variable-i2c_read)  <br> |
|  [**si5332\_i2c\_write\_f**](si5332_8h.md#typedef-si5332_i2c_write_f) | [**i2c\_write**](#variable-i2c_write)  <br> |
|  [**id\_state\_t**](structid__state__t.md) | [**id**](#variable-id)  <br> |
|  u8 | [**isConfigured**](#variable-isconfigured)  <br> |
|  u8 | [**isReady**](#variable-isready)  <br> |












































## Detailed Description


Holds the HAL callbacks, the currently/requested clock source and the complete shadow state (channels + MultiSynths) together with dirty bitmasks that track which parts still need to be written to the device by [**si5332\_commit()**](si5332_8c.md#function-si5332_commit). 


    
## Public Attributes Documentation




### variable ch 

```C++
ch_state_t si5332_t::ch[4];
```



Shadow state of the configurable output channels. 


        

<hr>



### variable clk\_src 

```C++
si5332_clk_src_t si5332_t::clk_src;
```



Clock source currently active on the device. 


        

<hr>



### variable clk\_src\_req 

```C++
si5332_clk_src_t si5332_t::clk_src_req;
```



Clock source requested by the application. 


        

<hr>



### variable dirty\_ch 

```C++
u8 si5332_t::dirty_ch;
```



Bitmask over channel indices with pending (uncommitted) changes. 


        

<hr>



### variable dirty\_id 

```C++
u8 si5332_t::dirty_id;
```



Bitmask over MultiSynth indices with pending (uncommitted) changes. 


        

<hr>



### variable hal\_ctx 

```C++
void* si5332_t::hal_ctx;
```



Opaque context pointer passed through to the HAL callbacks. 


        

<hr>



### variable i2cAddr 

```C++
u8 si5332_t::i2cAddr;
```



7-bit I2C address of the device. 


        

<hr>



### variable i2c\_read 

```C++
si5332_i2c_read_f si5332_t::i2c_read;
```



HAL register read callback. 


        

<hr>



### variable i2c\_write 

```C++
si5332_i2c_write_f si5332_t::i2c_write;
```



HAL register write callback. 


        

<hr>



### variable id 

```C++
id_state_t si5332_t::id[2];
```



Shadow state of the MultiSynth dividers. 


        

<hr>



### variable isConfigured 

```C++
u8 si5332_t::isConfigured;
```



Non-zero once [**si5332\_init()**](si5332_8c.md#function-si5332_init) has completed successfully. 


        

<hr>



### variable isReady 

```C++
u8 si5332_t::isReady;
```



Non-zero once HAL function pointers have been set by [**si5332\_initialize()**](si5332_8c.md#function-si5332_initialize). 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/si5332/si5332.c`

