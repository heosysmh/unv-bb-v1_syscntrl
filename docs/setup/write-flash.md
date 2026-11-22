 # Writing a new firmware to the flash memory

 The following instructions were tested with Efinity 2026.1.132 on Ubuntu 24.04.5LTS.

 ## Prerequisites
 Efinix Efinity must be installed including the USB drivers for the programmer. For details see the Efintiy Software Installation User Guide.

 ==**!!! Remove the SoM from the board to avoid damage due by misconfiguration of the IO-Voltages !!!**==

 ## Connecting the FTDI-JTAG to the system controller

 - Set DIP-Switch SW1701.1 to "ON".
 This connects the JTAG-Port and COM2 to the system controller

 ## Setting up the Efinity Software Pogrammer
 - Connect the baseboard (J1701 - DEBUG) to your computer with a micro-USB-cable.
 - Open the programmer application
    ***USB-Target*** should show *"(local) Kria Base-Board"*
 - In the ***Programming Mode*** section, select ***SPI Active using JATG Bridge***
    - Enable ***Auto configure JTAG Bridge Image***
    - Select `T20Q100F3_Jtag2Flash.bit` from the release archive as the bitstream file
        The *"FPGA"*-field should show *T20Q100F3*
    - In the ***Image*** section, select `sysCntrl_flash.hex` from the release archive as the bitstream file.
    The *"FPGA"*-field should show *"T20Q100F3"* as well.
- In the ***SPI Active Options*** section, ***Starting Flash Address*** should be `0x00000000`.
    - Enable ***Erase Before Programming***
- In the ***JTAG Otions*** section, set a ***JTAG Clock Speed*** of *7.5 MHZ* or *6 MHz*

## Writing the flash
-  In the ***Programming Mode*** section, klick on the *Start Program* button on the right
After a ~15s the console should show *"Programming operation completed"*
 
## Restarting the system controller
 - Set DIP-Switch SW1701.1 to "OFF".
 This disconnects the JTAG-Port and COM2 to the system controller
 - Repower the board or press the Reset-Button *PB2300* to restart the system controller
When board configuration has completed, the firmware toogles LED *SLD0* to indicate the it has started monitoring the power supplies. 

## Monitoring the power supplies
After board start-up, the system controller reads back all power supply monitors and sends status messages over the UART (FTDI COM2) once a second.
In order to reads these message, Set DIP-Switch SW1701.1 to "ON" after board start-up and open a terminal program set to *115200 Baud 8N1*.

## Current Limitations 
Because of a design error, connecting the FTDI-JTAG to the system controller makes the reset button and the power-on reset ineffective (FTDI reset-out is push-pull, not open-drain). As a consequence, the FTDI-JTAG must be disconnect for a regular start-up. A patch for this problem may be inserting a diode in the reset line (not tested yet).
