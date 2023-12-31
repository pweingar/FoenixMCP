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

void int_vicky(int pending_shift) {
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


/*
 * Interrupt dispatcher for Vicky Channel A interrupts (0 - 7)
 */
void int_vicky_channel_a() {
	int_vicky(0);
}

#if HAS_DUAL_SCREEN
/*
 * Interrupt dispatcher for Vicky Channel B interrupts (8 - 15)
 */
void int_vicky_channel_b() {
	int_vicky(8);
}
#endif