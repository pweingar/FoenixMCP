/**
 * Implementation of the command line interface
 */

#include <ctype.h>
#include <stdio.h>
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
#include "cli/test_cmds.h"
#include "dev/ps2.h"
#include "dev/rtc.h"
#include "dev/uart.h"
#include "uart_reg.h"
#include "rtc_reg.h"
#include "vicky_general.h"

#define MAX_HISTORY_DEPTH   5   /* Maximum number of commands we'll record */
#define MAX_COMMAND_SIZE    128 /* Maximum number of characters in a command line */
#define MAX_ARGC            32  /* Maximum number of arguments to a command */

/*
 * CLI special key code definitions
 */

#define CLI_FLAG_CTRL   0x0100  /* Flag indicating CTRL is pressed */
#define CLI_FLAG_SHIFT  0x0200  /* Flag indicating SHIFT is pressed */
#define CLI_FLAG_ALT    0x0400  /* Flag indicating ALT is pressed */
#define CLI_FLAG_OS     0x0800  /* Flag indicating OS key is pressed */
#define CLI_FLAG_FUNC   0x4000  /* Function keys: 0x4001 - 0x400C */
#define CLI_FLAG_SPEC   0x8000  /* Special keys: */
#define CLI_KEY_LEFT    0x8001
#define CLI_KEY_RIGHT   0x8002
#define CLI_KEY_UP      0x8003
#define CLI_KEY_DOWN    0x8004
#define CLI_KEY_DEL     0x8005
#define CLI_KEY_MONITOR 0x8010  /* A2560K Monitor key */
#define CLI_KEY_CTX     0x8011  /* A2560K CTX Switch key */
#define CLI_KEY_HELP    0x8012  /* A2560K Menu/Help key */

//
// Types
//

/*
 * States to interpret ANSI escape codes
 */
typedef enum {
    CLI_ES_BASE = 0,    // Base state
    CLI_ES_ESC,         // "ESC" seen
    CLI_ES_CSI,         // "ESC[" has been seen
    CLI_ES_SEMI         // Semicolon has been seen
} cli_state;

// Structure to hold a record about a command...

typedef struct s_cli_command {
    char *name;
    char *help;
    cli_cmd_handler handler;
} t_cli_command, *p_cli_command;

extern short cmd_sysinfo(short channel, int argc, const char * argv[]);
extern short cmd_cls(short channel, int argc, const char * argv[]);
extern short cmd_showint(short channel, int argc, const char * argv[]);
extern short cmd_getjiffies(short channel, int argc, const char * argv[]);
extern short cmd_get_ticks(short channel, int argc, const char * argv[]);
extern short cmd_screen(short channel, int argc, const char * argv[]);

/*
 * Variables
 */

/** The channel to use for interactions */
short g_current_channel = 0;

/** The history of previous commands issued */
char cli_history[MAX_HISTORY_DEPTH][MAX_COMMAND_SIZE];

/** The built-in commands supported */
const t_cli_command g_cli_commands[] = {
    { "?", "? : print this helpful message", cmd_help },
    { "HELP", "HELP : print this helpful message", cmd_help },
    { "CALL", "CALL <address> : execute code in supervisor mode at <address> ", mem_cmd_call },
    { "CD", "CD <path> : sets the current directory", cmd_cd },
    { "CLS", "CLS : clear the screen", cmd_cls },
    { "COPY", "COPY <src path> <dst path> : Copies files to destination", cmd_copy },
    { "DASM", "DASM <addr> [<count>] : print a memory disassembly", mem_cmd_dasm },
    { "DEL", "DEL <path> : delete a file or directory", cmd_del },
    { "DIR", "DIR <path> : print directory listing", cmd_dir },
    { "DISKFILL", "DISKFILL <drive #> <sector #> <byte value>", cmd_diskfill },
    { "DISKREAD", "DISKREAD <drive #> <sector #>", cmd_diskread },
    { "DUMP", "DUMP <addr> [<count>] : print a memory dump", mem_cmd_dump},
    { "GETJIFFIES", "GETJIFFIES : print the number of jiffies since bootup", cmd_getjiffies },
    { "GETTICKS", "GETTICKS : print number of ticks since reset", cmd_get_ticks },
    { "LABEL", "LABEL <drive#> <label> : set the label of a drive", cmd_label },
    { "LOAD", "LOAD <path> : load a file into memory", cmd_load },
    { "MKBOOT", "MKBOOT <drive #> -r | -b <boot sector path> | -s <start file path> : make a drive bootable", cmd_mkboot },
    { "MKDIR", "MKDIR <path> : create a directory", cmd_mkdir },
    { "PEEK8", "PEEK8 <addr> : print the byte at the address in memory", mem_cmd_peek8 },
    { "PEEK16", "PEEK16 <addr> : print the 16-bit word at the address in memory", mem_cmd_peek16 },
    { "PEEK32", "PEEK32 <addr> : print the 32-bit long word at the address in memory", mem_cmd_peek32 },
    { "POKE8", "POKE8 <addr> <value> : write the byte value to the address in memory", mem_cmd_poke8 },
    { "POKE16", "POKE16 <addr> <value> : write the 16-bit value to the address in memory", mem_cmd_poke16 },
    { "POKE32", "POKE32 <addr> <value> : write the 32-bit value to the address in memory", mem_cmd_poke32 },
    { "PWD", "PWD : prints the current directory", cmd_pwd },
    { "REN", "REN <old path> <new path> : rename a file or directory", cmd_rename },
    { "SET", "SET <name> <value> : set the value of a setting", cli_cmd_set },
    { "GET", "GET <name> : get the value of a setting", cli_cmd_get },
    { "SHOWINT", "SHOWINT : Show information about the interrupt registers", cmd_showint },
    { "SYSINFO", "SYSINFO : prints information about the system", cmd_sysinfo },
    { "TEST", "TEST <feature> : run a test about a feature", cmd_test },
    { "TYPE", "TYPE <path> : print the contents of a text file", cmd_type },
    { 0, 0 }
};

/**
 * Set the number of the channel to use for interactions
 *
 * @param channel the number of the text device to use
 */
void cli_channel_set(short channel) {
    g_current_channel = channel;
}

/**
 * Get the number of the channel to use for interactions
 *
 * @return channel the number of the text device to use
 */
short cli_channel_get() {
    return g_current_channel;
}

//
// List all the commands
//
short cmd_help(short channel, int argc, const char * argv[]) {
    p_cli_command command;

    for (command = (p_cli_command)g_cli_commands; (command != 0) && (command->name != 0); command++) {
        sys_chan_write(channel, command->help, strlen(command->help));
        sys_chan_write(channel, "\n", 2);
    }
    return 0;
}

short cmd_getjiffies(short channel, int argc, const char * argv[]) {
    char buffer[80];

    sprintf(buffer, "%ld\n", timers_jiffies());
    sys_chan_write(channel, buffer, strlen(buffer));;
    return 0;
}

/*
 * Print the number of ticks since last restart
 */
short cmd_get_ticks(short channel, int argc, const char * argv[]) {
    char buffer[80];

    sprintf(buffer, "%ld\n", rtc_get_jiffies());
    sys_chan_write(channel, buffer, strlen(buffer));
    return 0;
}

/*
 * Clear the screen
 */
short cmd_cls(short channel, int argc, const char * argv[]) {
    const char * ansi_cls = "\x1B[2J\x1B[H";

    sys_chan_write(channel, ansi_cls, strlen(ansi_cls));
    return 0;
}

/*
 * Display information about the system
 */
short cmd_sysinfo(short channel, int argc, const char * argv[]) {
    t_sys_info info;
    char buffer[80];

    sys_get_info(&info);

    sprintf(buffer, "System information:\nModel: %s", info.model_name);
    sys_chan_write(channel, buffer, strlen(buffer));

    sprintf(buffer, "\nCPU: %s", info.cpu_name);
    sys_chan_write(channel, buffer, strlen(buffer));

    sprintf(buffer, "\nSystem Memory: 0x%lX", info.system_ram_size);
    sys_chan_write(channel, buffer, strlen(buffer));

    sprintf(buffer, "\nPCB version: %s", (char*)&info.pcb_version);
    sys_chan_write(channel, buffer, strlen(buffer));

    sprintf(buffer, "\nFPGA Date: %08lX", info.fpga_date);
    sys_chan_write(channel, buffer, strlen(buffer));

    sprintf(buffer, "\nFPGA Model: %08lX", info.fpga_model);
    sys_chan_write(channel, buffer, strlen(buffer));

    sprintf(buffer, "\nFPGA Version: %04X.%04X", info.fpga_version, info.fpga_subver);
    sys_chan_write(channel, buffer, strlen(buffer));

    sprintf(buffer, "\nMCP version: v%02u.%02u.%04u\n", info.mcp_version, info.mcp_rev, info.mcp_build);
    sys_chan_write(channel, buffer, strlen(buffer));

    return 0;
}

/*
 * Show information about the interrupt registers
 */
short cmd_showint(short channel, int argc, const char * argv[]) {
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
short cli_exec(short channel, char * command, int argc, const char * argv[]) {
    const char * cmd_not_found = "Command not found.\n";
    p_cli_command commands = (p_cli_command)g_cli_commands;

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

void cli_rerepl() {
    while (1) {
        cli_repl(g_current_channel, 0);
    }
}

/**
 * Decode ANSI modifier codes
 *
 * @param modifiers the ANSI modifier codes from an escape sequence
 * @return CLI modifier flags
 */
short cli_translate_modifiers(short modifiers) {
    char buffer[10];
    short flags = 0;

    if (modifiers > 0) {
        modifiers--;
    }

    if (modifiers & 0x01) flags |= CLI_FLAG_SHIFT;
    if (modifiers & 0x02) flags |= CLI_FLAG_ALT;
    if (modifiers & 0x04) flags |= CLI_FLAG_CTRL;
    if (modifiers & 0x08) flags |= CLI_FLAG_OS;

    return flags;
}

/**
 * Translate escape sequences that end in a letter code
 *
 * @param modifiers optional parameter
 * @code the letter code of the key
 */
short cli_translate_alpha(short modifiers, char code) {
    short key_code = 0;

    key_code = cli_translate_modifiers(modifiers);

    switch (code) {
        case 'A':
            key_code |= CLI_KEY_UP;
            break;

        case 'B':
            key_code |= CLI_KEY_DOWN;
            break;

        case 'C':
            key_code |= CLI_KEY_RIGHT;
            break;

        case 'D':
            key_code |= CLI_KEY_LEFT;
            break;

        default:
            return 0;
    }

    return key_code;
}

/**
 * Translate escape sequences that end in a numeric code
 *
 * @param modifiers optional parameter
 * @code the numeric code of the key
 */
short cli_translate_numeric(short modifiers, short code) {
    short key_code = 0;

    key_code = cli_translate_modifiers(modifiers);

    if ((code >= 11) && (code <= 15)) {
        // Function keys 1 - 5
        key_code |= CLI_FLAG_FUNC | (code - 10);

    } else if ((code >= 17) && (code <= 21)) {
        // Function keys 6 - 10
        key_code |= CLI_FLAG_FUNC | (code - 11);

    } else if (code == 30) {
        // MONITOR key
        key_code |= CLI_KEY_MONITOR;

    } else if (code == 31) {
        // CTX SWITCH key
        key_code |= CLI_KEY_CTX;

    } else if (code == 32) {
        // MENU HELP key
        key_code |= CLI_KEY_HELP;

    } else if (code == 3) {
        // DELETE key
        key_code |= CLI_KEY_DEL;

    } else {
        // Unknown escape code
        key_code = 0;
    }

    return key_code;
}

/**
 * Get a character from the console, processing recognized escape sequences
 *
 * @param channel the number of the input channel
 * @return the 16-bit functional character code
 */
short cli_getchar(short channel) {
    char buffer[10];
    cli_state state = CLI_ES_BASE;      // Current state of the escape sequence
    short number1 = 0, number2 = 0;     // Two numbers that can be embedded in the sequence
    char c;                             // The current character read from the console

    do {
        c = sys_chan_read_b(channel);
        switch (state) {
            case CLI_ES_BASE:
                // We are not processing a sequence...
                if (c == CHAR_ESC) {
                    // Escape has been seen
                    state = CLI_ES_ESC;

                } else {
                    // It's an ordinary character, so return it
                    return c;
                }
                break;

            case CLI_ES_ESC:
                // Escape has been seen... check for CSI
                if (c == '[') {
                    // ESC [ has been seen...
                    state = CLI_ES_CSI;

                } else {
                    // Bad escape sequence...just return the character
                    state = CLI_ES_BASE;
                    return c;
                }
                break;

            case CLI_ES_CSI:
                // ESC [ has been seen... next is either a number, a letter, a semi-colon, or a tilda
                if (isdigit(c)) {
                    // It's a number... shift it onto number1
                    number1 = number1 * 10 + (c - '0');

                } else if (isalpha(c)) {
                    // It's a letter... treat as a code
                    return cli_translate_alpha(number1, c);

                } else if (c == ';') {
                    // Got a semicolon, go to that state
                    state = CLI_ES_SEMI;

                } else if (c == '~') {
                    // Got a tilda... end of numeric code with no parameters
                    return cli_translate_numeric(0, number1);

                } else {
                    // Bad sequence... just return the current character
                    state = CLI_ES_BASE;
                    return c;
                }
                break;

            case CLI_ES_SEMI:
                // Semicolon has been seen... next is either a number or a tilda
                if (isdigit(c)) {
                    // It's a number... shift it onto number1
                    number2 = number2 * 10 + (c - '0');

                } else if (c == '~') {
                    // Got a tilda... end of numeric code with parameters
                    return cli_translate_numeric(number2, number1);

                } else {
                    // Bad sequence... just return the current character
                    state = CLI_ES_BASE;
                    return c;
                }
                break;

            default:
                state = CLI_ES_BASE;
                return c;
        }
    } while (1);
}

/**
 * Read a line of input from the channel, allowing for editing of the line
 *
 * @param channel the input channel to use
 * @param command_line a character buffer in which to store the line
 * @return 0 on success, -1 to swap screens (if applicable), -2 to request command help
 */
short cli_readline(short channel, char * command_line) {
    char buffer[10];
    unsigned short key_code = 0;
    short i = 0, j = 0;
    short history = 0;

    // Make sure key echo is turned off
    sys_chan_ioctrl(channel, 0x04, 0, 0);

    // Zero out the command line buffer
    for (i = 0; i < MAX_COMMAND_SIZE; i++) {
        command_line[i] = 0;
    }

    i = 0;

    do {
        key_code = cli_getchar(channel);

        if ((key_code & 0xF000) == 0) {
            // Ordinary key...
            char c = (char)(key_code & 0x00ff);
            if (c < 0x20) {
                // Control key...
                if (c == CHAR_CR) {
                    // Newline... we're done
                    // TODO: add the line to the command history
                    return 0;

                } else if (c == CHAR_ESC) {
                    // ESC pressed... clear whole line

                    // Zero out the command line buffer
                    for (i = 0; i < MAX_COMMAND_SIZE; i++) {
                        command_line[i] = 0;
                    }
                    i = 0;
                    print(channel, "\x1b[3G\x1b[K");

                } else if (c == CHAR_BS) {
                    // Backspace
                    if (i > 0) {
                        i--;
                        print(channel, "\x1b[D");
                        for (j = i; j < strlen(command_line); j++) {
                            command_line[j] = command_line[j+1];
                        }
                        print(channel, "\x1b[1P");
                    }
                }

            } else {
                // Add key to command line
                command_line[i++] = c;
                sys_chan_write_b(channel, c);
            }

        } else {
            // Special editing key
            switch (key_code & 0xF0FF) {
                case CLI_KEY_LEFT:
                    // Move cursor to the left
                    if (key_code & CLI_FLAG_CTRL) {
                        i = 0;
                        print(channel, "\x1b[3G");
                    } else {
                        if (i > 0) {
                            i--;
                            print(channel, "\x1b[D");
                        }
                    }
                    break;

                case CLI_KEY_RIGHT:
                    // Move cursor right
                    if (key_code & CLI_FLAG_CTRL) {
                        sprintf(buffer, "\x1b[%dG", strlen(command_line) + 3);
                        print(channel, buffer);
                        i = strlen(command_line);
                    } else {
                        if (command_line[i] != 0) {
                            i++;
                            print(channel, "\x1b[C");
                        }
                    }
                    break;

                case CLI_KEY_UP:
                    // Go back one command in history
                    if ((history < MAX_HISTORY_DEPTH) && (cli_history[history][0] != 0)) {
                        strcpy(command_line, cli_history[history++]);
                        print(channel, "\x1b[3G\x1b[K");
                        print(channel, command_line);
                    }
                    break;

                case CLI_KEY_DOWN:
                    // Go forward one command in history
                    if (history > 0) {
                        strcpy(command_line, cli_history[--history]);
                        print(channel, "\x1b[3G\x1b[K");
                        print(channel, command_line);
                    }
                    break;

                case CLI_KEY_DEL:
                    // Delete the character under the cursor
                    for (j = i; j < strlen(command_line); j++) {
                        command_line[j] = command_line[j+1];
                    }
                    print(channel, "\x1b[1P");
                    break;

                case CLI_KEY_CTX:
                    // Switch the console screens (if applicable)
                    return -1;

                case CLI_KEY_HELP:
                    // Request the help screen
                    return -2;

                default:
                    // Unknown... do nothing
                    break;
            }
        }
    } while (1);

    return 0;
}

/**
 * Parse and attempt to execute a command line
 *
 * @param command_line the command line to process
 * @return the result of running the command line
 */
short cli_process_line(short channel, const char * command_line) {
    char * arg;
    char * token_save;
    char * delim = " ";
    int argc = 0;
    char * argv[MAX_ARGC];

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
        return cli_exec(channel, argv[0], argc, argv);
    }
}

//
// Enter the CLI's read-eval-print loop
//
short cli_repl(short channel, const char * init_cwd) {
    char command_line[MAX_COMMAND_SIZE];
    char cwd_buffer[MAX_PATH_LEN];
    short result = 0;
    short i = 0;

    g_current_channel = channel;

    if (init_cwd != 0) {
        result = sys_fsys_set_cwd(init_cwd);
        if (result) {
            char message[80];
            sprintf(message, "Unable to set startup directory: %s\n", err_message(result));
            print(g_current_channel, message);
        }
    }

    while (1) {
        sys_chan_write(g_current_channel, "\n", 1);
        // TODO: write the current directory to the status line
        // if(sys_fsys_get_cwd(cwd_buffer, MAX_PATH_LEN) == 0) {
        //     sys_chan_write(channel, cwd_buffer, strlen(cwd_buffer));
        // }
        sys_chan_write(g_current_channel, "\x10 ", 2);                           // Print our prompt

        result = cli_readline(g_current_channel, command_line);
        switch (result) {
            case -1:
                g_current_channel = (g_current_channel == 0) ? 1 : 0;
                break;

            case -2:
                print(g_current_channel, "\n");
                cmd_help(g_current_channel, 0, 0);
                break;

            default:
                // Otherwise, good command... lets add it to the history
                for (i = MAX_HISTORY_DEPTH - 1; i > 0; i--) {
                    // Copy previous commands down
                    strcpy(cli_history[i], cli_history[i-1]);
                }
                strcpy(cli_history[0], command_line);
                break;
        }
        // sys_chan_readline(channel, command_line, MAX_COMMAND_SIZE);   // Attempt to read line

        sys_chan_write(g_current_channel, "\n", 1);

        cli_process_line(g_current_channel, command_line);
    }

    return 0;
}

/**
 * Execute a batch file at the given path
 *
 * @param channel the number of the channel to write any messages to
 * @param path the path to the configuration file to load
 * @return 0 on success, any other number is an error
 */
short cli_exec_batch(short channel, const char * path) {
    char command_line[MAX_COMMAND_SIZE];

    short fd = sys_fsys_open(path, 0x01);   // Open for reading...
    if (fd > 0) {
        // Got a file...

        // Read a line from the file
        short result = 0;

        do {
            result = sys_chan_readline(fd, command_line, MAX_COMMAND_SIZE);
            if (result > 0) {
                // We got a line, so parse it
                cli_process_line(channel, command_line);
            }
        } while (result > 0);   // Until we don't get a line

        // Close the file
        sys_fsys_close(fd);

        return 0;
    } else {
        return fd;
    }
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
    short i;

    // Clear out the command history
    for (i = 0; i < MAX_HISTORY_DEPTH; i++) {
        cli_history[i][0] = 0;
    }

    cli_set_init();
    return 0;
}
