/**
 * Implementation of 68000 specific BIOS routines.
 * 
 * NOTE: these routines are not called directly but are instead called through TRAP#13
 */

#define TEXT_MATRIX_0 ((volatile char *)0x00004000)

typedef void (* interrupt_handler)();

int text_cursor_0 = 0;

int impl_bconout(char c) {
    TEXT_MATRIX_0[text_cursor_0++] = c;
    return 0;
}

// /*
//  * Set an exception handler
//  *
//  * If handler is the nil pointer, just return the current value.
//  *
//  * Inputs:
//  * number = the number of the 68000 exception vector (2 - 255)
//  * handler = pointer to the handler (must be coded as an interrupt handler)
//  *
//  * Return:
//  * the previous value
//  */
// long impl_setexc(unsigned short number, void (* handler)())  {
//     interrupt_handler * address = 0;
//     long result = 0;
//     if ((number > 1) && (number < 256)) {
//         address = (interrupt_handler *)(number * 2);
//         result = (long)(*address);
//         if (handler != 0) {
//             *address = handler;
//         }
//     }

//     return result;
// }

/*
 * Determine the correct BIOS function implementation to call and call it.
 */
int bios_dispatch(int function, int param0, int param1, int param2, int param3, int param4, int param5) {
    switch (function) {
        case 1:
            return impl_bconout(param0);

        default:
            return -1;
    }
}