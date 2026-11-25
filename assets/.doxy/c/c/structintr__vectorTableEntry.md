

# Struct intr\_vectorTableEntry



[**ClassList**](annotated.md) **>** [**intr\_vectorTableEntry**](structintr__vectorTableEntry.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  void \* | [**callBackRef**](#variable-callbackref)  <br> |
|  [**intrHandler\_t**](interrupt_8h.md#typedef-intrhandler_t) | [**handler**](#variable-handler)  <br> |












































## Public Attributes Documentation




### variable callBackRef 

```C++
void* intr_vectorTableEntry::callBackRef;
```



CallBackRef is the callback reference passed in by the upper layer when setting the Interrupt handler for specific interrupt ID, and it will passed back to Interrupt handler when it is invoked. 


        

<hr>



### variable handler 

```C++
intrHandler_t intr_vectorTableEntry::handler;
```



Interrupt Handler 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/interrupt.c`

