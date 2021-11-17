/*
 * Memory commands
 */

#include "log.h"
#include "cli.h"
#include "simpleio.h"
#include "sys_general.h"
#include "syscalls.h"
#include "mem_cmds.h"

/*
 * Print out the contents of a block of memory
 *
 * DUMP <address> [<count>]
 */
short mem_cmd_dump(short channel, int argc, char * argv[]) {
    unsigned char * address = 0;
    long count = 16;
    long i;


    TRACE("mem_cmd_dump");

    if (argc >= 2) {
        address = (unsigned char *)cli_eval_number(argv[1]);

        if (argc > 2) {
            count = cli_eval_number(argv[2]);
        }

        dump_buffer(channel, address, count, 2);

        return  0;
    } else {
        log(LOG_ERROR, "USAGE: DUMP <address> <count>");
        return -1;
    }
}

short mem_cmd_dasm(short channel, int argc, char * argv[]) {
    unsigned long address = 0;
    long count = 1000;
    long i;

    TRACE("mem_cmd_dasm");

#if defined(__m68k__) || defined(__M68K__)
    if (argc >= 2) {
        address = cli_eval_number(argv[1]);

        if (argc > 2) {
            count = cli_eval_number(argv[2]);
        }
        print(0, "=================\n");
        disasm(address, address + count);
        print(0, "=================\n");

        return  0;
    } else {
        log(LOG_ERROR, "USAGE: DASM <address> <count>");
        return -1;
    }
#else
    log(LOG_ERROR, "DASM only available on m68k machines");
    return -1;
#endif

}

/*
 * Write an 8-bit byte to memory
 *
 * POKE8 <address> <value>
 */
short mem_cmd_poke8(short channel, int argc, char * argv[]) {
    TRACE("mem_cmd_poke8");

    if (argc == 3) {
        unsigned char * address = (unsigned char *)cli_eval_number(argv[1]);
        unsigned char c = (unsigned char)cli_eval_number(argv[2]);
        *address = c;

        return 0;
    } else {
        log(LOG_ERROR, "USAGE: POKE8 <address> <value>");
        return -1;
    }
}

/*
 * Read an 8-bit byte from memory and display it
 *
 * PEEK8 <address>
 */
short mem_cmd_peek8(short channel, int argc, char * argv[]) {

    TRACE("mem_cmd_peek8");

    if (argc == 2) {
        unsigned char * address = (unsigned char *)cli_eval_number(argv[1]);
        unsigned char c = *address;
        print_hex_8(channel, c);

        return c;
    } else {
        log(LOG_ERROR, "USAGE: PEEK8 <address>");
        return -1;
    }
}

/*
 * Write an 16-bit word to memory
 *
 * POKE16 <address> <value>
 */
short mem_cmd_poke16(short channel, int argc, char * argv[]) {
    TRACE("mem_cmd_poke16");

    if (argc == 3) {
        unsigned short * address = (unsigned short *)cli_eval_number(argv[1]);
        unsigned short c = (unsigned short)cli_eval_number(argv[2]);
        *address = c;

        return 0;
    } else {
        log(LOG_ERROR, "USAGE: POKE16 <address> <value>");
        return -1;
    }
}

/*
 * Read an 16-bit word from memory and display it
 *
 * PEEK16 <address>
 */
short mem_cmd_peek16(short channel, int argc, char * argv[]) {
    TRACE("mem_cmd_peek16");

    if (argc == 2) {
        unsigned short * address = (unsigned char *)cli_eval_number(argv[1]);
        unsigned short c = *address;
        print_hex_16(channel, c);

        return c;
    } else {
        log(LOG_ERROR, "USAGE: PEEK16 <address>");
        return -1;
    }
}

/*
 * Write an 32-bit long word to memory
 *
 * POKE32 <address> <value>
 */
short mem_cmd_poke32(short channel, int argc, char * argv[]) {
    TRACE("mem_cmd_poke32");

    if (argc == 3) {
        unsigned long * address = (unsigned long *)cli_eval_number(argv[1]);
        unsigned long c = (unsigned long)cli_eval_number(argv[2]);
        *address = c;

        return 0;
    } else {
        log(LOG_ERROR, "USAGE: POKE32 <address> <value>");
        return -1;
    }
}

/*
 * Read an 32-bit long word from memory and display it
 *
 * PEEK32 <address>
 */
short mem_cmd_peek32(short channel, int argc, char * argv[]) {
    TRACE("mem_cmd_peek32");

    if (argc == 2) {
        unsigned long * address = (unsigned char *)cli_eval_number(argv[1]);
        unsigned long c = *address;
        print_hex_32(channel, c);

        return 0;
    } else {
        log(LOG_ERROR, "USAGE: PEEK32 <address>");
        return -1;
    }
}
