
build/bootloader.elf:     file format elf32-littleriscv


Disassembly of section .start:

f9000000 <_start>:
f9000000:	00014197          	auipc	gp,0x14
f9000004:	6c818193          	addi	gp,gp,1736 # f90146c8 <__global_pointer$>
f9000008:	00000597          	auipc	a1,0x0
f900000c:	0a05ae23          	sw	zero,188(a1) # f90000c4 <smp_lottery_lock>

f9000010 <smp_tyranny>:
f9000010:	f1402573          	csrr	a0,mhartid
f9000014:	04050663          	beqz	a0,f9000060 <init>

f9000018 <smp_slave>:
f9000018:	00000517          	auipc	a0,0x0
f900001c:	0ac52503          	lw	a0,172(a0) # f90000c4 <smp_lottery_lock>
f9000020:	fe050ce3          	beqz	a0,f9000018 <smp_slave>
f9000024:	0220000f          	fence	r,r
f9000028:	0000100f          	.word	0x0000100f
f900002c:	00000797          	auipc	a5,0x0
f9000030:	0947a783          	lw	a5,148(a5) # f90000c0 <smp_lottery_target>
f9000034:	00000513          	li	a0,0
f9000038:	00000593          	li	a1,0
f900003c:	00000613          	li	a2,0
f9000040:	00078067          	jr	a5

f9000044 <smp_unlock>:
f9000044:	00000597          	auipc	a1,0x0
f9000048:	06a5ae23          	sw	a0,124(a1) # f90000c0 <smp_lottery_target>
f900004c:	0110000f          	fence	w,w
f9000050:	00100513          	li	a0,1
f9000054:	00000597          	auipc	a1,0x0
f9000058:	06a5a823          	sw	a0,112(a1) # f90000c4 <smp_lottery_lock>
f900005c:	00008067          	ret

f9000060 <init>:
f9000060:	00014117          	auipc	sp,0x14
f9000064:	f0410113          	addi	sp,sp,-252 # f9013f64 <_sp>
f9000068:	81018513          	addi	a0,gp,-2032 # f9013ed8 <_data>
f900006c:	81018593          	addi	a1,gp,-2032 # f9013ed8 <_data>
f9000070:	00014617          	auipc	a2,0x14
f9000074:	e7460613          	addi	a2,a2,-396 # f9013ee4 <__bss_start>
f9000078:	00c5fc63          	bgeu	a1,a2,f9000090 <init+0x30>
f900007c:	00052283          	lw	t0,0(a0)
f9000080:	0055a023          	sw	t0,0(a1)
f9000084:	00450513          	addi	a0,a0,4
f9000088:	00458593          	addi	a1,a1,4
f900008c:	fec5e8e3          	bltu	a1,a2,f900007c <init+0x1c>
f9000090:	00014517          	auipc	a0,0x14
f9000094:	e5450513          	addi	a0,a0,-428 # f9013ee4 <__bss_start>
f9000098:	00014597          	auipc	a1,0x14
f900009c:	e5058593          	addi	a1,a1,-432 # f9013ee8 <_end>
f90000a0:	00b57863          	bgeu	a0,a1,f90000b0 <init+0x50>
f90000a4:	00052023          	sw	zero,0(a0)
f90000a8:	00450513          	addi	a0,a0,4
f90000ac:	feb56ce3          	bltu	a0,a1,f90000a4 <init+0x44>
f90000b0:	5a5130ef          	jal	f9013e54 <__libc_init_array>
f90000b4:	585130ef          	jal	f9013e38 <main>

f90000b8 <mainDone>:
f90000b8:	0000006f          	j	f90000b8 <mainDone>

f90000bc <_init>:
f90000bc:	00008067          	ret

f90000c0 <smp_lottery_target>:
f90000c0:	0000                	.insn	2, 0x0000
	...

f90000c4 <smp_lottery_lock>:
f90000c4:	0000                	.insn	2, 0x0000
f90000c6:	0000                	.insn	2, 0x0000
f90000c8:	011e                	.insn	2, 0x011e
f90000ca:	0000                	.insn	2, 0x0000
f90000cc:	0005                	.insn	2, 0x0005
f90000ce:	0004                	.insn	2, 0x0004
f90000d0:	002e                	.insn	2, 0x002e
f90000d2:	0000                	.insn	2, 0x0000
f90000d4:	0101                	.insn	2, 0x0101
f90000d6:	fb01                	.insn	2, 0xfb01
f90000d8:	0d0e                	.insn	2, 0x0d0e
f90000da:	0100                	.insn	2, 0x0100
f90000dc:	0101                	.insn	2, 0x0101
f90000de:	0001                	.insn	2, 0x0001
f90000e0:	0000                	.insn	2, 0x0000
f90000e2:	0001                	.insn	2, 0x0001
f90000e4:	0100                	.insn	2, 0x0100
f90000e6:	0101                	.insn	2, 0x0101
f90000e8:	021f 01ea f900      	.insn	6, 0xf90001ea021f
f90000ee:	0240                	.insn	2, 0x0240
f90000f0:	f900                	.insn	2, 0xf900
f90000f2:	0102                	.insn	2, 0x0102
f90000f4:	021f 020f 0254      	.insn	6, 0x0254020f021f
f90000fa:	f900                	.insn	2, 0xf900
f90000fc:	5401                	.insn	2, 0x5401
f90000fe:	0002                	.insn	2, 0x0002
f9000100:	01f9                	.insn	2, 0x01f9
f9000102:	0500                	.insn	2, 0x0500
f9000104:	0002                	.insn	2, 0x0002
f9000106:	0000                	.insn	2, 0x0000
f9000108:	03f9                	.insn	2, 0x03f9
f900010a:	0109                	.insn	2, 0x0109
f900010c:	08090a03          	lb	s4,128(s2)
f9000110:	0100                	.insn	2, 0x0100
f9000112:	08090303          	lb	t1,128(s2)
f9000116:	0100                	.insn	2, 0x0100
f9000118:	04090103          	lb	sp,64(s2)
f900011c:	0100                	.insn	2, 0x0100
f900011e:	04090303          	lb	t1,64(s2)
f9000122:	0100                	.insn	2, 0x0100
f9000124:	08090103          	lb	sp,128(s2)
f9000128:	0100                	.insn	2, 0x0100
f900012a:	04090203          	lb	tp,64(s2)
f900012e:	0100                	.insn	2, 0x0100
f9000130:	08090503          	lb	a0,128(s2)
f9000134:	0100                	.insn	2, 0x0100
f9000136:	08090103          	lb	sp,128(s2)
f900013a:	0100                	.insn	2, 0x0100
f900013c:	04090103          	lb	sp,64(s2)
f9000140:	0100                	.insn	2, 0x0100
f9000142:	04090103          	lb	sp,64(s2)
f9000146:	0100                	.insn	2, 0x0100
f9000148:	04090103          	lb	sp,64(s2)
f900014c:	0100                	.insn	2, 0x0100
f900014e:	04090503          	lb	a0,64(s2)
f9000152:	0100                	.insn	2, 0x0100
f9000154:	08090103          	lb	sp,128(s2)
f9000158:	0100                	.insn	2, 0x0100
f900015a:	04090103          	lb	sp,64(s2)
f900015e:	0100                	.insn	2, 0x0100
f9000160:	04090103          	lb	sp,64(s2)
f9000164:	0100                	.insn	2, 0x0100
f9000166:	08090103          	lb	sp,128(s2)
f900016a:	0100                	.insn	2, 0x0100
f900016c:	04090403          	lb	s0,64(s2)
f9000170:	0100                	.insn	2, 0x0100
f9000172:	08090303          	lb	t1,128(s2)
f9000176:	0100                	.insn	2, 0x0100
f9000178:	04090103          	lb	sp,64(s2)
f900017c:	0100                	.insn	2, 0x0100
f900017e:	04090103          	lb	sp,64(s2)
f9000182:	0100                	.insn	2, 0x0100
f9000184:	08090103          	lb	sp,128(s2)
f9000188:	0100                	.insn	2, 0x0100
f900018a:	04090203          	lb	tp,64(s2)
f900018e:	0100                	.insn	2, 0x0100
f9000190:	04090103          	lb	sp,64(s2)
f9000194:	0100                	.insn	2, 0x0100
f9000196:	04090103          	lb	sp,64(s2)
f900019a:	0100                	.insn	2, 0x0100
f900019c:	04090103          	lb	sp,64(s2)
f90001a0:	0100                	.insn	2, 0x0100
f90001a2:	04090103          	lb	sp,64(s2)
f90001a6:	0100                	.insn	2, 0x0100
f90001a8:	04090403          	lb	s0,64(s2)
f90001ac:	0100                	.insn	2, 0x0100
f90001ae:	08090103          	lb	sp,128(s2)
f90001b2:	0100                	.insn	2, 0x0100
f90001b4:	08090103          	lb	sp,128(s2)
f90001b8:	0100                	.insn	2, 0x0100
f90001ba:	04090203          	lb	tp,64(s2)
f90001be:	0100                	.insn	2, 0x0100
f90001c0:	04090103          	lb	sp,64(s2)
f90001c4:	0100                	.insn	2, 0x0100
f90001c6:	04090103          	lb	sp,64(s2)
f90001ca:	0100                	.insn	2, 0x0100
f90001cc:	04090403          	lb	s0,64(s2)
f90001d0:	0100                	.insn	2, 0x0100
f90001d2:	04090303          	lb	t1,64(s2)
f90001d6:	0100                	.insn	2, 0x0100
f90001d8:	04090203          	lb	tp,64(s2)
f90001dc:	0100                	.insn	2, 0x0100
f90001de:	04090503          	lb	a0,64(s2)
f90001e2:	0100                	.insn	2, 0x0100
f90001e4:	0409                	.insn	2, 0x0409
f90001e6:	0000                	.insn	2, 0x0000
f90001e8:	0101                	.insn	2, 0x0101
f90001ea:	6d6f682f          	.insn	4, 0x6d6f682f
f90001ee:	2f65                	.insn	2, 0x2f65
f90001f0:	616d                	.insn	2, 0x616d
f90001f2:	7474                	.insn	2, 0x7474
f90001f4:	6968                	.insn	2, 0x6968
f90001f6:	7361                	.insn	2, 0x7361
f90001f8:	7469672f          	.insn	4, 0x7469672f
f90001fc:	6966652f          	.insn	4, 0x6966652f
f9000200:	696e                	.insn	2, 0x696e
f9000202:	2f78                	.insn	2, 0x2f78
f9000204:	43737973          	csrrci	s2,0x437,6
f9000208:	746e                	.insn	2, 0x746e
f900020a:	6c72                	.insn	2, 0x6c72
f900020c:	2f77732f          	.insn	4, 0x2f77732f
f9000210:	6d65                	.insn	2, 0x6d65
f9000212:	6562                	.insn	2, 0x6562
f9000214:	6464                	.insn	2, 0x6464
f9000216:	6465                	.insn	2, 0x6465
f9000218:	735f 2f77 636d      	.insn	6, 0x636d2f77735f
f900021e:	2f75                	.insn	2, 0x2f75
f9000220:	74666f73          	csrrsi	t5,0x746,12
f9000224:	65726177          	.insn	4, 0x65726177
f9000228:	6174732f          	.insn	4, 0x6174732f
f900022c:	646e                	.insn	2, 0x646e
f900022e:	6c61                	.insn	2, 0x6c61
f9000230:	2f656e6f          	jal	t3,f9056526 <__global_pointer$+0x41e5e>
f9000234:	6f62                	.insn	2, 0x6f62
f9000236:	6f6c746f          	jal	s0,f90c792c <__global_pointer$+0xb3264>
f900023a:	6461                	.insn	2, 0x6461
f900023c:	7265                	.insn	2, 0x7265
f900023e:	2e2e002f          	.insn	4, 0x2e2e002f
f9000242:	6d6f632f          	.insn	4, 0x6d6f632f
f9000246:	6f6d                	.insn	2, 0x6f6d
f9000248:	006e                	.insn	2, 0x006e
f900024a:	2e2e                	.insn	2, 0x2e2e
f900024c:	6d6f632f          	.insn	4, 0x6d6f632f
f9000250:	6f6d                	.insn	2, 0x6f6d
f9000252:	2f6e                	.insn	2, 0x2f6e
f9000254:	72617473          	csrrci	s0,mhpmevent6h,2
f9000258:	2e74                	.insn	2, 0x2e74
f900025a:	4e470053          	.insn	4, 0x4e470053
f900025e:	2055                	.insn	2, 0x2055
f9000260:	5341                	.insn	2, 0x5341
f9000262:	3220                	.insn	2, 0x3220
f9000264:	342e                	.insn	2, 0x342e
f9000266:	0035                	.insn	2, 0x0035
f9000268:	0021                	.insn	2, 0x0021
f900026a:	0000                	.insn	2, 0x0000
f900026c:	0005                	.insn	2, 0x0005
f900026e:	0401                	.insn	2, 0x0401
f9000270:	028d                	.insn	2, 0x028d
f9000272:	f900                	.insn	2, 0xf900
f9000274:	c801                	.insn	2, 0xc801
f9000276:	0000                	.insn	2, 0x0000
f9000278:	00f9                	.insn	2, 0x00f9
f900027a:	0000                	.insn	2, 0x0000
f900027c:	d0f9                	.insn	2, 0xd0f9
f900027e:	4a01                	.insn	2, 0x4a01
f9000280:	0002                	.insn	2, 0x0002
f9000282:	eaf9                	.insn	2, 0xeaf9
f9000284:	0001                	.insn	2, 0x0001
f9000286:	5cf9                	.insn	2, 0x5cf9
f9000288:	0002                	.insn	2, 0x0002
f900028a:	01f9                	.insn	2, 0x01f9
f900028c:	0180                	.insn	2, 0x0180
f900028e:	0011                	.insn	2, 0x0011
f9000290:	1710                	.insn	2, 0x1710
f9000292:	0111                	.insn	2, 0x0111
f9000294:	0f12                	.insn	2, 0x0f12
f9000296:	0e1b0e03          	lb	t3,225(s6)
f900029a:	0e25                	.insn	2, 0x0e25
f900029c:	00000513          	li	a0,0
	...
f90002a8:	001c                	.insn	2, 0x001c
f90002aa:	0000                	.insn	2, 0x0000
f90002ac:	0002                	.insn	2, 0x0002
f90002ae:	0268                	.insn	2, 0x0268
f90002b0:	f900                	.insn	2, 0xf900
f90002b2:	0004                	.insn	2, 0x0004
f90002b4:	0000                	.insn	2, 0x0000
f90002b6:	0000                	.insn	2, 0x0000
f90002b8:	0000                	.insn	2, 0x0000
f90002ba:	f900                	.insn	2, 0xf900
f90002bc:	00c0                	.insn	2, 0x00c0
	...
f90002c6:	0000                	.insn	2, 0x0000
f90002c8:	4041                	.insn	2, 0x4041
f90002ca:	0000                	.insn	2, 0x0000
f90002cc:	7200                	.insn	2, 0x7200
f90002ce:	7369                	.insn	2, 0x7369
f90002d0:	01007663          	bgeu	zero,a6,f90002dc <smp_lottery_lock+0x218>
f90002d4:	0036                	.insn	2, 0x0036
f90002d6:	0000                	.insn	2, 0x0000
f90002d8:	7205                	.insn	2, 0x7205
f90002da:	3376                	.insn	2, 0x3376
f90002dc:	6932                	.insn	2, 0x6932
f90002de:	7032                	.insn	2, 0x7032
f90002e0:	5f31                	.insn	2, 0x5f31
f90002e2:	326d                	.insn	2, 0x326d
f90002e4:	3070                	.insn	2, 0x3070
f90002e6:	7a5f 6369 7273      	.insn	6, 0x727363697a5f
f90002ec:	7032                	.insn	2, 0x7032
f90002ee:	5f30                	.insn	2, 0x5f30
f90002f0:	697a                	.insn	2, 0x697a
f90002f2:	6566                	.insn	2, 0x6566
f90002f4:	636e                	.insn	2, 0x636e
f90002f6:	6965                	.insn	2, 0x6965
f90002f8:	7032                	.insn	2, 0x7032
f90002fa:	5f30                	.insn	2, 0x5f30
f90002fc:	6d7a                	.insn	2, 0x6d7a
f90002fe:	756d                	.insn	2, 0x756d
f9000300:	316c                	.insn	2, 0x316c
f9000302:	3070                	.insn	2, 0x3070
f9000304:	0800                	.insn	2, 0x0800
f9000306:	0a01                	.insn	2, 0x0a01
f9000308:	0b                	.byte	0x0b

Disassembly of section .text.uart_applyConfig:

f9013800 <uart_applyConfig>:
*          value using data length, parity, and stop bit settings from the configuration
*          structure, and writes this value to the UART frame configuration register.
*
******************************************************************************/
    static void uart_applyConfig(u32 reg, Uart_Config *config){
        write_u32(config->clockDivider, reg + UART_CLOCK_DIVIDER);
f9013800:	00c5a783          	lw	a5,12(a1)
    static inline u32 read_u32(u32 address){
        return *((volatile u32*) address);
    }
    
    static inline void write_u32(u32 data, u32 address){
        *((volatile u32*) address) = data;
f9013804:	00f52423          	sw	a5,8(a0)
        write_u32(((config->dataLength-1) << 0) | (config->parity << 8) | (config->stop << 16), reg + UART_FRAME_CONFIG);
f9013808:	0005a783          	lw	a5,0(a1)
f901380c:	fff78793          	addi	a5,a5,-1
f9013810:	0045a703          	lw	a4,4(a1)
f9013814:	00871713          	slli	a4,a4,0x8
f9013818:	00e7e7b3          	or	a5,a5,a4
f901381c:	0085a703          	lw	a4,8(a1)
f9013820:	01071713          	slli	a4,a4,0x10
f9013824:	00e7e7b3          	or	a5,a5,a4
f9013828:	00f52623          	sw	a5,12(a0)
    }
f901382c:	00008067          	ret

Disassembly of section .text.clint_uDelay:

f9013830 <clint_uDelay>:
*          and the time limit is non-negative, indicating that the delay has
*          not yet elapsed.
*
******************************************************************************/
    static void clint_uDelay(u32 usec, u32 hz, u32 reg){
        u32 mTimePerUsec = hz/1000000;
f9013830:	000f47b7          	lui	a5,0xf4
f9013834:	24078793          	addi	a5,a5,576 # f4240 <__stack_size+0xf41c0>
f9013838:	02f5d5b3          	divu	a1,a1,a5
    readReg_u32 (clint_getTimeLow , CLINT_TIME_ADDR)
f901383c:	0000c7b7          	lui	a5,0xc
f9013840:	ff878793          	addi	a5,a5,-8 # bff8 <__stack_size+0xbf78>
f9013844:	00f60633          	add	a2,a2,a5
        return *((volatile u32*) address);
f9013848:	00062783          	lw	a5,0(a2)
        u32 limit = clint_getTimeLow(reg) + usec*mTimePerUsec;
f901384c:	02a585b3          	mul	a1,a1,a0
f9013850:	00f58733          	add	a4,a1,a5
f9013854:	00062783          	lw	a5,0(a2)
        while((int32_t)(limit-(clint_getTimeLow(reg))) >= 0);
f9013858:	40f707b3          	sub	a5,a4,a5
f901385c:	fe07dce3          	bgez	a5,f9013854 <clint_uDelay+0x24>
f9013860:	00008067          	ret

Disassembly of section .text.bsp_init:

f9013864 <bsp_init>:
    *   1. UART baudrate
    *   2. 
    */
////////////////////////////////////////////////////////////////////////////////
    static void bsp_init()
    {
f9013864:	fe010113          	addi	sp,sp,-32
f9013868:	00112e23          	sw	ra,28(sp)
        Uart_Config uartConfig;
        uartConfig.dataLength   = BITS_8;
f901386c:	00800793          	li	a5,8
f9013870:	00f12023          	sw	a5,0(sp)
        uartConfig.parity       = NONE;
f9013874:	00012223          	sw	zero,4(sp)
        uartConfig.stop         = ONE;
f9013878:	00012423          	sw	zero,8(sp)
        uartConfig.clockDivider = BSP_CLINT_HZ/(BSP_UART_BAUDRATE*BSP_UART_DATA_LEN)-1;
f901387c:	05500793          	li	a5,85
f9013880:	00f12623          	sw	a5,12(sp)
        uart_applyConfig(BSP_UART_TERMINAL, &uartConfig);    
f9013884:	00010593          	mv	a1,sp
f9013888:	f8010537          	lui	a0,0xf8010
f901388c:	f75ff0ef          	jal	f9013800 <uart_applyConfig>
    }
f9013890:	01c12083          	lw	ra,28(sp)
f9013894:	02010113          	addi	sp,sp,32
f9013898:	00008067          	ret

Disassembly of section .text.spi_cmdAvailability:

f901389c <spi_cmdAvailability>:
f901389c:	00452503          	lw	a0,4(a0) # f8010004 <__stack_size+0xf800ff84>
 * @return The availability of command buffer space (lower 16 bits of SPI buffer)
 *
 ******************************************************************************/
    static u32 spi_cmdAvailability(u32 reg){
        return read_u32(reg + SPI_BUFFER) & 0xFFFF;
    }
f90138a0:	01051513          	slli	a0,a0,0x10
f90138a4:	01055513          	srli	a0,a0,0x10
f90138a8:	00008067          	ret

Disassembly of section .text.spi_rspOccupancy:

f90138ac <spi_rspOccupancy>:
f90138ac:	00452503          	lw	a0,4(a0)
 * @return The occupancy of response buffer space (upper 16 bits of SPI buffer)
 *
 ******************************************************************************/
    static u32 spi_rspOccupancy(u32 reg){
        return read_u32(reg + SPI_BUFFER) >> 16;
    }
f90138b0:	01055513          	srli	a0,a0,0x10
f90138b4:	00008067          	ret

Disassembly of section .text.spi_write:

f90138b8 <spi_write>:
 * @param reg The base address of the SPI register
 *
 * @param data The data to be written
 *
 ******************************************************************************/
    static void spi_write(u32 reg, u8 data){
f90138b8:	ff010113          	addi	sp,sp,-16
f90138bc:	00112623          	sw	ra,12(sp)
f90138c0:	00812423          	sw	s0,8(sp)
f90138c4:	00912223          	sw	s1,4(sp)
f90138c8:	00050413          	mv	s0,a0
f90138cc:	00058493          	mv	s1,a1
        while(spi_cmdAvailability(reg) == 0);
f90138d0:	00040513          	mv	a0,s0
f90138d4:	fc9ff0ef          	jal	f901389c <spi_cmdAvailability>
f90138d8:	fe050ce3          	beqz	a0,f90138d0 <spi_write+0x18>
        write_u32(data | SPI_CMD_WRITE, reg + SPI_DATA);
f90138dc:	1004e493          	ori	s1,s1,256
        *((volatile u32*) address) = data;
f90138e0:	00942023          	sw	s1,0(s0)
    }
f90138e4:	00c12083          	lw	ra,12(sp)
f90138e8:	00812403          	lw	s0,8(sp)
f90138ec:	00412483          	lw	s1,4(sp)
f90138f0:	01010113          	addi	sp,sp,16
f90138f4:	00008067          	ret

Disassembly of section .text.spi_read:

f90138f8 <spi_read>:
 * @param reg The base address of the SPI register
 *
 * @return The data read from the SPI data register
 *
 ******************************************************************************/   
    static u8 spi_read(u32 reg){
f90138f8:	ff010113          	addi	sp,sp,-16
f90138fc:	00112623          	sw	ra,12(sp)
f9013900:	00812423          	sw	s0,8(sp)
f9013904:	00050413          	mv	s0,a0
        while(spi_cmdAvailability(reg) == 0);
f9013908:	00040513          	mv	a0,s0
f901390c:	f91ff0ef          	jal	f901389c <spi_cmdAvailability>
f9013910:	fe050ce3          	beqz	a0,f9013908 <spi_read+0x10>
f9013914:	20000793          	li	a5,512
f9013918:	00f42023          	sw	a5,0(s0)
        write_u32(SPI_CMD_READ, reg + SPI_DATA);
        while(spi_rspOccupancy(reg) == 0);
f901391c:	00040513          	mv	a0,s0
f9013920:	f8dff0ef          	jal	f90138ac <spi_rspOccupancy>
f9013924:	fe050ce3          	beqz	a0,f901391c <spi_read+0x24>
        return *((volatile u32*) address);
f9013928:	00042503          	lw	a0,0(s0)
        return read_u32(reg + SPI_DATA);
    }
f901392c:	0ff57513          	zext.b	a0,a0
f9013930:	00c12083          	lw	ra,12(sp)
f9013934:	00812403          	lw	s0,8(sp)
f9013938:	01010113          	addi	sp,sp,16
f901393c:	00008067          	ret

Disassembly of section .text.spi_select:

f9013940 <spi_select>:
 *
 * @param reg The base address of the SPI register
 * @param slaveId The ID of the slave device to select
 *
 ******************************************************************************/
    static void spi_select(u32 reg, u32 slaveId){
f9013940:	ff010113          	addi	sp,sp,-16
f9013944:	00112623          	sw	ra,12(sp)
f9013948:	00812423          	sw	s0,8(sp)
f901394c:	00912223          	sw	s1,4(sp)
f9013950:	00050413          	mv	s0,a0
f9013954:	00058493          	mv	s1,a1
        while(spi_cmdAvailability(reg) == 0);
f9013958:	00040513          	mv	a0,s0
f901395c:	f41ff0ef          	jal	f901389c <spi_cmdAvailability>
f9013960:	fe050ce3          	beqz	a0,f9013958 <spi_select+0x18>
        write_u32(slaveId | 0x80 | SPI_CMD_SS, reg + SPI_DATA);
f9013964:	000017b7          	lui	a5,0x1
f9013968:	88078793          	addi	a5,a5,-1920 # 880 <__stack_size+0x800>
f901396c:	00f4e4b3          	or	s1,s1,a5
        *((volatile u32*) address) = data;
f9013970:	00942023          	sw	s1,0(s0)
    }
f9013974:	00c12083          	lw	ra,12(sp)
f9013978:	00812403          	lw	s0,8(sp)
f901397c:	00412483          	lw	s1,4(sp)
f9013980:	01010113          	addi	sp,sp,16
f9013984:	00008067          	ret

Disassembly of section .text.spi_diselect:

f9013988 <spi_diselect>:
 *
 * @param reg The base address of the SPI register
 * @param slaveId The ID of the slave device to deselect
 *
 ******************************************************************************/  
    static void spi_diselect(u32 reg, u32 slaveId){
f9013988:	ff010113          	addi	sp,sp,-16
f901398c:	00112623          	sw	ra,12(sp)
f9013990:	00812423          	sw	s0,8(sp)
f9013994:	00912223          	sw	s1,4(sp)
f9013998:	00050413          	mv	s0,a0
f901399c:	00058493          	mv	s1,a1
        while(spi_cmdAvailability(reg) == 0);
f90139a0:	00040513          	mv	a0,s0
f90139a4:	ef9ff0ef          	jal	f901389c <spi_cmdAvailability>
f90139a8:	fe050ce3          	beqz	a0,f90139a0 <spi_diselect+0x18>
        write_u32(slaveId | 0x00 | SPI_CMD_SS, reg + SPI_DATA);
f90139ac:	000017b7          	lui	a5,0x1
f90139b0:	80078793          	addi	a5,a5,-2048 # 800 <__stack_size+0x780>
f90139b4:	00f4e4b3          	or	s1,s1,a5
f90139b8:	00942023          	sw	s1,0(s0)
    }
f90139bc:	00c12083          	lw	ra,12(sp)
f90139c0:	00812403          	lw	s0,8(sp)
f90139c4:	00412483          	lw	s1,4(sp)
f90139c8:	01010113          	addi	sp,sp,16
f90139cc:	00008067          	ret

Disassembly of section .text.spi_applyConfig:

f90139d0 <spi_applyConfig>:
 * @param reg The base address of the SPI register
 * @param config Pointer to a Spi_Config structure containing the configuration settings
 *
 ******************************************************************************/
    static void spi_applyConfig(u32 reg, Spi_Config *config){
        write_u32((config->cpol << 0) | (config->cpha << 1) | (config->mode << 4), reg + SPI_CONFIG);
f90139d0:	0005a783          	lw	a5,0(a1)
f90139d4:	0045a703          	lw	a4,4(a1)
f90139d8:	00171713          	slli	a4,a4,0x1
f90139dc:	00e7e7b3          	or	a5,a5,a4
f90139e0:	0085a703          	lw	a4,8(a1)
f90139e4:	00471713          	slli	a4,a4,0x4
f90139e8:	00e7e7b3          	or	a5,a5,a4
f90139ec:	00f52423          	sw	a5,8(a0)
        write_u32(config->clkDivider, reg + SPI_CLK_DIVIDER);
f90139f0:	00c5a783          	lw	a5,12(a1)
f90139f4:	02f52023          	sw	a5,32(a0)
        write_u32(config->ssSetup, reg + SPI_SS_SETUP);
f90139f8:	0105a783          	lw	a5,16(a1)
f90139fc:	02f52223          	sw	a5,36(a0)
        write_u32(config->ssHold, reg + SPI_SS_HOLD);
f9013a00:	0145a783          	lw	a5,20(a1)
f9013a04:	02f52423          	sw	a5,40(a0)
        write_u32(config->ssDisable, reg + SPI_SS_DISABLE);
f9013a08:	0185a783          	lw	a5,24(a1)
f9013a0c:	02f52623          	sw	a5,44(a0)
    }
f9013a10:	00008067          	ret

Disassembly of section .text.spi_waitXferBusy:

f9013a14 <spi_waitXferBusy>:
* @brief This function wait for SPI Transfer to complete.
    * 
    * @param reg SPI base address 
*
******************************************************************************/
    static void spi_waitXferBusy(u32 reg){
f9013a14:	ff010113          	addi	sp,sp,-16
f9013a18:	00112623          	sw	ra,12(sp)
f9013a1c:	00812423          	sw	s0,8(sp)
f9013a20:	00050413          	mv	s0,a0

#ifdef SYSTEM_SPI_2_IO_CTRL
    	if(reg == SYSTEM_SPI_2_IO_CTRL) cmdFifo_depth = SYSTEM_SPI_2_IO_PARAMETER_CMD_FIFO_DEPTH;
#endif

    	bsp_uDelay(1);
f9013a24:	f8b00637          	lui	a2,0xf8b00
f9013a28:	04c4b5b7          	lui	a1,0x4c4b
f9013a2c:	40058593          	addi	a1,a1,1024 # 4c4b400 <__stack_size+0x4c4b380>
f9013a30:	00100513          	li	a0,1
f9013a34:	dfdff0ef          	jal	f9013830 <clint_uDelay>
    	while(spi_cmdAvailability(reg) != cmdFifo_depth);
f9013a38:	00040513          	mv	a0,s0
f9013a3c:	e61ff0ef          	jal	f901389c <spi_cmdAvailability>
f9013a40:	10000793          	li	a5,256
f9013a44:	fef51ae3          	bne	a0,a5,f9013a38 <spi_waitXferBusy+0x24>
    }
f9013a48:	00c12083          	lw	ra,12(sp)
f9013a4c:	00812403          	lw	s0,8(sp)
f9013a50:	01010113          	addi	sp,sp,16
f9013a54:	00008067          	ret

Disassembly of section .text.spiFlash_select:

f9013a58 <spiFlash_select>:
    * @param spi SPI port base address 
    * @param cs 32-bit bitwise setting. Set 1 to enable particular bit. 
*
******************************************************************************/

    static void spiFlash_select(u32 spi, u32 cs){
f9013a58:	ff010113          	addi	sp,sp,-16
f9013a5c:	00112623          	sw	ra,12(sp)
        spi_select(spi, cs);
f9013a60:	ee1ff0ef          	jal	f9013940 <spi_select>
    }
f9013a64:	00c12083          	lw	ra,12(sp)
f9013a68:	01010113          	addi	sp,sp,16
f9013a6c:	00008067          	ret

Disassembly of section .text.spiFlash_diselect:

f9013a70 <spiFlash_diselect>:
    * 
    * @param spi SPI port base address 
    * @param cs 32-bit bitwise setting. Set 1 to disable particular bit. 
*
******************************************************************************/
    static void spiFlash_diselect(u32 spi, u32 cs){
f9013a70:	ff010113          	addi	sp,sp,-16
f9013a74:	00112623          	sw	ra,12(sp)
        spi_diselect(spi, cs);
f9013a78:	f11ff0ef          	jal	f9013988 <spi_diselect>
    }
f9013a7c:	00c12083          	lw	ra,12(sp)
f9013a80:	01010113          	addi	sp,sp,16
f9013a84:	00008067          	ret

Disassembly of section .text.spiFlash_init_:

f9013a88 <spiFlash_init_>:
* @brief This function initialize SPI port with default settings
    * 
    * @param spi SPI port base address
*
******************************************************************************/
    static void spiFlash_init_(u32 spi){
f9013a88:	fd010113          	addi	sp,sp,-48
f9013a8c:	02112623          	sw	ra,44(sp)
f9013a90:	02812423          	sw	s0,40(sp)
f9013a94:	00050413          	mv	s0,a0
        Spi_Config spiCfg;
        spiCfg.cpol = 0;
f9013a98:	00012223          	sw	zero,4(sp)
        spiCfg.cpha = 0;
f9013a9c:	00012423          	sw	zero,8(sp)
        spiCfg.mode = 0;
f9013aa0:	00012623          	sw	zero,12(sp)
        spiCfg.clkDivider = 2;
f9013aa4:	00200793          	li	a5,2
f9013aa8:	00f12823          	sw	a5,16(sp)
        spiCfg.ssSetup = 5;
f9013aac:	00500713          	li	a4,5
f9013ab0:	00e12a23          	sw	a4,20(sp)
        spiCfg.ssHold = 2;
f9013ab4:	00f12c23          	sw	a5,24(sp)
        spiCfg.ssDisable = 7;
f9013ab8:	00700793          	li	a5,7
f9013abc:	00f12e23          	sw	a5,28(sp)
        spi_applyConfig(spi, &spiCfg);
f9013ac0:	00410593          	addi	a1,sp,4
f9013ac4:	f0dff0ef          	jal	f90139d0 <spi_applyConfig>
        spi_waitXferBusy(spi); 
f9013ac8:	00040513          	mv	a0,s0
f9013acc:	f49ff0ef          	jal	f9013a14 <spi_waitXferBusy>
    }
f9013ad0:	02c12083          	lw	ra,44(sp)
f9013ad4:	02812403          	lw	s0,40(sp)
f9013ad8:	03010113          	addi	sp,sp,48
f9013adc:	00008067          	ret

Disassembly of section .text.spiFlash_init:

f9013ae0 <spiFlash_init>:
    * @param spi SPI port base address
    * @param gpio GPIO port base address 
    * @param cs 32-bit bitwise chip select setting.
*
******************************************************************************/
    static void spiFlash_init(u32 spi, u32 cs){
f9013ae0:	ff010113          	addi	sp,sp,-16
f9013ae4:	00112623          	sw	ra,12(sp)
f9013ae8:	00812423          	sw	s0,8(sp)
f9013aec:	00912223          	sw	s1,4(sp)
f9013af0:	00050413          	mv	s0,a0
f9013af4:	00058493          	mv	s1,a1
        spiFlash_init_(spi);
f9013af8:	f91ff0ef          	jal	f9013a88 <spiFlash_init_>
        spiFlash_diselect(spi, cs);
f9013afc:	00048593          	mv	a1,s1
f9013b00:	00040513          	mv	a0,s0
f9013b04:	f6dff0ef          	jal	f9013a70 <spiFlash_diselect>
    }
f9013b08:	00c12083          	lw	ra,12(sp)
f9013b0c:	00812403          	lw	s0,8(sp)
f9013b10:	00412483          	lw	s1,4(sp)
f9013b14:	01010113          	addi	sp,sp,16
f9013b18:	00008067          	ret

Disassembly of section .text.spiFlash_manufacturer_id_:

f9013b1c <spiFlash_manufacturer_id_>:
    *
    * @param spi SPI port base address
    * @param cs 32-bit bitwise chip select setting.
*
******************************************************************************/
    static inline u8 spiFlash_manufacturer_id_(u32 spi, u32 cs){
f9013b1c:	ff010113          	addi	sp,sp,-16
f9013b20:	00112623          	sw	ra,12(sp)
f9013b24:	00812423          	sw	s0,8(sp)
f9013b28:	00050413          	mv	s0,a0
#if defined(GD25_FLASH)
        spi_write(spi, 0x9E);
#else
        spi_write(spi, READ_MANUFACTURER);
f9013b2c:	09f00593          	li	a1,159
f9013b30:	d89ff0ef          	jal	f90138b8 <spi_write>
        // spi_write(spi, 0x00);
        // spi_write(spi, 0x00);
        // spi_write(spi, 0x00);
#endif
        u8 mid = spi_read(spi);
f9013b34:	00040513          	mv	a0,s0
f9013b38:	dc1ff0ef          	jal	f90138f8 <spi_read>
f9013b3c:	00050413          	mv	s0,a0
        bsp_uDelay(300);
f9013b40:	f8b00637          	lui	a2,0xf8b00
f9013b44:	04c4b5b7          	lui	a1,0x4c4b
f9013b48:	40058593          	addi	a1,a1,1024 # 4c4b400 <__stack_size+0x4c4b380>
f9013b4c:	12c00513          	li	a0,300
f9013b50:	ce1ff0ef          	jal	f9013830 <clint_uDelay>
        return mid;
    }
f9013b54:	00040513          	mv	a0,s0
f9013b58:	00c12083          	lw	ra,12(sp)
f9013b5c:	00812403          	lw	s0,8(sp)
f9013b60:	01010113          	addi	sp,sp,16
f9013b64:	00008067          	ret

Disassembly of section .text.spiFlash_wake_:

f9013b68 <spiFlash_wake_>:
*        start communicating with the device.
    * 
    * @param spi SPI port base address
*
******************************************************************************/
    static void spiFlash_wake_(u32 spi){
f9013b68:	ff010113          	addi	sp,sp,-16
f9013b6c:	00112623          	sw	ra,12(sp)
        spi_write(spi, 0xAB);        
f9013b70:	0ab00593          	li	a1,171
f9013b74:	d45ff0ef          	jal	f90138b8 <spi_write>
    }
f9013b78:	00c12083          	lw	ra,12(sp)
f9013b7c:	01010113          	addi	sp,sp,16
f9013b80:	00008067          	ret

Disassembly of section .text.spiFlash_exit4ByteAddr_:

f9013b84 <spiFlash_exit4ByteAddr_>:
    * @param spi SPI port base address
    * @param cs 32-bit bitwise chip select setting
    * @param mid 8-bit SPI Flash Manufacturer ID.
*
******************************************************************************/
    static void spiFlash_exit4ByteAddr_(u32 spi, u32 cs, u8 mid){
f9013b84:	ff010113          	addi	sp,sp,-16
f9013b88:	00112623          	sw	ra,12(sp)
        switch(mid){
f9013b8c:	09d00793          	li	a5,157
f9013b90:	00f60c63          	beq	a2,a5,f9013ba8 <spiFlash_exit4ByteAddr_+0x24>
            case 0x9D: 
                spi_write(spi, 0x29);
                break; 
            default: 
                spi_write(spi, 0xE9);
f9013b94:	0e900593          	li	a1,233
f9013b98:	d21ff0ef          	jal	f90138b8 <spi_write>
                break; 
        }
    }
f9013b9c:	00c12083          	lw	ra,12(sp)
f9013ba0:	01010113          	addi	sp,sp,16
f9013ba4:	00008067          	ret
                spi_write(spi, 0x29);
f9013ba8:	02900593          	li	a1,41
f9013bac:	d0dff0ef          	jal	f90138b8 <spi_write>
                break; 
f9013bb0:	fedff06f          	j	f9013b9c <spiFlash_exit4ByteAddr_+0x18>

Disassembly of section .text.spiFlash_exit4ByteAddr:

f9013bb4 <spiFlash_exit4ByteAddr>:
*
* @param spi SPI port base address
* @param cs 32-bit bitwise chip select setting
*
******************************************************************************/
    static void spiFlash_exit4ByteAddr(u32 spi, u32 cs){
f9013bb4:	ff010113          	addi	sp,sp,-16
f9013bb8:	00112623          	sw	ra,12(sp)
f9013bbc:	00812423          	sw	s0,8(sp)
f9013bc0:	00912223          	sw	s1,4(sp)
f9013bc4:	01212023          	sw	s2,0(sp)
f9013bc8:	00050413          	mv	s0,a0
f9013bcc:	00058493          	mv	s1,a1
        spiFlash_select(spi,cs);
f9013bd0:	e89ff0ef          	jal	f9013a58 <spiFlash_select>
        u8 mid = spiFlash_manufacturer_id_(spi, cs); 
f9013bd4:	00048593          	mv	a1,s1
f9013bd8:	00040513          	mv	a0,s0
f9013bdc:	f41ff0ef          	jal	f9013b1c <spiFlash_manufacturer_id_>
f9013be0:	00050913          	mv	s2,a0
        spiFlash_diselect(spi,cs);
f9013be4:	00048593          	mv	a1,s1
f9013be8:	00040513          	mv	a0,s0
f9013bec:	e85ff0ef          	jal	f9013a70 <spiFlash_diselect>
        spi_waitXferBusy(spi);
f9013bf0:	00040513          	mv	a0,s0
f9013bf4:	e21ff0ef          	jal	f9013a14 <spi_waitXferBusy>
        spiFlash_select(spi,cs);
f9013bf8:	00048593          	mv	a1,s1
f9013bfc:	00040513          	mv	a0,s0
f9013c00:	e59ff0ef          	jal	f9013a58 <spiFlash_select>
        spiFlash_exit4ByteAddr_(spi, cs, mid); 
f9013c04:	00090613          	mv	a2,s2
f9013c08:	00048593          	mv	a1,s1
f9013c0c:	00040513          	mv	a0,s0
f9013c10:	f75ff0ef          	jal	f9013b84 <spiFlash_exit4ByteAddr_>
        spiFlash_diselect(spi,cs);
f9013c14:	00048593          	mv	a1,s1
f9013c18:	00040513          	mv	a0,s0
f9013c1c:	e55ff0ef          	jal	f9013a70 <spiFlash_diselect>
        spi_waitXferBusy(spi);
f9013c20:	00040513          	mv	a0,s0
f9013c24:	df1ff0ef          	jal	f9013a14 <spi_waitXferBusy>
    }
f9013c28:	00c12083          	lw	ra,12(sp)
f9013c2c:	00812403          	lw	s0,8(sp)
f9013c30:	00412483          	lw	s1,4(sp)
f9013c34:	00012903          	lw	s2,0(sp)
f9013c38:	01010113          	addi	sp,sp,16
f9013c3c:	00008067          	ret

Disassembly of section .text.spiFlash_wake:

f9013c40 <spiFlash_wake>:
*
* @param spi SPI port base address
* @param cs 32-bit bitwise chip select setting.
*
******************************************************************************/
    static void spiFlash_wake(u32 spi, u32 cs){
f9013c40:	ff010113          	addi	sp,sp,-16
f9013c44:	00112623          	sw	ra,12(sp)
f9013c48:	00812423          	sw	s0,8(sp)
f9013c4c:	00912223          	sw	s1,4(sp)
f9013c50:	00050413          	mv	s0,a0
f9013c54:	00058493          	mv	s1,a1
        spiFlash_select(spi,cs);
f9013c58:	e01ff0ef          	jal	f9013a58 <spiFlash_select>
        spiFlash_wake_(spi);
f9013c5c:	00040513          	mv	a0,s0
f9013c60:	f09ff0ef          	jal	f9013b68 <spiFlash_wake_>
        spiFlash_diselect(spi,cs);
f9013c64:	00048593          	mv	a1,s1
f9013c68:	00040513          	mv	a0,s0
f9013c6c:	e05ff0ef          	jal	f9013a70 <spiFlash_diselect>
        spi_waitXferBusy(spi);
f9013c70:	00040513          	mv	a0,s0
f9013c74:	da1ff0ef          	jal	f9013a14 <spi_waitXferBusy>
        bsp_uDelay(100); // make sure the Flash fully awake
f9013c78:	f8b00637          	lui	a2,0xf8b00
f9013c7c:	04c4b5b7          	lui	a1,0x4c4b
f9013c80:	40058593          	addi	a1,a1,1024 # 4c4b400 <__stack_size+0x4c4b380>
f9013c84:	06400513          	li	a0,100
f9013c88:	ba9ff0ef          	jal	f9013830 <clint_uDelay>
    }
f9013c8c:	00c12083          	lw	ra,12(sp)
f9013c90:	00812403          	lw	s0,8(sp)
f9013c94:	00412483          	lw	s1,4(sp)
f9013c98:	01010113          	addi	sp,sp,16
f9013c9c:	00008067          	ret

Disassembly of section .text.spiFlash_f2m_:

f9013ca0 <spiFlash_f2m_>:
    * @param flashAddress The flash address to read the data
    * @param memoryAddress The RAM address to write the data
    * @param size The size of data to copy
*
******************************************************************************/
    static void spiFlash_f2m_(u32 spi, u32 flashAddress, u32 memoryAddress, u32 size){
f9013ca0:	fe010113          	addi	sp,sp,-32
f9013ca4:	00112e23          	sw	ra,28(sp)
f9013ca8:	00812c23          	sw	s0,24(sp)
f9013cac:	00912a23          	sw	s1,20(sp)
f9013cb0:	01212823          	sw	s2,16(sp)
f9013cb4:	01312623          	sw	s3,12(sp)
f9013cb8:	00050913          	mv	s2,a0
f9013cbc:	00058493          	mv	s1,a1
f9013cc0:	00060413          	mv	s0,a2
f9013cc4:	00068993          	mv	s3,a3
        spi_write(spi, 0x0B);
f9013cc8:	00b00593          	li	a1,11
f9013ccc:	bedff0ef          	jal	f90138b8 <spi_write>
        spi_write(spi, flashAddress >> 16);
f9013cd0:	0104d593          	srli	a1,s1,0x10
f9013cd4:	0ff5f593          	zext.b	a1,a1
f9013cd8:	00090513          	mv	a0,s2
f9013cdc:	bddff0ef          	jal	f90138b8 <spi_write>
        spi_write(spi, flashAddress >>  8);
f9013ce0:	0084d593          	srli	a1,s1,0x8
f9013ce4:	0ff5f593          	zext.b	a1,a1
f9013ce8:	00090513          	mv	a0,s2
f9013cec:	bcdff0ef          	jal	f90138b8 <spi_write>
        spi_write(spi, flashAddress >>  0);
f9013cf0:	0ff4f593          	zext.b	a1,s1
f9013cf4:	00090513          	mv	a0,s2
f9013cf8:	bc1ff0ef          	jal	f90138b8 <spi_write>
        spi_write(spi, 0);
f9013cfc:	00000593          	li	a1,0
f9013d00:	00090513          	mv	a0,s2
f9013d04:	bb5ff0ef          	jal	f90138b8 <spi_write>
        uint8_t *ram = (uint8_t *) memoryAddress;
        for(u32 idx = 0;idx < size;idx++){
f9013d08:	00000493          	li	s1,0
f9013d0c:	0180006f          	j	f9013d24 <spiFlash_f2m_+0x84>
            u8 value = spi_read(spi);
f9013d10:	00090513          	mv	a0,s2
f9013d14:	be5ff0ef          	jal	f90138f8 <spi_read>
            *ram++ = value;
f9013d18:	00a40023          	sb	a0,0(s0)
        for(u32 idx = 0;idx < size;idx++){
f9013d1c:	00148493          	addi	s1,s1,1
            *ram++ = value;
f9013d20:	00140413          	addi	s0,s0,1
        for(u32 idx = 0;idx < size;idx++){
f9013d24:	ff34e6e3          	bltu	s1,s3,f9013d10 <spiFlash_f2m_+0x70>
        }
    }
f9013d28:	01c12083          	lw	ra,28(sp)
f9013d2c:	01812403          	lw	s0,24(sp)
f9013d30:	01412483          	lw	s1,20(sp)
f9013d34:	01012903          	lw	s2,16(sp)
f9013d38:	00c12983          	lw	s3,12(sp)
f9013d3c:	02010113          	addi	sp,sp,32
f9013d40:	00008067          	ret

Disassembly of section .text.spiFlash_f2m:

f9013d44 <spiFlash_f2m>:
* @param flashAddress The flash address to read the data
* @param memoryAddress The RAM address to write the data
* @param size The size of data to copy
*
******************************************************************************/
    static void spiFlash_f2m(u32 spi, u32 cs, u32 flashAddress, u32 memoryAddress, u32 size){
f9013d44:	fe010113          	addi	sp,sp,-32
f9013d48:	00112e23          	sw	ra,28(sp)
f9013d4c:	00812c23          	sw	s0,24(sp)
f9013d50:	00912a23          	sw	s1,20(sp)
f9013d54:	01212823          	sw	s2,16(sp)
f9013d58:	01312623          	sw	s3,12(sp)
f9013d5c:	01412423          	sw	s4,8(sp)
f9013d60:	00050413          	mv	s0,a0
f9013d64:	00058493          	mv	s1,a1
f9013d68:	00060913          	mv	s2,a2
f9013d6c:	00068993          	mv	s3,a3
f9013d70:	00070a13          	mv	s4,a4
        spiFlash_select(spi,cs);
f9013d74:	ce5ff0ef          	jal	f9013a58 <spiFlash_select>
        spiFlash_f2m_(spi, flashAddress, memoryAddress, size);
f9013d78:	000a0693          	mv	a3,s4
f9013d7c:	00098613          	mv	a2,s3
f9013d80:	00090593          	mv	a1,s2
f9013d84:	00040513          	mv	a0,s0
f9013d88:	f19ff0ef          	jal	f9013ca0 <spiFlash_f2m_>
        spiFlash_diselect(spi,cs);
f9013d8c:	00048593          	mv	a1,s1
f9013d90:	00040513          	mv	a0,s0
f9013d94:	cddff0ef          	jal	f9013a70 <spiFlash_diselect>
    }
f9013d98:	01c12083          	lw	ra,28(sp)
f9013d9c:	01812403          	lw	s0,24(sp)
f9013da0:	01412483          	lw	s1,20(sp)
f9013da4:	01012903          	lw	s2,16(sp)
f9013da8:	00c12983          	lw	s3,12(sp)
f9013dac:	00812a03          	lw	s4,8(sp)
f9013db0:	02010113          	addi	sp,sp,32
f9013db4:	00008067          	ret

Disassembly of section .text.bspMain:

f9013db8 <bspMain>:
#define USER_SOFTWARE_FLASH  0x00100000
#define USER_SOFTWARE_SIZE   0x13800

#define SINGLE_SPI 1 //define DUAL_SPI for dual data SPI or QUAD_SPI for quad data SPI

void bspMain() {
f9013db8:	ff010113          	addi	sp,sp,-16
f9013dbc:	00112623          	sw	ra,12(sp)
#ifndef SIM
	spiFlash_init(SPI, SPI_CS);
f9013dc0:	00000593          	li	a1,0
f9013dc4:	f8014537          	lui	a0,0xf8014
f9013dc8:	d19ff0ef          	jal	f9013ae0 <spiFlash_init>
	spiFlash_wake(SPI, SPI_CS);
f9013dcc:	00000593          	li	a1,0
f9013dd0:	f8014537          	lui	a0,0xf8014
f9013dd4:	e6dff0ef          	jal	f9013c40 <spiFlash_wake>
	spiFlash_exit4ByteAddr(SPI, SPI_CS);
f9013dd8:	00000593          	li	a1,0
f9013ddc:	f8014537          	lui	a0,0xf8014
f9013de0:	dd5ff0ef          	jal	f9013bb4 <spiFlash_exit4ByteAddr>
#ifdef SINGLE_SPI
	spiFlash_f2m(SPI, SPI_CS, USER_SOFTWARE_FLASH, USER_SOFTWARE_MEMORY, USER_SOFTWARE_SIZE);
f9013de4:	00014737          	lui	a4,0x14
f9013de8:	80070713          	addi	a4,a4,-2048 # 13800 <__stack_size+0x13780>
f9013dec:	f90006b7          	lui	a3,0xf9000
f9013df0:	00100637          	lui	a2,0x100
f9013df4:	00000593          	li	a1,0
f9013df8:	f8014537          	lui	a0,0xf8014
f9013dfc:	f49ff0ef          	jal	f9013d44 <spiFlash_f2m>
#else 
    	#error "You must either define SINGLE_SPI to use single data line SPI, DUAL_SPI to use dual data line SPI or QUAD_SPI to use quad data line SPI."
#endif
#endif

	asm("fence.i; nop; nop; nop; nop; nop; nop"); 
f9013e00:	0000100f          	fence.i
f9013e04:	00000013          	nop
f9013e08:	00000013          	nop
f9013e0c:	00000013          	nop
f9013e10:	00000013          	nop
f9013e14:	00000013          	nop
f9013e18:	00000013          	nop
	void (*userMain)() = (void (*)())USER_SOFTWARE_MEMORY;
    #ifdef SMP
        smp_unlock(userMain);
f9013e1c:	f9000537          	lui	a0,0xf9000
f9013e20:	a24ec0ef          	jal	f9000044 <smp_unlock>
    #endif
	userMain();
f9013e24:	f90007b7          	lui	a5,0xf9000
f9013e28:	000780e7          	jalr	a5 # f9000000 <_start>
}
f9013e2c:	00c12083          	lw	ra,12(sp)
f9013e30:	01010113          	addi	sp,sp,16
f9013e34:	00008067          	ret

Disassembly of section .text.main:

f9013e38 <main>:
******************************************************************************/
#include "type.h"
#include "bsp.h"
#include "bootloaderConfig.h"

void main() {
f9013e38:	ff010113          	addi	sp,sp,-16
f9013e3c:	00112623          	sw	ra,12(sp)
    bsp_init();
f9013e40:	a25ff0ef          	jal	f9013864 <bsp_init>
    bspMain();
f9013e44:	f75ff0ef          	jal	f9013db8 <bspMain>
}
f9013e48:	00c12083          	lw	ra,12(sp)
f9013e4c:	01010113          	addi	sp,sp,16
f9013e50:	00008067          	ret

Disassembly of section .text.__libc_init_array:

f9013e54 <__libc_init_array>:
f9013e54:	ff010113          	addi	sp,sp,-16
f9013e58:	00812423          	sw	s0,8(sp)
f9013e5c:	01212023          	sw	s2,0(sp)
f9013e60:	81018793          	addi	a5,gp,-2032 # f9013ed8 <_data>
f9013e64:	81018413          	addi	s0,gp,-2032 # f9013ed8 <_data>
f9013e68:	00112623          	sw	ra,12(sp)
f9013e6c:	00912223          	sw	s1,4(sp)
f9013e70:	40878933          	sub	s2,a5,s0
f9013e74:	02878063          	beq	a5,s0,f9013e94 <__libc_init_array+0x40>
f9013e78:	40295913          	srai	s2,s2,0x2
f9013e7c:	00000493          	li	s1,0
f9013e80:	00042783          	lw	a5,0(s0)
f9013e84:	00148493          	addi	s1,s1,1
f9013e88:	00440413          	addi	s0,s0,4
f9013e8c:	000780e7          	jalr	a5
f9013e90:	ff24e8e3          	bltu	s1,s2,f9013e80 <__libc_init_array+0x2c>
f9013e94:	81018793          	addi	a5,gp,-2032 # f9013ed8 <_data>
f9013e98:	81018413          	addi	s0,gp,-2032 # f9013ed8 <_data>
f9013e9c:	40878933          	sub	s2,a5,s0
f9013ea0:	40295913          	srai	s2,s2,0x2
f9013ea4:	00878e63          	beq	a5,s0,f9013ec0 <__libc_init_array+0x6c>
f9013ea8:	00000493          	li	s1,0
f9013eac:	00042783          	lw	a5,0(s0)
f9013eb0:	00148493          	addi	s1,s1,1
f9013eb4:	00440413          	addi	s0,s0,4
f9013eb8:	000780e7          	jalr	a5
f9013ebc:	ff24e8e3          	bltu	s1,s2,f9013eac <__libc_init_array+0x58>
f9013ec0:	00c12083          	lw	ra,12(sp)
f9013ec4:	00812403          	lw	s0,8(sp)
f9013ec8:	00412483          	lw	s1,4(sp)
f9013ecc:	00012903          	lw	s2,0(sp)
f9013ed0:	01010113          	addi	sp,sp,16
f9013ed4:	00008067          	ret
