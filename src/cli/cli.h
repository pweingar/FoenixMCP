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
  * Set the path of the command shell
  *
  * @param path the path to the command processor executable (0 or empty string for default)
  */
 extern void cli_command_set(const char * path);

 /**
  * Gets the path of the command shell
  *
  * @param path pointer to the buffer to store the path (empty string means default)
  */
 extern void cli_command_get(char * path);

/**
 * Initialize the CLI
 *
 * Returns:
 * 0 on success, negative number on error
 */
extern short cli_init();

/**
 * Start the read-eval-print loop
 *
 * @param channel the channel to use for interactions
 * @param init_cwd the initial current working directory
 */
extern short cli_start_repl(short channel, const char * init_cwd);

/**
 * Reactivate the CLI's read-eval-print loop after a command has completed
 */
extern void cli_rerepl();

/**
 * Enter the CLI's read-eval-print loop
 *
 * @param channel the channel to use for interactions
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

/**
 * Execute a batch file at the given path
 *
 * @param channel the number of the channel to write any messages to
 * @param path the path to the configuration file to load
 * @return 0 on success, any other number is an error
 */
extern short cli_exec_batch(short channel, const char * path);

/**
 * Set the number of the screen to use for interactions
 *
 * @param screen the number of the text device to use
 */
extern void cli_txt_screen_set(short screen);

/**
 * Get the number of the screen used for interactions
 *
 * @return the number of the text device to use
 */
extern short cli_txt_screen_get();

/**
 * Indicate that the current working directory has changed
 */
extern void cli_flag_cwd();

#endif
