/**
 * Implementation of the command line interface
 */

#include <ctype.h>
#include <string.h>
#include "log.h"
#include "types.h"
#include "interrupt.h"
#include "simpleio.h"
#include "syscalls.h"
#include "sys_general.h"
#include "timers.h"
#include "cli/cli.h"
#include "cli/dos_cmds.h"
#include "cli/mem_cmds.h"
#include "cli/settings.h"
#include "cli/sound_cmds.h"
#include "dev/ps2.h"
#include "dev/rtc.h"
#include "dev/uart.h"
#include "uart_reg.h"
#include "rtc_reg.h"
#include "vicky_general.h"

#define MAX_COMMAND_SIZE    128
#define MAX_ARGC            32

//
// Types
//

// Structure to hold a record about a command...

typedef struct s_cli_command {
    char *name;
    char *help;
    cli_cmd_handler handler;
} t_cli_command, *p_cli_command;

extern short cmd_sysinfo(short channel, int argc, char * argv[]);
extern short cmd_cls(short channel, int argc, char * argv[]);
extern short cmd_showint(short channel, int argc, char * argv[]);
extern short cmd_getjiffies(short channel, int argc, char * argv[]);
extern short cmd_testuart(short channel, int argc, char * argv[]);
extern short cmd_get_ticks(short channel, int argc, char * argv[]);
extern short cmd_testrtc(short channel, int argc, char * argv[]);
extern short cmd_testpanic(short channel, int argc, char * argv[]);

/*
 * Variables
 */

short g_current_channel = 0;

const t_cli_command g_cli_commands[] = {
    { "?", "? : print this helpful message", cmd_help },
    { "HELP", "HELP : print this helpful message", cmd_help },
    { "CD", "CD <path> : sets the current directory", cmd_cd },
    { "CLS", "CLS : clear the screen", cmd_cls },
    { "DEL", "DEL <path> : delete a file or directory", cmd_del },
    { "DIR", "DIR <path> : print directory listing", cmd_dir },
    { "DISKFILL", "DISKFILL <drive #> <sector #> <byte value>", cmd_diskfill },
    { "DISKREAD", "DISKREAD <drive #> <sector #>", cmd_diskread },
    { "DUMP", "DUMP <addr> [<count>] : print a memory dump", mem_cmd_dump},
    { "GETJIFFIES", "GETJIFFIES : print the number of jiffies since bootup", cmd_getjiffies },
    { "GETTICKS", "GETTICKS : print number of ticks since reset", cmd_get_ticks },
    { "LABEL", "LABEL <drive#> <label> : set the label of a drive", cmd_label },
    { "LOAD", "LOAD <path> : load a file into memory", cmd_load },
    { "MKDIR", "MKDIR <path> : create a directory", cmd_mkdir },
    { "PEEK8", "PEEK8 <addr> : print the byte at the address in memory", mem_cmd_peek8 },
    { "PEEK16", "PEEK16 <addr> : print the 16-bit word at the address in memory", mem_cmd_peek16 },
    { "PEEK32", "PEEK32 <addr> : print the 32-bit long word at the address in memory", mem_cmd_peek32 },
    { "POKE8", "POKE8 <addr> <value> : write the byte value to the address in memory", mem_cmd_poke8 },
    { "POKE16", "POKE16 <addr> <value> : write the 16-bit value to the address in memory", mem_cmd_poke16 },
    { "POKE32", "POKE32 <addr> <value> : write the 32-bit value to the address in memory", mem_cmd_poke32 },
    { "PWD", "PWD : prints the current directory", cmd_pwd },
    // { "REN", "REN <old path> <new path> : rename a file or directory", cmd_rename },
    { "RUN", "RUN <path> : execute a binary file",  cmd_run },
    { "SET", "SET <name> <value> : set the value of a setting", cli_cmd_set },
    { "GET", "GET <name> : get the value of a setting", cli_cmd_get },
    { "SHOWINT", "SHOWINT : Show information about the interrupt registers", cmd_showint },
    { "SYSINFO", "SYSINFO : prints information about the system", cmd_sysinfo },
    { "TESTCREATE", "TESTCREATE <path> : tries to create a file", cmd_testcreate },
    { "TESTIDE", "TESTIDE : fetches and prints the IDE MBR repeatedly", cmd_testide },
    { "TESTMEM", "TESTMEM : Test core RAM", mem_test },
    { "TESTOPL3", "TESTOPL3 : play a tone on the OPL3", opl3_test },
    { "TESTPANIC", "TESTPANIC : Do a division by 0 to test the panic screen", cmd_testpanic },
    { "TESTPSG", "TESTPSG : play some notes on the PSG", psg_test },
    { "TESTRTC", "TESTRTC : poll the RTC for a periodic interrupt", cmd_testrtc },
    { "TESTUART", "TESTUART : echo key presses between terminal and console", cmd_testuart },
    { "TYPE", "TYPE <path> : print the contents of a text file", cmd_type },
    { 0, 0 }
};

//
// List all the commands
//
int cmd_help(short channel, int argc, char * argv[]) {
    p_cli_command command;

    for (command = g_cli_commands; (command != 0) && (command->name != 0); command++) {
        sys_chan_write(channel, command->help, strlen(command->help));
        sys_chan_write(channel, "\n", 2);
    }
    return 0;
}

short cmd_testpanic(short channel, int argc, char * argv[]) {
    volatile int x = 0;
    return argc / x;
}

short cmd_getjiffies(short channel, int argc, char * argv[]) {
    char buffer[80];

    sprintf(buffer, "%d\n", timers_jiffies());
    sys_chan_write(channel, buffer, strlen(buffer));;
    return 0;
}

/*
 * Try using the RTC periodic interrupt in polled mode
 */
short cmd_testrtc(short channel, int argc, char * argv[]) {
    char buffer[80];
    char * spinner = "|/-\\";
    short count = 0;
    long ticks;

    *RTC_RATES = 0x0e;          /* Periodic interrupt rate: 250 ms */
    *RTC_ENABLES = RTC_PIE;     /* Turn on the periodic interrupt */
    int_enable(INT_RTC);

    ticks = rtc_get_ticks();

    sprintf(buffer, "Waiting for updated ticks starting from %d\n", ticks);
    sys_chan_write(channel, buffer, strlen(buffer));

    while (1) {
        if (ticks < rtc_get_ticks()) {
            /* We got the periodic interrupt */

            sprintf(buffer, "Tick! %d\n", ticks);
            sys_chan_write(channel, buffer, strlen(buffer));

            ticks = rtc_get_ticks();
        }
    }
}

/*
 * Print the number of ticks since last restart
 */
short cmd_get_ticks(short channel, int argc, char * argv[]) {
    char buffer[80];

    sprintf(buffer, "%d\n", rtc_get_ticks());
    sys_chan_write(channel, buffer, strlen(buffer));
    return 0;
}

short cmd_testuart(short channel, int argc, char * argv[]) {
    char c;
    char buffer[80];

    uart_init(0);
    uart_setbps(0, UART_115200);
    uart_setlcr(0, LCR_DATABITS_8 | LCR_STOPBIT_1 | LCR_PARITY_NONE);

    sprintf(buffer, "COM1: 115200, no parity, 1 stop bit, 8 data bits\nPress ESC to finish.\n");
    sys_chan_write(0, buffer, strlen(buffer));

    while (1) {
        c = kbd_getc();
        if (c != 0) {
            if (c == 0x1b) {
                return 0;
            }
            uart_put(0, c);
        } else if (uart_has_bytes(0)) {
            c = uart_get(0);
            sys_chan_write_b(channel, c);
        }
    }
}

/*
 * Clear the screen
 */
short cmd_cls(short channel, int argc, char * argv[]) {
    const char * ansi_cls = "\x1B[2J\x1B[H";

    sys_chan_write(channel, ansi_cls, strlen(ansi_cls));
    return 0;
}

/*
 * Display information about the system
 */
short cmd_sysinfo(short channel, int argc, char * argv[]) {
    t_sys_info info;
    char buffer[80];

    sys_get_info(&info);
    print(channel, "System information:\nModel: ");
    print(channel, info.model_name);

    print(channel, "\nCPU: ");
    print(channel, info.cpu_name);

    print(channel, "\nMemory: ");
    print_hex_32(channel, info.system_ram_size);

    print(channel, "\nGABE version: ");
    print_hex_16(channel, info.gabe_number);
    print(channel, ".");
    print_hex_16(channel, info.gabe_version);
    print(channel, ".");
    print_hex_16(channel, info.gabe_subrev);

    print(channel, "\nVICKY version: ");
    print_hex_16(channel, info.vicky_rev);

    sprintf(buffer, "\nMCP version: v%02d.%02d.%04d", info.mcp_version, info.mcp_rev, info.mcp_build);
    print(channel, buffer);

    print(channel, "\n");

    return 0;
}

/*
 * Show information about the interrupt registers
 */
short cmd_showint(short channel, int argc, char * argv[]) {
    char buffer[80];

    TRACE("cmd_showint");

    print(channel, "Interrupt control registers...\n\n");
    print(channel, "          GRP0 GRP1 GRP2\n");

    sprintf(buffer, "Polarity: %04x %04x %04x\n", *POL_GRP0, *POL_GRP1, *POL_GRP2);
    print(channel, buffer);

    sprintf(buffer, "    Edge: %04x %04x %04x\n", *EDGE_GRP0, *EDGE_GRP1, *EDGE_GRP2);
    print(channel, buffer);

    sprintf(buffer, "    Mask: %04x %04x %04x\n", *MASK_GRP0, *MASK_GRP1, *MASK_GRP2);
    print(channel, buffer);

    sprintf(buffer, " Pending: %04x %04x %04x\n", *PENDING_GRP0, *PENDING_GRP1, *PENDING_GRP2);
    print(channel, buffer);

    return 0;
}

//
// Attempt to execute a command
//
// Inputs:
//  command = the upper case name of the command (first word of the command line)
//  parameters = the string of parameters to be passed to the command
//
short cli_exec(short channel, char * command, int argc, char * argv[]) {
    const char * cmd_not_found = "Command not found.\n";
    p_cli_command commands = g_cli_commands;

    log3(LOG_INFO, "cli_exec: '", argv[0], "'");
    log_num(LOG_INFO, "argc = ", argc);

    while ((commands != 0) && (commands->name != 0)) {
        // Does the command match the name?
        if (strcmp(commands->name, command) == 0) {
            // Found it, execute the handler
            return commands->handler(channel, argc, argv);
        } else {
            // No match, keep checking...
            commands++;
        }
    }

    /* No built-in command that matched... try to run a binary file */
    return cmd_run(channel, argc, argv);
}

char * strtok_r(char * source, const char * delimiter, char ** saveptr) {
    char * x = *saveptr;
    char * y;

    /* Skip over leading delimiters */
    for (x = *saveptr; *x && (*x == delimiter[0]); x++) {

    }

    /* If we reached the end of the string, return NULL */
    if (*x == 0) {
        return 0;
    }

    for (y = x; *y && (*y != delimiter[0]); y++) {

    }

    /* If we reached the end of the string, return x */
    if (*y == 0) {
        *saveptr = y;
        return x;
    }

    /* Otherwise, make that position in the source string NULL, and return x */
    *y = 0;
    *saveptr = y + 1;
    return x;
}

short cli_rerepl() {
    while (1) {
        cli_repl(g_current_channel);
    }
}

//
// Enter the CLI's read-eval-print loop
//
short cli_repl(short channel) {
    char command_line[MAX_COMMAND_SIZE];
    char * arg;
    char * token_save;
    char * delim = " ";
    int argc = 0;
    char * argv[MAX_ARGC];

    g_current_channel = channel;

    while (1) {
        sys_chan_write(channel, "\n> ", 3);                           // Print our prompt
        sys_chan_readline(channel, command_line, MAX_COMMAND_SIZE);   // Attempt to read line
        sys_chan_write(channel, "\n", 1);

        for (argc = 0, token_save = command_line; argc < MAX_ARGC; argc++) {
            arg = strtok_r(command_line, delim, &token_save);
            if (arg != 0) {
                argv[argc] = arg;
            } else {
                break;
            }
        }

        if (argc > 0) {
            int i;
            for (i = 0; i < strlen(argv[0]); i++) {
                argv[0][i] = toupper(argv[0][i]);
            }

            // Try to execute the command
            cli_exec(channel, argv[0], argc, argv);
        }
    }

    return 0;
}

long cli_eval_dec(const char * arg) {
    long n = 0;

    TRACE("cli_eval_dec");

    while (*arg) {
        if (*arg == ':') {
            arg++;
            continue;
        } else if (isdigit(*arg)) {
            n *= 10;
            n += *arg - '0';
        } else {
            break;
        }

        arg++;
    }

    return n;
}

long cli_eval_hex(const char * arg) {
    long n = 0;

    TRACE("cli_eval_hex");

    while (*arg) {
        if (*arg == ':') {
            arg++;
            continue;
        } else if (isdigit(*arg)) {
            n *= 16;
            n += *arg - '0';
        } else if ((*arg >= 'a') && (*arg <= 'f')) {
            n *= 16;
            n += *arg - 'a' + 10;
        } else if ((*arg >= 'A') && (*arg <= 'F')) {
            n *= 16;
            n += *arg - 'A' + 10;
        } else {
            break;
        }

        arg++;
    }

    return n;
}

long cli_eval_bin(const char * arg) {
    long n = 0;

    TRACE("cli_eval_bin");

    while (*arg) {
        if (*arg == ':') {
            arg++;
            continue;
        } else if ((*arg == '0') || (*arg == '1')) {
            n *= 2;
            n += *arg - '0';
        } else {
            break;
        }

        arg++;
    }

    return n;
}

/*
 * Evaluate an argument to a number
 */
long cli_eval_number(const char * arg) {
    int len = strlen(arg);

    TRACE("cli_eval_number");

    if (len > 2) {
        if (arg[0] == '0') {
            if (arg[1] == 'x') {
                return cli_eval_hex(&arg[2]);
            } else if (arg[1] == 'b') {
                return cli_eval_bin(&arg[2]);
            }
        }
    }

    if (len > 1) {
        if (arg[0] == '$') {
            return cli_eval_hex(&arg[1]);
        } else if (arg[0] == '%') {
            return cli_eval_bin(&arg[1]);
        }
    }

    return cli_eval_dec(arg);
}

//
// Initialize the CLI
//
// Returns:
//  0 on success, negative number on error
//
short cli_init() {
    cli_set_init();
    return 0;
}
