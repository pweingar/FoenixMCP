/*
 * Registers and memory blocks for VICKY II on the C256 Machines (U, U+, FMX)
 */

#ifndef __VICKYII_C256_H
#define __VICKYII_C256_H

/*
 * Master control registers
 */

#define VKY2_MSTR_CTRL_L            ((volatile unsigned char __attribute__((far)) *)0xaf0000)
#define VKY2_MSTR_CTRL_TEXT         0x0001      /* Enable: TEXT mode */
#define VKY2_MSTR_CTRL_OVERLAY      0x0002      /* Enable: TEXT and GRAPHICS overlay mode */
#define VKY2_MSTR_CTRL_GRAPHICS     0x0004      /* Enable: GRAPHICS mode */
#define VKY2_MSTR_CTRL_BITMAP       0x0008      /* Enable: BITMAP engine */
#define VKY2_MSTR_CTRL_TILE         0x0010      /* Enable: TILE engine */
#define VKY2_MSTR_CTRL_SPRITE       0x0020      /* Enable: SPRITE engine */
#define VKY2_MSTR_CTRL_GAMMA        0x0040      /* Enable: Gamma correction */
#define VKY2_MSTR_CTRL_DISABLE      0x0080      /* Disable the video display */

#define VKY2_MSTR_CTRL_H            ((volatile unsigned char __attribute__((far)) *)0xaf0001)
#define VKY2_MSTR_CTRL_40MHZ        0x01        /* Set pixel clock: 0 = 25Mhz (640x480 or 320x240), 1 = 40MHz (800x600 or 400x300) */
#define VKY2_MSTR_CTRL_PIXDBL       0x02        /* Enable: pixel doubling */

/*
 * Gamma control register
 */

#define VKY2_GAMMA_CTRL             ((volatile unsigned char __attribute__((far)) *)0xaf0002)
#define VKY2_GAMMA_CTRL_INPUT       0x01        /* 0 = DipSwitch Chooses GAMMA on/off, 1 = Software Control */
#define VKY2_GAMMA_CTRL_SOFT        0x02        /* 0 = GAMMA Table is not Applied, 1 = GAMMA Table is Applied */
#define VKY2_SYNC_OFF               0x04        /* 0 = Normal Operation, 1 = Turn Off Sync to get sleep mode from Monitor */
#define VKY2_GAMMA_DP_SW_VAL        0x08        /* READ ONLY - Actual DIP Switch Value */
#define VKY2_HIRES_DP_SW_VAL        0x10        /* READ ONLY - 0 = Hi-Res on BOOT ON, 1 = Hi-Res on BOOT OFF */

/*
 * Border control registers
 */

#define VKY2_BRDR_CTRL              ((volatile unsigned char __attribute__((far)) *)0xaf0004)
#define VKY2_BRDR_ENABLE            0x01        /* Enable the border */
#define VKY2_BRDR_SCROLL_MASK       0x70        /* X scroll... values 0 .. 7 */

#define VKY2_BRDR_COLOR_B           ((volatile unsigned char __attribute__((far)) *)0xaf0005)
#define VKY2_BRDR_COLOR_G           ((volatile unsigned char __attribute__((far)) *)0xaf0006)
#define VKY2_BRDR_COLOR_R           ((volatile unsigned char __attribute__((far)) *)0xaf0007)
#define VKY2_BRDR_X_SIZE            ((volatile unsigned char __attribute__((far)) *)0xaf0008)
#define VKY2_BRDR_Y_SIZE            ((volatile unsigned char __attribute__((far)) *)0xaf0009)

/*
 * Graphics Background Color Registers
 */

#define VKY2_BACKGND_COLOR_B        ((volatile unsigned char __attribute__((far)) *)0xaf000d)
#define VKY2_BACKGND_COLOR_G        ((volatile unsigned char __attribute__((far)) *)0xaf000e)
#define VKY2_BACKGND_COLOR_R        ((volatile unsigned char __attribute__((far)) *)0xaf000f)

/*
 * Cursor control registers
 */

#define VKY2_TXT_CRSR_CTRL          ((volatile unsigned char __attribute__((far)) *)0xaf0010)
#define VKY2_TXT_CRSR_ENABLE        0x01
#define VKY2_TXT_CRSR_FLASH_1HZ     0x00    /* Flash rate: 00 - 1/Sec */
#define VKY2_TXT_CRSR_FLASH_2HZ     0x02    /* Flash rate: 01 - 2/Sec */
#define VKY2_TXT_CRSR_FLASH_4HZ     0x04    /* Flash rate: 10 - 4/Sec */
#define VKY2_TXT_CRSR_FLASH_5HZ     0x06    /* Flash rate: 11 - 5/Sec */

#define VKY2_TXT_CRSR_CHARACTER     ((volatile unsigned char __attribute__((far)) *)0xaf0012)
#define VKY2_TXT_CRSR_COLOR         ((volatile unsigned char __attribute__((far)) *)0xaf0013)
#define VKY2_TXT_CRSR_X             ((volatile unsigned short __attribute__((far)) *)0xaf0014)
#define VKY2_TXT_CRSR_Y             ((volatile unsigned short __attribute__((far)) *)0xaf0016)

/*
 * Bitmap registers
 */

typedef volatile unsigned char __far24 *pointer_24;

#define BM0_Control_Reg             ((volatile unsigned char __attribute__((far)) *)0xaf0108)
#define BM0_Addy_Pointer_Reg        ((volatile pointer_24 __attribute__((far)) *)0xaf0109)

/*
 * For the moment... skip the rest of VICKYII registers
 */

/*
 * Text mode memory areas
 */

#define VKY2_FONT_MEMORY            ((volatile unsigned char __attribute__((far)) *)0xaf8000)
#define VKY2_TEXT_MATRIX            ((volatile unsigned char __attribute__((far)) *)0xafa000)
#define VKY2_COLOR_MATRIX           ((volatile unsigned char __attribute__((far)) *)0xafc000)
#define VKY2_TXT_FORE_LUT           ((volatile unsigned char __attribute__((far)) *)0xaf1f40)
#define VKY2_TXT_BACK_LUT           ((volatile unsigned char __attribute__((far)) *)0xaf1f80)

/*
 * Graphics LUTs
 */

#define LUT_0                       ((volatile unsigned char __attribute__((far)) *)0xaf2000)

/*
 * VRAM
 */

#define VRAM_Bank0                  ((volatile unsigned char __attribute__((far)) *)0xb00000)

#endif
