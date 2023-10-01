/*
 * Definitions for the interrupt controls for  the C256 machines
 */

#include <calypsi/intrinsics65816.h>
#include <string.h>

#include "features.h"
#include "interrupt.h"
#include "log.h"

// FoenixMCP interrupt number -> C256 group and mask:

// INT_SOF_A           0x00	-->	0x00, 0x01 (Main screen start-of-frame)
// INT_SOL_A           0x01	-->	0x00, 0x02 (Main screen start-of-line)
// INT_VICKY_A_1       0x02	-->	0x01, 0x02 (Sprite Collision)
// INT_VICKY_A_2       0x03	-->	0x01, 0x04 (Bitmap Collision)
// INT_VICKY_A_3       0x04	-->	0x02, 0x08 (VDMA Interrupt)
// INT_VICKY_A_4       0x05	-->	0x02, 0x10 (Tile Collision)
// INT_RESERVED_1      0x06	-->	0x02, 0x40 (External Expansion)
// INT_VICKY_A_DAC     0x07
// INT_SOF_B           0x08
// INT_SOL_B           0x09
// INT_VICKY_B_1       0x0A
// INT_VICKY_B_2       0x0B
// INT_VICKY_B_3       0x0C
// INT_VICKY_B_4       0x0D
// INT_RESERVED_2      0x0E
// INT_VICKY_B_DAC     0x0F

// INT_KBD_PS2         0x10	-->	0x01, 0x01 (PS/2 Keyboard)
// INT_KBD_A2560K      0x11
// INT_MOUSE           0x12	-->	0x00, 0x80 (PS/2 Mouse)
// INT_COM1            0x13	-->	0x01, 0x10 (Serial port 1)
// INT_COM2            0x14	-->	0x01, 0x08 (Serial port 2)
// INT_LPT1            0x15	-->	0x01, 0x40 (LPT)
// INT_FDC             0x16	-->	0x00, 0x40 (Floppy drive)
// INT_MIDI            0x17	-->	0x01, 0x20 (MIDI)
// INT_TIMER0          0x18	-->	0x00, 0x04 (Timer 0)
// INT_TIMER1          0x19	-->	0x00, 0x08 (Timer 1)
// INT_TIMER2          0x1A	-->	0x00, 0x10 (Timer 2)
// INT_TIMER3          0x1B
// INT_TIMER4          0x1C
// INT_RESERVED_3      0x1D
// INT_RESERVED_4      0x1E
// INT_RTC             0x1F	-->	0x00, 0x20 (Real time clock)

// INT_PATA            0x20	-->	0x03, 0x04 (IDE)
// INT_SDC_INS         0x21	-->	0x02, 0x80 (SDC inserted)
// INT_SDC             0x22	-->	0x01, 0x80 (SDC)
// INT_OPM_INT         0x23	-->	0x03, 0x02 (OPM)
// INT_OPN2_EXT        0x24	-->	0x03, 0x01 (OPN)
// INT_OPL3_EXT        0x25	-->	0x02, 0x01 (OPL3)
// INT_RESERVED_5      0x26
// INT_RESERVED_6      0x27
// INT_BEATRIX_0       0x28
// INT_BEATRIX_1       0x29
// INT_BEATRIX_2       0x2A
// INT_BEATRIX_3       0x2B
// INT_RESERVED_7      0x2C
// INT_DAC1_PB         0x2D    /* DAC1 Playback Done (48K) */
// INT_RESERVED_8      0x2E    /* Reserved */
// INT_DAC0_PB         0x2F    /* DAC0 Playback Done (44.1K) */

//
// Interrupt Handler Vectors
//

p_int_handler int_handle_00;
p_int_handler int_handle_01;
p_int_handler int_handle_02;
p_int_handler int_handle_03;
p_int_handler int_handle_04;
p_int_handler int_handle_05;
p_int_handler int_handle_06;
p_int_handler int_handle_07;

p_int_handler int_handle_10;
p_int_handler int_handle_11;
p_int_handler int_handle_12;
p_int_handler int_handle_13;
p_int_handler int_handle_14;
p_int_handler int_handle_15;
p_int_handler int_handle_16;
p_int_handler int_handle_17;

p_int_handler int_handle_20;
p_int_handler int_handle_21;
p_int_handler int_handle_22;
p_int_handler int_handle_23;
p_int_handler int_handle_24;
p_int_handler int_handle_25;
p_int_handler int_handle_26;
p_int_handler int_handle_27;

p_int_handler int_handle_30;
p_int_handler int_handle_31;
p_int_handler int_handle_32;
p_int_handler int_handle_33;
p_int_handler int_handle_34;
p_int_handler int_handle_35;
p_int_handler int_handle_36;
p_int_handler int_handle_37;

/**
 * @brief Mapping of FoenixMCP interrupt numbers to C256 GABE group numbers (0xff indicates an unassigned interrupt number)
 * 
 */
static unsigned short g_int_group[] = {
	0x00, 0x00,	0x01, 0x01,	0x02, 0x02,	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x01, 0xff, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
	0x03, 0x02, 0x01, 0x03, 0x03, 0x02,	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/**
 * @brief Mapping of FoenixMCP interrupt numbers to C256 GABE mask numbers (0xff indicates an unassigned interrupt number)
 * 
 */
static unsigned short g_int_mask[] = {
	0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x40, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x01, 0xff, 0x80, 0x10, 0x08, 0x40, 0x40, 0x20, 0x04, 0x08, 0x10, 0xff, 0xff, 0xff, 0xff, 0x20,
	0x04, 0x80, 0x80, 0x02, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/*
 * Return the group number for the interrupt number
 */
unsigned short int_group(unsigned short n) {
	return g_int_group[n];
}

/*
 * Return the mask bit for the interrupt number
 */
unsigned short int_mask(unsigned short n) {
	return g_int_mask[n];
}

/*
 * Initialize the interrupt registers
 */
void int_init() {
	int i;
	p_int_handler * int_handlers = &int_handle_00;

	// Clear all the interrupt handlers
	for (i = 0; i < 4 * 8; i++) {
		int_handlers[i] = 0;
	}

    // At Reset, all of those already have those values
	// the Pol are @ 0x0000 and normally pending are reseted, but it is not impossible that some might be triggered during init

	*EDGE_GRP0 = 0xFF;
	*EDGE_GRP1 = 0xFF;
	*EDGE_GRP2 = 0xFF;
	*EDGE_GRP3 = 0xFF;

	*MASK_GRP0 = 0xFF;
	*MASK_GRP1 = 0xFF;
	*MASK_GRP2 = 0xFF;
	*MASK_GRP3 = 0xFF;

	// Clear all the pending flags

	*PENDING_GRP0 = 0xFF;
	*PENDING_GRP1 = 0xFF;
	*PENDING_GRP2 = 0xFF;
	*PENDING_GRP3 = 0xFF;
}

/*
 * Enable all interrupts
 *
 * Returns:
 * a machine dependent representation of the interrupt masking prior to enabling
 */
short int_enable_all() {
	// NOTE: this code uses Calypsi specific intrinsic functions
	//       and does a cast that may not be valid

	short status = (short)__get_interrupt_state();
	__enable_interrupts();
	return status;
}

/*
 * Disable all interrupts
 *
 * Returns:
 * a machine dependent representation of the interrupt masking prior to disabling
 */
short int_disable_all() {
	// NOTE: this code uses Calypsi specific intrinsic functions
	//       and does a cast that may not be valid

	short status = (short)__get_interrupt_state();
	__disable_interrupts();
	return status;
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

	if ((group != 0xff) && (mask != 0xff)) {
		// Only set the mask if the mask and group numbers are valid
		uint8_t new_mask = MASK_GRP0[group] | mask;

		/* Set the mask bit for the interrupt in the correct MASK register */
		MASK_GRP0[group] = new_mask;
	}

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

	printf("Enable interrupt %d => group: %d, mask: %d\n", n, group, mask);

	if ((group != 0xff) && (mask != 0xff)) {
		// Only set the mask if the mask and group numbers are valid
		uint8_t new_mask = MASK_GRP0[group] & ~mask;

		/* Clear the mask bit for the interrupt in the correct MASK register */
		MASK_GRP0[group] = new_mask;
	}
}

/**
 * @brief Convert group and mask to an index into a "array" of interrupt handler pointers
 * 
 * @param group the number of the interrupt's group
 * @param mask the interupt's mask bit
 * @return int the offset to the handler (-1 on error)
 */
static int int_group_mask_to_offset(unsigned short group, unsigned short mask) {
	if ((group != 0xff) && (mask != 0xff)) {
		unsigned short position = 0;
		switch (mask) {
			case 0x01:
				position = 0;
				break;

			case 0x02:
				position = 1;
				break;

			case 0x04:
				position = 2;
				break;

			case 0x08:
				position = 3;
				break;

			case 0x10:
				position = 4;
				break;

			case 0x20:
				position = 5;
				break;

			case 0x40:
				position = 6;
				break;

			case 0x80:
				position = 7;
				break;

			default:
				break;
		}

		return group * 8 + position;
	} else {
		return -1;
	}
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
	p_int_handler * handler_ref = 0;
	p_int_handler old_handler = 0;

	/* Find the group (the relevant interrupt mask register) for the interrupt */
	unsigned short group = int_group(n);

	/* Find the mask for the interrupt */
	unsigned short mask = int_mask(n);

	switch(group) {
		case 0:
			switch(mask) {
				case 1:
					handler_ref = &int_handle_00;
					break;
				case 2:
					handler_ref = &int_handle_01;
					break;
				case 4:
					handler_ref = &int_handle_02;
					break;
				case 8:
					handler_ref = &int_handle_03;
					break;
				case 16:
					handler_ref = &int_handle_04;
					break;
				case 32:
					handler_ref = &int_handle_05;
					break;
				case 64:
					handler_ref = &int_handle_06;
					break;
				case 128:
					handler_ref = &int_handle_07;
					break;
				default:
					return 0;
			}
			break;

		case 1:
			switch(mask) {
				case 1:
					handler_ref = &int_handle_10;
					break;
				case 2:
					handler_ref = &int_handle_11;
					break;
				case 4:
					handler_ref = &int_handle_12;
					break;
				case 8:
					handler_ref = &int_handle_13;
					break;
				case 16:
					handler_ref = &int_handle_14;
					break;
				case 32:
					handler_ref = &int_handle_15;
					break;
				case 64:
					handler_ref = &int_handle_16;
					break;
				case 128:
					handler_ref = &int_handle_17;
					break;
				default:
					return  0;
			}
			break;

		case 2:
			switch(mask) {
				case 1:
					handler_ref = &int_handle_20;
					break;
				case 2:
					handler_ref = &int_handle_21;
					break;
				case 4:
					handler_ref = &int_handle_22;
					break;
				case 8:
					handler_ref = &int_handle_23;
					break;
				case 16:
					handler_ref = &int_handle_24;
					break;
				case 32:
					handler_ref = &int_handle_25;
					break;
				case 64:
					handler_ref = &int_handle_26;
					break;
				case 128:
					handler_ref = &int_handle_27;
					break;
				default:
					return 0;
			}
			break;

		case 3:
			switch(mask) {
				case 1:
					handler_ref = &int_handle_30;
					break;
				case 2:
					handler_ref = &int_handle_31;
					break;
				case 4:
					handler_ref = &int_handle_32;
					break;
				case 8:
					handler_ref = &int_handle_33;
					break;
				case 16:
					handler_ref = &int_handle_34;
					break;
				case 32:
					handler_ref = &int_handle_35;
					break;
				case 64:
					handler_ref = &int_handle_36;
					break;
				case 128:
					handler_ref = &int_handle_37;
					break;
				default:
					return 0;
			}
			break;

		default:
			return 0;
	}

	old_handler = *handler_ref;
	*handler_ref = handler;

	return old_handler;
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

	if ((group != 0xff) && (mask != 0xff)) {
		// Only query the pending mask if the mask and group numbers are valid
		return (PENDING_GRP0[group] & mask);

	} else {
		// If the mask or group number are invalid, just return false
		return 0;
	}
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

	if ((group != 0xff) && (mask != 0xff)) {
		// Only set the mask if the mask and group numbers are valid
		uint8_t new_mask = PENDING_GRP0[group] | mask;

		/* Set the bit for the interrupt to mark it as cleared */
		PENDING_GRP0[group] = new_mask;
	}
}

/**
 * @brief Handle incomming IRQ signal
 * 
 * NOTE: this routine might not be fast enough in C and have to be replaced by hand written
 *       assembly code... but we will try it this way first.
 */
__attribute__((interrupt(0xffee))) void int_handle_irq() {
	uint8_t mask_bits = 0;

	// Process any pending interrupts in group 0
	mask_bits = *PENDING_GRP0;
	if (mask_bits) {
		if ((mask_bits & 0x01) && int_handle_00) int_handle_00();	// Start of frame
		if ((mask_bits & 0x02) && int_handle_01) int_handle_01();	// Start of line
		if ((mask_bits & 0x04) && int_handle_02) int_handle_02();	// Timer 0
		if ((mask_bits & 0x08) && int_handle_03) int_handle_03();	// Timer 1
		if ((mask_bits & 0x10) && int_handle_04) int_handle_04();	// Timer 2
		if ((mask_bits & 0x20) && int_handle_05) int_handle_05();	// Realtime Clock
		if ((mask_bits & 0x40) && int_handle_06) int_handle_06();	// Floppy Disk Controller
		if ((mask_bits & 0x80) && int_handle_07) int_handle_07();	// PS/2 Mouse

		// Clear the pending bits for group 0
		*PENDING_GRP0 = mask_bits;
	}

	// Process any pending interrupts in group 1
	mask_bits = *PENDING_GRP1;
	if (mask_bits) {
		volatile __attribute__((far)) char * text = (char *)0xafa000;
		*text = *text + 1;

		if ((mask_bits & 0x01) && int_handle_10) {
			volatile __attribute__((far)) char * text = (char *)0xafa001;
			*text = *text + 1;
			int_handle_10();	// PS/2 Keyboard
		} 
		if ((mask_bits & 0x02) && int_handle_11) int_handle_11();	// VICKY II Sprite Collision
		if ((mask_bits & 0x04) && int_handle_12) int_handle_12();	// VICKY II Bitmap Collision
		if ((mask_bits & 0x08) && int_handle_13) int_handle_13();	// Serial Port #2
		if ((mask_bits & 0x10) && int_handle_14) int_handle_14();	// Serial Port #1
		if ((mask_bits & 0x20) && int_handle_15) int_handle_15();	// MIDI Controller
		if ((mask_bits & 0x40) && int_handle_16) int_handle_16();	// Parallel Port
		if ((mask_bits & 0x80) && int_handle_17) int_handle_17();	// SD Controller

		// Clear the pending bits for group 1
		*PENDING_GRP1 = mask_bits;
	}

	// Process any pending interrupts in group 2
	mask_bits = *PENDING_GRP2;
	if (mask_bits) {
		if ((mask_bits & 0x01) && int_handle_20) int_handle_20();	// OPL3
		// if (mask_bits & 0x02) int_handle_21(); // Currently unused
		// if (mask_bits & 0x04) int_handle_22(); // Currently unused
		if ((mask_bits & 0x08) && int_handle_23) int_handle_23();	// VICKY II VDMA
		if ((mask_bits & 0x10) && int_handle_24) int_handle_24();	// VICKY II Tile Collision
		// if (mask_bits & 0x20) int_handle_25(); // Currently unused
		if ((mask_bits & 0x40) && int_handle_26) int_handle_26();	// External Expansion
		if ((mask_bits & 0x80) && int_handle_27) int_handle_27();	// SD Insert

		// Clear the pending bits for group 2
		*PENDING_GRP2 = mask_bits;
	}

	// Process any pending interrupts in group 3
	mask_bits = *PENDING_GRP3;
	if (mask_bits) {
		if ((mask_bits & 0x01) && int_handle_30) int_handle_30();	// OPN2
		if ((mask_bits & 0x02) && int_handle_31) int_handle_31();	// OPM
		if ((mask_bits & 0x04) && int_handle_32) int_handle_32();	// IDE
		// // if (mask_bits & 0x08) int_handle_33(); // Currently unused
		// // if (mask_bits & 0x10) int_handle_34(); // Currently unused
		// // if (mask_bits & 0x20) int_handle_35(); // Currently unused
		// // if (mask_bits & 0x40) int_handle_36(); // Currently unused
		// // if (mask_bits & 0x80) int_handle_37(); // Currently unused

		// Clear the pending bits for group 3
		*PENDING_GRP3 = mask_bits;
	}
}

/**
 * @brief Handle incomming NMI signal
 */
__attribute__((interrupt(0xffea))) void int_handle_nmi() {
}