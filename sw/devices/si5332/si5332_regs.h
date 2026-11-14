/*
 * Si5332-GM2 Rev D Configuration Register Export Header File
 *
 * This file represents a series of Skyworks Si5332-GM2 Rev D 
 * register writes that can be performed to load a single configuration 
 * on a device. It was created by a Skyworks ClockBuilder Pro
 * export tool.
 *
 * Part:		                                       Si5332-GM2 Rev D
 * Design ID:                                          KRIABB
 * Includes Pre/Post Download Control Register Writes: Yes
 * Created By:                                         ClockBuilder Pro v4.17 [2025-11-06]
 * Timestamp:                                          2026-09-22 20:58:03 GMT+02:00
 *
 * A complete design report corresponding to this export is included at the end 
 * of this header file.
 *
 */

#ifndef SI5332_GM2_REVD_REG_CONFIG_HEADER
#define SI5332_GM2_REVD_REG_CONFIG_HEADER

#define SI5332_GM2_REVD_REG_CONFIG_NUM_REGS				61

typedef struct
{
	unsigned int address; /* 8-bit register address */
	unsigned char value; /* 8-bit register data */

} si5332_gm2_revd_register_t;

si5332_gm2_revd_register_t const si5332_gm2_revd_registers[SI5332_GM2_REVD_REG_CONFIG_NUM_REGS] =
{

	/* Start configuration preamble */
	/*    Set device in Ready mode */
	{ 0x0006, 0x01 },
	/* End configuration preamble */

	/* Start configuration registers */
	{ 0x0017, 0x4B },
	{ 0x0018, 0x52 },
	{ 0x0019, 0x49 },
	{ 0x001A, 0x41 },
	{ 0x001B, 0x42 },
	{ 0x001C, 0x42 },
	{ 0x0024, 0x01 },
	{ 0x0029, 0x00 },
	{ 0x002B, 0x5F },
	{ 0x003C, 0x00 },
	{ 0x0048, 0x00 },
	{ 0x0054, 0x00 },
	{ 0x0060, 0x00 },
	{ 0x0067, 0x2B },
	{ 0x0068, 0xFB },
	{ 0x0069, 0x00 },
	{ 0x006A, 0x07 },
	{ 0x006B, 0x00 },
	{ 0x006C, 0x1B },
	{ 0x0073, 0x00 },
	{ 0x0074, 0x00 },
	{ 0x0075, 0x01 },
	{ 0x007A, 0x07 },
	{ 0x007C, 0x00 },
	{ 0x007D, 0x00 },
	{ 0x007F, 0x07 },
	{ 0x0081, 0x00 },
	{ 0x0082, 0x00 },
	{ 0x0089, 0x07 },
	{ 0x008B, 0x00 },
	{ 0x008C, 0x00 },
	{ 0x008E, 0x07 },
	{ 0x0090, 0x00 },
	{ 0x0091, 0x00 },
	{ 0x0098, 0x07 },
	{ 0x009A, 0x00 },
	{ 0x009B, 0x00 },
	{ 0x009D, 0x07 },
	{ 0x009F, 0x00 },
	{ 0x00A0, 0x00 },
	{ 0x00A7, 0x03 },
	{ 0x00A8, 0x01 },
	{ 0x00A9, 0x00 },
	{ 0x00AA, 0x00 },
	{ 0x00AB, 0x00 },
	{ 0x00AC, 0x07 },
	{ 0x00AE, 0x00 },
	{ 0x00AF, 0x00 },
	{ 0x00B6, 0x00 },
	{ 0x00B7, 0x02 },
	{ 0x00B9, 0x06 },
	{ 0x00BA, 0x7E },
	{ 0x00BB, 0x2F },
	{ 0x00BC, 0xDB },
	{ 0x00BD, 0x04 },
	{ 0x00BE, 0x10 },
	{ 0x00BF, 0x00 },
	{ 0x00C0, 0x1B },
	{ 0x00C1, 0x1B },
	/* End configuration registers */

	/* Start configuration postamble */
	/*    Set device in Active mode */
	{ 0x0006, 0x02 },
	/* End configuration postamble */

};

/*
 * Design Report
 *
 * Overview
 * ========
 * 
 * Part:               Si5332ABCD-GM2 Rev D
 * Project File:       G:\Projekte\kria_bbrd\CBPro\Si5332-GM2-RevD-KRIABB-Project.slabtimeproj
 * Design ID:          KRIABB
 * Created By:         ClockBuilder Pro v4.17 [2025-11-06]
 * Timestamp:          2026-09-22 20:58:03 GMT+02:00
 * 
 * Design Rule Check
 * =================
 * 
 * Errors:
 * - No errors
 * 
 * Warnings:
 * - No warnings
 * 
 * Device Grade
 * ============
 * Maximum Output Frequency: 25 MHz
 * Frequency Synthesis Mode: Integer
 * Frequency Plan Grade:     D
 * Minimum Base OPN:         Si5332D*
 * 
 * Base       Output Clock         Supported Frequency Synthesis Modes
 * OPN Grade  Frequency Range      (Typical Jitter)
 * ---------  ------------------  --------------------------------------------
 * Si5332A    5 MHz to 334 MHz    Integer (~230 fs) and fractional (~500 fs)
 * Si5332B    5 MHz to 200 MHz    "
 * Si5332C    5 MHz to 334 MHz    Integer only (~230 fs)
 * Si5332D*   5 MHz to 200 MHz    "
 * 
 * * Based on your calculated frequency plan, a Si5332D grade device is
 * sufficient for your design. For more in-system configuration flexibility
 * (higher frequencies and/or to enable fractional synthesis), consider
 * selecting device grade Si5332A when specifying an ordering part number (OPN)
 * for your application. See the datasheet Ordering Guide for more information.
 * 
 * Design
 * ======
 * Base I2C Address: 0x6A
 * 
 * Universal Hardware Input Pins:
 *    INPUT1 (P10): None                          
 *    INPUT2 (P19): None                          
 *    INPUT3 (P20): None                          
 *    INPUT4 (P31): None                          
 *    INPUT5 (P32): None                          
 *    INPUT6 (P36): None                          
 *    INPUT7 (P37): None                          
 * 
 * Inputs:
 *    XAXB: 27 MHz
 *          Crystal Mode
 *          Adjusted Capacitance: 9 pF
 *  CLKIN2: Unused
 *  CLKIN3: Unused
 * 
 * Outputs:
 *    OUT0: Unused
 *    OUT1: Unused
 *    OUT2: Unused
 *    OUT3: Unused
 *    OUT4: Unused
 *    OUT5: Unused
 *    OUT6: 25 MHz LVCMOS (in-phase) 3.3 V 50 Ohms, Disabled-State: Stop Low
 *          Power-up state: Enabled
 *    OUT7: Unused
 * 
 * Frequency Plan
 * ==============
 * 
 * Fpfd = 27 MHz
 * Fvco = 2.375 GHz
 * 
 * P divider = 1
 * M = 87.9629629629629629... [ 87 + 26/27 ]
 * N dividers:
 *    N0:
 *       Unused
 *    N1:
 *       Unused
 * 
 * O dividers:
 *    O0:
 *       Value: 95
 *       OUT6: 25 MHz, Error: 0 ppm
 *    O1:
 *       Unused
 *    O2:
 *       Unused
 *    O3:
 *       Unused
 *    O4:
 *       Unused
 * 
 * R dividers:
 *    R0 = Unused
 *    R1 = Unused
 *    R2 = Unused
 *    R3 = Unused
 *    R4 = Unused
 *    R5 = Unused
 *    R6 = 1
 *    R7 = Unused
 * 
 * Estimated Power
 * ===============
 * Assumptions:
 * 
 * VDD:      1.8 V
 * Ta:       25 �C
 * Theta-JA: 16.50 �C/W (JEDEC Board with 2 m/s airflow)
 * 
 *                               Overall  On Chip
 * Condition                     Power    Power    Ta    Tj
 * ----------------------------  -------  -------  ----  ----
 * Typical Ta, Voltage, Current  70 mW    70 mW    25 C  26 C
 * 
 *                                      -----------------------
 *                                              Typical        
 *                                      -----------------------
 *                                      Voltage  Current  Power
 *           Output  Frequency  Format    (V)     (mA)    (mW) 
 *           ------  ---------  ------  -------  -------  -----
 * VDDA                                    1.80       19     35
 * VDD Dig                                 1.80        5      8
 * VDD Xtal                                1.80        2      4
 * VDDO0     OUT0       Unused        
 * VDDO1     OUT1       Unused        
 * VDDO2     OUT2       Unused        
 *           OUT3       Unused        
 * VDDO3     OUT4       Unused        
 *           OUT5       Unused        
 * VDDO4     OUT6       25 MHz  LVCMOS (in-phase)     3.30        7     22
 * VDDO5     OUT7       Unused        
 *                                      -------  -------  -----
 *                                         1.80       26     47
 *                                         3.30        7     22
 *                                      -------  -------  -----
 *                                        Total              70
 *                                      -------  -------  -----
 * 
 * Note:
 * 
 * - Tj is junction temperature. Tj must be less than 125 �C (on Si5332-GM2
 *   Revision D) for device to comply with datasheet specifications. Tj = Ta +
 *   Theta_JA*On_Chip_Power.
 * - Overall power includes on-chip power dissipation and adds differential load
 *   power dissipation to estimate total power requirements.
 * - Above are estimates only: power and temperature should be measured on your
 *   PCB.
 * 
 * Settings
 * ========
 * 
 * Location    Setting Name     Decimal Value      Hex Value        
 * ----------  ---------------  -----------------  -----------------
 * 0x17[7:0]   DESIGN_ID0       75                 0x4B             
 * 0x18[7:0]   DESIGN_ID1       82                 0x52             
 * 0x19[7:0]   DESIGN_ID2       73                 0x49             
 * 0x1A[7:0]   DESIGN_ID3       65                 0x41             
 * 0x1B[7:0]   DESIGN_ID4       66                 0x42             
 * 0x1C[7:0]   DESIGN_ID5       66                 0x42             
 * 0x24[1:0]   IMUX_SEL         1                  0x1              
 * 0x29[1:0]   OMUX4_SEL0       0                  0x0              
 * 0x29[6:4]   OMUX4_SEL1       0                  0x0              
 * 0x2B[7:0]   HSDIV0A_DIV      95                 0x5F             
 * 0x3C[0]     ID0A_SS_ENA      0                  0x0              
 * 0x3C[2:1]   ID0A_SS_MODE     0                  0x0              
 * 0x48[0]     ID0B_SS_ENA      0                  0x0              
 * 0x48[2:1]   ID0B_SS_MODE     0                  0x0              
 * 0x54[0]     ID1A_SS_ENA      0                  0x0              
 * 0x54[2:1]   ID1A_SS_MODE     0                  0x0              
 * 0x60[0]     ID1B_SS_ENA      0                  0x0              
 * 0x60[2:1]   ID1B_SS_MODE     0                  0x0              
 * 0x67[14:0]  IDPA_INTG        11259              0x2BFB           
 * 0x69[14:0]  IDPA_RES         7                  0x0007           
 * 0x6B[14:0]  IDPA_DEN         27                 0x001B           
 * 0x73[1:0]   CLKIN_2_CLK_SEL  0                  0x0              
 * 0x74[1:0]   CLKIN_3_CLK_SEL  0                  0x0              
 * 0x75[4:0]   P_VAL            1                  0x01             
 * 0x7A[3:0]   OUT0_MODE        7                  0x7              
 * 0x7C[2:0]   OUT0_SKEW        0                  0x0              
 * 0x7D[0]     OUT0_STOP_HIGHZ  0                  0x0              
 * 0x7D[5:4]   OUT0_CMOS_INV    0                  0x0              
 * 0x7D[6]     OUT0_DIFF_INV    0                  0x0              
 * 0x7F[3:0]   OUT1_MODE        7                  0x7              
 * 0x81[2:0]   OUT1_SKEW        0                  0x0              
 * 0x82[0]     OUT1_STOP_HIGHZ  0                  0x0              
 * 0x82[5:4]   OUT1_CMOS_INV    0                  0x0              
 * 0x82[6]     OUT1_DIFF_INV    0                  0x0              
 * 0x89[3:0]   OUT2_MODE        7                  0x7              
 * 0x8B[2:0]   OUT2_SKEW        0                  0x0              
 * 0x8C[0]     OUT2_STOP_HIGHZ  0                  0x0              
 * 0x8C[5:4]   OUT2_CMOS_INV    0                  0x0              
 * 0x8C[6]     OUT2_DIFF_INV    0                  0x0              
 * 0x8E[3:0]   OUT3_MODE        7                  0x7              
 * 0x90[2:0]   OUT3_SKEW        0                  0x0              
 * 0x91[0]     OUT3_STOP_HIGHZ  0                  0x0              
 * 0x91[5:4]   OUT3_CMOS_INV    0                  0x0              
 * 0x91[6]     OUT3_DIFF_INV    0                  0x0              
 * 0x98[3:0]   OUT4_MODE        7                  0x7              
 * 0x9A[2:0]   OUT4_SKEW        0                  0x0              
 * 0x9B[0]     OUT4_STOP_HIGHZ  0                  0x0              
 * 0x9B[5:4]   OUT4_CMOS_INV    0                  0x0              
 * 0x9B[6]     OUT4_DIFF_INV    0                  0x0              
 * 0x9D[3:0]   OUT5_MODE        7                  0x7              
 * 0x9F[2:0]   OUT5_SKEW        0                  0x0              
 * 0xA0[0]     OUT5_STOP_HIGHZ  0                  0x0              
 * 0xA0[5:4]   OUT5_CMOS_INV    0                  0x0              
 * 0xA0[6]     OUT5_DIFF_INV    0                  0x0              
 * 0xA7[3:0]   OUT6_MODE        3                  0x3              
 * 0xA8[5:0]   OUT6_DIV         1                  0x01             
 * 0xA9[2:0]   OUT6_SKEW        0                  0x0              
 * 0xAA[0]     OUT6_STOP_HIGHZ  0                  0x0              
 * 0xAA[5:4]   OUT6_CMOS_INV    0                  0x0              
 * 0xAA[6]     OUT6_DIFF_INV    0                  0x0              
 * 0xAB[2]     OUT6_CMOS_STR    0                  0x0              
 * 0xAB[1:0]   OUT6_CMOS_SLEW   0                  0x0              
 * 0xAC[3:0]   OUT7_MODE        7                  0x7              
 * 0xAE[2:0]   OUT7_SKEW        0                  0x0              
 * 0xAF[0]     OUT7_STOP_HIGHZ  0                  0x0              
 * 0xAF[5:4]   OUT7_CMOS_INV    0                  0x0              
 * 0xAF[6]     OUT7_DIFF_INV    0                  0x0              
 * 0xB6[0]     OUT0_OE          0                  0x0              
 * 0xB6[1]     OUT1_OE          0                  0x0              
 * 0xB6[3]     OUT2_OE          0                  0x0              
 * 0xB6[4]     OUT3_OE          0                  0x0              
 * 0xB6[6]     OUT4_OE          0                  0x0              
 * 0xB6[7]     OUT5_OE          0                  0x0              
 * 0xB7[1]     OUT6_OE          1                  0x1              
 * 0xB7[2]     OUT7_OE          0                  0x0              
 * 0xB9[0]     XOSC_DIS         0                  0x0              
 * 0xB9[1]     IBUF0_DIS        1                  0x1              
 * 0xB9[2]     IBUF1_DIS        1                  0x1              
 * 0xB9[3]     IMUX_DIS         0                  0x0              
 * 0xB9[4]     PDIV_DIS         0                  0x0              
 * 0xB9[5]     PLL_DIS          0                  0x0              
 * 0xBA[5]     ID0_DIS          1                  0x1              
 * 0xBA[6]     ID1_DIS          1                  0x1              
 * 0xBA[0]     HSDIV0_DIS       0                  0x0              
 * 0xBA[1]     HSDIV1_DIS       1                  0x1              
 * 0xBA[2]     HSDIV2_DIS       1                  0x1              
 * 0xBA[3]     HSDIV3_DIS       1                  0x1              
 * 0xBA[4]     HSDIV4_DIS       1                  0x1              
 * 0xBB[0]     OMUX0_DIS        1                  0x1              
 * 0xBB[1]     OMUX1_DIS        1                  0x1              
 * 0xBB[2]     OMUX2_DIS        1                  0x1              
 * 0xBB[3]     OMUX3_DIS        1                  0x1              
 * 0xBB[4]     OMUX4_DIS        0                  0x0              
 * 0xBB[5]     OMUX5_DIS        1                  0x1              
 * 0xBC[0]     OUT0_DIS         1                  0x1              
 * 0xBC[1]     OUT1_DIS         1                  0x1              
 * 0xBC[3]     OUT2_DIS         1                  0x1              
 * 0xBC[4]     OUT3_DIS         1                  0x1              
 * 0xBC[6]     OUT4_DIS         1                  0x1              
 * 0xBC[7]     OUT5_DIS         1                  0x1              
 * 0xBD[1]     OUT6_DIS         0                  0x0              
 * 0xBD[2]     OUT7_DIS         1                  0x1              
 * 0xBE[7:0]   PLL_MODE         16                 0x10             
 * 0xBF[0]     XOSC_CINT_ENA    0                  0x0              
 * 0xC0[5:0]   XOSC_CTRIM_XIN   27                 0x1B             
 * 0xC1[5:0]   XOSC_CTRIM_XOUT  27                 0x1B
 * 
 *
 */

#endif
