/*
 * Registers and memory blocks for VICKY III for the A2560K
 */

#ifndef __VICKYIII_General_H
#define __VICKYIII_General_H

/*
 * Screen Channel A
 */

#define MasterControlReg_A		    ((volatile uint32_t *)0x00C40000)
#define VKY3_MCR_TEXT_EN            0x00000001  /* Text Mode Enable */
#define VKY3_MCR_TEXT_OVRLY         0x00000002  /* Text Mode overlay */
#define VKY3_MCR_GRAPH_EN           0x00000004  /* Graphic Mode Enable */
#define VKY3_MCR_BITMAP_EN          0x00000008  /* Bitmap Engine Enable */
#define VKY3_MCR_RESOLUTION_MASK    0x00000300  /* Resolution - 00: 640x480, 01:800x600, 10: 1024x768, 11: 640x400 */
#define VKY3_MCR_640x480            0x00000000
#define VKY3_MCR_800x600            0x00000100
#define VKY3_MCR_1024x768           0x00000200
#define VKY3_MCR_640x400            0x00000300
#define VKY3_MCR_DOUBLE_EN          0x00000400  /* Doubling Pixel */
#define VKY3_MCR_GAMMA_EN           0x00010000  /* GAMMA Enable */
#define VKY3_MCR_MANUAL_GAMMA_EN    0x00020000  /* Enable Manual GAMMA Enable */
#define VKY3_MCR_BLANK_EN           0x00040000  /* Turn OFF sync (to monitor in sleep mode) */


/* Access to DIP switch information (read only) */
#define VKY3_DIP_REG                ((volatile unsigned short *)0x00C40002)
/* Bits 0 - 12: Master Control Register data */
#define VKY3_DIP_GAMMA              0x2000      /* DIP switch indication for Gamma correction */
#define VKY3_DIP_HIRES              0x4000      /* DIP switch for high resolution mode */
#define VKY3_PLL_ACTIVE_CLK         0x8000      /* Active Clock --- 0: 25.175Mhz, 1: 40Mhz */

#define BorderControlReg_L_A	    ((volatile uint32_t *)0x00C40004)
#define VKY3_BRDR_EN                0x00000001  /* Border Enable */
#define VKY3_X_SCROLL_MASK          0x00000070  /* X Scroll */
#define VKY3_X_SIZE_MASK            0x00003f00  /* X Size */
#define VKY3_Y_SIZE_MASK            0x003f0000  /* Y Size */

#define BorderControlReg_H_A 	    ((volatile uint32_t *)0x00C40008)
#define BackGroundControlReg_A	    ((volatile uint32_t *)0x00C4000C)
#define CursorControlReg_L_A	    ((volatile uint32_t *)0x00C40010)
#define CursorControlReg_H_A	    ((volatile uint32_t *)0x00C40014)

#define LineInterrupt0_A		    ((volatile uint16_t *)0x00C40018)
#define LineInterrupt1_A		    ((volatile uint16_t *)0x00C4001A)
#define LineInterrupt2_A		    ((volatile uint16_t *)0x00C4001C)
#define LineInterrupt3_A		    ((volatile uint16_t *)0x00C4001E)

#define MousePointer_Mem_A		    ((volatile uint16_t *)0x00C40400)
#define MousePtr_A_CTRL_Reg		    ((volatile uint16_t *)0x00C40C00)
#define MousePtr_En                 0x0001

#define MousePtr_A_X_Pos		    ((volatile uint16_t *)0x00C40C02)
#define MousePtr_A_Y_Pos		    ((volatile uint16_t *)0x00C40C04)
#define MousePtr_A_Mouse0		    ((volatile uint16_t *)0x00C40C0A)
#define MousePtr_A_Mouse1		    ((volatile uint16_t *)0x00C40C0C)
#define MousePtr_A_Mouse2		    ((volatile uint16_t *)0x00C40C0E)

#define ScreenText_A			    ((volatile char *)0x00C60000)	    /* Text matrix */
#define ColorText_A				    ((volatile uint8_t *)0x00C68000)	/* Color matrix */
#define FG_CLUT_A 				    ((volatile uint16_t *)0x00C6C400)	/* Foreground LUT */
#define BG_CLUT_A 				    ((volatile uint16_t *)0x00C6C440)	/* Background LUT */

/*
 * Screen Channel B
 */

#define MasterControlReg_B		    ((volatile uint32_t *)0x00C80000)
#define BorderControlReg_L_B	    ((volatile uint32_t *)0x00C80004)
#define BorderControlReg_H_B 	    ((volatile uint32_t *)0x00C80008)
#define BackGroundControlReg_B	    ((volatile uint32_t *)0x00C8000C)
#define CursorControlReg_L_B	    ((volatile uint32_t *)0x00C80010)
#define CursorControlReg_H_B	    ((volatile uint32_t *)0x00C80014)

#define LineInterrupt0_B		    ((volatile uint16_t *)0x00C80018)
#define LineInterrupt1_B		    ((volatile uint16_t *)0x00C8001A)
#define LineInterrupt2_B		    ((volatile uint16_t *)0x00C8001C)
#define LineInterrupt3_B		    ((volatile uint16_t *)0x00C8001E)

#define MousePointer_Mem_B		    ((volatile uint16_t *)0x00C80400)
#define MousePtr_B_CTRL_Reg		    ((volatile uint16_t *)0x00C80C00)

#define MousePtr_B_X_Pos		    ((volatile uint16_t *)0x00C80C02)
#define MousePtr_B_Y_Pos		    ((volatile uint16_t *)0x00C80C04)
#define MousePtr_B_Mouse0		    ((volatile uint16_t *)0x00C80C0A)
#define MousePtr_B_Mouse1		    ((volatile uint16_t *)0x00C80C0C)
#define MousePtr_B_Mouse2		    ((volatile uint16_t *)0x00C80C0E)

#define ScreenText_B			    ((volatile char *)0x00CA0000)		/* Text matrix */
#define ColorText_B				    ((volatile uint8_t *)0x00CA8000)	/* Color matrix */
#define FG_CLUT_B 				    ((volatile uint16_t *)0x00CAC400)	/* Foreground LUT */
#define BG_CLUT_B 				    ((volatile uint16_t *)0x00CAC440)	/* Background LUT */

#define BM0_Control_Reg			    ((volatile uint16_t *)0x00C80100)
#define BM0_Addy_Pointer_Reg	    ((volatile uint16_t *)0x00C80104)

#define Sprite_0_CTRL			    ((volatile uint16_t *)0x00C81000)
#define Sprite_0_ADDY_HI		    ((volatile uint16_t *)0x00C81002)
#define Sprite_0_POS_X	     	    ((volatile uint16_t *)0x00C81004)
#define Sprite_0_POS_Y	   		    ((volatile uint16_t *)0x00C81006)

/*
 * Color lookup tables
 */

#define LUT_0					    ((volatile uint8_t *)0x00C82000)
#define LUT_1					    ((volatile uint8_t *)0x00C82400)
#define LUT_2					    ((volatile uint8_t *)0x00C82800)
#define LUT_3					    ((volatile uint8_t *)0x00C82C00)
#define LUT_4					    ((volatile uint8_t *)0x00C83000)
#define LUT_5					    ((volatile uint8_t *)0x00C83400)
#define LUT_6					    ((volatile uint8_t *)0x00C83800)
#define LUT_7					    ((volatile uint8_t *)0x00C83C00)

/*
 * Text mode font memory
 */

#define VICKY_TXT_FONT_A            ((volatile unsigned char *)0x00C48000)  /* $00C48000..$00C48FFF    - FONT MEMORY Channel A */
#define VICKY_TXT_FONT_B            ((volatile unsigned char *)0x00C88000)  /* $00C88000..$00C88FFF    - FONT MEMORY Channel B */

/*
 * Location of VRAM
 */

#define VRAM_Bank0				    ((volatile uint8_t *)0x00800000)
#define VRAM_Bank1				    ((volatile uint8_t *)0x00A00000)

#endif
