/*
 * Startup file for the Foenix/MCP
 */

#include <string.h>
#include "sys_general.h"
#include "interrupt.h"
#include "gabe_reg.h"
#include "superio.h"
#include "syscalls.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/text_screen_iii.h"
#include "dev/pata.h"
#include "dev/ps2.h"
// #include "dev/kbd_mo.h"
#include "dev/sdc.h"
#include "dev/uart.h"
#include "snd/codec.h"
#include "snd/psg.h"
#include "snd/sid.h"
#include "fatfs/ff.h"
// #include "cli/cli.h"
#include "log.h"

const char* VolumeStr[FF_VOLUMES] = { "@S", "@F", "@H" };

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

 void print(short channel, char * message) {
     sys_chan_write(channel, message, strlen(message));
 }

 unsigned char number[5];
 unsigned char hex_digits[] = "0123456789ABCDEF";

 void print_hex(short channel, unsigned short x) {
     short digit;

     digit = (x & 0xf0) >> 4;
     number[0] = hex_digits[digit];

     digit = (x & 0x0f);
     number[1] = hex_digits[digit];

     number[2] = 0;

     print(channel, number);
 }

 void print_hex_16(short channel, unsigned short x) {
     short digit;

     digit = (x >> 12) & 0x000f;
     number[0] = hex_digits[digit];

     digit = (x >> 8) & 0x000f;
     number[1] = hex_digits[digit];

     digit = (x >> 4) & 0x000f;
     number[2] = hex_digits[digit];

     digit = x & 0x000f;
     number[3] = hex_digits[digit];

     number[4] = 0;

     print(channel, number);
 }

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
    short res;

    text_init();          // Initialize the text channels
    DEBUG("Foenix/MCP starting up...");

    /* Initialize the interrupt system */
    int_init();

    /* Set the power LED to purple */
    *RGB_LED_L = 0x00FF;
    *RGB_LED_H = 0x00FF;

    /* Initialize the SuperIO chip */
    init_superio();

    /* Mute the PSG */
    psg_mute_all();

    /* Initialize the CODEC */
    init_codec();

    /* Initialize the SID chips */
    sid_init_all();

    /* Play the SID test bong on the Gideon SID implementation */
    sid_test_internal();

    cdev_init_system();   // Initialize the channel device system
    DEBUG("Channel device system ready.");

    bdev_init_system();   // Initialize the channel device system
    DEBUG("Block device system ready.");

    if (res = con_install()) {
        print_error(0, "FAILED: Console installation", res);
    } else {
        DEBUG("Console installed.");
    }

    if (res = pata_install()) {
        print_error(0, "FAILED: PATA driver installation", res);
    } else {
        DEBUG("PATA driver installed.");
    }

    if (res = sdc_install()) {
        print_error(0, "FAILED: SDC driver installation", res);
    } else {
        DEBUG("SDC driver installed.");
    }

    // At this point, we should be able to call into to console to print to the screens

    if (res = ps2_init()) {
        print_error(0, "FAILED: PS/2 keyboard initialization", res);
    } else {
        DEBUG("PS/2 keyboard initialized.");
    }

    // if (res = kbdmo_init()) {
    //     print_error(0, "FAILED: A2560K built-in keyboard initialization", res);
    // } else {
    //     DEBUG("A2560K built-in keyboard initialized.");
    // }

    // if (res = cli_init()) {
    //     print_error(0, "FAILED: CLI initialization", res);
    // } else {
    //     DEBUG("CLI initialized.");
    // }

    print(0, "MASK_GRP1: ");
    unsigned short grp1 = *MASK_GRP1;
    print_hex_16(0, grp1);
    print(0, "\n");

    /* Enable all interrupts */
    // int_enable_all();
}

// void try_mo_scancodes(short screen) {
//     // volatile unsigned short * kbd_mo_data = ((volatile unsigned short *)0x00C00040);
//     // volatile unsigned short * kbd_mo_stat = ((volatile unsigned short *)0x00C00042);
//
//     print(screen, "mo> ");
//
//     do {
//         // unsigned short status = *kbd_mo_stat;
//         // if ((status & KBD_MO_STAT_EMPTY) != KBD_MO_STAT_EMPTY) {
//         //     unsigned short data = *kbd_mo_data;
//         //     print(screen, "[");
//         //     print_hex_16(screen, status);
//         //     print(screen, "]: {");
//         //
//         //     print_hex_16(screen, data);
//         //     print(screen, "}\n ");
//         // // }
//         // }
//
//         unsigned short scancode = kbdmo_get_scancode_poll();
//         if (scancode != 0) {
//             print(screen, "[");
//             print_hex_16(screen, scancode);
//             print(screen, "]\n");
//         }
//
//     } while (1);
// }

// void try_mo_chars(short screen) {
//     char buffer[2];
//     buffer[1] = 0;
//
//     print(screen, "mo> ");
//
//     while (1) {
//         unsigned char c = kbdmo_getc_poll();
//         if (c) {
//             // text_put_raw(screen, c);
//             return;
//         }
//     }
// }

void uart_send(short uart, char * message) {
    int i, j;

    for (i = 0; i < strlen(message); i++) {
        uart_put(uart, message[i]);

    }
}

void uart_test_send(short uart) {
    while (1) {
        int j;
        uart_put(uart, 'a');
        for (j = 1; j < 10000; j++) ;
    }
}

static long g_sof_counter = 0;

/*
 * Interrupt handler for Channel A's Start of Frame
 */
void int_sof_a() {
    g_sof_counter++;

    long counter_mod = g_sof_counter % 60;

    if (counter_mod == 0) {
        /* Set the power LED to red */
        *RGB_LED_L = 0x00FF;
        *RGB_LED_H = 0x0000;
    } else if (counter_mod == 30) {
        /* Set the power LED to blue */
        *RGB_LED_L = 0x0000;
        *RGB_LED_H = 0x00FF;
    }

    /* Acknowledge the interrupt before leaving */
    int_clear(SOF_A_INT00);
}

void try_format(short screen, char * path) {
    FATFS fs;           /* Filesystem object */
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */
    BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

    /* Format the HDD with default parameters */
    res = f_mkfs(path, 0, work, sizeof work);
    if (res) {
        print(screen, "Could not format drive.\n");
        return;
    }

    /* Give a work area to the default drive */
    f_mount(&fs, path, 0);

    /* Create a file as new */
    res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
    if (res) {
        print(screen, "Could not create hello.txt.\n");
        return;
    }

    /* Write a message */
    f_write(&fil, "Hello, World!\r\n", 15, &bw);
    if (bw != 15) {
        print(screen, "Error writing file.\n");
        return;
    }

    /* Close the file */
    f_close(&fil);

    /* Unregister work area */
    f_mount(0, "", 0);
}

void try_write(short screen, char * path) {
    FATFS fs;           /* Filesystem object */
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */

    /* Give a work area to the default drive */
    f_mount(&fs, path, 0);

    /* Create a file as new */
    res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
    if (res) {
        print(screen, "Could not create hello.txt: ");
        print_hex_16(screen, res);
        print(screen, "\n");
        return;
    }

    /* Write a message */
    f_write(&fil, "Hello, World!\r\n", 15, &bw);
    if (bw != 15) {
        print(screen, "Error writing file.\n");
        return;
    }

    /* Close the file */
    f_close(&fil);

    /* Unregister work area */
    f_mount(0, "", 0);
}

unsigned char test_block_1[512];
unsigned char test_block_2[512];

void try_bdev_getput(short screen, short dev) {
    int i;
    for (i = 0; i < 512; i++) {
        test_block_1[i] = (unsigned short)i & 0xff;
    }

    short n = bdev_write(dev, 0x010000, test_block_1, 512);
    if (n != 512) {
        print(screen, "Could not write block.\n");
        return;
    }

    n = bdev_read(dev, 0x010000, test_block_2, 512);
    if (n != 512) {
        print(screen, "Could not read block.\n");
        return;
    }

    for (i = 0; i < 512; i++) {
        if (test_block_1[i] != test_block_2[i]) {
            print(screen, "Block did not verify.\n");
            return;
        }
    }

    print(screen, "BDEV read/write success.\n");
}

int main(int argc, char * argv[]) {
    short result;

    initialize();

    print(CDEV_CONSOLE, "Text Channel A\n");
    print(CDEV_EVID, "Text Channel B\n");

    // uart_test_send(0);

    /* Register a handler for the SOF interrupt and enable it */
    int_register(0x00, int_sof_a);
    int_enable(0x00);

    // uart_init(0);
    // uart_test_send(0);

    // test_get_mbr(0, BDEV_SDC);
    // try_write(0, "0:");
    // dos_cmd_dir(0, "0:");
    // try_mo(0);
    // try_mo_scancodes(0);
    // try_bdev_getput(0, BDEV_SDC);

    cli_repl(0);

    DEBUG("Stopping.");

    /* Infinite loop... */
    while (1) {};
}
