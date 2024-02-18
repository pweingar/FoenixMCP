/*
 * Macros needed to support system calls
 */

#ifndef _sys_macros_h_
#define _sys_macros_h_

/*
 * Define the machine-specific system call function prefix
 */

#ifdef __CALYPSI_CORE_65816__
#define SYSTEMCALL __attribute__((simple_call))
#else
#define SYSTEMCALL
#endif

#endif