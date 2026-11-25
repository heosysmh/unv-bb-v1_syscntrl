

# Struct intrHandle\_t



[**ClassList**](annotated.md) **>** [**intrHandle\_t**](structintrHandle__t.md)



_Interrupt controller instance state (opaque handle, see_ [_**interrupt.h**_](interrupt_8h.md) _)._






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**intr\_vectorTableEntry**](structintr__vectorTableEntry.md) | [**HandlerTable**](#variable-handlertable)  <br> |
|  u32 | [**baseAddress**](#variable-baseaddress)  <br> |
|  u8 | [**threshold**](#variable-threshold)  <br> |












































## Public Attributes Documentation




### variable HandlerTable 

```C++
intr_vectorTableEntry intrHandle_t::HandlerTable[36];
```



Vector table of interrupt handlers 


        

<hr>



### variable baseAddress 

```C++
u32 intrHandle_t::baseAddress;
```



Base address of the PLIC controller. 


        

<hr>



### variable threshold 

```C++
u8 intrHandle_t::threshold;
```



Currently configured priority threshold. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/interrupt.c`

