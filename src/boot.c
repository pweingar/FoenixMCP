/**
 * @file boot.c
 *
 * Routines to support the boot process
 */

#include <stdio.h>
#include <string.h>
#include "boot.h"
#include "gabe_reg.h"
#include "log.h"
#include "simpleio.h"
#include "syscalls.h"
#include "sys_general.h"
#include "vicky_general.h"
#include "cli/cli.h"
#include "dev/txt_screen.h"

#include "rsrc/font/quadrotextFONT.h"
#if MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "rsrc/bitmaps/splash_a2560u.h"
#elif MODEL == MODEL_FOENIX_A2560K
#include "rsrc/bitmaps/splash_a2560k.h"
#endif

#define SPLASH_WAIT_SEC 15

/*
 * Important scan codes
 */
#define SC_F1       0x3B
#define SC_F2       0x3C
#define SC_F3       0x3D
#define SC_SPACE    0x39
#define SC_RETURN   0x1C

/* TODO: move this to constants.h */
#define MAX_PATH_LEN        256

#define BOOT_SECTOR_BUFFER  ((volatile unsigned char *)0x00004000)
#define BOOT_SECTOR_VBR_OFF 0x060
#define BOOT_SECTOR_MBR_OFF 0x000

const char * MCP_INIT_SDC = "/sd/system/mcp.init";  /**< Path to config file on the SD card */
const char * MCP_INIT_FDC = "/fd/system/mcp.init";  /**< Path to config file on the floppy drive */
const char * MCP_INIT_HDC = "/hd/system/mcp.init";  /**< Path to config file on the IDE drive */

short cli_screen = 0;                               /**< The default screen to use for the REPL of the CLI */
char cli_command_path[MAX_PATH_LEN];                /**< Path to the command processor (empty string for built-in) */

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
short is_bootable(unsigned short * sector, short device) {
    short bootable = 0;

    switch(device) {
        case BDEV_FDC:
        case BDEV_SDC:
            break;

        case BDEV_HDC:
            break;

        default:
            break;
    }

    return bootable;
}

/**
 * Run the code in the boot sector
 *
 * @param device the number of the block device for the sector
 */
void boot_sector_run(short device) {
    FUNC_V_2_V boot_sector = (FUNC_V_2_V)(BOOT_SECTOR_BUFFER + 0x060);

    switch(device) {
        case BDEV_FDC:
        case BDEV_SDC:
            boot_sector = (FUNC_V_2_V)(BOOT_SECTOR_BUFFER + BOOT_SECTOR_VBR_OFF);
            boot_sector();
            break;

        case BDEV_HDC:
            boot_sector = (FUNC_V_2_V)(BOOT_SECTOR_BUFFER + BOOT_SECTOR_MBR_OFF);
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
    long target_jiffies = 0;
    int i = 0;
    const unsigned char * pixels;
    volatile unsigned char * vram = VRAM_Bank0;
    t_sys_info info;
    char f1[3], f2[3], f3[3];
    char space[10], cr_text[10];

#if MODEL == MODEL_FOENIX_A2560K
    screen = 1;
#else
    screen = 0;
#endif

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
    screen = 1;
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

    sprintf(buffer, "\x1b[HFOENIX/MCP V: %02d.%02d.%04d\n", info.mcp_version, info.mcp_rev, info.mcp_build);
    print(screen, buffer);
    str_upcase(info.model_name, entry);
    sprintf(buffer, "       MODEL: %s\n", entry);
    print(screen, buffer);
    str_upcase(info.cpu_name, entry);
    sprintf(buffer, "         CPU: %s\n", entry);
    print(screen, buffer);
    sprintf(buffer, "       BOARD: %s\n", info.pcb_version);
    print(screen, buffer);
    sprintf(buffer, "      FPGA V: %02d.%02d.%04d\n", info.fpga_model, info.fpga_version, info.fpga_subver);
    print(screen, buffer);
    sprintf(buffer, "   FPGA DATE: %06X\n", info.fpga_date);
    print(screen, buffer);

    /* Wait until the target duration has been reached _or_ the user presses a key */
    sprintf(buffer, "Booting using the default device.");
    target_jiffies = sys_time_jiffies() + SPLASH_WAIT_SEC * 60;
    while (target_jiffies > sys_time_jiffies()) {
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
    }

    txt_init_screen(screen);
    print(screen, buffer);

    return device;
}

/**
 * Start the boot process after initializing the MCP
 *
 * @param device the number of the block device to use for booting (-1 to go straight to CLI)
 */
void boot_from_bdev(short device) {
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
                    break;

                case 0x0001:
                    // Boot from SDC
                    device = BDEV_SDC;
                    log(LOG_INFO, "Boot DIP set for SDC");
                    break;

                case 0x0002:
                    // Boot from Floppy
                    device = BDEV_FDC;
                    log(LOG_INFO, "Boot DIP set for FDC");
                    break;

                default:
                    // Boot straight to REPL
                    log(LOG_INFO, "Boot DIP set for REPL");
                    device = -1;
                    break;
            }
            break;

        default:
            break;
    }

    // if (device >= 0) {
    //     // Try to load the boot sector
    //     short result = bdev_read(device, 0, BOOT_SECTOR_BUFFER, 512);
    //     if (result == 0) {
    //         // Check to see if it's bootable
    //         switch (device) {
    //             bootable = is_bootable(BOOT_SECTOR_BUFFER, device);
    //         }
    //     }
    // }

    // if (bootable) {
    //     // If bootable, run it
    //     boot_sector_run(device);
    //
    // } else {
        // If not bootable...
        if (device >= 0) {
            // Execute startup file on boot device (if present)
            switch (device) {
                case BDEV_SDC:
                    print(cli_screen, "Reading configuration from SDC.\n");
                    if (cli_exec_batch(cli_screen, MCP_INIT_SDC) != 0) {
                        cli_exec_batch(cli_screen, MCP_INIT_HDC);
                    }
                    break;

                case BDEV_FDC:
                    print(cli_screen, "Reading configuration from floppy.\n");
                    if (cli_exec_batch(cli_screen, MCP_INIT_FDC) != 0) {
                        cli_exec_batch(cli_screen, MCP_INIT_HDC);
                    }
                    break;

                case BDEV_HDC:
                    print(cli_screen, "Reading configuration from PATA.\n");
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
            // No over-ride provided... boot the default
            cli_repl(cli_screen);
        }
    // }
}
