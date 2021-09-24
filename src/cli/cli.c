/**
 * Implementation of the command line interface
 */

#include <ctype.h>
#include <string.h>
#include "types.h"
#include "syscalls.h"
#include "cli/cli.h"
#include "cli/dos_cmds.h"

#define MAX_COMMAND_SIZE    128

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
 int cmd_help(short channel, char * parameters) {
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
    { "LOAD", "LOAD <path> -- load a file into memory", cmd_load },
    { "MBR", "MBR @S: | @F: | @H: -- fetch and display the MBR of the drive", cmd_dump_mbr},
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
short cli_exec(short channel, char * command, char * parameters) {
    const char * cmd_not_found = "Command not found.\n";

    p_cli_command commands;

    commands = g_cli_commands;

    while ((commands != 0) && (commands->name != 0)) {
        // Does the command match the name?
        if (strcmp(commands->name, command) == 0) {
            // Found it, execute the handler
            return commands->handler(channel, parameters);
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

static char * command_line[MAX_COMMAND_SIZE];

//
// Enter the CLI's read-eval-print loop
//
short cli_repl(short channel) {
    char * command;
    char * parameters;

    const char * welcome = "\n\nFoenix/MCP Command Line Utility... online.\nType \"HELP\" or \"?\" for help.\n\n";
    sys_chan_write(channel, welcome, strlen(welcome));

    while (1) {
        sys_chan_write(channel, "\n> ", 3);                           // Print our prompt
        sys_chan_readline(channel, command_line, MAX_COMMAND_SIZE);   // Attempt to read line
        sys_chan_write(channel, "\n", 1);

        // Start at the beginning of the command line
        command = command_line;

        // Skip over leading white space
        while (*command != 0 && isspace(*command)) {
            command++;
        }

        // Start at the beginning of the command
        parameters = command;

        // Skip over any non-space character... converting to uppercase as we go
        while (*parameters != 0 && !isspace(*parameters)) {
            if (islower(*parameters)) {
                *parameters = toupper(*parameters);
            }
            parameters++;
        }

        // If we're not at the end of the line
        if (*parameters != 0) {
            // Mark this spot with a 0... end of command
            *parameters++ = 0;
        }

        // If we are at a space, move to the first non-space character
        while (*parameters != 0 && isspace(*parameters)) {
            parameters++;
        }

        // Try to execute the command
        cli_exec(channel, command, parameters);
    }

    return 0;
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
