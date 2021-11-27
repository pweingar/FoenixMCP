/*
 * Declarations for the interrupt system
 */

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "sys_general.h"

/* Type declaration for an interrupt handler */
typedef void (*p_int_handler)();

/*
 * Interrupt control registers
 */

#if MODEL == MODEL_FOENIX_A2560K

#define PENDING_GRP0 		((volatile unsigned short *)0xFEC00100)
#define PENDING_GRP1 		((volatile unsigned short *)0xFEC00102)
#define PENDING_GRP2 		((volatile unsigned short *)0xFEC00104)

#define POL_GRP0 			((volatile unsigned short *)0xFEC00108)
#define POL_GRP1 			((volatile unsigned short *)0xFEC0010A)
#define POL_GRP2 			((volatile unsigned short *)0xFEC0010C)

#define EDGE_GRP0 			((volatile unsigned short *)0xFEC00110)
#define EDGE_GRP1 			((volatile unsigned short *)0xFEC00112)
#define EDGE_GRP2 			((volatile unsigned short *)0xFEC00114)

#define MASK_GRP0 			((volatile unsigned short *)0xFEC00118)
#define MASK_GRP1 			((volatile unsigned short *)0xFEC0011A)
#define MASK_GRP2 			((volatile unsigned short *)0xFEC0011C)

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS

#define PENDING_GRP0 		((volatile unsigned short *)0x00B00100)
#define PENDING_GRP1 		((volatile unsigned short *)0x00B00102)
#define PENDING_GRP2 		((volatile unsigned short *)0x00B00104)

#define POL_GRP0 			((volatile unsigned short *)0x00B00108)
#define POL_GRP1 			((volatile unsigned short *)0x00B0010A)
#define POL_GRP2 			((volatile unsigned short *)0x00B0010C)

#define EDGE_GRP0 			((volatile unsigned short *)0x00B00110)
#define EDGE_GRP1 			((volatile unsigned short *)0x00B00112)
#define EDGE_GRP2 			((volatile unsigned short *)0x00B00114)

#define MASK_GRP0 			((volatile unsigned short *)0x00B00118)
#define MASK_GRP1 			((volatile unsigned short *)0x00B0011A)
#define MASK_GRP2 			((volatile unsigned short *)0x00B0011C)

#endif


/*
 * For A2560K: $00C00100..$00C001FF - Interrupt Controllers
 */

/*
 * Interrupt Reg0 - 16bits - GROUP0 (GRP0)
 */

/* Video Related Interrupts - Text Channel */

#define SOF_A_INT00             0x0001 /* Interrupt 0x00: Start of Frame Channel A */
#define SOL_A_INT01	            0x0002 /* Interrupt 0x01: Start of Line Channel A */
#define VICKY_A_INT02           0x0004 /* Interrupt 0x02: Channel A */
#define VICKY_A_INT03   	    0x0008 /* Interrupt 0x03: Channel A */
#define VICKY_A_INT04	        0x0010 /* Interrupt 0x04: Channel A */
#define VICKY_A_INT05	        0x0020 /* Interrupt 0x05: Channel A */
#define RESERVED0		        0x0040
#define VICKY_A_DAC_HP07        0x0080 /* Interrupt 0x06:  */

/* Video Related Interrupts - Graphic Channel */

#define SOF_B_INT08             0x0100 /* Interrupt 0x08: Start of Line Channel B */
#define SOL_B_INT09             0x0200 /* Interrupt 0x09: Start of Line Channel B */
#define VICKY_B_INT0A           0x0400 /* Interrupt 0x0A: Channel B */
#define VICKY_B_INT0B           0x0800 /* Interrupt 0x0B: Channel B */
#define VICKY_B_INT0C           0x1000 /* Interrupt 0x0C: Channel B */
#define VICKY_B_INT0D           0x2000 /* Interrupt 0x0D: Channel B */
#define RESERVED1               0x4000
#define VICKY_B_DAC_HP_INT0F    0x8000 /* Interrupt 0x0F: Channel B */

/*
 * Interrupt Reg1 - 16bits GROUP1 (GRP1)
 */

/* SuperIO */

#define SPIO_KBD_INT10      0x0001		// Interrupt 0x10: Super IO Keyboard
#define A2560K_KBD_INT11	0x0002		// Interrupt 0x11: This is the A2560K Internal Keyboard and also the U KBD Interrupt
#define SPIO_MOUSE_INT12	0x0004		// Interrupt 0x12: Super IO Mouse
#define SPIO_COM1_INT13     0x0008		// Interrupt 0x13: Super IO COM1
#define SPIO_COM2_INT14     0x0010		// Interrupt 0x14: Super IO COM2
#define SPIO_LPT1_INT15     0x0020		// Interrupt 0x15: Super IO LPT1
#define SPIO_FDC_INT16      0x0040		// Interrupt 0x16: Super IO Floppy Disk Controller
#define SPIO_MPU401_INT17   0x0080		// Interrupt 0x17: Super IO MPU401 (MIDI)

/* Timers */

#define TIMER0_INT18        0x0100		// Interrupt 0x18: Timer0 Clocked with CPU Clock
#define TIMER1_INT19        0x0200		// Interrupt 0x19: Timer1 Clocked with CPU Clock
#define TIMER2_INT1A        0x0400		// Interrupt 0x1A: Timer2 Clocked with CPU Clock
#define TIMER3_INT1B        0x0800		// Interrupt 0x1B: Timer3 Clocked with SOF Channel A
#define TIMER4_INT1C        0x1000		// Interrupt 0x1C: Timer4 Clocked with SOF Channel B
#define RESERVED3           0x2000		// No interrupt - Reserved
#define RESERVED4           0x4000		// No Interrupt - Reserved
#define RTC_INT1F           0x8000		// Interrupt 0x1F: RTC

/*
 * Interrupt Reg2 - 16bits  GROUP2 (GRP2)
 */

/* Mostly Music / Sound Related Interrupts */

#define IDE_INT20		    0x0001		// IDE HDD Generated Interrupt
#define SDCARD_INS_INT21    0x0002		// SDCard Insert Interrupt
#define SD_INT22			0x0004		// SDCard Controller Interrupt
#define INT_OPM_INT23		0x0008		// Internal OPM Interrupt
#define EXT_OPN2_INT24      0x0010		// External OPN2 Interrupt
#define EXT_OPL3_INT25      0x0020		// External OPL3 Interrupt
#define RESERVED5           0x0040		// No interrupt - Reserved
#define RESERVED6           0x0080		// No interrupt - Reserved

#define BEATRIX_INT28       0x0100		// Beatrix Interrupt 0
#define BEATRIX_INT29       0x0200		// Beatrix Interrupt 1
#define BEATRIX_INT2A       0x0400		// Beatrix Interrupt 2
#define BEATRIX_INT2B       0x0800		// Beatrix Interrupt 3
#define RESERVED7           0x1000		// No Interrupt - Reserved
#define DAC1_PB_INT2D       0x2000		// DAC1 Playback Done Interrupt (48K)
#define RESERVED8           0x4000		// No Interrupt - Reserved
#define DAC0_PB_INT2F		0x8000		// DAC0 Playback Done Interrupt (44.1K)

/*
 * Define standard interrupt numbers to be used for enabling, disabling an interrupt or setting its handler
 */

#define INT_SOF_A           0x00    /* Vicky Channel A Start of Frame */
#define INT_SOL_A           0x01    /* Vicky Channel A Start of Line */
#define INT_VICKY_A_1       0x02
#define INT_VICKY_A_2       0x03
#define INT_VICKY_A_3       0x04
#define INT_VICKY_A_4       0x05
#define INT_RESERVED_1      0x06
#define INT_VICKY_A_DAC     0x07
#define INT_SOF_B           0x08    /* Vicky Channel B Start of Frame */
#define INT_SOL_B           0x09    /* Vicky Channel B Start of Line */
#define INT_VICKY_B_1       0x0A
#define INT_VICKY_B_2       0x0B
#define INT_VICKY_B_3       0x0C
#define INT_VICKY_B_4       0x0D
#define INT_RESERVED_2      0x0E
#define INT_VICKY_B_DAC     0x0F

#define INT_KBD_PS2         0x10    /* SuperIO - PS/2 Keyboard */
#define INT_KBD_A2560K      0x11    /* SuperIO - A2560K Built in keyboard (Mo) */
#define INT_MOUSE           0x12    /* SuperIO - PS/2 Mouse */
#define INT_COM1            0x13    /* SuperIO - COM1 */
#define INT_COM2            0x14    /* SuperIO - COM2 */
#define INT_LPT1            0x15    /* SuperIO - LPT1 */
#define INT_FDC             0x16    /* SuperIO - Floppy Drive Controller */
#define INT_MIDI            0x17    /* SuperIO - MIDI */
#define INT_TIMER0          0x18    /* Timer 0, Clocked with the CPU Clock */
#define INT_TIMER1          0x19    /* Timer 1, Clocked with the CPU Clock */
#define INT_TIMER2          0x1A    /* Timer 2, Clocked with the CPU Clock */
#define INT_TIMER3          0x1B    /* Timer 3, Clocked with the SOF Channel A */
#define INT_TIMER4          0x1C    /* Timer 4, Clocked with the SOF Channel B */
#define INT_RESERVED_3      0x1D    /* Reserved */
#define INT_RESERVED_4      0x1E    /* Reserved */
#define INT_RTC             0x1F    /* Real Time Clock */

#define INT_PATA            0x20    /* IDE/PATA Hard drive interrupt */
#define INT_SDC_INS         0x21    /* SD card inserted */
#define INT_SDC             0x22    /* SD card controller */
#define INT_OPM_INT         0x23    /* Internal OPM */
#define INT_OPN2_EXT        0x24    /* External OPN */
#define INT_OPL3_EXT        0x25    /* External OPL */
#define INT_RESERVED_5      0x26    /* Reserved */
#define INT_RESERVED_6      0x27    /* Reserved */
#define INT_BEATRIX_0       0x28    /* Beatrix 0 */
#define INT_BEATRIX_1       0x29    /* Beatrix 1 */
#define INT_BEATRIX_2       0x2A    /* Beatrix 2 */
#define INT_BEATRIX_3       0x2B    /* Beatrix 3 */
#define INT_RESERVED_7      0x2C    /* Reserved */
#define INT_DAC1_PB         0x2D    /* DAC1 Playback Done (48K) */
#define INT_RESERVED_8      0x2E    /* Reserved */
#define INT_DAC0_PB         0x2F    /* DAC0 Playback Done (44.1K) */

/*
 * Initialize the interrupt registers
 */
extern void int_init();

/*
 * Enable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 *
 * Returns:
 * a machine dependent representation of the interrupt masking prior to enabling
 */
extern short int_enable_all();

/*
 * Disable all interrupts
 *
 * NOTE: this is actually provided in the low level assembly
 *
 * Returns:
 * a machine dependent representation of the interrupt masking prior to disabling
 */
extern short int_disable_all();

/*
 * Restore interrupt masking state returned by a previous call to int_enable/int_disable
 *
 * NOTE: this is actually provided in the low level assembly
 *
 * Inputs:
 * int_mask = machine dependent representation of the interrupt masking
 */
extern void int_restore(short int_mask);

/*
 * Disable an interrupt by masking it
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
extern void int_disable(unsigned short n);

/*
 * Enable an interrupt
 *
 * Inputs:
 * n = the number of the interrupt
 */
extern void int_enable(unsigned short n);

/*
 * Register a handler for a given interrupt.
 *
 * Inputs:
 * n = the number of the interrupt
 * handler = pointer to the interrupt handler to register
 *
 * Returns:
 * the pointer to the previous interrupt handler
 */
extern p_int_handler int_register(unsigned short n, p_int_handler handler);

/*
 * Return true (non-zero) if an interrupt is pending for the given interrupt
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 *
 * Returns:
 * non-zero if interrupt n is pending, 0 if not
 */
extern short int_pending(unsigned short n);

/*
 * Acknowledge an interrupt (clear out its pending flag)
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
extern void int_clear(unsigned short n);

#endif
