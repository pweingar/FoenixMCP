/**
 * @file boot.h
 *
 * Routines to support the boot process
 */

#ifndef __BOOT_H
#define __BOOT_H

#define BOOT_DEFAULT    -1
#define BOOT_SAFE       -2

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

/*
 * Load and display the boot splash screen on the graphics screen
 *
 * @return boot device selected by user
 */
extern short boot_screen();

/**
 * Start the boot process after initializing the MCP
 *
 * @param device the number of the block device to use for booting (-1 to go straight to CLI)
 */
extern void boot_from_bdev(short device);

#endif
