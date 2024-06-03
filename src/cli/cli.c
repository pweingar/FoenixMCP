/**
 * Implementation of the command line interface
 */

#include "log_level.h"
#ifndef DEFAULT_LOG_LEVEL
    #define DEFAULT_LOG_LEVEL LOG_TRACE
#endif


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "sys_types.h"
#include "interrupt.h"
#include "simpleio.h"
#include "syscalls.h"
#include "sys_general.h"
#include "timers.h"
#include "cli/cli.h"
#include "cli/dos_copy.h"
#include "cli/dos_cmds.h"
#include "cli/mem_cmds.h"
#include "cli/settings.h"
#include "cli/sound_cmds.h"
#include "cli/test_cmds.h"
#include "dev/console.h"
#include "dev/ps2.h"
#include "dev/rtc.h"
#include "dev/uart.h"
#include "uart_reg.h"
#include "rtc_reg.h"
#include "utilities.h"
#include "variables.h"
#include "vicky_general.h"
#include "version.h"

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

// DEBUG: if I uncomment this then I get a crash
#define sys_txt_get_region txt_get_region
#define sys_txt_set_region txt_set_region

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
	struct s_cli_command * next;
} t_cli_command, *p_cli_command;

extern short cmd_sysinfo(short channel, int argc, const char * argv[]);
extern short cmd_cls(short channel, int argc, const char * argv[]);
extern short cmd_showint(short channel, int argc, const char * argv[]);
extern short cmd_getjiffies(short channel, int argc, const char * argv[]);
extern short cmd_get_ticks(short channel, int argc, const char * argv[]);
extern short cmd_screen(short channel, int argc, const char * argv[]);
extern short cmd_credits(short channel, int argc, const char * argv[]);

/*
 * Variables
 */

short cli_screen = 0;                               /**< The default screen to use for the REPL of the CLI */
char cli_command_path[MAX_PATH_LEN];                /**< The path to the command processor */

/** The channel to use for interactions */
short g_current_channel = 0;

short g_channels_swapped = 1;

/** Flag to indicate that the current working directory has changed */
short g_cwd_changed = 0;

/** Pointer  to the first registered command */
static p_cli_command first_command = 0;

/** The history of previous commands issued */
char cli_history[MAX_HISTORY_DEPTH][MAX_COMMAND_SIZE];

/** Record of system information */
t_sys_info cli_sys_info;

/*
 * Command table
 */

/**
 * @brief Get the first registered command
 * 
 * @return p_cli_command pointer to the first registered command
 */
p_cli_command cli_command_first() {
	return first_command;
}

/**
 * @brief Given the pointer to a command, return the pointer to the next command
 * 
 * @param command the current command
 * @return p_cli_command pointer to the next command (0 if none)
 */
p_cli_command cli_command_next(p_cli_command command) {
	if (command) {
		return command->next;
	} else {
		return 0;
	}
}

/**
 * @brief Add a new command to the list of commands
 * 
 * @param name the name of the command to add
 * @param help a simple helpful description for the command
 * @param handler the code to run for the command
 */
void cli_command_register(char * name, char * help, cli_cmd_handler handler) {
	p_cli_command new_command = (p_cli_command)malloc(sizeof(t_cli_command));
	if (new_command) {
		new_command->name = name;
		new_command->help = help;
		new_command->handler = handler;
		new_command->next = 0;

		if (first_command) {
			// There is already at least one command... add it to the next available spot
			p_cli_command current_command = first_command;
			p_cli_command previous_command = 0;
			
			// Find the place to insert the command
			while ((current_command) && (strcmp(current_command->name, name) < 0)) {
				previous_command = current_command;
				current_command = current_command->next;
			}

			if (previous_command) {
				// We are inserting into the middle or end of the list
				new_command->next = previous_command->next;
				previous_command->next = new_command;

			} else {
				// We are inserting at the beginning of the list
				new_command->next = first_command;
				first_command = new_command;
			}

		} else {
			// This is the first comnand added
			first_command = new_command;
		}
	}
}



/**
 * Set the path of the command shell
 *
 * @param path the path to the command processor executable (0 or empty string for default)
 */
void cli_command_set(const char * path) {
    short i = 0;
    short result = 0;

    if (path) {
        // Copy the desired path... without any trailing newline
        for (i = 0; i < strlen(path); i++) {
            char c = path[i];
            if ((c == CHAR_NL) || (c == CHAR_CR)) {
                cli_command_path[i] = 0;
                break;
            }
            cli_command_path[i] = c;
        }
        result = sys_var_set("SHELL", cli_command_path);
        if (result) {
            log(LOG_ERROR, "Unable to set SHELL");
        }

    } else {
        // Set to the default CLI
        result = sys_var_set("SHELL", 0);
        if (result) {
            log(LOG_ERROR, "Unable to set SHELL");
        }

    }
}

/**
 * Gets the path of the command shell
 *
 * @param path pointer to the buffer to store the path (empty string means default)
 */
void cli_command_get(char * path) {
    // Copy the desired path
    const char * set_path = sys_var_get("SHELL");
    if (set_path) {
        strncpy(path, set_path, MAX_PATH_LEN);
    } else {
        path[0] = 0;
    }
}

/**
 * Set the number of the screen to use for interactions
 *
 * @param screen the number of the text device to use
 */
void cli_txt_screen_set(short screen) {
    if (sys_chan_device(0) != screen) {
        sys_chan_swap(0, 1);
        g_channels_swapped = 1;
    }
}

/**
 * Get the number of the screen used for interactions
 *
 * @return the number of the text device to use
 */
short cli_txt_screen_get() {
    return sys_chan_device(CDEV_CONSOLE); // FIXME that seems to always return rather than e.g. TXT_SCREEN_A2560U
}

//
// List all the commands
//
short cmd_help(short channel, int argc, const char * argv[]) {
    for (p_cli_command command = cli_command_first(); command != 0; command = cli_command_next(command)) {
		if ((command->name) && (command->help)) {
			printf("%s\n", command->help);
		}
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
    t_extent text_size, pixel_size;
    char buffer[80];

    sprintf(buffer, "\nSystem information:\nModel: %s\n", cli_sys_info.model_name);
    print(channel, buffer);

    sprintf(buffer, "CPU: %s\n", cli_sys_info.cpu_name);
    print(channel, buffer);

    sprintf(buffer, "Clock (kHz): %u\n", cli_sys_info.cpu_clock_khz);
    print(channel, buffer);

    sprintf(buffer, "System Memory: 0x%lX\n", cli_sys_info.system_ram_size);
    print(channel, buffer);

    sprintf(buffer, "FPGA Model: %08lX\n", cli_sys_info.fpga_model);
    print(channel, buffer);

    sprintf(buffer, "FPGA Version: %04X.%04X\n", cli_sys_info.fpga_version, cli_sys_info.fpga_subver);
    print(channel, buffer);

    sprintf(buffer, "MCP version: v%02u.%02u.%04u\n", cli_sys_info.mcp_version, cli_sys_info.mcp_rev, cli_sys_info.mcp_build);
    print(channel, buffer);

    short screen = sys_chan_device(channel);

    sys_txt_get_sizes(screen, &text_size, &pixel_size);
    sprintf(buffer, "Screen#%d size: %dx%d characters, %dx%d pixels.\n", screen, text_size.width, text_size.height, pixel_size.width, pixel_size.height);
    print(channel, buffer);

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
    log3(LOG_INFO, "cli_exec: '", argv[0], "'");
    log_num(LOG_INFO, "argc = ", argc);

	for (p_cli_command command = cli_command_first(); command != 0; command = cli_command_next(command)) {
		if (command->name) {
			if (strcmp(command->name, command) == 0) {
				// Found it, execute the handler
            	return command->handler(channel, argc, argv);
			}
		}
	}

    /* No built-in command that matched... try to run a binary file */
    return cmd_run(channel, argc, argv);
}

/**
 * Make sure all the console settings are setup so that the console works correctly
 */
void cli_ensure_console(short channel) {
    // Make sure the console is set up correctly for the CLI
    sys_chan_ioctrl(channel, CON_IOCTRL_ECHO_OFF, 0, 0);
    sys_chan_ioctrl(channel, CON_IOCTRL_ANSI_ON, 0, 0);
    sys_chan_ioctrl(channel, CON_IOCTRL_CURS_ON, 0, 0);

    // Make sure the screen has text enabled
    txt_set_mode(sys_chan_device(channel), TXT_MODE_TEXT);
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

char line[128];

/**
 * Print out the credits
 */
short cmd_credits(short channel, int argc, const char * argv[]) {
    short scan_code = 0;

    print(channel, "\x1b[2J\x1b[1;2H");
    print_banner(channel, cli_sys_info.model);

    print(channel, "\n");

    sprintf(line, "| Version       | %02d.%02d-alpha+%04d                                  |\n", VER_MAJOR, VER_MINOR, VER_BUILD);
    print_box(channel, "{-------------------------------------------------------------------}\n");
    print_box(channel, "| Foenix/MCP - A simple OS for Foenix Retro Systems computers       |\n");
    print_box(channel, ">---------------!---------------------------------------------------<\n");
    print_box(channel, line);
    print_box(channel, ">---------------#---------------------------------------------------<\n");
    print_box(channel, "| License       | BSD-3-Clause                                      |\n");
    print_box(channel, ">---------------#---------------------------------------------------<\n");
    print_box(channel, "| Creators      | Foenix Retro Systems - Stefany Allaire            |\n");
    print_box(channel, "|               >---------------------------------------------------<\n");
    print_box(channel, "|               | Foenix/MCP - Peter Weingartner                    |\n");
    print_box(channel, ">---------------#---------------------------------------------------<\n");
    print_box(channel, "| License       | BSD-3-Clause                                      |\n");
    print_box(channel, ">---------------#---------------------------------------------------<\n");
    print_box(channel, "| Creators      | Foenix Retro Systems - Stefany Allaire            |\n");
    print_box(channel, "|               >---------------------------------------------------<\n");
    print_box(channel, "|               | Foenix/MCP - Peter Weingartner                    |\n");
    print_box(channel, ">---------------#---------------------------------------------------<\n");
    print_box(channel, "| Contributors  | H\x86kan Th\x94rngren, Jesus Garcia, Vincent B.         |\n");
    print_box(channel, ">---------------#---------------------------------------------------<\n");
    print_box(channel, "| Included Code | FatFS - http://elm-chan.org/fsw/ff/00index_e.html |\n");
    print_box(channel, "[---------------@---------------------------------------------------]\n");

    print(channel, "\n\x1b[1mMake the machine yours!\x1b[0m\n");

    print(channel, "\n\x1b[91m//END-OF-LINE\n\x1b[37m");

    do {
        scan_code = sys_kbd_scancode();
    } while (scan_code != 0x01);

    print(channel,"\x1b[2J\x1b[H");
    return 0;
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
                        sprintf(buffer, "\x1b[%dG", (short)(3 + strlen(command_line)));
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
                        i = strlen(command_line);
                        print(channel, "\x1b[3G\x1b[K");
                        print(channel, command_line);
                    }
                    break;

                case CLI_KEY_DOWN:
                    // Go forward one command in history
                    if (history > 0) {
                        strcpy(command_line, cli_history[--history]);
                        i = strlen(command_line);
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
                    if (key_code & CLI_FLAG_OS) {
                        return -3;
                    } else {
                        return -2;
                    }

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
short cli_process_line(short channel, char * command_line) {
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

void cli_draw_window(short channel, const char * status, short is_active) {
    const char * title_header = "Foenix/MCP - ";
    unsigned char foreground, background;
    char buffer[128];
    t_point cursor;
    t_rect region, old_region, full_region;
    short i = 0, j;

    short dev = sys_chan_device(channel);
    if (dev == ERR_BADCHANNEL) {
        ERROR1("cli_draw_window on bad channel %d", channel);
        return;
    }

    // Save the current region and cursor location
    sys_txt_get_xy(dev, &cursor);
    sys_txt_get_region(dev, &old_region);
    sys_txt_get_color(dev, &foreground, &background);

    // Return to the full region and get its dimensions
    region.origin.x = 0;
    region.origin.y = 0;
    region.size.width = 0;
    region.size.height = 0;
    sys_txt_set_region(dev, &region);
    sys_txt_get_region(dev, &full_region);

    // Display the titlebar
    i = 0;
    sys_txt_set_xy(dev, 0, 0);
    if (channel == 0) {
        sys_txt_set_color(dev, background, foreground);
    }
    for (j = 0; j < strlen(title_header); j++) {
        buffer[i++] = title_header[j];
    }
    for (j = 0; j < strlen(status); j++) {
        buffer[i++] = status[j];
    }
    while (i < full_region.size.width) {
        if (is_active) {
            buffer[i++] = ' ';
        } else {
            buffer[i++] = 0xB0;
        }
    }
    buffer[i++] = 0;
    print(channel, buffer);

    // Restore the region and cursor location
    sys_txt_set_color(dev, foreground, background);
    sys_txt_set_region(dev, &old_region);
    sys_txt_set_xy(dev, cursor.x, cursor.y);

    // Set cursor visibility based on if the screen is active
    for (i = 0; i < cli_sys_info.screens; i++) {
        sys_chan_ioctrl(i, i == channel ? CON_IOCTRL_CURS_ON : CON_IOCTRL_CURS_OFF, 0, 0);
    }
}

/**
 * Initialize the text screen (set up regions, windows, etc.)
 */
void cli_setup_screen(short channel, const char * path, short is_active) {
    t_rect full_region, command_region;
    char message[80];

    short dev = sys_chan_device(channel);

    // Get the size of the screen
    full_region.origin.x = 0;
    full_region.origin.y = 0;
    full_region.size.width = 0;
    full_region.size.height = 0;
    sys_txt_set_region(dev, &full_region);
    sys_txt_get_region(dev, &full_region);

    // Clear the screen
    print(channel, "\x1b[2J\x1b[H");

    // Figure out the size of the command box and its region
    command_region.origin.x = 0;
    command_region.origin.y = 1;
    command_region.size.width = full_region.size.width;
    command_region.size.height = full_region.size.height - 1;

    // Restrict the region to the command panel
    sys_txt_set_region(dev, &command_region);

    // Draw the window
    cli_draw_window(channel, path, is_active);
    print(channel, "\x1b[2J\x1b[1;2H");

    print_banner(channel, cli_sys_info.model);

    sprintf(message, "\nFoenix/MCP v%02u.%02u.%04u\n\n", (unsigned int)cli_sys_info.mcp_version, (unsigned int)cli_sys_info.mcp_rev, (unsigned int)cli_sys_info.mcp_build);
    print(channel, message);
    print(channel, "Type HELP or ? for help.\n");
}

//
// Enter the CLI's read-eval-print loop
//
short cli_repl(short channel) {
    char command_line[MAX_COMMAND_SIZE];
    char cwd_buffer[MAX_PATH_LEN];
    short result = 0;
    short i = 0;
    t_point cursor;
    short old_channel;

    old_channel = channel;
    g_channels_swapped = 1;

    g_cwd_changed = 1;
    cursor.x = 0;
    cursor.y = 0;

    while (1) {
        // Refresh window if the current working directory has changed
        if (g_cwd_changed || g_channels_swapped) {
            g_cwd_changed = 0;

            // Get and display the new working directory
            if (sys_fsys_get_cwd(cwd_buffer, MAX_PATH_LEN) == 0) {
                // char message[80];
                // sprintf(message, "%d", strlen(cwd_buffer));
                print(CDEV_CONSOLE, "");
#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
                if (g_channels_swapped) {
                    // If channel has changed, deactivate old channel
                    cli_draw_window(CDEV_EVID, cwd_buffer, 0);
                    old_channel = g_current_channel;
                    g_channels_swapped = 0;
                }
#endif
                cli_draw_window(CDEV_CONSOLE, cwd_buffer, 1);
            }
        }

        sys_chan_write(g_current_channel, "\x10 ", 2);                           // Print our prompt
        result = cli_readline(g_current_channel, command_line);
        switch (result) {
            case -1:
                // g_current_channel = (g_current_channel == 0) ? 1 : 0;
                sys_chan_swap(0, 1);
                g_channels_swapped = 1;
                break;

            case -2:
                print(g_current_channel, "\n");
                cmd_help(g_current_channel, 0, 0);
                break;

            case -3:
                // Print the credits
                print(g_current_channel, "\n");
                cmd_credits(g_current_channel, 0, 0);
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

        print(g_current_channel, "\n");
        cli_process_line(g_current_channel, command_line);

        print(g_current_channel, "\n");
        sys_txt_get_xy(sys_chan_device(g_current_channel), &cursor);
    }

    return 0;
}

/**
 * Start the read-eval-print loop
 *
 * @param channel the channel to use for interactions
 * @param init_cwd the initial current working directory
 */
short cli_start_repl(short channel, const char * init_cwd) {
    char cwd_buffer[MAX_PATH_LEN];
    short result = 0;
    short i = 0;
    t_point cursor;

    g_current_channel = channel;

    // Make sure we can see text properly on the channel
    cli_ensure_console(g_current_channel);

    if (init_cwd != 0) {
        result = sys_fsys_set_cwd(init_cwd);
        if (result) {
            char message[80];
            sprintf(message, "Unable to set startup directory: %s\n", err_message(result));
            print(g_current_channel, message);
        }
    }

    // Start up the command shell
    cli_command_get(cli_command_path);
    if (cli_command_path[0] != 0) {
        // Over-ride path provided, boot it
        char * argv[1] = { cli_command_path };

        result = sys_proc_run(cli_command_path, 1, argv);
        if (result) {
            print(0, "Unable to start ");
            print(0, cli_command_path);
            print(0, ": ");
            print(0, err_message(result));
            while (1) ;
        }
        return 0;
    } else {
        // Set up the screen(s)
        cli_setup_screen(channel, init_cwd, 1);             // Initialize our main main screen
        if (cli_sys_info.screens > 1) {
            for (i = 0; i < cli_sys_info.screens; i++) {             // Set up each screen we aren't using
                if (i != channel) {
                    cli_setup_screen(i, init_cwd, 0);
                }
            }
        }

        return cli_repl(channel);
    }
}

/**
 * Reactivate the CLI's read-eval-print loop after a command has completed
 */
void cli_rerepl() {
    // Start up the command shell
    cli_command_get(cli_command_path);
    if (cli_command_path[0] != 0) {
        char * argv[1] = {cli_command_path};

        // Over-ride path provided, boot it
        short result = sys_proc_run(cli_command_path, 1, argv);
        if (result) {
            print(0, "Unable to start ");
            print(0, cli_command_path);
            print(0, ": ");
            print(0, err_message(result));
            while (1) ;
        }

    } else {
        while (1) {
            // Make sure we can see text properly on the channel
            cli_ensure_console(g_current_channel);

            print(g_current_channel, "\n\n");
            cli_repl(g_current_channel);
        }
    }
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

/**
 * Indicate that the current working directory has changed
 */
void cli_flag_cwd() {
    g_cwd_changed = 1;
}

/**
 * @brief Initialize the table of commands
 * 
 */
void cli_init_commands() {
	cli_command_register("?", "? : print this helpful message", cmd_help);
    cli_command_register("HELP", "HELP : print this helpful message", cmd_help);
    cli_command_register("CALL", "CALL <address> : execute code in supervisor mode at <address> ", mem_cmd_call);
    cli_command_register("CD", "CD <path> : sets the current directory", cmd_cd);
    cli_command_register("CLS", "CLS : clear the screen", cmd_cls);
    cli_command_register("COPY", "COPY <src path> <dst path> : Copies files to destination", cmd_copy);
    cli_command_register("CREDITS", "CREDITS : Print out the credits", cmd_credits);
    cli_command_register("DEL", "DEL <path> : delete a file or directory", cmd_del);
    cli_command_register("DIR", "DIR <path> : print directory listing", cmd_dir);
    cli_command_register("DISKFILL", "DISKFILL <drive #> <sector #> <byte value>", cmd_diskfill);
    cli_command_register("DISKREAD", "DISKREAD <drive #> <sector #>", cmd_diskread);
    cli_command_register("DUMP", "DUMP <addr> [<count>] : print a memory dump", mem_cmd_dump);
    cli_command_register("GETJIFFIES", "GETJIFFIES : print the number of jiffies since bootup", cmd_getjiffies);
    cli_command_register("GETTICKS", "GETTICKS : print number of ticks since reset", cmd_get_ticks);
    cli_command_register("LABEL", "LABEL <drive#> <label> : set the label of a drive", cmd_label);
    cli_command_register("LOAD", "LOAD <path> : load a file into memory", cmd_load);
    cli_command_register("MKBOOT", "MKBOOT <drive #> -r | -b <boot sector path> | -s <start file path> : make a drive bootable", cmd_mkboot);
    cli_command_register("MKDIR", "MKDIR <path> : create a directory", cmd_mkdir);
    cli_command_register("PEEK8", "PEEK8 <addr> : print the byte at the address in memory", mem_cmd_peek8);
    cli_command_register("PEEK16", "PEEK16 <addr> : print the 16-bit word at the address in memory", mem_cmd_peek16);
    cli_command_register("PEEK32", "PEEK32 <addr> : print the 32-bit long word at the address in memory", mem_cmd_peek32);
    cli_command_register("POKE8", "POKE8 <addr> <value> : write the byte value to the address in memory", mem_cmd_poke8);
    cli_command_register("POKE16", "POKE16 <addr> <value> : write the 16-bit value to the address in memory", mem_cmd_poke16);
    cli_command_register("POKE32", "POKE32 <addr> <value> : write the 32-bit value to the address in memory", mem_cmd_poke32);
    cli_command_register("PWD", "PWD : prints the current directory", cmd_pwd);
    cli_command_register("REN", "REN <old path> <new path> : rename a file or directory", cmd_rename);
    cli_command_register("SET", "SET <name> <value> : set the value of a setting", cli_cmd_set);
    cli_command_register("GET", "GET <name> : get the value of a setting", cli_cmd_get);
    cli_command_register("SHOWINT", "SHOWINT : Show information about the interrupt registers", cmd_showint);
    cli_command_register("SYSINFO", "SYSINFO : prints information about the system", cmd_sysinfo);
    cli_command_register("TYPE", "TYPE <path> : print the contents of a text file", cmd_type);

	// Register CPU specific commands
	switch (cli_sys_info.cpu) {
		case CPU_M68000:
		case CPU_M68040:
		case CPU_M68040V:
		case CPU_M680EC40:
			// Register commands used on 680x0 machines only
			cli_command_register("DASM", "DASM <addr> [<count>] : print a memory disassembly", mem_cmd_dasm);
			break;

		default:
			break;
	}
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

    // Get the system information we'll use in several places
    sys_get_info(&cli_sys_info);

	// Register commands
	cli_init_commands();

    cli_set_init();
    return 0;
}
