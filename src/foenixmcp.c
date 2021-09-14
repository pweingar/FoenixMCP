/*
 * Startup file for the Foenix/MCP
 */

#include <string.h>
#include "sys_general.h"
#include "syscalls.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/text_screen_iii.h"
#include "dev/pata.h"
#include "dev/ps2.h"
#include "dev/sdc.h"
#include "fatfs/ff.h"
#include "log.h"

/*
 * Initialize the kernel systems.
 */
void initialize() {
    text_init();          // Initialize the text channels
    DEBUG("Foenix/MCP starting up...");

    cdev_init_system();   // Initialize the channel device system
    DEBUG("Channel device system ready.");

    bdev_init_system();   // Initialize the channel device system
    DEBUG("Block device system ready.");

    if (con_install()) {
        DEBUG("FAILED: Console installation.");
    } else {
        DEBUG("Console installed.");
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

    if (bdev_init(BDEV_SDC)) {
        DEBUG("Unable to initialize the SDC!");
    } else {
        DEBUG("SDC initialized.");
    }
}

void print(short channel, char * message) {
  syscall(SYS_CHAN_WRITE, channel, message, strlen(message));
}

void print_hex(short channel, unsigned short x) {
    char number[3];
    short digit;
    char hex_digits[] = "0123456789ABCDEF";

    digit = (x & 0xf0) >> 4;
    number[0] = hex_digits[digit];

    digit = (x & 0x0f);
    number[1] = hex_digits[digit];

    number[2] = 0;

    print(channel, number);
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

int main(int argc, char * argv[]) {
    short x;
    short result;
    char buffer[512];
    initialize();

    for (x = 0; x < 512; x++) {
        buffer[x] = 0;
    }

    print(CDEV_CONSOLE, "Hello from Screen A!\n");
    print(CDEV_EVID, "Hello from Screen B!\n");

    // print(1, "Hard drive sector 0:\n");
    // result = bdev_read(BDEV_SDC, 0, buffer, 512);
    // if (result > 0) {
    //     for (x = 0; x < result; x++) {
    //         if (x % 16 == 0) {
    //             print(1, "\n");
    //         }
    //         print_hex(1, buffer[x]);
    //         print(1, " ");
    //     }
    //     print(1, "\n");
    // } else if (result < 0) {
    //     DEBUG("IDE returned an error.");
    // } else {
    //     DEBUG("IDE returned nothing.");
    // }

    print(1, "\nSDC directory:\n");
    dos_cmd_dir(1);

    repl(1);

    DEBUG("Stopping.");

    /* Infinite loop... */
    while (1) {};
}
