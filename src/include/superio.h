/*
 * Define the SuperIO control registers
 *
 * See official documentation on the LPC47M107S for details on what
 * these registers all do:
 * https://ww1.microchip.com/downloads/en/DeviceDoc/47m10x.pdf
 */

#ifndef __SUPERIO_H
#define __SUPERIO_H

#include <stdint.h>

#define SUPERIO_BASE (0xFEC02000)


#define CONFIG_0x2E_REG ((volatile uint8_t *)0xFEC0202E)
#define CONFIG_0x2F_REG ((volatile uint8_t *)0xFEC0202F)

#define PME_STS_REG 	((volatile uint8_t *)0xFEC02100)
#define PME_EN_REG 		((volatile uint8_t *)0xFEC02102)

#define PME_STS1_REG	((volatile uint8_t *)0xFEC02104)
#define PME_STS2_REG	((volatile uint8_t *)0xFEC02105)
#define PME_STS3_REG	((volatile uint8_t *)0xFEC02106)
#define PME_STS4_REG	((volatile uint8_t *)0xFEC02107)
#define PME_STS5_REG	((volatile uint8_t *)0xFEC02108)

#define PME_EN1_REG		((volatile uint8_t *)0xFEC0210A)
#define PME_EN2_REG		((volatile uint8_t *)0xFEC0210B)
#define PME_EN3_REG		((volatile uint8_t *)0xFEC0210C)
#define PME_EN4_REG		((volatile uint8_t *)0xFEC0210D)
#define PME_EN5_REG		((volatile uint8_t *)0xFEC0210E)

#define SMI_STS1_REG	((volatile uint8_t *)0xFEC02110)
#define SMI_STS2_REG	((volatile uint8_t *)0xFEC02111)
#define SMI_STS3_REG	((volatile uint8_t *)0xFEC02112)
#define SMI_STS4_REG	((volatile uint8_t *)0xFEC02113)
#define SMI_STS5_REG	((volatile uint8_t *)0xFEC02114)

#define SMI_EN1_REG		((volatile uint8_t *)0xFEC02116)
#define SMI_EN2_REG		((volatile uint8_t *)0xFEC02117)
#define SMI_EN3_REG		((volatile uint8_t *)0xFEC02118)
#define SMI_EN4_REG		((volatile uint8_t *)0xFEC02119)
#define SMI_EN5_REG		((volatile uint8_t *)0xFEC0211A)

#define MSC_ST_REG			    ((volatile uint8_t *)0xFEC0211C)
#define FORCE_DISK_CHANGE       ((volatile uint8_t *)0xFEC0211E)
#define FLOPPY_DATA_RATE	    ((volatile uint8_t *)0xFEC0211F)

#define UART1_FIFO_CTRL_SHDW    ((volatile uint8_t *)0xFEC02120)
#define UART2_FIFO_CTRL_SHDW	((volatile uint8_t *)0xFEC02121)
#define DEV_DISABLE_REG		    ((volatile uint8_t *)0xFEC02122)

#define GP10_REG			    ((volatile uint8_t *)0xFEC02123)
#define GP11_REG			    ((volatile uint8_t *)0xFEC02124)
#define GP12_REG			    ((volatile uint8_t *)0xFEC02125)
#define GP13_REG			    ((volatile uint8_t *)0xFEC02126)
#define GP14_REG			    ((volatile uint8_t *)0xFEC02127)
#define GP15_REG			    ((volatile uint8_t *)0xFEC02128)
#define GP16_REG			    ((volatile uint8_t *)0xFEC02129)
#define GP17_REG			    ((volatile uint8_t *)0xFEC0212A)

#define GP20_REG			    ((volatile uint8_t *)0xFEC0212B)
#define GP21_REG			    ((volatile uint8_t *)0xFEC0212C)
#define GP22_REG			    ((volatile uint8_t *)0xFEC0212D)
#define GP23_REG			    ((volatile uint8_t *)0xFEC0212E)
#define GP24_REG			    ((volatile uint8_t *)0xFEC0212F)
#define GP25_REG			    ((volatile uint8_t *)0xFEC02130)
#define GP26_REG			    ((volatile uint8_t *)0xFEC02131)
#define GP27_REG			    ((volatile uint8_t *)0xFEC02132)

#define GP30_REG			    ((volatile uint8_t *)0xFEC02133)
#define GP31_REG			    ((volatile uint8_t *)0xFEC02134)
#define GP32_REG		  	    ((volatile uint8_t *)0xFEC02135)
#define GP33_REG		 	    ((volatile uint8_t *)0xFEC02136)
#define GP34_REG			    ((volatile uint8_t *)0xFEC02137)
#define GP35_REG			    ((volatile uint8_t *)0xFEC02138)
#define GP36_REG			    ((volatile uint8_t *)0xFEC02139)
#define GP37_REG			    ((volatile uint8_t *)0xFEC0213A)

#define GP40_REG			((volatile uint8_t *)0xFEC0213B)
#define GP41_REG			((volatile uint8_t *)0xFEC0213C)
#define GP42_REG			((volatile uint8_t *)0xFEC0213D)
#define GP43_REG			((volatile uint8_t *)0xFEC0213E)

#define GP50_REG			((volatile uint8_t *)0xFEC0213F)
#define GP51_REG			((volatile uint8_t *)0xFEC02140)
#define GP52_REG			((volatile uint8_t *)0xFEC02141)
#define GP53_REG			((volatile uint8_t *)0xFEC02142)
#define GP54_REG			((volatile uint8_t *)0xFEC02143)
#define GP55_REG			((volatile uint8_t *)0xFEC02144)
#define GP56_REG			((volatile uint8_t *)0xFEC02145)
#define GP57_REG			((volatile uint8_t *)0xFEC02146)

#define GP60_REG			((volatile uint8_t *)0xFEC02147)
#define GP61_REG			((volatile uint8_t *)0xFEC02148)

#define GP1_REG				((volatile uint8_t *)0xFEC0214B)
#define GP2_REG				((volatile uint8_t *)0xFEC0214C)
#define GP3_REG				((volatile uint8_t *)0xFEC0214D)
#define GP4_REG				((volatile uint8_t *)0xFEC0214E)
#define GP5_REG				((volatile uint8_t *)0xFEC0214F)
#define GP6_REG				((volatile uint8_t *)0xFEC02150)

#define FAN1_REG			((volatile uint8_t *)0xFEC02156)
#define FAN2_REG			((volatile uint8_t *)0xFEC02157)
#define FAN_CTRL_REG		((volatile uint8_t *)0xFEC02158)
#define FAN1_TACH_REG		((volatile uint8_t *)0xFEC02159)
#define FAN2_TACH_REG		((volatile uint8_t *)0xFEC0215A)
#define FAN1_PRELOAD_REG	((volatile uint8_t *)0xFEC0215B)
#define FAN2_PRELOAD_REG	((volatile uint8_t *)0xFEC0215C)

#define LED1_REG			((volatile uint8_t *)0xFEC0215D)
#define LED2_REG			((volatile uint8_t *)0xFEC0215E)
#define KEYBOARD_SCAN_CODE	((volatile uint8_t *)0xFEC0215F)

#endif
