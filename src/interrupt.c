/*
 * Definitions for the interrupt controls
 */

#include "interrupt.h"
#include "dev/text_screen_iii.h"

#define MAX_HANDLERS 48

p_int_handler g_int_handler[MAX_HANDLERS];

/*
 * Return the group number for the interrupt number
 *
 * For the m68000 machines, this will just be the high nibble of the number
 */
unsigned short int_group(unsigned short n) {
	return ((n >> 4) & 0x0f);
}

/*
 * Return the mask bit for the interrupt number
 *
 * For the m68000 machines, this will just be the bit corresponding to the lower nibble
 */
unsigned short int_mask(unsigned short n) {
	return (1 << (n & 0x0f));
}

/*
 * Initialize the interrupt registers
 */
void int_init() {
	int i;

	for (i = 0; i < MAX_HANDLERS; i++) {
		g_int_handler[i] = 0;
	}

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
void int_disable(unsigned short n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	unsigned short group = int_group(n);

	/* Find the mask for the interrupt */
	unsigned short mask = int_mask(n);
	unsigned short new_mask = MASK_GRP0[group] | mask;

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
void int_enable(unsigned short n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	unsigned short group = int_group(n);

	/* Find the mask for the interrupt */
	unsigned short mask = int_mask(n);
	unsigned short new_mask = MASK_GRP0[group] & ~mask;

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
p_int_handler int_register(unsigned short n, p_int_handler handler) {
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
short int_pending(unsigned short n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	unsigned short group = int_group(n);

	/* Find the mask for the interrupt */
	unsigned short mask = int_mask(n);

	/* Set the mask bit for the interrupt in the correct MASK register */
	return (PENDING_GRP0[group] & mask);
}

/*
 * Acknowledge an interrupt (clear out its pending flag)
 *
 * Inputs:
 * n = the number of the interrupt: n[7..4] = group number, n[3..0] = individual number.
 */
void int_clear(unsigned short n) {
	/* Find the group (the relevant interrupt mask register) for the interrupt */
	unsigned short group = int_group(n);

	/* Find the mask for the interrupt */
	unsigned short mask = int_mask(n);
	unsigned short new_mask = PENDING_GRP0[group] | mask;

	/* Set the bit for the interrupt to mark it as cleared */
	PENDING_GRP0[group] = new_mask;
}

/*
 * Interrupt dispatcher for Vicky Channel A interrupts (0 - 7)
 */
void int_vicky_channel_a() {
	unsigned short n;
	unsigned short mask = 1;
	unsigned short pending = *PENDING_GRP0 & 0xff;

	/* Acknowledge all the pending interrupts:
	 * NOTE: we have to do this, even if there is no handler for the interrupt */
	*PENDING_GRP0 = 0x00ff;

	if (pending != 0) {
		for (n = 0; n < 8; n++) {
			if (pending & mask) {
				p_int_handler handler = g_int_handler[n];

				if (handler) {
					/* If we got a handler, call it */
					handler();
				}
			}

			// Compute the next mask
			mask = mask << 1;
		}
	}
}

/*
 * Interrupt dispatcher for Vicky Channel B interrupts (8 - 15)
 */
void int_vicky_channel_b() {
	unsigned short n;
	unsigned short mask = 1;
	unsigned short pending = (*PENDING_GRP0 >> 8) & 0xff;

	/* Acknowledge all the pending interrupts:
	 * NOTE: we have to do this, even if there is no handler for the interrupt */
	*PENDING_GRP0 = 0xff00;

	if (pending != 0) {
		for (n = 8; n < 16; n++) {
			if (pending & mask) {
				p_int_handler handler = g_int_handler[n];
				if (handler) {
					/* If we got a handler, call it */
					handler();
				}
			}

			// Compute the next mask
			mask = mask << 1;
		}
	}
}
