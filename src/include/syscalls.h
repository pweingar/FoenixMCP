/*
 * Declarations for calling into the Foenix/MCP kernel
 *
 * Code calling into the MCP should include this or a similar set of bindings
 * The kernel will use this to allow for system calls to be replaced if needed
 * by other code.
 *
 */

#ifndef __SYSCALLS_M68K_H
#define __SYSCALLS_M68K_H

#include "types.h"

#define SYS_CHAN_WRITE_B    1
#define SYS_CHAN_WRITE      2

/*
 * Call into the kernel (provided by assembly)
 */
extern int32_t syscall(int32_t function, ...);

#endif
