/*
 * Registers and memory blocks for VICKY III on the A2560U and A2560U+
 */

#ifndef __VICKYIII_General_H
#define __VICKYIII_General_H

/*
 * Screen Channel A
 */

#define MasterControlReg_A		    ((volatile unsigned long *)0x00B40000)
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
#define VKY3_DIP_REG                ((volatile unsigned short *)0x00B40002)
/* Bits 0 - 12: Master Control Register data */
#define VKY3_DIP_GAMMA              0x2000      /* DIP switch indication for Gamma correction */
#define VKY3_DIP_HIRES              0x4000      /* DIP switch for high resolution mode */
#define VKY3_PLL_ACTIVE_CLK         0x8000      /* Active Clock --- 0: 25.175Mhz, 1: 40Mhz */

#define BorderControlReg_L_A	    ((volatile unsigned long *)0x00B40004)
#define VKY3_BRDR_EN                0x00000001  /* Border Enable */
#define VKY3_X_SCROLL_MASK          0x00000070  /* X Scroll */
#define VKY3_X_SIZE_MASK            0x00003f00  /* X Size */
#define VKY3_Y_SIZE_MASK            0x003f0000  /* Y Size */

#define BorderControlReg_H_A 	    ((volatile unsigned long *)0x00B40008)
#define BackGroundControlReg_A	    ((volatile unsigned long *)0x00B4000C)
#define CursorControlReg_L_A	    ((volatile unsigned long *)0x00B40010)
#define CursorControlReg_H_A	    ((volatile unsigned long *)0x00B40014)

#define LineInterrupt0_A		    ((volatile unsigned short *)0x00B40018)
#define LineInterrupt1_A		    ((volatile unsigned short *)0x00B4001A)
#define LineInterrupt2_A		    ((volatile unsigned short *)0x00B4001C)
#define LineInterrupt3_A		    ((volatile unsigned short *)0x00B4001E)

#define MousePointer_Mem_A		    ((volatile unsigned short *)0x00B40400)
#define MousePtr_A_CTRL_Reg		    ((volatile unsigned short *)0x00B40C00)
#define MousePtr_En                 0x0001

#define MousePtr_A_X_Pos		    ((volatile unsigned short *)0x00B40C02)
#define MousePtr_A_Y_Pos		    ((volatile unsigned short *)0x00B40C04)
#define MousePtr_A_Mouse0		    ((volatile unsigned short *)0x00B40C0A)
#define MousePtr_A_Mouse1		    ((volatile unsigned short *)0x00B40C0C)
#define MousePtr_A_Mouse2		    ((volatile unsigned short *)0x00B40C0E)

#define ScreenText_A			    ((volatile char *)0x00B60000)	    /* Text matrix */
#define ColorText_A				    ((volatile unsigned char *)0x00B68000)	/* Color matrix */
#define FG_CLUT_A 				    ((volatile unsigned short *)0x00B6C400)	/* Foreground LUT */
#define BG_CLUT_A 				    ((volatile unsigned short *)0x00B6C440)	/* Background LUT */

#define BM0_Control_Reg			    ((volatile unsigned long *)0x00B40100)
#define BM0_Addy_Pointer_Reg	    ((volatile unsigned long *)0x00B40104)

#define Sprite_0_CTRL			    ((volatile unsigned short *)0x00B41000)
#define Sprite_0_ADDY_HI		    ((volatile unsigned short *)0x00B41002)
#define Sprite_0_POS_X	     	    ((volatile unsigned short *)0x00B41004)
#define Sprite_0_POS_Y	   		    ((volatile unsigned short *)0x00B41006)

/*
 * Color lookup tables
 */

#define LUT_0					    ((volatile unsigned char *)0x00B42000)
#define LUT_1					    ((volatile unsigned char *)0x00B42400)
#define LUT_2					    ((volatile unsigned char *)0x00B42800)
#define LUT_3					    ((volatile unsigned char *)0x00B42C00)
#define LUT_4					    ((volatile unsigned char *)0x00B43000)
#define LUT_5					    ((volatile unsigned char *)0x00B43400)
#define LUT_6					    ((volatile unsigned char *)0x00B43800)
#define LUT_7					    ((volatile unsigned char *)0x00B43C00)

/*
 * Text mode font memory
 */

#define VICKY_TXT_FONT_A            ((volatile unsigned char *)0x00B48000)  /* $00C48000..$00C48FFF    - FONT MEMORY Channel A */

/*
 * Location of VRAM
 */

#define VRAM_Bank0				    ((volatile unsigned char *)0x00C00000)

#endif
