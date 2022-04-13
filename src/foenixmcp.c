/*
 * Startup file for the Foenix/MCP
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys_general.h"
#include "simpleio.h"
#include "log.h"
#include "indicators.h"
#include "interrupt.h"
#include "gabe_reg.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "superio.h"
#include "dev/kbd_mo.h"
#endif

#include "syscalls.h"
#include "timers.h"
#include "boot.h"
#include "memory.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/fdc.h"
#include "dev/txt_screen.h"
#include "dev/txt_a2560k_a.h"
#include "dev/txt_a2560k_b.h"
#include "dev/pata.h"
#include "dev/ps2.h"
#include "dev/rtc.h"
#include "dev/sdc.h"
#include "dev/uart.h"
#include "vicky_general.h"
#include "snd/codec.h"
#include "snd/psg.h"
#include "snd/sid.h"
#include "snd/yamaha.h"
#include "fatfs/ff.h"
#include "cli/cli.h"
#include "rsrc/font/MSX_CP437_8x8.h"

const char* VolumeStr[FF_VOLUMES] = { "sd", "fd", "hd" };

extern unsigned long __memory_start;

#if MODEL == MODEL_FOENIX_A2560K
/*
 * Initialize the SuperIO registers
 */
 void init_superio(void) {
    *GP10_REG = 0x01;
    *GP11_REG = 0x01;
    *GP12_REG = 0x01;
    *GP13_REG = 0x01;
    *GP14_REG = 0x05;
    *GP15_REG = 0x05;
    *GP16_REG = 0x05;
    *GP17_REG = 0x05;

 	*GP20_REG = 0x00;
 	*GP24_REG = 0x01;
 	*GP25_REG = 0x05;
 	*GP26_REG = 0x84;

 	*GP30_REG = 0x01;
 	*GP31_REG = 0x01;
 	*GP32_REG = 0x01;
 	*GP33_REG = 0x04; // FAN1 GPIO Config
 	*GP34_REG = 0x01;
 	*GP35_REG = 0x01;
 	*GP36_REG = 0x01;
 	*GP37_REG = 0x01;

 	*GP42_REG = 0x01;
 	*GP43_REG = 0x01;

 	*GP50_REG = 0x05;
 	*GP51_REG = 0x05;
 	*GP52_REG = 0x05;
 	*GP53_REG = 0x04;
 	*GP54_REG = 0x05;
 	*GP55_REG = 0x04;
 	*GP56_REG = 0x05;
 	*GP57_REG = 0x04;

 	*GP60_REG = 0x84;
 	*GP61_REG = 0x84;

 	*GP1_REG = 0x00;
 	*GP2_REG = 0x01;
 	*GP3_REG = 0x00;
 	*GP4_REG = 0x00;
 	*GP5_REG = 0x00;
 	*GP6_REG = 0x00;

 	*LED1_REG = 0x01;
 	*LED2_REG = 0x02;

    *FAN1_REG = 0xE0;       // <= Value to change to Get the Fan running.
                            // See doc for more options, need to set $80 to get it started and use other bits to change the PWN...
    *FAN_CTRL_REG = 0x01;
 }
#endif

void print_error(short channel, char * message, short code) {
    print(channel, message);
    print(channel, ": ");
    print_hex_16(channel, code);
    print(channel, "\n");
}

/*
 * Initialize the kernel systems.
 */
void initialize() {
    long target_jiffies;
    int i;
    short res;

    /* Set the logging level */
    log_setlevel(LOG_ERROR);

    /* Initialize the memory system */
    mem_init(__memory_start);

    // /* Hide the mouse */
    mouse_set_visible(0);

    /* Initialize the text channels */
    txt_init();
    txt_a2560k_a_install();
    txt_a2560k_b_install();
    txt_init_screen(1);
    txt_init_screen(0);
    log(LOG_INFO, "Text system initialized");

    /* Initialize the indicators */
    ind_init();
    log(LOG_INFO, "Indicators initialized");

    /* Initialize the interrupt system */
    int_init();

#if MODEL == MODEL_FOENIX_A2560K
    /* Initialize the SuperIO chip */
    init_superio();
#endif

    /* Mute the PSG */
    psg_mute_all();

    /* Initialize and mute the SID chips */
    sid_init_all();

    /* Initialize the Yamaha sound chips (well, turn their volume down at least) */
    ym_init();

    /* Initialize the CODEC */
    init_codec();

    cdev_init_system();   // Initialize the channel device system
    log(LOG_INFO, "Channel device system ready.");

    bdev_init_system();   // Initialize the channel device system
    log(LOG_INFO, "Block device system ready.");

    if (res = con_install()) {
        log_num(LOG_ERROR, "FAILED: Console installation", res);
    } else {
        log(LOG_INFO, "Console installed.");
    }

    /* Initialize the timers the MCP uses */
    timers_init();

    /* Initialize the real time clock */
    rtc_init();

    target_jiffies = sys_time_jiffies() + 300;     /* 5 seconds minimum */
    log_num(LOG_DEBUG, "target_jiffies assigned: ", target_jiffies);

    /* Enable all interrupts */
    int_enable_all();
    log(LOG_TRACE, "Interrupts enabled");

    /* Play the SID test bong on the Gideon SID implementation */
    sid_test_internal();

    if (res = pata_install()) {
        log_num(LOG_ERROR, "FAILED: PATA driver installation", res);
    } else {
        log(LOG_INFO, "PATA driver installed.");
    }

    if (res = sdc_install()) {
        log_num(LOG_ERROR, "FAILED: SDC driver installation", res);
    } else {
        log(LOG_INFO, "SDC driver installed.");
    }

// #if MODEL == MODEL_FOENIX_A2560K
//     if (res = fdc_install()) {
//         log_num(LOG_ERROR, "FAILED: Floppy drive initialization", res);
//     } else {
//         log(LOG_INFO, "Floppy drive initialized.");
//     }
// #endif

    // At this point, we should be able to call into to console to print to the screens

    if (res = ps2_init()) {
        print_error(0, "FAILED: PS/2 keyboard initialization", res);
    } else {
        log(LOG_INFO, "PS/2 keyboard initialized.");
    }

#if MODEL == MODEL_FOENIX_A2560K
    if (res = kbdmo_init()) {
        log_num(LOG_ERROR, "FAILED: A2560K built-in keyboard initialization", res);
    } else {
        log(LOG_INFO, "A2560K built-in keyboard initialized.");
    }
#endif

    if (res = cli_init()) {
        log_num(LOG_ERROR, "FAILED: CLI initialization", res);
    } else {
        log(LOG_INFO, "CLI initialized.");
    }

    if (res = fsys_init()) {
        log_num(LOG_ERROR, "FAILED: file system initialization", res);
    } else {
        log(LOG_INFO, "File system initialized.");
    }

    // /* Wait until the target duration has been reached _or_ the user presses a key */
    // while (target_jiffies > sys_time_jiffies()) {
    //     short scan_code = sys_kbd_scancode();
    //     if (scan_code != 0) {
    //         break;
    //     }
    // }
}

#define BOOT_DEFAULT    -1  // User chose default, or the time to over-ride has passed

int main(int argc, char * argv[]) {
    short result;
    short i;

    initialize();

    // Make sure the command path is set to the default before we get started
    cli_command_set("");

    // Display the splash screen and wait for user input
    short boot_dev = boot_screen();

    // Start the boot process
    boot_from_bdev(boot_dev);

    log(LOG_INFO, "Stopping.");

    /* Infinite loop... */
    while (1) {};
}
