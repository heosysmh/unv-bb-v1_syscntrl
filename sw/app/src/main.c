///////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2025 SaxonSoc contributors
//  SPDX license identifier: MIT
//  Full license header bsp/efinix/EfxSapphireSoc/include/LICENSE.MD
//////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
*
* @file main.c: i2cMasterInterruptDemo
*
* @brief This demo is functionally similar to i2cMasterDemo but includes interrupt support
*        to prevent the program from getting stuck if the slave device fails to respond.
*        It demonstrates the use of a timeout interrupt to handle stalled I2C communication.
*        The program operates as an I2C master, performing both single- and multi-byte reads
*        from a slave device, which can be emulated using I2CSlaveDemo.
*        It assumes it is the sole master on the bus and performs data transfers in a blocking manner.
*
******************************************************************************/
#include <stdint.h>
#include "bsp.h"
#include "riscv.h"
#include "soc.h"
#include "gpio.h"

#include "i2cWrapper.h"
#include "interrupt.h" 
#include "tpsm8286x.h"
#include "si5332.h"
#include "pi4msd5v945x.h"


#define FMC1_VADJ	1.8f
#define FMC2_VADJ	1.8f

#define FMC_VADJ_MAX	1.8f

#define POWER_DELAY			10000u

#define GPIO_IN_VCCO_PS_EN_SHIFT	0
#define GPIO_IN_VCCO_PL_EN_SHIFT	1
#define GPIO_IN_SDPWR_EN_SHIFT		2
#define GPIO_IN_HDA14P_SHIFT		3
#define GPIO_IN_HDA14N_SHIFT		4
#define GPIO_IN_MIO35_SHIFT			5
#define GPIO_IN_FMC1_SUP_PG_SHIFT	6
#define GPIO_IN_FMC2_SUP_PG_SHIFT	7
#define GPIO_IN_FMC1_MOD_PG_SHIFT	8
#define GPIO_IN_FMC2_MOD_PG_SHIFT	9
#define GPIO_IN_2V5_PG_SHIFT		10
#define GPIO_IN_SYS_PG_SHIFT		11
#define GPIO_IN_SZG_PG_SHIFT		12
#define GPIO_IN_LMK_INTR_SHIFT		13
#define GPIO_IN_ETH_PME_SHIFT		14

#define GPIO_OUT_FMC1_SUP_EN_SHIFT	0
#define GPIO_OUT_FMC2_SUP_EN_SHIFT	1
#define GPIO_OUT_FMC1_MOD_EN_SHIFT	2
#define GPIO_OUT_FMC2_MOD_EN_SHIFT	3
#define GPIO_OUT_POR_B_SHIFT		4
#define GPIO_OUT_ETH_RSTN_SHIFT		5

#define PLIC_BASE				SYSTEM_PLIC_CTRL

#define I2C_PMBUS_BASE 			SYSTEM_I2C_1_IO_CTRL					// Base address of I2C-Module
#define I2C_PMBUS_DEV_ID		0x1
#define I2C_PMBUS_CORE_FREQ 	SYSTEM_CLINT_HZ							// Core Frequency of I2C-Module
#define I2C_PMBUS_IF_FREQ  		100000U  								// I2C Frequency
#define I2C_PMBUS_INTR_ID 		SYSTEM_PLIC_SYSTEM_I2C_1_IO_INTERRUPT	// Interrupt ID of I2C-Module
#define I2C_PMBUS_INTR_PRIO	1U
#define I2C_PMBUS_INTR_HART	0U

#define I2C_SYSCFG_BASE 		SYSTEM_I2C_0_IO_CTRL					// Base address of I2C-Module
#define I2C_SYSCFG_DEV_ID		0x0
#define I2C_SYSCFG_CORE_FREQ 	SYSTEM_CLINT_HZ							// Core Frequency of I2C-Module
#define I2C_SYSCFG_IF_FREQ  	100000U  								// I2C Frequency
#define I2C_SYSCFG_INTR_ID 		SYSTEM_PLIC_SYSTEM_I2C_0_IO_INTERRUPT	// Interrupt ID of I2C-Module
#define I2C_SYSCFG_INTR_PRIO	2U
#define I2C_SYSCFG_INTR_HART	0U

#define FMC1_VADJ_I2C_ADDR		0x4E
#define FMC2_VADJ_I2C_ADDR		0x4F

#define CLOCK_GEN_I2C_ADDR		0x6A
#define BUS_SWITCH_I2C_ADDR		0x77

#define I2C_CH_SYZYGY			PI4MSD5V945X_CH0
#define I2C_CH_FMC2				PI4MSD5V945X_CH1
#define I2C_CH_FMC1				PI4MSD5V945X_CH2
#define I2C_CH_MBRD				PI4MSD5V945X_CH3

#define GPIO_IN					SYSTEM_GPIO_0_IO_CTRL
#define GPIO_OUT				SYSTEM_GPIO_1_IO_CTRL

static i2cHandle_t* i2c_pmbus;
static i2cHandle_t* i2c_sysCntrl;

static intrHandle_t* intr_cntrl;

static tpsm8286_t* fmc_vadj[2];

static si5332_t* clockGen;

static pi4msd5v945x_t* i2c_busSwitch;

void trap_entry();




void init(){

	intr_cntrl = intr_initialize(PLIC_BASE);
	intr_setThreshold(intr_cntrl, 0, 0);

	/*
	 * Configure I2C-Interface for power-management (i.e. power converters and monitors)
	 */

	i2cHandle_config_t i2cCfg;

	i2cCfg.baseAddress 	= I2C_PMBUS_BASE;
	i2cCfg.deviceId    	= I2C_PMBUS_DEV_ID;
	i2cCfg.intrId	   	= I2C_PMBUS_INTR_ID;
	i2cCfg.intrPrio		= I2C_PMBUS_INTR_PRIO;
	i2cCfg.intrHart		= I2C_PMBUS_INTR_HART;
	i2cCfg.cfg.samplingClockDivider	= 3;
	i2cCfg.cfg.timeout              = I2C_PMBUS_CORE_FREQ/10;                 	// timeout = 0.1s
	i2cCfg.cfg.tsuDat               = I2C_PMBUS_CORE_FREQ/I2C_PMBUS_IF_FREQ/3;  // tsuDat = 3.33us
	i2cCfg.cfg.tLow                 = I2C_PMBUS_CORE_FREQ/I2C_PMBUS_IF_FREQ/2;  // tLow   = 5us
	i2cCfg.cfg.tHigh                = I2C_PMBUS_CORE_FREQ/I2C_PMBUS_IF_FREQ/2;  // tHigh  = 5us
	i2cCfg.cfg.tBuf                 = I2C_PMBUS_CORE_FREQ/I2C_PMBUS_IF_FREQ;    // tBuf   = 10us

	i2c_pmbus = i2c_initialize(&i2cCfg, intr_cntrl);

	/*
	 * Configure I2C-Interface for system configuration (i.e. clock generator, add-on modules)
	 */
	i2cCfg.baseAddress 	= I2C_SYSCFG_BASE;
	i2cCfg.deviceId    	= I2C_SYSCFG_DEV_ID;
	i2cCfg.intrId	   	= I2C_SYSCFG_INTR_ID;
	i2cCfg.intrPrio		= I2C_SYSCFG_INTR_PRIO;
	i2cCfg.intrHart		= I2C_SYSCFG_INTR_HART;
	i2cCfg.cfg.samplingClockDivider	= 3;
	i2cCfg.cfg.timeout              = I2C_SYSCFG_CORE_FREQ/10;                 	  // timeout = 0.1s
	i2cCfg.cfg.tsuDat               = I2C_SYSCFG_CORE_FREQ/I2C_SYSCFG_IF_FREQ/3;  // tsuDat = 3.33us
	i2cCfg.cfg.tLow                 = I2C_SYSCFG_CORE_FREQ/I2C_SYSCFG_IF_FREQ/2;  // tLow   = 5us
	i2cCfg.cfg.tHigh                = I2C_SYSCFG_CORE_FREQ/I2C_SYSCFG_IF_FREQ/2;  // tHigh  = 5us
	i2cCfg.cfg.tBuf                 = I2C_SYSCFG_CORE_FREQ/I2C_SYSCFG_IF_FREQ;    // tBuf   = 10us

	i2c_sysCntrl = i2c_initialize(&i2cCfg, intr_cntrl);




	//configure RISC-V interrupt CSR
	//Set the machine trap vector (trap.S)
	csr_write(mtvec, trap_entry);
	//Enable machine external interrupts
	csr_write(mie, MIE_MEIE);
	//Enable interrupts
	csr_write(mstatus, csr_read(mstatus) | MSTATUS_MPP | MSTATUS_MIE);


	fmc_vadj[0] = tpsm8286_initialize(0, FMC1_VADJ_I2C_ADDR, (tpsm8286_i2c_write_f)i2c_writeReg, (tpsm8286_i2c_read_f)i2c_readReg, (void*)i2c_pmbus);
	fmc_vadj[1] = tpsm8286_initialize(1, FMC2_VADJ_I2C_ADDR, (tpsm8286_i2c_write_f)i2c_writeReg, (tpsm8286_i2c_read_f)i2c_readReg, (void*)i2c_pmbus);

	i2c_busSwitch = pi4msd5v945x_initialize(BUS_SWITCH_I2C_ADDR, (pi4msd5v945x_i2c_write_f)i2c_writeDirect, (pi4msd5v945x_i2c_read_f)i2c_readDirect, (void*)i2c_sysCntrl);

	clockGen = si5332_initialize(CLOCK_GEN_I2C_ADDR, (si5332_i2c_write_f)i2c_writeReg, (si5332_i2c_read_f)i2c_writeReg, (void*)i2c_sysCntrl);

}


void eth_release_reset(void)
{
	u32 reg = gpio_getOutput(GPIO_OUT);

	reg |= (1 << GPIO_OUT_ETH_RSTN_SHIFT);

	gpio_setOutput(GPIO_OUT,reg);
}

void eth_invoke_reset(void)
{
	u32 reg = gpio_getOutput(GPIO_OUT);

	reg &= ~(1 << GPIO_OUT_ETH_RSTN_SHIFT);
	gpio_setOutput(GPIO_OUT,reg);

	bsp_uDelay(10);

	reg |= (1 << GPIO_OUT_ETH_RSTN_SHIFT);
	gpio_setOutput(GPIO_OUT,reg);
}

void som_release_por(void)
{
	u32 reg = gpio_getOutput(GPIO_OUT);

	reg |= (1 << GPIO_OUT_POR_B_SHIFT);

	gpio_setOutput(GPIO_OUT,reg);
}

void fmc_supply_set_enable(int enable)
{
	u32 reg = gpio_getOutput(GPIO_OUT);

	if(enable == 1) reg |= ( (1 << GPIO_OUT_FMC1_SUP_EN_SHIFT) | (1 << GPIO_OUT_FMC2_SUP_EN_SHIFT) );
	else reg &= ~( (1 << GPIO_OUT_FMC1_SUP_EN_SHIFT) | (1 << GPIO_OUT_FMC2_SUP_EN_SHIFT) );

	gpio_setOutput(GPIO_OUT,reg);
}

/******************************************************************************
*
* @brief This main function demonstrates the functionality of I2C Master. It performs
*        single byte and multi-byte write-read operations with error checks.
*        The I2C Master communicates with an I2C Slave (or another compatible I2C
*        device) to perform the tests.
*
******************************************************************************/
void main() {

    bsp_init();

    if((FMC1_VADJ > FMC_VADJ_MAX) | (FMC2_VADJ > FMC_VADJ_MAX))
	{
		bsp_printf("!!! ERROR: FMC1_Vadj out out range !!!\r\n");
		return;
	}

    gpio_setOutput(GPIO_OUT,0x0);

    init(); // Initialize
    bsp_printf("Board configuration start\r\n");

    u8 data;
	int regOk = 0;
	u32 gpioIn = 0;

    //Wait until SoM enables PL and PS supplies
    u32 mask = (1 << GPIO_IN_VCCO_PL_EN_SHIFT) | (1 << GPIO_IN_VCCO_PS_EN_SHIFT) | (1 << GPIO_IN_SYS_PG_SHIFT);
    while((gpioIn  & mask) != mask )
    {
    	gpioIn = gpio_getInput(GPIO_IN);
    }
    //Wait until voltages are stable
    bsp_uDelay(POWER_DELAY);

    bsp_printf("Configure Clock-Generator\r\n");
	pi4msd5v945x_setChannels(i2c_busSwitch, I2C_CH_MBRD);
	si5332_apply_config(clockGen);

	//Wait for clock outputs to be activated
	bsp_uDelay(POWER_DELAY);

	bsp_printf("Release ETH-PHY reset\r\n");
	eth_release_reset();

    //Enable FMC power
    fmc_supply_set_enable(1);
    //Wait until voltages are stable
    bsp_uDelay(POWER_DELAY);

    //Configure DC/DC-Converters
    tpsm8286_set_cntrl(fmc_vadj[0], TPSM8286_FPWM_AT_VC_MASK | TPSM8286_FPWM_MASK |
    		TPSM8286_OUTPUT_DISCHARGE_MASK | TPSM8286_HICCUP_EN_MASK | TPSM8286_VOLTAGE_RAMP_SPEED_1MV);

    tpsm8286_set_cntrl(fmc_vadj[1], TPSM8286_FPWM_AT_VC_MASK | TPSM8286_FPWM_MASK |
        		TPSM8286_OUTPUT_DISCHARGE_MASK | TPSM8286_HICCUP_EN_MASK | TPSM8286_VOLTAGE_RAMP_SPEED_1MV);


    tpsm8286_set_voltage(fmc_vadj[0], FMC1_VADJ);
    tpsm8286_set_voltage(fmc_vadj[1], FMC2_VADJ);

	tpsm8286_check_voltageReg(fmc_vadj[0],&regOk);
	if(regOk == 1)
	{
		tpsm8286_check_voltageReg(fmc_vadj[1],&regOk);
		if(regOk == 1)
		{
			bsp_printf("Enable FMCs\r\n");
			tpsm8286_set_enable(fmc_vadj[0], 1);
			tpsm8286_set_enable(fmc_vadj[1], 1);
			//gpio_setOutput(GPIO_OUT,0xF);
		}
	}


	bsp_printf("Release POR \r\n");
	som_release_por();

	bsp_printf("Finished. \r\n");
	while(1){};

}





