/*
 * Definitions for the interrupt controls managed by GAVIN.
 */

#include <stdint.h>
#include <string.h>

#include "features.h"
#include "interrupt.h"

#define MAX_HANDLERS (16*4)

p_int_handler g_int_handler[MAX_HANDLERS]; // Public because referenced by the asm interrupt handler.

static uint16_t int_group(uint16_t n);
static uint16_t int_mask(uint16_t n);
static FUNC_V_2_V int_install_cpu_vector(int number, void (*handler)());


/*
 * Initialize the interrupt registers
 */
void int_init() {
	int i;

	memset(g_int_handler, 0, sizeof(g_int_handler));

    // At Reset, all of those already have those values
	// the Pol are @ 0x0000 and normally pending are reseted, but it is not impossible that some might be triggered during init
	*EDGE_GRP0 = 0xFFFF;
	*EDGE_GRP1 = 0xFFFF;
	*EDGE_GRP2 = 0xFFFF;

	*MASK_GRP0 = 0xFFFF;
	*MASK_GRP1 = 0xFFFF;
	*MASK_GRP2 = 0xFFFF;


#if CPU_FAMILY==m68k
	{
		extern void m68k_int_superio_ps2(void);
		extern void m68k_int_maurice(void);
		extern void m68k_int_maurice(void);
		extern void m68k_int_superio_mouse(void);
		extern void m68k_int_superio_com1(void);
		extern void m68k_int_superio_com2(void);
		extern void m68k_int_superio_lpt1(void);
		extern void m68k_int_superio_fdc(void);
		extern void m68k_int_superio_midi(void);
		extern void m68k_int_timer0(void);
		extern void m68k_int_timer1(void);
		extern void m68k_int_timer2(void);
		extern void m68k_int_timer3(void);
		extern void m68k_int_timer4(void);
		extern void m68k_int_x1D(void);
		extern void m68k_int_x1E(void);
		extern void m68k_int_rtc(void);
		extern void m68k_int_hdc(void);
		extern void m68k_int_sdc_insert(void);
		extern void m68k_int_sdc(void);
		extern void m68k_int_opm(void);
		extern void m68k_int_opn2(void);
		extern void m68k_int_opl3(void);
		extern void m68k_int_x26(void);
		extern void m68k_int_x27(void);
		extern void m68k_int_beatrix0(void);
		extern void m68k_int_beatrix1(void);
		extern void m68k_int_beatrix2(void);
		extern void m68k_int_beatrix3(void);
		extern void m68k_int_x2C(void);
		extern void m68k_int_dac1(void);
		extern void m68k_int_x2E(void);
		extern void m68k_int_dac0(void);

		const FUNC_V_2_V vecs[] = {
			m68k_int_superio_ps2,  // 64 - Interrupt 0x10 - SuperIO - PS/2 Keyboard
			m68k_int_maurice,      // 65 - Interrupt 0x11 - A2560K Built-in Keyboard (Mo)
			m68k_int_superio_mouse,// 66 - Interrupt 0x12 - SuperIO - PS/2 Mouse
			m68k_int_superio_com1, // 67 - Interrupt 0x13 - SuperIO - COM1
			m68k_int_superio_com2, // 68 - Interrupt 0x14 - SuperIO - COM2
			m68k_int_superio_lpt1, // 69 - Interrupt 0x15 - SuperIO - LPT1
			m68k_int_superio_fdc,  // 70 - Interrupt 0x16 - SuperIO - Floppy Disk Controller
			m68k_int_superio_midi, // 71 - Interrupt 0x17 - SuperIO - MIDI
			m68k_int_timer0,    // 72 - Interrupt 0x18 - Timer 0
			m68k_int_timer1,    // 73 - Interrupt 0x19 - Timer 1
			m68k_int_timer2,    // 74 - Interrupt 0x1A - Timer 2
			m68k_int_timer3,    // 75 - Interrupt 0x1B - Timer 3
			m68k_int_timer4,    // 76 - Interrupt 0x1C - Timer 4
			m68k_int_x1D,     	// 77 - Interrupt 0x1D - Reserved
			m68k_int_x1E,     	// 78 - Interrupt 0x1E - Reserved
			m68k_int_rtc,     	// 79 - Interrupt 0x1F - Real Time Clock
			m68k_int_hdc,     	// 80 - Interrupt 0x20 - IDE HDD Generated Interrupt
			m68k_int_sdc_insert,// 81 - Interrupt 0x21 - SDCard Insert
			m68k_int_sdc,     	// 82 - Interrupt 0x22 - SDCard Controller
			m68k_int_opm,     	// 83 - Interrupt 0x23 - Internal OPM
			m68k_int_opn2,     	// 84 - Interrupt 0x24 - External OPN2
			m68k_int_opl3,     	// 85 - Interrupt 0x25 - External OPL3
			m68k_int_x26,     	// 86 - Interrupt 0x26 - Reserved
			m68k_int_x27,     	// 87 - Interrupt 0x27 - Reserved
			m68k_int_beatrix0,  // 88 - Interrupt 0x28 - Beatrix Interrupt 0
			m68k_int_beatrix1,  // 89 - Interrupt 0x29 - Beatrix Interrupt 1
			m68k_int_beatrix2,  // 90 - Interrupt 0x2A - Beatrix Interrupt 2
			m68k_int_beatrix3,  // 91 - Interrupt 0x2B - Beatrix Interrupt 3
			m68k_int_x2C,     	// 92 - Interrupt 0x2C - Reserved
			m68k_int_dac1,     	// 93 - Interrupt 0x2D - DAC1 Playback Done Interrupt (48K)
			m68k_int_x2E,     	// 94 - Interrupt 0x2E - Reserved
			m68k_int_dac0,     	// 95 - Interrupt 0x2F - DAC0 Playback Done Interrupt (44.1K)
		};
		FUNC_V_2_V *h = (FUNC_V_2_V*)&vecs[0];
		for (i=64; i<64+(sizeof(vecs)/sizeof(FUNC_V_2_V))-1; i++)
			int_install_cpu_vector(i,*h++);
	}
#endif
}

/*
 * Disable an interrupt by masking it
 *
 * Interrupt number is made by the group number and number within the group.
 * For instance, the RTC interrupt would be 0x1F and the Channel A SOF interrupt would be 0x00.
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
void int_disable(uint16_t n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	uint16_t group = int_group(n);

	/* Find the mask for the interrupt */
	uint16_t mask = int_mask(n);
	uint16_t new_mask = MASK_GRP0[group] | mask;

	/* Set the mask bit for the interrupt in the correct MASK register */
	MASK_GRP0[group] = new_mask;
}

/**
 * Sets a 
 * 
 * @param number the vector number (will be multiplied by the size of an address, which depends on the processor)
 * @param handler pointer to the handler to set. If 1, the handler is not set.
 * @return the previous handler.
 */
static FUNC_V_2_V int_install_cpu_vector(int number, void (*handler)()) {
	*((uint32_t*)(number*4)) = (uint32_t)handler;
	return 0L;

	FUNC_V_2_V *address = (FUNC_V_2_V*)(number*4/*sizeof(void*)*/);
	FUNC_V_2_V old = *address;
	
	if (handler != (FUNC_V_2_V)1) {
#if (CPU >= CPU_M68000 && CPU <= CPU_M680EC40) || (CPU == CPU_MC68060)
		if (address < (FUNC_V_2_V*)0x400) // Don't allow to set something that's not a 68K vector
#endif
		*address = handler;
	}

	return old;
}

/*
 * Enable an interrupt
 *
 * Interrupt number is made by the group number and number within the group.
 * For instance, the RTC interrupt would be 0x1F and the Channel A SOF interrupt would be 0x00.
 * And interrupt number of 0xFF specifies that all interrupts should be disabled.
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
void int_enable(uint16_t n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	uint16_t group = int_group(n);

	/* Find the mask for the interrupt */
	uint16_t mask = int_mask(n);
	uint16_t new_mask = MASK_GRP0[group] & ~mask;

	/* Clear the mask bit for the interrupt in the correct MASK register */
	MASK_GRP0[group] = new_mask;
}

/*
 * Register a handler for a given interrupt.
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 * handler = pointer to the interrupt handler to register
 *
 * Returns:
 * the pointer to the previous interrupt handler
 */
p_int_handler int_register(uint16_t n, p_int_handler handler) {
	if (n < MAX_HANDLERS) {
		p_int_handler old_handler = g_int_handler[n];
		g_int_handler[n] = handler;
	    return old_handler;
	} else {
		return 0;
	}
}

/*
 * Return true (non-zero) if an interrupt is pending for the given interrupt
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 *
 * Returns:
 * non-zero if interrupt n is pending, 0 if not
 */
short int_pending(uint16_t n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	uint16_t group = int_group(n);

	/* Find the mask for the interrupt */
	uint16_t mask = int_mask(n);

	/* Set the mask bit for the interrupt in the correct MASK register */
	return (PENDING_GRP0[group] & mask);
}

/*
 * Acknowledge an interrupt (clear out its pending flag)
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
void int_clear(uint16_t n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	uint16_t group = int_group(n);

	/* Find the mask for the interrupt */
	uint16_t mask = int_mask(n);
	uint16_t new_mask = PENDING_GRP0[group] | mask;

	/* Set the bit for the interrupt to mark it as cleared */
	PENDING_GRP0[group] = new_mask;
}

/*
 * Return the group number for the interrupt number
 *
 * For the m68000 machines, this will just be the high nibble of the number
 */
static uint16_t int_group(uint16_t n) {
	return ((n >> 4) & 0x0f);
}

/*
 * Return the mask bit for the interrupt number
 *
 * For the m68000 machines, this will just be the bit corresponding to the lower nibble
 */
static uint16_t int_mask(uint16_t n) {
	return (1 << (n & 0x0f));
}

/**
 * Process VICKY interrupt
 * Inputs:
 * pending_shift = 0 for channel A, and 8 for channel B
 */
void int_vicky(uint16_t pending_shift) {
	uint16_t n;
	uint16_t mask;
	uint16_t group_mask = (0x00ff << pending_shift);
	uint16_t pending = *PENDING_GRP0 & group_mask;
	p_int_handler *handler = &g_int_handler[pending_shift];

	/* Acknowledge all the pending interrupts:
	 * NOTE: we have to do this, even if there is no handler for the interrupt */
	*PENDING_GRP0 = group_mask;

	if (pending == 0)
		return; // That's unexpected ! Pending interrupt occurred (int handler was called) but got acknowledged before we got here ?!
	 
	for (n = pending_shift, mask = (1 << pending_shift); n < (pending_shift+8); n++, mask <<= 1, handler++) {
		if ((pending & mask) && *handler) {
			/* If we got a handler, call it */
			(*handler)();
		}
	}
}
