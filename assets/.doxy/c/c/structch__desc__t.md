

# Struct ch\_desc\_t



[**ClassList**](annotated.md) **>** [**ch\_desc\_t**](structch__desc__t.md)



_Static register/bit layout descriptor for one output channel._ [More...](#detailed-description)






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u8 | [**oe\_bit**](#variable-oe_bit)  <br> |
|  u8 | [**oe\_reg**](#variable-oe_reg)  <br> |
|  u8 | [**omux\_dis\_bit**](#variable-omux_dis_bit)  <br> |
|  u8 | [**out\_dis\_bit**](#variable-out_dis_bit)  <br> |
|  u8 | [**out\_dis\_reg**](#variable-out_dis_reg)  <br> |
|  u8 | [**reg\_div**](#variable-reg_div)  <br> |
|  u8 | [**reg\_mode**](#variable-reg_mode)  <br> |
|  u8 | [**reg\_omux**](#variable-reg_omux)  <br> |












































## Detailed Description


One entry per entry in [**gChDesc**](si5332_8c.md#variable-gchdesc); holds all register addresses and bit positions needed to configure and enable/disable a given output. 


    
## Public Attributes Documentation




### variable oe\_bit 

```C++
u8 ch_desc_t::oe_bit;
```



Bit position of this output's OE bit within [**oe\_reg**](structch__desc__t.md#variable-oe_reg). 


        

<hr>



### variable oe\_reg 

```C++
u8 ch_desc_t::oe_reg;
```



Address of the output-enable register ([**REG\_OE\_0\_5**](si5332_8c.md#define-reg_oe_0_5) or [**REG\_OE\_6\_7**](si5332_8c.md#define-reg_oe_6_7)). 


        

<hr>



### variable omux\_dis\_bit 

```C++
u8 ch_desc_t::omux_dis_bit;
```



Bit position of the OMUXx\_DIS bit in [**REG\_PD\_OMUX**](si5332_8c.md#define-reg_pd_omux). 


        

<hr>



### variable out\_dis\_bit 

```C++
u8 ch_desc_t::out_dis_bit;
```



Bit position of the OUTx\_DIS bit within [**out\_dis\_reg**](structch__desc__t.md#variable-out_dis_reg). 


        

<hr>



### variable out\_dis\_reg 

```C++
u8 ch_desc_t::out_dis_reg;
```



Address of the OUTx\_DIS power-down register (0xBC or 0xBD). 


        

<hr>



### variable reg\_div 

```C++
u8 ch_desc_t::reg_div;
```



Address of the OUTx\_DIV (R divider) register. 


        

<hr>



### variable reg\_mode 

```C++
u8 ch_desc_t::reg_mode;
```



Address of the OUTx\_MODE register. 


        

<hr>



### variable reg\_omux 

```C++
u8 ch_desc_t::reg_omux;
```



Address of the OMUXx\_SEL0/SEL1 register. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/si5332/si5332.c`

