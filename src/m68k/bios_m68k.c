/**
 * Implementation of 68000 specific BIOS routines.
 * 
 * NOTE: these routines are not called directly but are instead called through TRAP#13
 */

#include "types.h"

#define TEXT_MATRIX_0 ((volatile char *)0x00004000)

typedef void (* int32_terrupt_handler)();

int32_t text_cursor_0 = 0;

int32_t impl_bconout(char c) {
    TEXT_MATRIX_0[text_cursor_0++] = c;
    return 0;
}

// /*
//  * Set an exception handler
//  *
//  * If handler is the nil point32_ter, just return the current value.
//  *
//  * Inputs:
//  * number = the number of the 68000 exception vector (2 - 255)
//  * handler = point32_ter to the handler (must be coded as an int32_terrupt handler)
//  *
//  * Return:
//  * the previous value
//  */
// long impl_setexc(unsigned short number, void (* handler)())  {
//     int32_terrupt_handler * address = 0;
//     long result = 0;
//     if ((number > 1) && (number < 256)) {
//         address = (int32_terrupt_handler *)(number * 2);
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
int32_t bios_dispatch(int32_t function, int32_t param0, int32_t param1, int32_t param2, int32_t param3, int32_t param4, int32_t param5) {
    switch (function) {
        case 1:
            return impl_bconout(param0);

        default:
            return -1;
    }
}