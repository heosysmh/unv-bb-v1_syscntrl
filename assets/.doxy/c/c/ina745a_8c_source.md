

# File ina745a.c

[**File List**](files.md) **>** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md) **>** [**ina745a**](dir_dfd4869667a9d947f6105bade284a2f8.md) **>** [**ina745a.c**](ina745a_8c.md)

[Go to the documentation of this file](ina745a_8c.md)


```C++

// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2026 Matthias Haefner - Heosys


#include <string.h>

#include "ina745a.h"
#include "userDef.h"    /* provides INA745A_NUM_INSTANCES */

/* ------------------------------------------------------------------------- */
/* Register addresses (Table 7-1)                                            */
/* ------------------------------------------------------------------------- */
#define REG_CONFIG      0x00u   
#define REG_ADC_CONFIG      0x01u   
#define REG_VBUS        0x05u   
#define REG_DIETEMP     0x06u   
#define REG_CURRENT     0x07u   
#define REG_POWER       0x08u   
#define REG_ENERGY      0x09u   
#define REG_CHARGE      0x0Au   
#define REG_DIAG_ALRT       0x0Bu   
#define REG_COL         0x0Cu   
#define REG_CUL         0x0Du   
#define REG_BOVL        0x0Eu   
#define REG_BUVL        0x0Fu   
#define REG_TEMP_LIMIT      0x10u   
#define REG_PWR_LIMIT       0x11u   
#define REG_MANUFACTURER_ID 0x3Eu   

/* ------------------------------------------------------------------------- */
/* CONFIG register bit positions (Table 7-3)                                 */
/* ------------------------------------------------------------------------- */
#define CONFIG_RST_BIT      15u 
#define CONFIG_RSTACC_BIT   14u 
#define CONFIG_CONVDLY_SHIFT    6u  
#define CONFIG_CONVDLY_MASK (0xFFu << CONFIG_CONVDLY_SHIFT)

/* ------------------------------------------------------------------------- */
/* DIAG_ALRT register bit positions (Table 7-11)                             */
/* ------------------------------------------------------------------------- */
#define DIAG_ALATCH_BIT     15u
#define DIAG_CNVR_BIT       14u
#define DIAG_SLOWALERT_BIT  13u
#define DIAG_APOL_BIT       12u
#define DIAG_ENERGYOF_BIT   11u
#define DIAG_CHARGEOF_BIT   10u
#define DIAG_MATHOF_BIT     9u
#define DIAG_TMPOL_BIT      7u
#define DIAG_CURRENTOL_BIT  6u
#define DIAG_CURRENTUL_BIT  5u
#define DIAG_BUSOL_BIT      4u
#define DIAG_BUSUL_BIT      3u
#define DIAG_POL_BIT        2u
#define DIAG_CNVRF_BIT      1u
#define DIAG_MEMSTAT_BIT    0u

/*
 * Number of INA745A instances that can be managed simultaneously
 * (#INA745A_NUM_INSTANCES) is defined centrally in userDef.h, analogous
 * to TPSM8286_NUM_OF_INSTANCES for tpsm8286x.
 */

#define CHK(x)  do { ina745a_err_t _e = (x); if (_e != INA745A_OK) return _e; } while (0)

/* ------------------------------------------------------------------------- */
/* Data types                                                                 */
/* ------------------------------------------------------------------------- */

struct ina745a_t {
    u8 isReady;     
    u8 isConfigured;    
    u8 i2cAddr;     
    void *hal_ctx;      
    ina745a_i2c_write_f i2c_write;  
    ina745a_i2c_read_f  i2c_read;   
};

static struct ina745a_t gInstances[INA745A_NUM_INSTANCES];

/* ------------------------------------------------------------------------- */
/* Low-level register access                                                 */
/* ------------------------------------------------------------------------- */

static ina745a_err_t reg_write16(ina745a_t *d, u8 addr, u16 val)
{
    u8 buf[2] = { (u8)(val >> 8), (u8)(val & 0xFFu) };

    return d->i2c_write(d->hal_ctx, d->i2cAddr, addr, buf, sizeof(buf));
}

static ina745a_err_t reg_read16(ina745a_t *d, u8 addr, u16 *val)
{
    u8 buf[2];

    CHK(d->i2c_read(d->hal_ctx, d->i2cAddr, addr, buf, sizeof(buf)));
    *val = (u16)(((u16)buf[0] << 8) | buf[1]);
    return INA745A_OK;
}

static ina745a_err_t reg_read24(ina745a_t *d, u8 addr, u32 *val)
{
    u8 buf[3];

    CHK(d->i2c_read(d->hal_ctx, d->i2cAddr, addr, buf, sizeof(buf)));
    *val = ((u32)buf[0] << 16) | ((u32)buf[1] << 8) | buf[2];
    return INA745A_OK;
}

static ina745a_err_t reg_read40(ina745a_t *d, u8 addr, u64 *val)
{
    u8 buf[5];

    CHK(d->i2c_read(d->hal_ctx, d->i2cAddr, addr, buf, sizeof(buf)));
    *val = ((u64)buf[0] << 32) | ((u64)buf[1] << 24) | ((u64)buf[2] << 16)
         | ((u64)buf[3] << 8)  |  (u64)buf[4];
    return INA745A_OK;
}

/* ------------------------------------------------------------------------- */
/* Math helpers                                                              */
/* ------------------------------------------------------------------------- */

static s32 sign_extend32(u32 value, u8 bits)
{
    u32 m = 1u << (bits - 1);

    return (s32)((value ^ m) - m);
}

static s64 sign_extend64(u64 value, u8 bits)
{
    u64 m = 1ULL << (bits - 1);

    return (s64)((value ^ m) - m);
}

static s32 clampi(s32 x, s32 lo, s32 hi)
{
    return (x < lo) ? lo : (x > hi) ? hi : x;
}

static s32 round_f(float x)
{
    return (s32)((x >= 0.0f) ? (x + 0.5f) : (x - 0.5f));
}

/* ------------------------------------------------------------------------- */
/* ADC_CONFIG packing/unpacking (Table 7-4)                                  */
/* ------------------------------------------------------------------------- */

static ina745a_err_t adc_config_pack(const ina745a_adc_config_t *cfg, u16 *raw)
{
    if (((u32)cfg->mode   & ~0xFu) || ((u32)cfg->vbusct & ~0x7u) ||
        ((u32)cfg->vsenct & ~0x7u) || ((u32)cfg->tct    & ~0x7u) ||
        ((u32)cfg->avg    & ~0x7u))
        return INA745A_E_PARAM;

    *raw = (u16)(((u16)cfg->mode   & 0xFu) << 12)
         | (u16)(((u16)cfg->vbusct & 0x7u) << 9)
         | (u16)(((u16)cfg->vsenct & 0x7u) << 6)
         | (u16)(((u16)cfg->tct    & 0x7u) << 3)
         |  (u16)((u16)cfg->avg    & 0x7u);
    return INA745A_OK;
}

static void adc_config_unpack(u16 raw, ina745a_adc_config_t *cfg)
{
    cfg->mode   = (ina745a_mode_t)((raw >> 12) & 0xFu);
    cfg->vbusct = (ina745a_convtime_t)((raw >> 9) & 0x7u);
    cfg->vsenct = (ina745a_convtime_t)((raw >> 6) & 0x7u);
    cfg->tct    = (ina745a_convtime_t)((raw >> 3) & 0x7u);
    cfg->avg    = (ina745a_avg_t)(raw & 0x7u);
}

/* ------------------------------------------------------------------------- */
/* API                                                                        */
/* ------------------------------------------------------------------------- */

/* Public API: documented in ina745a.h only. */
ina745a_t* ina745a_initialize(u32 devId, u8 i2c_addr, ina745a_i2c_write_f w, ina745a_i2c_read_f r, void *ctx)
{
    ina745a_t *pDev;

    if (!w || !r)
        return 0;
    if (i2c_addr < INA745A_ADDR_MIN || i2c_addr > INA745A_ADDR_MAX)
        return 0;
    if (devId >= INA745A_NUM_INSTANCES)
        return 0;

    pDev = &gInstances[devId];

    memset(pDev, 0, sizeof(*pDev));
    pDev->i2cAddr   = i2c_addr;
    pDev->i2c_write = w;
    pDev->i2c_read  = r;
    pDev->hal_ctx   = ctx;
    pDev->isReady   = 1;
    return pDev;
}

ina745a_err_t ina745a_init(ina745a_t *pDev, const ina745a_adc_config_t *cfg)
{
    u16 manuf, raw;

    if (!pDev || !cfg)
        return INA745A_E_PARAM;
    if (!pDev->isReady)
        return INA745A_E_NOT_INIT;

    pDev->isConfigured = 0;

    CHK(reg_read16(pDev, REG_MANUFACTURER_ID, &manuf));
    if (manuf != INA745A_MANUFACTURER_ID)
        return INA745A_E_MANUF_ID;

    CHK(adc_config_pack(cfg, &raw));
    CHK(reg_write16(pDev, REG_ADC_CONFIG, raw));

    pDev->isConfigured = 1;
    return INA745A_OK;
}

ina745a_err_t ina745a_reset(ina745a_t *pDev)
{
    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isReady)
        return INA745A_E_NOT_INIT;

    pDev->isConfigured = 0;
    return reg_write16(pDev, REG_CONFIG, (u16)(1u << CONFIG_RST_BIT));
}

/* Read-modify-write so that the CONVDLY field is preserved. */
ina745a_err_t ina745a_reset_accumulators(ina745a_t *pDev)
{
    u16 cfg;

    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_CONFIG, &cfg));
    cfg = (u16)((cfg & ~(1u << CONFIG_RST_BIT)) | (1u << CONFIG_RSTACC_BIT));
    return reg_write16(pDev, REG_CONFIG, cfg);
}

/* Read-modify-write: RST/RSTACC and reserved bits are left untouched. */
ina745a_err_t ina745a_set_conversion_delay(ina745a_t *pDev, u8 steps_2ms)
{
    u16 cfg;

    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_CONFIG, &cfg));
    cfg = (u16)(cfg & ~CONFIG_CONVDLY_MASK);
    cfg = (u16)(cfg | ((u16)steps_2ms << CONFIG_CONVDLY_SHIFT));
    return reg_write16(pDev, REG_CONFIG, cfg);
}

ina745a_err_t ina745a_set_adc_config(ina745a_t *pDev, const ina745a_adc_config_t *cfg)
{
    u16 raw;

    if (!pDev || !cfg)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(adc_config_pack(cfg, &raw));
    return reg_write16(pDev, REG_ADC_CONFIG, raw);
}

ina745a_err_t ina745a_get_adc_config(ina745a_t *pDev, ina745a_adc_config_t *cfg)
{
    u16 raw;

    if (!pDev || !cfg)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_ADC_CONFIG, &raw));
    adc_config_unpack(raw, cfg);
    return INA745A_OK;
}

ina745a_err_t ina745a_set_mode(ina745a_t *pDev, ina745a_mode_t mode)
{
    u16 raw;

    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_ADC_CONFIG, &raw));
    raw = (u16)((raw & (u16)~(0xFu << 12)) | (((u16)mode & 0xFu) << 12));
    return reg_write16(pDev, REG_ADC_CONFIG, raw);
}

ina745a_err_t ina745a_get_bus_voltage(ina745a_t *pDev, float *volts)
{
    u16 raw;

    if (!pDev || !volts)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_VBUS, &raw));
    *volts = (float)raw * INA745A_BUS_VOLTAGE_LSB_V;
    return INA745A_OK;
}

ina745a_err_t ina745a_get_die_temperature(ina745a_t *pDev, float *celsius)
{
    u16 raw;
    s32 count;

    if (!pDev || !celsius)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_DIETEMP, &raw));
    count = sign_extend32((u32)((raw >> 4) & 0xFFFu), 12);
    *celsius = (float)count * INA745A_TEMP_LSB_C;
    return INA745A_OK;
}

ina745a_err_t ina745a_get_current(ina745a_t *pDev, float *amps)
{
    u16 raw;

    if (!pDev || !amps)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_CURRENT, &raw));
    *amps = (float)(s16)raw * INA745A_CURRENT_LSB_A;
    return INA745A_OK;
}

ina745a_err_t ina745a_get_power(ina745a_t *pDev, float *watts)
{
    u32 raw;

    if (!pDev || !watts)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read24(pDev, REG_POWER, &raw));
    *watts = (float)raw * INA745A_POWER_LSB_W;
    return INA745A_OK;
}

ina745a_err_t ina745a_get_energy(ina745a_t *pDev, double *joules)
{
    u64 raw;

    if (!pDev || !joules)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read40(pDev, REG_ENERGY, &raw));
    *joules = (double)raw * INA745A_ENERGY_LSB_J;
    return INA745A_OK;
}

ina745a_err_t ina745a_get_charge(ina745a_t *pDev, double *coulombs)
{
    u64 raw;
    s64 count;

    if (!pDev || !coulombs)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read40(pDev, REG_CHARGE, &raw));
    count = sign_extend64(raw, 40);
    *coulombs = (double)count * INA745A_CHARGE_LSB_C;
    return INA745A_OK;
}

ina745a_err_t ina745a_get_measurements(ina745a_t *pDev, ina745a_measurements_t *out)
{
    u16 vbus_raw, dietemp_reg, current_reg;
    u32 power_reg;
    u64 energy_reg, charge_reg;

    if (!pDev || !out)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_VBUS,    &vbus_raw));
    CHK(reg_read16(pDev, REG_DIETEMP, &dietemp_reg));
    CHK(reg_read16(pDev, REG_CURRENT, &current_reg));
    CHK(reg_read24(pDev, REG_POWER,   &power_reg));
    CHK(reg_read40(pDev, REG_ENERGY,  &energy_reg));
    CHK(reg_read40(pDev, REG_CHARGE,  &charge_reg));

    out->vbus_raw    = vbus_raw;
    out->dietemp_raw = (s16)sign_extend32((u32)((dietemp_reg >> 4) & 0xFFFu), 12);
    out->current_raw = (s16)current_reg;
    out->power_raw   = power_reg;
    out->energy_raw  = energy_reg;
    out->charge_raw  = sign_extend64(charge_reg, 40);

    out->bus_voltage_V = (float)out->vbus_raw    * INA745A_BUS_VOLTAGE_LSB_V;
    out->die_temp_C     = (float)out->dietemp_raw * INA745A_TEMP_LSB_C;
    out->current_A      = (float)out->current_raw * INA745A_CURRENT_LSB_A;
    out->power_W        = (float)out->power_raw   * INA745A_POWER_LSB_W;
    out->energy_J        = (double)out->energy_raw * INA745A_ENERGY_LSB_J;
    out->charge_C        = (double)out->charge_raw * INA745A_CHARGE_LSB_C;

    return INA745A_OK;
}

ina745a_err_t ina745a_set_current_limits(ina745a_t *pDev, float over_limit_A, float under_limit_A)
{
    s32 over_c, under_c;

    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    over_c  = clampi(round_f(over_limit_A  / INA745A_CURRENT_LSB_A), -32768, 32767);
    under_c = clampi(round_f(under_limit_A / INA745A_CURRENT_LSB_A), -32768, 32767);

    CHK(reg_write16(pDev, REG_COL, (u16)(s16)over_c));
    CHK(reg_write16(pDev, REG_CUL, (u16)(s16)under_c));
    return INA745A_OK;
}

ina745a_err_t ina745a_set_bus_voltage_limits(ina745a_t *pDev, float overvoltage_V, float undervoltage_V)
{
    s32 over_c, under_c;

    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    over_c  = clampi(round_f(overvoltage_V  / INA745A_BUS_VOLTAGE_LSB_V), 0, 0x7FFF);
    under_c = clampi(round_f(undervoltage_V / INA745A_BUS_VOLTAGE_LSB_V), 0, 0x7FFF);

    CHK(reg_write16(pDev, REG_BOVL, (u16)over_c));
    CHK(reg_write16(pDev, REG_BUVL, (u16)under_c));
    return INA745A_OK;
}

ina745a_err_t ina745a_set_temperature_limit(ina745a_t *pDev, float overtemp_C)
{
    s32 count;

    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    count = clampi(round_f(overtemp_C / INA745A_TEMP_LSB_C), -2048, 2047);
    return reg_write16(pDev, REG_TEMP_LIMIT, (u16)(((u16)((u32)count & 0xFFFu)) << 4));
}

ina745a_err_t ina745a_set_power_limit(ina745a_t *pDev, float overpower_W)
{
    s32 count;

    if (!pDev)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    count = clampi(round_f(overpower_W / INA745A_PWR_LIMIT_LSB_W), 0, 0xFFFF);
    return reg_write16(pDev, REG_PWR_LIMIT, (u16)count);
}

ina745a_err_t ina745a_configure_alert(ina745a_t *pDev, const ina745a_alert_cfg_t *cfg)
{
    u16 raw;

    if (!pDev || !cfg)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    raw = (u16)((cfg->alatch        ? (1u << DIAG_ALATCH_BIT)    : 0u)
              | (cfg->conv_ready_en ? (1u << DIAG_CNVR_BIT)      : 0u)
              | (cfg->slow_alert    ? (1u << DIAG_SLOWALERT_BIT) : 0u)
              | ((cfg->polarity == INA745A_ALERT_ACTIVE_HIGH) ? (1u << DIAG_APOL_BIT) : 0u));

    return reg_write16(pDev, REG_DIAG_ALRT, raw);
}

ina745a_err_t ina745a_get_diag_flags(ina745a_t *pDev, ina745a_diag_flags_t *out)
{
    u16 raw;

    if (!pDev || !out)
        return INA745A_E_PARAM;
    if (!pDev->isConfigured)
        return INA745A_E_NOT_INIT;

    CHK(reg_read16(pDev, REG_DIAG_ALRT, &raw));

    out->energy_overflow     = (u8)((raw >> DIAG_ENERGYOF_BIT)  & 1u);
    out->charge_overflow     = (u8)((raw >> DIAG_CHARGEOF_BIT)  & 1u);
    out->math_overflow       = (u8)((raw >> DIAG_MATHOF_BIT)    & 1u);
    out->temp_over_limit     = (u8)((raw >> DIAG_TMPOL_BIT)     & 1u);
    out->current_over_limit  = (u8)((raw >> DIAG_CURRENTOL_BIT) & 1u);
    out->current_under_limit = (u8)((raw >> DIAG_CURRENTUL_BIT) & 1u);
    out->bus_over_limit      = (u8)((raw >> DIAG_BUSOL_BIT)     & 1u);
    out->bus_under_limit     = (u8)((raw >> DIAG_BUSUL_BIT)     & 1u);
    out->power_over_limit    = (u8)((raw >> DIAG_POL_BIT)       & 1u);
    out->conversion_ready    = (u8)((raw >> DIAG_CNVRF_BIT)     & 1u);
    out->memory_ok           = (u8)((raw >> DIAG_MEMSTAT_BIT)   & 1u);

    return INA745A_OK;
}

ina745a_err_t ina745a_get_manufacturer_id(ina745a_t *pDev, u16 *id)
{
    if (!pDev || !id)
        return INA745A_E_PARAM;
    if (!pDev->isReady)
        return INA745A_E_NOT_INIT;

    return reg_read16(pDev, REG_MANUFACTURER_ID, id);
}
```


