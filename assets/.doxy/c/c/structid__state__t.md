

# Struct id\_state\_t



[**ClassList**](annotated.md) **>** [**id\_state\_t**](structid__state__t.md)



_Shadow state of one fractional MultiSynth divider (ID0 or ID1)._ [More...](#detailed-description)






















## Public Attributes

| Type | Name |
| ---: | :--- |
|  u32 | [**den**](#variable-den)  <br> |
|  u32 | [**num**](#variable-num)  <br> |
|  u8 | [**refcnt**](#variable-refcnt)  <br> |












































## Detailed Description


The ID output frequency is `f_id =  VCO_HZ / (num/den)`. Several output channels may share one ID as long as the resulting R divider stays within 1..63 (see [**try\_share\_id()**](si5332_8c.md#function-try_share_id)). 


    
## Public Attributes Documentation




### variable den 

```C++
u32 id_state_t::den;
```



Divider denominator. 


        

<hr>



### variable num 

```C++
u32 id_state_t::num;
```



Divider numerator; N = num / den (VCO / N = ID output frequency). 


        

<hr>



### variable refcnt 

```C++
u8 id_state_t::refcnt;
```



Number of output channels currently using this ID. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/devices/si5332/si5332.c`

