/*
 * Memory commands
 */

#ifndef __MEM_CMDS_H
#define __MEM_CMDS_H

#include "dis68k.h"

/*
 * Print out the contents of a block of memory
 *
 * DUMP <address> [<count>]
 */
extern short mem_cmd_dump(short channel, int argc, char * argv[]);

/*
 * Print out the dissassembly of a block of memory
 *
 * DASM <address> [<count>]
 */
extern short mem_cmd_dasm(short channel, int argc, char * argv[]);

/*
 * Write an 8-bit byte to memory
 *
 * POKE8 <address> <value>
 */
extern short mem_cmd_poke8(short channel, int argc, char * argv[]);

/*
 * Read an 8-bit byte from memory and display it
 *
 * PEEK8 <address>
 */
extern short mem_cmd_peek8(short channel, int argc, char * argv[]);

/*
 * Write an 16-bit word to memory
 *
 * POKE16 <address> <value>
 */
extern short mem_cmd_poke16(short channel, int argc, char * argv[]);

/*
 * Read an 16-bit word from memory and display it
 *
 * PEEK16 <address>
 */
extern short mem_cmd_peek16(short channel, int argc, char * argv[]);

/*
 * Write an 32-bit long word to memory
 *
 * POKE32 <address> <value>
 */
extern short mem_cmd_poke32(short channel, int argc, char * argv[]);

/*
 * Read an 32-bit long word from memory and display it
 *
 * PEEK32 <address>
 */
extern short mem_cmd_peek32(short channel, int argc, char * argv[]);

#endif
