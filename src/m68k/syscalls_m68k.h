/*
 * Declarations for calling into the Foenix/MCP BIOS on the 68000 series
 *
 * Code calling into the MCP should include this or a similar set of bindings
 * The kernel will use this to allow for system calls to be replaced if needed
 * by other code.
 *
 */

#ifndef __SYSCALLS_M68K_H
#define __SYSCALLS_M68K_H

#include "types.h"

// /*
//  * Set an exception handler
//  *
//  * Inputs:
//  * number = the number of the 68000 exception vector
//  * handler = pointer to the handler (must be coded as an interrupt handler)
//  */
// extern int setexc(unsigned short number, void (* handler)());

/*
 * Print a character to the text screen
 *
 * Inputs:
 * c = character to print
 */
extern int32_t bconout(char c);

#endif
