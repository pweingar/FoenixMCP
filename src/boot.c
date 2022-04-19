/**
 * @file boot.c
 *
 * Routines to support the boot process
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "boot.h"
#include "constants.h"
#include "errors.h"
#include "gabe_reg.h"
#include "log.h"
#include "simpleio.h"
#include "syscalls.h"
#include "sys_general.h"
#include "vicky_general.h"
#include "cli/cli.h"
#include "dev/kbd_mo.h"
#include "dev/txt_screen.h"

#include "rsrc/font/quadrotextFONT.h"
#if MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "rsrc/bitmaps/splash_a2560u.h"
#elif MODEL == MODEL_FOENIX_A2560K
#include "rsrc/bitmaps/splash_a2560k.h"
#endif

#define SPLASH_WAIT_SEC     10      /* How many seconds to wait on the splash screen */

/*
 * Important scan codes
 */
#define SC_F1       0x3B
#define SC_F2       0x3C
#define SC_F3       0x3D
#define SC_SPACE    0x39
#define SC_RETURN   0x1C

/* TODO: move this to constants.h */


#define BOOT_SECTOR_BUFFER  ((unsigned char *)0x00004000)
#define BOOT_CODE_MBR_OFF   0x000                   /* Offset to the code in the MBR */
#define BOOT_CPUID_MBR_OFF  0x004                   /* Offset to the CPUID in the MBR */
#define BOOT_SIG_MBR_OFF    0x006                   /* Offset to the boot signature in the MBR */

#define BOOT_SIG            0xF0E1                  /* Foenix/MCP boot signature expected */

const char * MCP_INIT_SDC = "/sd/system/mcp.init";  /**< Path to config file on the SD card */
const char * MCP_INIT_FDC = "/fd/system/mcp.init";  /**< Path to config file on the floppy drive */
const char * MCP_INIT_HDC = "/hd/system/mcp.init";  /**< Path to config file on the IDE drive */

// Colors for the A2560K keyboard LED matrix
const unsigned short kbd_colors[] = {0x000F, 0x0FF, 0x00F0, 0x0FF0, 0x0F70, 0x0F00};

short cli_screen = 0;                               /**< The default screen to use for the REPL of the CLI */
char cli_command_path[MAX_PATH_LEN];                /**< Path to the command processor (empty string for built-in) */

/**
 * On the A2560K, animate the LEDs based on the current time while we're waiting for a key press
 *
 * @param max_ticks the value of the jiffy counter when the boot screen will end
 * @param ticks the current value of the jiffy counter
 * @param min_ticks the starting value of the jiffy counter
 */
void boot_animate_keyboard(unsigned long max_ticks, unsigned long ticks, unsigned long min_ticks) {
#if MODEL == MODEL_FOENIX_A2560K
    const int animation_steps = 7;
    int current_step = (int)(((ticks - min_ticks) * animation_steps) / (max_ticks - min_ticks));
    int i;

    for (i = 0; i < current_step; i++) {
        kbdmo_set_led_matrix_row(current_step - i - 1, kbd_colors[5 - i]);
    }
#endif
}

/**
 * Set the path of the command shell
 *
 * @param path the path to the command processor executable (0 or empty string for default)
 */
void cli_command_set(const char * path) {
    if (path) {
        // Copy the desired path
        strncpy(cli_command_path, path, MAX_PATH_LEN);

    } else {
        // Set to the default CLI
        cli_command_path[0] = 0;
    }
}

/**
 * Gets the path of the command shell
 *
 * @param path pointer to the buffer to store the path (empty string means default)
 */
void cli_command_get(char * path) {
    // Copy the desired path
    strncpy(path, cli_command_path, MAX_PATH_LEN);
}

/**
 * Determine if a sector loaded from a device is bootable
 *
 * @param sector pointer to where the sector is stored in memory
 * @param device the number of the block device
 *
 * @return 0 if not bootable, non-zero if bootable
 */
short is_bootable(unsigned char * sector, short device) {
    switch(device) {
        case BDEV_FDC:
            // TODO: handled floppy drives
            break;

        case BDEV_SDC:
        case BDEV_HDC:
            // The SDC and HDC boot off the MBR...
            // Check for the CPUID and boot signature
            if ((sector[BOOT_CPUID_MBR_OFF] == CPU_M68000) ||
                (sector[BOOT_CPUID_MBR_OFF] == CPU_M68040)) {
                if ((sector[BOOT_SIG_MBR_OFF] == ((BOOT_SIG >> 8) & 0x00FF)) &&
                    (sector[BOOT_SIG_MBR_OFF+1] == (BOOT_SIG & 0x00FF))) {
                    // The CPU is supported, and the boot signature is correct
                    return 1;
                }
            }
            break;

        default:
            // Otherwise: we're not bootable
            break;
    }

    // If we have reached this point, the sector is not bootable
    return 0;
}

/**
 * Run the code in the boot sector
 *
 * @param device the number of the block device for the sector
 */
void boot_sector_run(short device) {
    FUNC_V_2_V boot_sector = 0;

    switch(device) {
        case BDEV_FDC:
            // TODO: support floppy drives
            break;

        case BDEV_SDC:
        case BDEV_HDC:
            // The SDC and HDC both boot off the MBR
            boot_sector = (FUNC_V_2_V)(BOOT_SECTOR_BUFFER);
            boot_sector();
            break;

        default:
            break;
    }
}

void make_key_name(const char * original, char * buffer) {
    int x;
    for (x = 0; x < strlen(original); x++) {
        buffer[x] = 0x80 | original[x];
    }
    buffer[strlen(original)] = 0;
}

/*
 * Load and display the boot splash screen on the graphics screen
 *
 * @return boot device selected by user
 */
short boot_screen() {
    t_rect region;
    short device = BOOT_DEFAULT;
    short screen = 0;
    char buffer[256];
    char entry[50];
    unsigned long target_jiffies = 0;
    unsigned long min_jiffies = 0;
    unsigned long current_jiffies = 0;
    int i = 0;
    const unsigned char * pixels;
    volatile unsigned char * vram = VRAM_Bank0;
    t_sys_info info;
    char f1[3], f2[3], f3[3];
    char space[10], cr_text[10];

    // We'll display boot information on the common screen
    screen = 0;

    /* Turn off the screen */
    txt_set_mode(screen, 0);

    for (i = 0; i < 256; i++) {
        LUT_0[4*i] = splashscreen_lut[4*i];
        LUT_0[4*i+1] = splashscreen_lut[4*i+1];
        LUT_0[4*i+2] = splashscreen_lut[4*i+2];
        LUT_0[4*i+3] = splashscreen_lut[4*i+3];
    }

    /* Copy the bitmap to video RAM */
    for (pixels = splashscreen_pix; *pixels != 0;) {
        unsigned char count = *pixels++;
        unsigned char pixel = *pixels++;
        for (i = 0; i < count; i++) {
            *vram++ = pixel;
        }
    }

    /* Set up the bitmap */
    *BM0_Addy_Pointer_Reg = 0;
    *BM0_Control_Reg = 1;

    /* Set a background color for the bitmap mode */
#if MODEL == MODEL_FOENIX_A2560K
    *BackGroundControlReg_B = 0x00202020;
    screen = 0;
#else
    *BackGroundControlReg_A = 0x00202020;
    screen = 0;
#endif

    /* Display the splashscreen at 640x480 without a border */
    txt_set_resolution(screen, 640, 680);
    txt_set_border(screen, 0, 0);
    txt_set_font(screen, 8, 8, quadrotextFONT);

    region.origin.x = 0;
    region.origin.y = 0;
    region.size.width = 0;
    region.size.height = 0;
    txt_set_region(screen, &region);
    txt_setsizes(screen);
    txt_set_mode(screen, TXT_MODE_TEXT | TXT_MODE_BITMAP);

    /* Disable the cursor, set the color, clear the text screen, and display the text message */
    txt_set_cursor(screen, 0, 0, 0);    // Disable the cursor
    txt_set_color(screen, 15, 0);       // White on transparent
    txt_fill(screen, ' ');              // Clear the screen

    make_key_name("F1", f1);
    make_key_name("F2", f2);
    make_key_name("F3", f3);
    make_key_name("SPACE", space);
    make_key_name("RETURN", cr_text);

    sprintf(buffer, "BOOT: %s=FLOPPY, %s=SD CARD, %s=HARD DRIVE, %s=DEFAULT, %s=SAFE", f1, f2, f3, space, cr_text);
    txt_set_xy(screen, (80 - strlen(buffer)) / 2, 58);
    sys_chan_write(screen, buffer, strlen(buffer));

    // Get the information about the system
    sys_get_info(&info);

    region.origin.x = 49;
    region.origin.y = 1;
    region.size.width = 40;
    region.size.height = 20;
    txt_set_region(screen, &region);

    sprintf(buffer, "\x1b[HFOENIX/MCP V: %02u.%04u.%04u\n", info.mcp_version, info.mcp_rev, info.mcp_build);
    print(screen, buffer);
    str_upcase(info.model_name, entry);
    sprintf(buffer, "       MODEL: %s\n", entry);
    print(screen, buffer);
    str_upcase(info.cpu_name, entry);
    sprintf(buffer, "         CPU: %s\n", entry);
    print(screen, buffer);
    sprintf(buffer, " CLOCK (KHZ): %u\n", info.cpu_clock_khz);
    print(screen, buffer);
    sprintf(buffer, "      FPGA V: %u.%02u.%04u\n", (unsigned int)info.fpga_model, info.fpga_version, info.fpga_subver);
    print(screen, buffer);

    /* Wait until the target duration has been reached _or_ the user presses a key */
    sprintf(buffer, "Booting from default device...\n");
    min_jiffies = sys_time_jiffies();
    target_jiffies = min_jiffies + SPLASH_WAIT_SEC * 60;
    current_jiffies = sys_time_jiffies();
    while (target_jiffies > current_jiffies) {
        boot_animate_keyboard(target_jiffies, current_jiffies, min_jiffies);

        short scan_code = sys_kbd_scancode();
        if (scan_code != 0) {
            switch (scan_code) {
                case SC_F1:
                    device = BDEV_FDC;
                    sprintf(buffer, "Booting from floppy drive.");
                    break;

                case SC_F2:
                    device = BDEV_SDC;
                    sprintf(buffer, "Booting from SD card.");
                    break;

                case SC_F3:
                    device = BDEV_HDC;
                    sprintf(buffer, "Booting from hard drive.");
                    break;

                case SC_RETURN:
                    device = BOOT_SAFE;
                    sprintf(buffer, "Booting directly to the command line.");
                    break;

                default:
                    device = BOOT_DEFAULT;
                    break;
            }
            break;
        }

        current_jiffies = sys_time_jiffies();
    }

    txt_init_screen(screen);
    print(screen, buffer);

#if MODEL == MODEL_FOENIX_A2560K
    // Turn off the keyboard LEDs
    kbdmo_set_led_matrix_fill(0);
#endif

    return device;
}

/**
 * Start the boot process after initializing the MCP
 *
 * @param device the number of the block device to use for booting (-1 to go straight to CLI)
 */
void boot_from_bdev(short device) {
    char initial_path[10];
    unsigned short boot_dip = 0;        // The setting on the user and boot mode DIP switches
    short bootable = 0;                 // Is the boot sector of the selected device bootable?

    // Get the boot device
    switch (device) {
        case BOOT_DEFAULT:
            // User chose the default. Look at the DIP switches to determine the boot source
            boot_dip = *GABE_DIP_REG & GABE_DIP_BOOT_MASK;
            switch (boot_dip) {
                case 0x0000:
                    // Boot from IDE
                    device = BDEV_HDC;
                    log(LOG_INFO, "Boot DIP set for IDE");
                    strcpy(initial_path, "/hd");
                    break;

                case 0x0001:
                    // Boot from SDC
                    device = BDEV_SDC;
                    log(LOG_INFO, "Boot DIP set for SDC");
                    strcpy(initial_path, "/sd");
                    break;

                case 0x0002:
                    // Boot from Floppy
                    device = BDEV_FDC;
                    log(LOG_INFO, "Boot DIP set for FDC");
                    strcpy(initial_path, "/fd");
                    break;

                default:
                    // Boot straight to REPL
                    log(LOG_INFO, "Boot DIP set for REPL");
                    strcpy(initial_path, "/sd");
                    device = -1;
                    break;
            }
            break;

        default:
            break;
    }

    if (device >= 0) {
        int i;

        for (i = 0; i < 512; i++) {
            // Zero out the buffer
            BOOT_SECTOR_BUFFER[i] = 0;
        }

        // Try to load the boot sector
        short result = bdev_read(device, 0, BOOT_SECTOR_BUFFER, 512);
        if (result > 0) {
            // Check to see if it's bootable
            bootable = is_bootable(BOOT_SECTOR_BUFFER, device);
        }
    }

    if (bootable) {
        // If bootable, run it
        boot_sector_run(device);

    } else {
        // If not bootable...
        if (device >= 0) {
            // Execute startup file on boot device (if present)
            switch (device) {
                case BDEV_SDC:
                    strcpy(initial_path, "/sd");
                    if (cli_exec_batch(cli_screen, MCP_INIT_SDC) != 0) {
                        cli_exec_batch(cli_screen, MCP_INIT_HDC);
                    }
                    break;

                case BDEV_FDC:
                    strcpy(initial_path, "/fd");
                    if (cli_exec_batch(cli_screen, MCP_INIT_FDC) != 0) {
                        cli_exec_batch(cli_screen, MCP_INIT_HDC);
                    }
                    break;

                case BDEV_HDC:
                    strcpy(initial_path, "/hd");
                    cli_exec_batch(cli_screen, MCP_INIT_HDC);
                    break;

                default:
                    break;
            }
        }

        // Start up the command shell
        if (cli_command_path[0] != 0) {
            // Over-ride path provided, boot it
            sys_proc_run(cli_command_path, 0, 0);

        } else {
            print_hex_32(0, (unsigned long)&cli_screen);
            print(0, "starting CLI\n");
            // No over-ride provided... boot the default
            cli_start_repl(cli_screen, initial_path);
        }
    }
}

/**
 * Make the indicated drive non booting by erasing the boot information
 *
 * @param device the number of the block device to use for booting (-1 to go straight to CLI)
 * @return 0 on success, any other number is an error
 */
short boot_non_booting(short device) {
    unsigned char * buffer;
    short result = 0;

    buffer = (unsigned char *)malloc(FSYS_SECTOR_SZ);
    if (buffer != 0) {
        // Try to read the current sector
        short n = sys_bdev_read(device, 0, buffer, FSYS_SECTOR_SZ);
        if (n == FSYS_SECTOR_SZ) {
            // Boot record read... clear it out
            for (int i = 0; i < 0x0DA; i++) {
                buffer[i] = 0;
            }

            // Try to write it back
            n = sys_bdev_write(device, 0, buffer, FSYS_SECTOR_SZ);
            if (n == FSYS_SECTOR_SZ) {
                // Success!
                result = 0;
            } else {
                result = DEV_CANNOT_WRITE;
            }

        } else {
            result = DEV_CANNOT_READ;
        }

    } else {
        result = ERR_OUT_OF_MEMORY;
    }

    // Clear up the memory we grabbed...
    if (buffer) {
        free(buffer);
    }

    return result;
}

const unsigned char boot_from_file_sector[] = {
    0x60, 0x00, 0x00, 0x06,         //          bra.w boot
    CPU_M68000, 0x00, 0xf0, 0xe1,   //          dc.b CPU_M68000, 0, 0xf0, 0xe1
    0x30, 0x3c, 0x00, 0x40,         // boot:    move.w #$40,d0
    0x43, 0xfa, 0x00, 0x0e,         //          lea (path,pc),a1
    0x22, 0x09,                     //          move.l a1,d1
    0x42, 0x82,                     //          clr.l d2
    0x42, 0x83,                     //          clr.l d3
    0x4e, 0x4f,                     //          trap #15
    0x4e, 0x71,                     // bootloop nop
    0x60, 0xfc                      //          bra bootloop
};

/**
 * Make the indicated drive booting from a file
 *
 * @param device the number of the block device to use for booting (-1 to go straight to CLI)
 * @param path the path to the file to boot from
 * @return 0 on success, any other number is an error
 */
short boot_set_file(short device, const char * path) {
    unsigned char * buffer, x;
    short result = 0, i = 0;

    buffer = (unsigned char *)malloc(FSYS_SECTOR_SZ);
    if (buffer != 0) {
        // Try to read the current sector
        short n = sys_bdev_read(device, 0, buffer, FSYS_SECTOR_SZ);
        if (n == FSYS_SECTOR_SZ) {
            int sector_len = sizeof(boot_from_file_sector);
            int path_len = strlen(path);

            // Boot record read... clear it out
            for (i = 0; i < 0x1B0; i++) {
                buffer[i] = 0;
            }

            // Copy the boot code over
            for (i = 0; i < sector_len; i++) {
                buffer[i] = boot_from_file_sector[i];
            }

            // Insert the path
            for (i = 0; i < path_len; i++) {
                buffer[i + sector_len] = path[i];
            }
            buffer[path_len + sector_len] = 0;

            // Try to write it back
            n = sys_bdev_write(device, 0, buffer, FSYS_SECTOR_SZ);
            if (n == FSYS_SECTOR_SZ) {
                // Success!
                result = 0;
            } else {
                result = DEV_CANNOT_WRITE;
            }

        } else {
            result = DEV_CANNOT_READ;
        }

    } else {
        result = ERR_OUT_OF_MEMORY;
    }

    // Clear up the memory we grabbed...
    if (buffer) {
        free(buffer);
    }

    return result;
}
