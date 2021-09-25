/**
 * Implementation of the command line interface
 */

#include <ctype.h>
#include <string.h>
#include "log.h"
#include "types.h"
#include "syscalls.h"
#include "cli/cli.h"
#include "cli/dos_cmds.h"
#include "cli/mem_cmds.h"

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

/*
 * Commands
 */

 extern const t_cli_command g_cli_commands[];

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

//
// CLI variables
//

const t_cli_command g_cli_commands[] = {
    { "?", "? -- print this helpful message", cmd_help },
    { "HELP", "HELP -- print this helpful message", cmd_help },
    { "DIR", "DIR <path> -- print directory listing", cmd_dir },
    { "DUMP", "DUMP <address> [<count>] -- print a memory dump", mem_cmd_dump},
    { "LOAD", "LOAD <path> -- load a file into memory", cmd_load },
    { "PEEK8", "PEEK8 <address> -- print the byte at the address in memory", mem_cmd_peek8},
    { "PEEK16", "PEEK16 <address> -- print the 16-bit word at the address in memory", mem_cmd_peek16},
    { "PEEK32", "PEEK32 <address> -- print the 32-bit long word at the address in memory", mem_cmd_peek32},
    { "POKE8", "POKE8 <address> <value> -- write the byte value to the address in memory", mem_cmd_poke8},
    { "POKE16", "POKE16 <address> <value> -- write the 16-bit word value to the address in memory", mem_cmd_poke16},
    { "POKE32", "POKE32 <address> <value> -- write the 32-bit long word value to the address in memory", mem_cmd_poke32},
    { "TYPE", "TYPE <path> -- print the contents of a text file", cmd_type },
    { 0, 0 }
};

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

    // Built in command not found..
    // TODO: search the current drive for an executable file
    sys_chan_write(channel, cmd_not_found, strlen(cmd_not_found));
    return -1;
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

    const char * welcome = "\n\nFoenix/MCP Command Line Utility... online.\nType \"HELP\" or \"?\" for help.\n\n";
    sys_chan_write(channel, welcome, strlen(welcome));

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
    return 0;
}
