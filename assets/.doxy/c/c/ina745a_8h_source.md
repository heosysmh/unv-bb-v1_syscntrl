

# File ina745a.h

[**File List**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**ina745a**](dir_dfd4869667a9d947f6105bade284a2f8.md) **>** [**ina745a.h**](ina745a_8h.md)

[Go to the documentation of this file](ina745a_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_INA745A_H_
#define SRC_INA745A_H_

#include <type.h>

/* ------------------------------------------------------------------------- */
/* Status codes                                                              */
/* ------------------------------------------------------------------------- */
typedef enum {
    INA745A_OK         =  0,    
    INA745A_E_PARAM    = -1,    
    INA745A_E_I2C      = -2,    
    INA745A_E_NOT_INIT = -3,    
    INA745A_E_MANUF_ID = -4     
} ina745a_err_t;

/* ------------------------------------------------------------------------- */
/* Low-level HAL                                                              */
/* ------------------------------------------------------------------------- */
typedef ina745a_err_t (*ina745a_i2c_write_f)(void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);
typedef ina745a_err_t (*ina745a_i2c_read_f) (void *ctx, u8 i2c_addr, u8 reg_addr, u8 *data, u32 len);

typedef struct ina745a_t ina745a_t;

/* ------------------------------------------------------------------------- */
/* I2C address range (datasheet Table 6-2: 16 addresses from A0/A1 strapping) */
/* ------------------------------------------------------------------------- */
#define INA745A_ADDR_MIN    0x40u   
#define INA745A_ADDR_MAX    0x4Fu   

#define INA745A_MANUFACTURER_ID 0x5449u

/* ------------------------------------------------------------------------- */
/* Fixed LSB / conversion factors (datasheet Table 8-1)                      */
/* ------------------------------------------------------------------------- */
#define INA745A_CURRENT_LSB_A       0.0012f     
#define INA745A_BUS_VOLTAGE_LSB_V   0.003125f   
#define INA745A_TEMP_LSB_C          0.125f      
#define INA745A_POWER_LSB_W         0.00024f    
#define INA745A_ENERGY_LSB_J        0.00384     
#define INA745A_CHARGE_LSB_C        0.000075    
#define INA745A_PWR_LIMIT_LSB_W     0.06144f    

/* ------------------------------------------------------------------------- */
/* ADC configuration types (register field values match the datasheet)      */
/* ------------------------------------------------------------------------- */

typedef enum {
    INA745A_MODE_SHUTDOWN               = 0x0,  
    INA745A_MODE_TRIG_BUSV              = 0x1,  
    INA745A_MODE_TRIG_TEMP              = 0x4,  
    INA745A_MODE_TRIG_TEMP_BUSV         = 0x5,  
    INA745A_MODE_TRIG_TEMP_CURRENT      = 0x6,  
    INA745A_MODE_TRIG_TEMP_CURRENT_BUSV = 0x7,  
    INA745A_MODE_CONT_BUSV              = 0x9,  
    INA745A_MODE_CONT_TEMP              = 0xC,  
    INA745A_MODE_CONT_TEMP_BUSV         = 0xD,  
    INA745A_MODE_CONT_TEMP_CURRENT      = 0xE,  
    INA745A_MODE_CONT_TEMP_CURRENT_BUSV = 0xF   
} ina745a_mode_t;

typedef enum {
    INA745A_CT_50US   = 0,  
    INA745A_CT_84US   = 1,  
    INA745A_CT_150US  = 2,  
    INA745A_CT_280US  = 3,  
    INA745A_CT_540US  = 4,  
    INA745A_CT_1052US = 5,  
    INA745A_CT_2074US = 6,  
    INA745A_CT_4120US = 7   
} ina745a_convtime_t;

typedef enum {
    INA745A_AVG_1    = 0,   
    INA745A_AVG_4    = 1,   
    INA745A_AVG_16   = 2,   
    INA745A_AVG_64   = 3,   
    INA745A_AVG_128  = 4,   
    INA745A_AVG_256  = 5,   
    INA745A_AVG_512  = 6,   
    INA745A_AVG_1024 = 7    
} ina745a_avg_t;

typedef struct {
    ina745a_mode_t     mode;    
    ina745a_convtime_t vbusct;  
    ina745a_convtime_t vsenct;  
    ina745a_convtime_t tct;     
    ina745a_avg_t      avg;     
} ina745a_adc_config_t;

/* ------------------------------------------------------------------------- */
/* Alert / diagnostic types (DIAG_ALRT register, Table 7-11)                 */
/* ------------------------------------------------------------------------- */
typedef enum {
    INA745A_ALERT_ACTIVE_LOW  = 0,  
    INA745A_ALERT_ACTIVE_HIGH = 1   
} ina745a_alert_pol_t;

typedef struct {
    u8 alatch;      
    u8 conv_ready_en;   
    u8 slow_alert;      
    ina745a_alert_pol_t polarity;   
} ina745a_alert_cfg_t;

typedef struct {
    u8 energy_overflow; 
    u8 charge_overflow; 
    u8 math_overflow;   
    u8 temp_over_limit; 
    u8 current_over_limit;  
    u8 current_under_limit; 
    u8 bus_over_limit;  
    u8 bus_under_limit; 
    u8 power_over_limit;    
    u8 conversion_ready;    
    u8 memory_ok;       
} ina745a_diag_flags_t;

/* ------------------------------------------------------------------------- */
/* Measurement results                                                        */
/* ------------------------------------------------------------------------- */
typedef struct {
    float  bus_voltage_V;   
    float  die_temp_C;  
    float  current_A;   
    float  power_W;     
    double energy_J;    
    double charge_C;    

    u16 vbus_raw;       
    s16 dietemp_raw;    
    s16 current_raw;    
    u32 power_raw;      
    u64 energy_raw;     
    s64 charge_raw;     
} ina745a_measurements_t;

/* ------------------------------------------------------------------------- */
/* API                                                                        */
/* ------------------------------------------------------------------------- */

ina745a_t* ina745a_initialize(u32 devId, u8 i2c_addr, ina745a_i2c_write_f w, ina745a_i2c_read_f r, void *ctx);

ina745a_err_t ina745a_init(ina745a_t *pDev, const ina745a_adc_config_t *cfg);

ina745a_err_t ina745a_reset(ina745a_t *pDev);

ina745a_err_t ina745a_reset_accumulators(ina745a_t *pDev);

ina745a_err_t ina745a_set_conversion_delay(ina745a_t *pDev, u8 steps_2ms);

ina745a_err_t ina745a_set_adc_config(ina745a_t *pDev, const ina745a_adc_config_t *cfg);

ina745a_err_t ina745a_get_adc_config(ina745a_t *pDev, ina745a_adc_config_t *cfg);

ina745a_err_t ina745a_set_mode(ina745a_t *pDev, ina745a_mode_t mode);


ina745a_err_t ina745a_get_bus_voltage(ina745a_t *pDev, float *volts);

ina745a_err_t ina745a_get_die_temperature(ina745a_t *pDev, float *celsius);

ina745a_err_t ina745a_get_current(ina745a_t *pDev, float *amps);

ina745a_err_t ina745a_get_power(ina745a_t *pDev, float *watts);

ina745a_err_t ina745a_get_energy(ina745a_t *pDev, double *joules);

ina745a_err_t ina745a_get_charge(ina745a_t *pDev, double *coulombs);

ina745a_err_t ina745a_get_measurements(ina745a_t *pDev, ina745a_measurements_t *out);


ina745a_err_t ina745a_set_current_limits(ina745a_t *pDev, float over_limit_A, float under_limit_A);

ina745a_err_t ina745a_set_bus_voltage_limits(ina745a_t *pDev, float overvoltage_V, float undervoltage_V);

ina745a_err_t ina745a_set_temperature_limit(ina745a_t *pDev, float overtemp_C);

ina745a_err_t ina745a_set_power_limit(ina745a_t *pDev, float overpower_W);

ina745a_err_t ina745a_configure_alert(ina745a_t *pDev, const ina745a_alert_cfg_t *cfg);

ina745a_err_t ina745a_get_diag_flags(ina745a_t *pDev, ina745a_diag_flags_t *out);

ina745a_err_t ina745a_get_manufacturer_id(ina745a_t *pDev, u16 *id);

#endif /* SRC_INA745A_H_ */
```


