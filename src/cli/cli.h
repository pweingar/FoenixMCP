/**
 * Command Line Interface
 */

#ifndef __CLI_H
#define __CLI_H

/*
 * A function pointer for command implementations:
 *  short cmd_foo(short screen, char * parameters) { ... }
 */
typedef short (*cli_cmd_handler)(short screen, int argc, const char * argv[]);

/**
 * About the CLI...
 * The CLI will interpret simple commands from the console in the style of CP/M, DOS, Unix, etc.
 * There are built-in commands, but if a command is issued that is not a built-in, the CLI
 * will look for an executable file with that name somewhere in the search path.
 */

/**
 * Initialize the CLI
 *
 * Returns:
 * 0 on success, negative number on error
 */
extern short cli_init();

/**
 * Enter the CLI's read-eval-print loop
 *
 * Inputs:
 * channel = the console channel to use for I/O
 */
extern short cli_repl(short channel);

/*
 * Evaluate an argument to a number
 */
extern long cli_eval_number(const char * arg);

/*
 * Print a help message
 */
extern short cmd_help(short channel, int argc, const char * argv[]);

#endif
