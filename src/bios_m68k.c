/**
 * Implementation of 68000 specific BIOS routines.
 * 
 * NOTE: these routines are not called directly but are instead called through TRAP#13
 */

#define TEXT_MATRIX_0 ((volatile char *)0x00AFA000)

typedef void (* interrupt_handler)();

static int text_cursor_0 = 0;

int impl_bconout(long context, char c) {
    TEXT_MATRIX_0[text_cursor_0++] = c;
    return 0;
}

/*
 * Set an exception handler
 *
 * If handler is the nil pointer, just return the current value.
 *
 * Inputs:
 * number = the number of the 68000 exception vector (2 - 255)
 * handler = pointer to the handler (must be coded as an interrupt handler)
 *
 * Return:
 * the previous value
 */
long impl_setexc(long context, unsigned short number, void (* handler)())  {
    interrupt_handler * address = 0;
    long result = 0;
    if ((number > 1) && (number < 256)) {
        address = (interrupt_handler *)(number * 2);
        result = (long)(*address);
        if (handler != 0) {
            *address = handler;
        }
    }

    return result;
}