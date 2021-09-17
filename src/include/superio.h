/*
 * Define the SuperIO control registers
 *
 * See official documentation on the LPC47M107S for details on what
 * these registers all do:
 * https://ww1.microchip.com/downloads/en/DeviceDoc/47m10x.pdf
 */

#ifndef __SUPERIO_H
#define __SUPERIO_H

#define PME_STS_REG 	((volatile unsigned char *)0x00C02100)
#define PME_EN_REG 		((volatile unsigned char *)0x00C02102)

#define PME_STS1_REG	((volatile unsigned char *)0x00C02104)
#define PME_STS2_REG	((volatile unsigned char *)0x00C02105)
#define PME_STS3_REG	((volatile unsigned char *)0x00C02106)
#define PME_STS4_REG	((volatile unsigned char *)0x00C02107)
#define PME_STS5_REG	((volatile unsigned char *)0x00C02108)

#define PME_EN1_REG		((volatile unsigned char *)0x00C0210A)
#define PME_EN2_REG		((volatile unsigned char *)0x00C0210B)
#define PME_EN3_REG		((volatile unsigned char *)0x00C0210C)
#define PME_EN4_REG		((volatile unsigned char *)0x00C0210D)
#define PME_EN5_REG		((volatile unsigned char *)0x00C0210E)

#define SMI_STS1_REG	((volatile unsigned char *)0x00C02110)
#define SMI_STS2_REG	((volatile unsigned char *)0x00C02111)
#define SMI_STS3_REG	((volatile unsigned char *)0x00C02112)
#define SMI_STS4_REG	((volatile unsigned char *)0x00C02113)
#define SMI_STS5_REG	((volatile unsigned char *)0x00C02114)

#define SMI_EN1_REG		((volatile unsigned char *)0x00C02116)
#define SMI_EN2_REG		((volatile unsigned char *)0x00C02117)
#define SMI_EN3_REG		((volatile unsigned char *)0x00C02118)
#define SMI_EN4_REG		((volatile unsigned char *)0x00C02119)
#define SMI_EN5_REG		((volatile unsigned char *)0x00C0211A)

#define MSC_ST_REG			    ((volatile unsigned char *)0x00C0211C)
#define FORCE_DISK_CHANGE       ((volatile unsigned char *)0x00C0211E)
#define FLOPPY_DATA_RATE	    ((volatile unsigned char *)0x00C0211F)

#define UART1_FIFO_CTRL_SHDW    ((volatile unsigned char *)0x00C02120)
#define UART2_FIFO_CTRL_SHDW	((volatile unsigned char *)0x00C02121)
#define DEV_DISABLE_REG		    ((volatile unsigned char *)0x00C02122)

#define GP10_REG			    ((volatile unsigned char *)0x00C02123)
#define GP11_REG			    ((volatile unsigned char *)0x00C02124)
#define GP12_REG			    ((volatile unsigned char *)0x00C02125)
#define GP13_REG			    ((volatile unsigned char *)0x00C02126)
#define GP14_REG			    ((volatile unsigned char *)0x00C02127)
#define GP15_REG			    ((volatile unsigned char *)0x00C02128)
#define GP16_REG			    ((volatile unsigned char *)0x00C02129)
#define GP17_REG			    ((volatile unsigned char *)0x00C0212A)

#define GP20_REG			    ((volatile unsigned char *)0x00C0212B)
#define GP21_REG			    ((volatile unsigned char *)0x00C0212C)
#define GP22_REG			    ((volatile unsigned char *)0x00C0212D)
#define GP23_REG			    ((volatile unsigned char *)0x00C0212E)
#define GP24_REG			    ((volatile unsigned char *)0x00C0212F)
#define GP25_REG			    ((volatile unsigned char *)0x00C02130)
#define GP26_REG			    ((volatile unsigned char *)0x00C02131)
#define GP27_REG			    ((volatile unsigned char *)0x00C02132)

#define GP30_REG			    ((volatile unsigned char *)0x00C02133)
#define GP31_REG			    ((volatile unsigned char *)0x00C02134)
#define GP32_REG		  	    ((volatile unsigned char *)0x00C02135)
#define GP33_REG		 	    ((volatile unsigned char *)0x00C02136)
#define GP34_REG			    ((volatile unsigned char *)0x00C02137)
#define GP35_REG			    ((volatile unsigned char *)0x00C02138)
#define GP36_REG			    ((volatile unsigned char *)0x00C02139)
#define GP37_REG			    ((volatile unsigned char *)0x00C0213A)

#define GP40_REG			((volatile unsigned char *)0x00C0213B)
#define GP41_REG			((volatile unsigned char *)0x00C0213C)
#define GP42_REG			((volatile unsigned char *)0x00C0213D)
#define GP43_REG			((volatile unsigned char *)0x00C0213E)

#define GP50_REG			((volatile unsigned char *)0x00C0213F)
#define GP51_REG			((volatile unsigned char *)0x00C02140)
#define GP52_REG			((volatile unsigned char *)0x00C02141)
#define GP53_REG			((volatile unsigned char *)0x00C02142)
#define GP54_REG			((volatile unsigned char *)0x00C02143)
#define GP55_REG			((volatile unsigned char *)0x00C02144)
#define GP56_REG			((volatile unsigned char *)0x00C02145)
#define GP57_REG			((volatile unsigned char *)0x00C02146)

#define GP60_REG			((volatile unsigned char *)0x00C02147)
#define GP61_REG			((volatile unsigned char *)0x00C02148)

#define GP1_REG				((volatile unsigned char *)0x00C0214B)
#define GP2_REG				((volatile unsigned char *)0x00C0214C)
#define GP3_REG				((volatile unsigned char *)0x00C0214D)
#define GP4_REG				((volatile unsigned char *)0x00C0214E)
#define GP5_REG				((volatile unsigned char *)0x00C0214F)
#define GP6_REG				((volatile unsigned char *)0x00C02150)

#define FAN1_REG			((volatile unsigned char *)0x00C02156)
#define FAN2_REG			((volatile unsigned char *)0x00C02157)
#define FAN_CTRL_REG		((volatile unsigned char *)0x00C02158)
#define FAN1_TACH_REG		((volatile unsigned char *)0x00C02159)
#define FAN2_TACH_REG		((volatile unsigned char *)0x00C0215A)
#define FAN1_PRELOAD_REG	((volatile unsigned char *)0x00C0215B)
#define FAN2_PRELOAD_REG	((volatile unsigned char *)0x00C0215C)

#define LED1_REG			((volatile unsigned char *)0x00C0215D)
#define LED2_REG			((volatile unsigned char *)0x00C0215E)
#define KEYBOARD_SCAN_CODE	((volatile unsigned char *)0x00C0215F)

#endif
