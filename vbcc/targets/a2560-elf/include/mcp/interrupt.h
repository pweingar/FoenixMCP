/*
 * Declarations for the interrupt system
 */

#ifndef __MCP_INTERRUPT_H
#define __MCP_INTERRUPT_H

/* Type declaration for an interrupt handler */
typedef void (*p_int_handler)();

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


#endif
