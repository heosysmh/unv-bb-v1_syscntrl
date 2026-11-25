
# File List

Here is a list of all files with brief descriptions:


* **dir** [**sw**](dir_01fcd3835fb4e7d9331b722d86291b65.md)     
    * **dir** [**app**](dir_fd9e68481a64957b5aa550f8031e1091.md)     
        * **dir** [**src**](dir_4d8bb3f988c5738b11212c0e5c5a2a68.md)     
            * **file** [**boardConfig.h**](boardConfig_8h.md) _Single configuration point of the system controller application. Set your FMC voltages and clock config here._     
            * **file** [**boardGpio.c**](boardGpio_8c.md) _Implementation of GPIO access (interface to FPGA)._     
            * **file** [**boardGpio.h**](boardGpio_8h.md) _Implementation of GPIO access (interface to FPGA)._     
            * **file** [**clkGen.c**](clkGen_8c.md) _Implementation of the clock generator function group (SI5332)._     
            * **file** [**clkGen.h**](clkGen_8h.md) _Clock generator function group (si5332)._     
            * **file** [**i2cBus.c**](i2cBus_8c.md) _Implementation of the I2C infrastructure ownership._     
            * **file** [**i2cBus.h**](i2cBus_8h.md) _Ownership of the I2C infrastructure of the board._     
            * **file** [**interrupt.c**](interrupt_8c.md) _Implementation of the PLIC interrupt controller abstraction._     
            * **file** [**interrupt.h**](interrupt_8h.md) _PLIC-based interrupt controller abstraction for the application._     
            * **file** [**main.c**](main_8c.md) _Board bring-up and supervision of the carrier board._     
            * **file** [**power.c**](power_8c.md) _Implementation of the power functions._     
            * **file** [**power.h**](power_8h.md) _Power functions of the board: FMC Vadj regulators (tpsm8286x) and the power monitors (ina745a)._     
            * **file** [**userDef.h**](userDef_8h.md) _Instance-pool sizes for the drivers and device drivers used by this application._     
    * **dir** [**devices**](dir_b1d9dda0f5d22dbe57e7280d3502a0d9.md)     
        * **dir** [**ina745a**](dir_dfd4869667a9d947f6105bade284a2f8.md)     
            * **file** [**ina745a.c**](ina745a_8c.md) _Driver implementation for the TI INA745A/B digital power monitor._     
            * **file** [**ina745a.h**](ina745a_8h.md) _Driver for the Texas Instruments INA745A/B 40V, 16-Bit, Precision I2C Output Digital Power Monitor with integrated 800µΩ shunt (EZShunt)._     
        * **dir** [**pi4msd5v945x**](dir_519e93f1bdf9c64a9ee465d9f69baf9a.md)     
            * **file** [**pi4msd5v945x.c**](pi4msd5v945x_8c.md) _Driver implementation for the Diodes PI4MSD5V945x I2C bus switch._     
            * **file** [**pi4msd5v945x.h**](pi4msd5v945x_8h.md) _Driver for the Diodes PI4MSD5V945x 4-channel I2C bus switch._     
        * **dir** [**si5332**](dir_f47f1daa29e3b569dba4f7284fbb0336.md)     
            * **file** [**si5332.c**](si5332_8c.md) _Driver implementation for the Skyworks Si5332A-D-GM2 clock generator._     
            * **file** [**si5332.h**](si5332_8h.md) _Driver for the Skyworks Si5332A-D-GM2 (8 outputs, 40-QFN), fixed VCO at 2.5 GHz, switchable input source (27 MHz crystal / 100 MHz CLKIN\_2)._     
            * **file** [**si5332\_cfg.c**](si5332__cfg_8c.md) _Static configuration tables._     
            * **file** [**si5332\_cfg.h**](si5332__cfg_8h.md) _Static configuration tables for the Si5332-GM2 Rev D (VCO 2.5 GHz)._     
        * **dir** [**tpsm8286x**](dir_d611db536405966ae215d07169916749.md)     
            * **file** [**tpsm8286x.c**](tpsm8286x_8c.md) _Driver implementation for the TI TPSM8286x step-down power module._     
            * **file** [**tpsm8286x.h**](tpsm8286x_8h.md) _Driver for the TI TPSM8286x step-down power module with I2C (PMBus-style) output voltage control._     
    * **dir** [**drivers**](dir_258f8fd52823f65304266634174f6293.md)     
        * **dir** [**i2c**](dir_6bfd9f8c6bb0ff18ae4323cf33a90ad8.md)     
            * **file** [**i2cWrapper.c**](i2cWrapper_8c.md) _Implementation of the I2C master wrapper._     
            * **file** [**i2cWrapper.h**](i2cWrapper_8h.md) _I2C master wrapper around the BSP I2C driver._     

