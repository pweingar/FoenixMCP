/*
 * Startup file for the Foenix/MCP
 */

#include <string.h>
#include "sys_general.h"
#include "gabe_reg.h"
#include "superio.h"
#include "syscalls.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/text_screen_iii.h"
#include "dev/pata.h"
#include "dev/ps2.h"
#include "dev/sdc.h"
#include "dev/uart.h"
#include "snd/codec.h"
#include "snd/sid.h"
#include "fatfs/ff.h"
#include "log.h"

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

/*
 * Initialize the kernel systems.
 */
void initialize() {
    text_init();          // Initialize the text channels
    DEBUG("Foenix/MCP starting up...");

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
    sid_init();

    /* Play the SID test bong on the Gideon SID implementation */
    sid_test_internal();

    cdev_init_system();   // Initialize the channel device system
    DEBUG("Channel device system ready.");

    bdev_init_system();   // Initialize the channel device system
    DEBUG("Block device system ready.");

    if (con_install()) {
        DEBUG("FAILED: Console installation.");
    } else {
        DEBUG("Console installed.");
    }

    if (pata_install()) {
        DEBUG("FAILED: PATA driver installation.");
    } else {
        DEBUG("PATA driver installed.");
    }

    if (sdc_install()) {
        DEBUG("FAILED: SDC driver installation.");
    } else {
        DEBUG("SDC driver installed.");
    }

    // At this point, we should be able to call into to console to print to the screens

    if (ps2_init()) {
        DEBUG("FAILED: PS/2 initialization.");
    } else {
        DEBUG("PS/2 initialized.");
    }

    if (bdev_init(BDEV_HDC)) {
        DEBUG("Unable to initialize the PATA!");
    } else {
        DEBUG("PATA initialized.");
    }

    if (bdev_init(BDEV_SDC)) {
        DEBUG("Unable to initialize the SDC!");
    } else {
        DEBUG("SDC initialized.");
    }
}

void print(short channel, char * message) {
    //syscall(KFN_CHAN_WRITE, channel, message, strlen(message));
    char * c;
    for (c = message; *c; c++) {
        text_put_raw(channel, *c);
    }
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

#define KBD_MO_STAT_FULL    0x4000
#define KBD_MO_STAT_EMPTY   0x8000

void try_mo(short screen) {
    volatile unsigned short * kbd_mo_data = ((volatile unsigned short *)0x00C00040);
    volatile unsigned short * kbd_mo_stat = ((volatile unsigned short *)0x00C00042);

    print(screen, "mo> ");

    do {
        unsigned short status = *kbd_mo_stat;
        if ((status & KBD_MO_STAT_EMPTY) != KBD_MO_STAT_EMPTY) {
            unsigned short data = *kbd_mo_data;
            print(screen, "[");
            print_hex_16(screen, status);
            print(screen, "]: {");

            print_hex_16(screen, data);
            print(screen, "}\n ");
        // }
        }

        // unsigned short data = *kbd_mo_data;
        // if (data != 0) {
        //     print(screen, "{");
        //     print_hex_16(screen, data);
        //     print(screen, "}\n ");
        // }
    } while (1);
}

DIR my_dir;
FILINFO my_file;
FATFS my_fs;
char line[255];

short dos_cmd_dir(short screen) {
    FRESULT fres;

    TRACE("dos_cmd_dir");

    fres = f_mount(&my_fs, "0:", 0);
    TRACE("f_mount");
    if (fres == FR_OK) {
        fres = f_opendir(&my_dir, "/");
        TRACE("f_opendir");
        if (fres == FR_OK) {
            do {
                fres = f_readdir(&my_dir, &my_file);
                TRACE("f_readdir");
                if ((fres == FR_OK) && (my_file.fname[0] != 0)) {
                    if ((my_file.fattrib & AM_HID) == 0) {
                        chan_write(screen, my_file.fname, strlen(my_file.fname));
                        if (my_file.fattrib & AM_DIR) {
                            chan_write_b(screen, '/');
                        }
                        chan_write_b(screen, '\n');
                    }
                } else {
                    break;
                }
            } while(1);

            f_closedir(&my_dir);
        } else {
            char * err = "Could not open directory.\r";
            chan_write(screen, err, strlen(err));
        }

        f_mount(0, "", 0);
    } else {
        char * err = "Could not mount drive.\r";
        chan_write(screen, err, strlen(err));
    }

    return 0;
}


void repl(short screen) {
    print(screen, "> ");
    while (1) {
        char c = kbd_getc_poll();
        if (c) {
            char buffer[2];
            buffer[0] = c;
            buffer[1] = 0;
            print(screen, buffer);
        }
    }
}

void test_get_mbr(short screen, short device) {
    short x;
    short result;
    char buffer[512];

    for (x = 0; x < 512; x++) {
        buffer[x] = 0;
    }

    print(1, "Master Boot Record:\n");
    result = bdev_read(device, 0, buffer, 512);
    if (result > 0) {
        for (x = 0; x < result; x++) {
            if (x % 16 == 0) {
                print(screen, "\n");
            }
            print_hex(screen, buffer[x]);
            print(screen, " ");
        }
        print(screen, "\n");
    } else if (result < 0) {
        DEBUG("IDE returned an error.");
    } else {
        DEBUG("IDE returned nothing.");
    }
}

void uart_send(short uart, char * message) {
    int i;

    uart_init(uart);
    for (i = 0; i < strlen(message); i++) {
        uart_put(uart, message[i]);
    }
}

int main(int argc, char * argv[]) {
    short result;

    initialize();

    print(CDEV_CONSOLE, "Foenix/MCP\n\nText Channel A\n");
    print(CDEV_EVID, "Foenix/MCP\n\nText Channel B\n");

    // Try to get the MBR of the HDD
    // test_get_mbr(CDEV_EVID, BDEV_HDD);

    // print(1, "\nSDC directory:\n");
    // dos_cmd_dir(1);

    // Try the A2560K built in keybaord
    // try_mo(1);

    // lpt_initialize();
    //
    // result = lpt_write((p_channel)0, "Hello\n", 5);
    // if (result) {
    //     print(CDEV_CONSOLE, "\nError printing.\n");
    // } else {
    //     print(CDEV_CONSOLE, "\nPrinting success.\n");
    // }

    DEBUG("Stopping.");

    /* Infinite loop... */
    while (1) {};
}
