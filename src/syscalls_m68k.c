/*
 * Definitions for calling into the Foenix/MCP BIOS on the 68000 series
 */

#include "syscalls_m68k.h"

/*
 * Call into the BIOS by issuing a TRAP #13
 */
extern int bios(int function, ...);

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
// int setexc(unsigned short number, void (* handler)()) {
//   return bios(5, number, (int)handler);
// }

/*
 * Print a character to the text screen
 *
 * Inputs:
 * c = character to print
 */
int bconout(char c) {
  return bios(1, c);
}