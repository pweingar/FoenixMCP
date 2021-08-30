/*
 * Definitions for calling into the Foenix/MCP BIOS on the 68000 series
 */

#include "syscalls_m68k.h"

/*
 * Call into the BIOS by issuing a TRAP #13
 */
extern int bios(unsigned short function, ...);

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
long setexc(unsigned short number, void (* handler)()) {
  return (long)bios(5, number, handler);
}

/*
 * Print a character to the text screen
 *
 * Inputs:
 * c = character to print
 */
int bconout(char c) {
  return (int)bios(1, c);
}