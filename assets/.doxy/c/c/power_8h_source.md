

# File power.h

[**File List**](files.md) **>** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md) **>** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md) **>** [**power.h**](power_8h.md)

[Go to the documentation of this file](power_8h.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#ifndef SRC_POWER_H_
#define SRC_POWER_H_

#include <type.h>

#include "boardConfig.h"

/* =========================================================================== */
/* 5. Power monitors: per-rail configuration                                   */
/* =========================================================================== */
/*    name,        addr,                    pg,                                       vov,   iov,   tov */
#define PWR_MON_TABLE \
    X(12V,         I2CBUS_PWRMON_12V_ADDR,  PWR_PG_SYS,                            13.20f,  5.0f, 95.0f) \
    X(3V3,         I2CBUS_PWRMON_3V3_ADDR,  PWR_PG_SYS,                             3.60f, 12.0f, 95.0f) \
    X(5V0_SOM,     I2CBUS_PWRMON_5V0_ADDR,  PWR_PG_SYS,                             5.50f,  5.0f, 95.0f) \
    X(1V8,         I2CBUS_PWRMON_1V8_ADDR,  PWR_PG_SYS,                             1.98f,  5.0f, 95.0f) \
    X(VCCADJ_FMC1, I2CBUS_PWRMON_FMC1_ADDR, PWR_PG_FMC1_SUP,  (BOARD_FMC_VADJ_MAX_V+0.2f),  4.5f, 95.0f) \
    X(VCCADJ_FMC2, I2CBUS_PWRMON_FMC2_ADDR, PWR_PG_FMC2_SUP,  (BOARD_FMC_VADJ_MAX_V+0.2f),  4.5f, 95.0f)

/* --------------------------------------------------------------------------- */
/* Status codes                                                                 */
/* --------------------------------------------------------------------------- */
typedef enum {
    PWR_OK          =  0,   
    PWR_E_PARAM     = -1,   
    PWR_E_INIT      = -2,   
    PWR_E_DEVICE    = -3,   
    PWR_E_RANGE     = -4,   
    PWR_E_REGULATION= -5    
} pwr_err_t;

/* --------------------------------------------------------------------------- */
/* Power-good / ALERT nets                                                      */
/* --------------------------------------------------------------------------- */
typedef enum {
    PWR_PG_SYS = 0,     
    PWR_PG_FMC1_SUP,    
    PWR_PG_FMC2_SUP,    
    PWR_PG_COUNT        
} pwrPgLine_t;

/* --------------------------------------------------------------------------- */
/* Monitored rails (generated from PWR_MON_TABLE)                               */
/* --------------------------------------------------------------------------- */
#define X(name, addr, pg, vov, iov, tov)    PWR_RAIL_##name,
typedef enum {
    PWR_MON_TABLE
    PWR_RAIL_COUNT      
} pwrRail_t;
#undef X

/* --------------------------------------------------------------------------- */
/* FMC Vadj regulators                                                          */
/* --------------------------------------------------------------------------- */
typedef enum {
    PWR_FMC1 = 0,       
    PWR_FMC2,           
    PWR_FMC_COUNT       
} pwrFmc_t;

/* --------------------------------------------------------------------------- */
/* Measurements and faults                                                      */
/* --------------------------------------------------------------------------- */
typedef struct {
    float bus_voltage_V;    
    float current_A;        
    float die_temp_C;       
} pwrMon_values_t;

typedef struct {
    u8 over_voltage;    
    u8 over_current;    
    u8 over_temp;       
    u8 over_power;      
    u8 memory_fail;     
} pwrMon_fault_t;

/* --------------------------------------------------------------------------- */
/* API                                                                          */
/* --------------------------------------------------------------------------- */

pwr_err_t pwr_init(void);


pwr_err_t pwr_fmcVadj_configure(void);

pwr_err_t pwr_fmcVadj_isRegulating(pwrFmc_t fmc);

pwr_err_t pwr_fmcVadj_enable(int enable);


pwr_err_t pwr_mon_configure(void);

pwr_err_t pwr_mon_read(pwrRail_t rail, pwrMon_values_t *values);

pwr_err_t pwr_mon_poll(void);

pwr_err_t pwr_checkAlerts(void);

pwr_err_t pwr_mon_getFaults(pwrRail_t rail, pwrMon_fault_t *faults);

void pwr_mon_clearFaults(pwrRail_t rail);

void pwr_mon_reportFaults(void);

const char* pwr_railName(pwrRail_t rail);

#endif /* SRC_POWER_H_ */
```


