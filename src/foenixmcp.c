/*
 * Startup file for the Foenix/MCP
 */

#include <stdlib.h>
#include <string.h>
#include "sys_general.h"
#include "simpleio.h"
#include "log.h"
// #include "interrupt.h"
// #include "gabe_reg.h"
#if MODEL == MODEL_FOENIX_A2560K
#include "superio.h"
#endif
#include "syscalls.h"
// #include "dev/block.h"
// #include "dev/channel.h"
// #include "dev/console.h"
#include "dev/text_screen_iii.h"
// #include "dev/pata.h"
// #include "dev/ps2.h"
// #include "dev/kbd_mo.h"
// #include "dev/rtc.h"
// #include "dev/sdc.h"
// #include "dev/uart.h"
#include "vicky_general.h"
// #include "snd/codec.h"
// #include "snd/psg.h"
// #include "snd/sid.h"
#include "fatfs/ff.h"
// #include "cli/cli.h"
/* #include "rsrc/bitmaps/splash_a2560k.h"*/

const char* VolumeStr[FF_VOLUMES] = { "sdc", "fdc", "hdc" };

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
 	*GP24_REG = 0x84;

 	*GP30_REG = 0x01;
 	*GP31_REG = 0x01;
 	*GP32_REG = 0x01;
 	*GP33_REG = 0x01;
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
 }
#endif

// /*
//  * Load and display the splash screen
//  */
// void load_splashscreen() {
//     int i;
//
//     /* Turn off the screen */
//     *MasterControlReg_A = VKY3_MCR_BLANK_EN;
//
//     /* Copy the splash screen LUT */
//     for (i = 0; i < sizeof(splash_screen_cmap); i++) {
//         LUT_0[i] = splash_screen_cmap[i][0];
//         LUT_0[i+1] = splash_screen_cmap[i][1];
//         LUT_0[i+2] = splash_screen_cmap[i][2];
//     }
//
//     /* Copy the bitmap to video RAM */
//     for (i = 0; i < sizeof(splash_screen_bmap); i++) {
//         VRAM_Bank0[i] = splash_screen_bmap[i];
//     }
//
//     /* Set up the bitmap */
//     *BM0_Addy_Pointer_Reg = 0;
//     *BM0_Control_Reg = 1;
//
//     /* Turn off the border */
//     *BorderControlReg_L_A = 0;
//
//     /* Display the splashscreen: 320x200 */
//     *MasterControlReg_A = VKY3_MCR_BITMAP_EN | VKY3_MCR_GRAPH_EN | VKY3_MCR_DOUBLE_EN;
//
//     for (i = 0; i < 4096*1024; i++) ;
// }

 void print_error(short channel, char * message, short code) {
     print(channel, message);
     print(channel, ": ");
     print_hex_16(channel, code);
     print(channel, "\n");
 }

extern short current_color;

/*
 * Initialize the kernel systems.
 */
void initialize() {
    int i;
    short res;

    /* Set the logging level */
    log_setlevel(LOG_ERROR);

    // /* Hide the mouse */
    mouse_set_visible(0);

    /* Display the splash screen */
    /* load_splashscreen(); */

    /* Initialize the text channels */
    text_init();

    /* Initialize the interrupt system */
    int_init();

#if MODEL == MODEL_FOENIX_A2560K
    /* Set the power LED to purple */
    *RGB_LED_L = 0x00FF;
    *RGB_LED_H = 0x00FF;

    /* Initialize the SuperIO chip */
    init_superio();
#endif


    /* Mute the PSG */
    psg_mute_all();

    /* Initialize the CODEC */
    init_codec();

    /* Initialize the SID chips */
    sid_init_all();

    /* Play the SID test bong on the Gideon SID implementation */
    sid_test_internal();

    cdev_init_system();   // Initialize the channel device system
    log(LOG_INFO, "Channel device system ready.");

    bdev_init_system();   // Initialize the channel device system
    log(LOG_INFO, "Block device system ready.");

    if (res = con_install()) {
        log_num(LOG_ERROR, "FAILED: Console installation", res);
    } else {
        log(LOG_INFO, "Console installed.");
    }

    /* Initialize the real time clock */
    rtc_init();
//
//     if (res = pata_install()) {
//         log_num(LOG_ERROR, "FAILED: PATA driver installation", res);
//     } else {
//         log(LOG_INFO, "PATA driver installed.");
//     }
//
    if (res = sdc_install()) {
        log_num(LOG_ERROR, "FAILED: SDC driver installation", res);
    } else {
        log(LOG_INFO, "SDC driver installed.");
    }

    // At this point, we should be able to call into to console to print to the screens

    if (res = ps2_init()) {
        print_error(0, "FAILED: PS/2 keyboard initialization", res);
    } else {
        DEBUG("PS/2 keyboard initialized.");
    }
//
//     if (res = kbdmo_init()) {
//         log_num(LOG_ERROR, "FAILED: A2560K built-in keyboard initialization", res);
//     } else {
//         log(LOG_INFO, "A2560K built-in keyboard initialized.");
//     }
//
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

    /* Enable all interrupts */
    int_enable_all();

}

int main(int argc, char * argv[]) {
    const char * color_bars = "\x1b[31m\x0b\x0c\x1b[35m\x0b\x0c\x1b[33m\x0b\x0c\x1b[32m\x0b\x0c\x1b[36m\x0b\x0c";

#if MODEL == MODEL_FOENIX_A2560U
    const char * title_1 = "\x1b[37m   A   2222  55555  666   000  U   U";
    const char * title_2 = "\x1b[37m  A A      2 5     6     0   0 U   U";
    const char * title_3 = "\x1b[37m AAAAA  222   555  6666  0   0 U   U";
    const char * title_4 = "\x1b[37m A   A 2         5 6   6 0   0 U   U";
    const char * title_5 = "\x1b[37m A   A 22222 5555   666   000   UUU";
#elif MODEL == MODEL_FOENIX_A2560U
    const char * title_1 = "\x1b[37m   A   2222  55555  666   000  U   U   +";
    const char * title_2 = "\x1b[37m  A A      2 5     6     0   0 U   U   +";
    const char * title_3 = "\x1b[37m AAAAA  222   555  6666  0   0 U   U +++++";
    const char * title_4 = "\x1b[37m A   A 2         5 6   6 0   0 U   U   +";
    const char * title_5 = "\x1b[37m A   A 22222 5555   666   000   UUU    +";
#elif MODEL == MODEL_FOENIX_A2560K
    const char * title_1 = "\x1b[37m   A   2222  55555  666   000  K   K";
    const char * title_2 = "\x1b[37m  A A      2 5     6     0   0 K  K";
    const char * title_3 = "\x1b[37m AAAAA  222   555  6666  0   0 KKK";
    const char * title_4 = "\x1b[37m A   A 2         5 6   6 0   0 K  K";
    const char * title_5 = "\x1b[37m A   A 22222 5555   666   000  K   K";
#else
    const char * title_1 = "\x1b[37m FFFFF  OOO  EEEEE N   N IIIII X   X";
    const char * title_2 = "\x1b[37m F     O   O E     NN  N   I    X X";
    const char * title_3 = "\x1b[37m FFF   O   O EEE   N N N   I     X";
    const char * title_4 = "\x1b[37m F     O   O E     N  NN   I    X X";
    const char * title_5 = "\x1b[37m F      OOO  EEEEE N   N IIIII X   X";
#endif

    char welcome[255];
    short result;

    initialize();

    sprintf(welcome, "    %s%s\n   %s %s\n  %s  %s\n %s   %s\n%s    %s\n\n", color_bars, title_1, color_bars, title_2, color_bars, title_3, color_bars, title_4, color_bars, title_5);
    sys_chan_write(0, welcome, strlen(welcome));

    sprintf(welcome, "Foenix/MCP v%02d.%02d-alpha+%04d\n\nType \"HELP\" or \"?\" for command summary.", VER_MAJOR, VER_MINOR, VER_BUILD);
    sys_chan_write(0, welcome, strlen(welcome));

    cli_repl(0);

    log(LOG_INFO, "Stopping.");

    /* Infinite loop... */
    while (1) {};
}
