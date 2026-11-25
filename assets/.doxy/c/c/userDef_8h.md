

# File userDef.h



[**FileList**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**userDef.h**](userDef_8h.md)

[Go to the source code of this file](userDef_8h_source.md)

_Instance-pool sizes for the drivers and device drivers used by this application._ [More...](#detailed-description)

































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**I2C\_NUM\_OF\_INSTANCES**](userDef_8h.md#define-i2c_num_of_instances)  `2`<br> |
| define  | [**INA745A\_NUM\_INSTANCES**](userDef_8h.md#define-ina745a_num_instances)  `6`<br> |
| define  | [**TPSM8286\_NUM\_OF\_INSTANCES**](userDef_8h.md#define-tpsm8286_num_of_instances)  `2`<br> |

## Detailed Description


The board itself is configured in [**boardConfig.h**](boardConfig_8h.md). The counts below must match the board configuration. 


    
## Macro Definition Documentation





### define I2C\_NUM\_OF\_INSTANCES 

```C++
#define I2C_NUM_OF_INSTANCES `2`
```



[**drivers/i2c/i2cWrapper.c**](i2cWrapper_8c.md): number of I2C controller instances (PMBus, SysCfg). 


        

<hr>



### define INA745A\_NUM\_INSTANCES 

```C++
#define INA745A_NUM_INSTANCES `6`
```



devices/ina745a: one instance per monitored rail (see PWR\_MON\_TABLE in [**power.h**](power_8h.md)). 


        

<hr>



### define TPSM8286\_NUM\_OF\_INSTANCES 

```C++
#define TPSM8286_NUM_OF_INSTANCES `2`
```



devices/tpsm8286x: FMC1\_VADJ, FMC2\_VADJ. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `sw/app/src/userDef.h`

