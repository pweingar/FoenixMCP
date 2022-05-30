/**
 * @file vky_chan_a.h
 *
 * Define register addresses needed for A2560K channel A text driver
 */

#ifndef __VKY_CHAN_A_H
#define __VKY_CHAN_A_H

/** Master Control Register for Channel A, and its supported bits */
#define VKY3_A_MCR          ((volatile unsigned long *)0xFEC40000)
#define VKY3_A_MCR_TEXT     0x00000001  /**< Text mode enable bit */
#define VKY3_A_MCR_SLEEP    0x00040000  /**< Monitor sleep (synch disable) bit */
#define VKY3_A_1024x768     0x00000800  /**< Bit to select 1024x768 screen resolution */
#define VKY3_A_HIRES        0x40000000  /**< Bit to indicate on read if hi-res display is requested on the DIP switches */
#define VKY3_A_CLK40        0x80000000  /**< Indicate if PLL is 25MHz (0) or 40MHz (1) */

/** Border control register for Channel A */
#define VKY3_A_BCR          ((volatile unsigned long *)0xFEC40004)
#define VKY3_A_BCR_ENABLE   0x00000001  /**< Bit to enable the display of the border */

/** Border color register for Channel A */
#define VKY3_A_BRDCOLOR     ((volatile unsigned long *)0xFEC40008)

/** Cursor Control Register for Channel A */
#define VKY3_A_CCR          ((volatile unsigned long *)0xFEC40010)
#define VKY3_A_CCR_ENABLE   0x00000001  /**< Bit to enable the display of the cursor */
#define VKY3_A_CCR_RATE0    0x00000002  /**< Bit0 to specify the blink rate */
#define VKY3_A_CCR_RATE1    0x00000004  /**< Bit1 to specify the blink rate */

/** Cursor Position Register for Channel A */
#define VKY3_A_CPR          ((volatile unsigned long *)0xFEC40014)

/** Font Manager Registers for Channel A */
#define VKY3_A_FM0          ((volatile unsigned long *)0xFEC40020)
#define VKY3_A_FM1          ((volatile unsigned long *)0xFEC40024)

/** Font memory block for Channel A */
#define VKY3_A_FONT_MEMORY  ((volatile unsigned char *)0xFEC48000)

/** Text Matrix for Channel A */
#define VKY3_A_TEXT_MATRIX  ((volatile unsigned char *)0xFEC60000)

/** Color Matrix for Channel A */
#define VKY3_A_COLOR_MATRIX ((volatile unsigned char *)0xFEC68000)

/* Text Color LUTs for Channel A */
#define VKY3_A_LUT_SIZE     16
#define VKY3_A_TEXT_LUT_FG  ((volatile unsigned long *)0xFEC6C400)  /**< Text foreground color look up table for channel A */
#define VKY3_A_TEXT_LUT_BG  ((volatile unsigned long *)0xFEC6C440)  /**< Text background color look up table for channel A */

#endif
