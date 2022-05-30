/**
 * @file boot.h
 *
 * Routines to support the boot process
 */

#ifndef __BOOT_H
#define __BOOT_H

#define BOOT_DEFAULT    -1
#define BOOT_SAFE       -2

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

/**
 * Make the indicated drive non booting by erasing the boot information
 *
 * @param device the number of the block device to use for booting (-1 to go straight to CLI)
 * @return 0 on success, any other number is an error
 */
extern short boot_non_booting(short device);

/**
 * Make the indicated drive booting from a file
 *
 * @param device the number of the block device to use for booting (-1 to go straight to CLI)
 * @param path the path to the file to boot from
 * @return 0 on success, any other number is an error
 */
extern short boot_set_file(short device, const char * path);

#endif
