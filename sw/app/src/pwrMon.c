#include "bsp.h"
#include "riscv.h"
#include "soc.h"

#include "ina745a.h"

static u8 pwrMonI2cAddr[6] = {0x40, 0x41, 0x43, 0x44, 0x4C, 0x4D};

static ina745a_t* pwrMon[6];

void pwrMon_init(void)
{
  static const char * const pwrMonName[6] = {
    "12V", "3V3", "5V0_SOM", "1V8", "VCCADJ_FMC1", "VCCADJ_FMC2"
    };
    ina745a_adc_config_t pmCfg = {
        .mode   = INA745A_MODE_CONT_TEMP_CURRENT_BUSV,
        .vbusct = INA745A_CT_1052US,
        .vsenct = INA745A_CT_1052US,
        .tct    = INA745A_CT_1052US,
        .avg    = INA745A_AVG_16
    };

    for (u32 i = 0; i < 6; i++)
    {
        if (ina745a_init(pwrMon[i], &pmCfg) != INA745A_OK)
        {
            bsp_printf("ERROR: Power-Monitor %s initialization failed\r\n", pwrMonName[i]);
            continue;
        }

        float busV = 0.0f, curA = 0.0f;

        ina745a_get_bus_voltage(pwrMon[i], &busV);
        ina745a_get_current(pwrMon[i], &curA);
        bsp_printf("Power-Monitor %s: Vbus=%d mV, I=%d mA\r\n",
                    pwrMonName[i], (int)(busV * 1000.0f), (int)(curA * 1000.0f));
    } 
}