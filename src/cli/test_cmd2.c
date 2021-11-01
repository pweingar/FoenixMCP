/*
 * Test commands
 */

#include <ctype.h>
#include <string.h>

#include "cli.h"
#include "cli/test_cmds.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/fsys.h"
#include "dev/rtc.h"
#include "dev/uart.h"
#include "fatfs/ff.h"
#include "interrupt.h"
#include "rtc_reg.h"
#include "simpleio.h"
#include "syscalls.h"
#include "sys_general.h"
#include "uart_reg.h"

typedef struct s_cli_test_feature {
    const char * name;
    const char * help;
    cli_cmd_handler handler;
} t_cli_test_feature, * p_cli_test_feature;

/*
 * Tests...
 */

short cli_test_uart(short channel, int argc, char * argv[]) {
    char c;
    char buffer[80];

    uart_init(0);
    uart_setbps(0, UART_115200);
    uart_setlcr(0, LCR_DATABITS_8 | LCR_STOPBIT_1 | LCR_PARITY_NONE);

    sprintf(buffer, "COM1: 115200, no parity, 1 stop bit, 8 data bits\nPress ESC to finish.\n");
    sys_chan_write(0, buffer, strlen(buffer));

    while (1) {
        c = kbd_getc();
        if (c != 0) {
            if (c == 0x1b) {
                return 0;
            }
            uart_put(0, c);
        } else if (uart_has_bytes(0)) {
            c = uart_get(0);
            sys_chan_write_b(channel, c);
        }
    }
}

short cli_test_panic(short channel, int argc, char * argv[]) {
    volatile int x = 0;
    return argc / x;
}

/*
 * Try using the RTC periodic interrupt in polled mode
 */
short cli_test_rtc(short channel, int argc, char * argv[]) {
    char buffer[80];
    char * spinner = "|/-\\";
    short count = 0;
    long ticks;

    *RTC_RATES = 0x0e;          /* Periodic interrupt rate: 250 ms */
    *RTC_ENABLES = RTC_PIE;     /* Turn on the periodic interrupt */
    int_enable(INT_RTC);

    ticks = rtc_get_ticks();

    sprintf(buffer, "Waiting for updated ticks starting from %d\n", ticks);
    sys_chan_write(channel, buffer, strlen(buffer));

    while (1) {
        if (ticks < rtc_get_ticks()) {
            /* We got the periodic interrupt */

            sprintf(buffer, "Tick! %d\n", ticks);
            sys_chan_write(channel, buffer, strlen(buffer));

            ticks = rtc_get_ticks();
        }
    }
}

/*
 * Test the memory
 */
short cli_mem_test(short channel, int argc, char * argv[]) {
    volatile unsigned char * memory = 0x00000000;
    t_sys_info sys_info;
    const long mem_start = 0x00050000;
    long mem_end;
    char message[80];
    long i;

    sys_get_info(&sys_info);
    mem_end = sys_info.system_ram_size;

    sprintf(message, "\x1B[H\x1B[2JTesting memory...");
    sys_chan_write(channel, message, strlen(message));

    for (i = mem_start; i < mem_end; i++) {
        memory[i] = 0x55;
        if (memory[i] != 0x55) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0x55... read %02X at %08X\n\n", memory[i], i);
            sys_chan_write(channel, message, strlen(message));
            return -1;
        }

        memory[i] = 0xAA;
        if (memory[i] != 0xAA) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0xAA... read %02X at %08X\n\n", memory[i], i);
            sys_chan_write(channel, message, strlen(message));
            return -1;
        }

        memory[i] = 0x00;
        if (memory[i] != 0x00) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0x00... read %02X at %08\n\nX", memory[i], i);
            sys_chan_write(channel, message, strlen(message));
            return -1;
        }

        if ((i % 1024) == 0) {
            sprintf(message, "\x1B[H\x1B[0KMemory tested: %08X", i);
            sys_chan_write(channel, message, strlen(message));
        }
    }

    sprintf(message, "\x1B[H\x1B[2JMemory passed basic tests.\n\n");
    sys_chan_write(channel, message, strlen(message));

    return 0;
}

/*
 * Test the IDE interface by reading the MBR
 */
short cli_test_ide(short screen, int argc, char * argv[]) {
    unsigned char buffer[512];
    short i;
    short scancode;
    short n = 0;

    while (1) {
        n = bdev_read(BDEV_HDC, 0, buffer, 512);
        if (n <= 0) {
            err_print(screen, "Unable to read MBR", n);
            return n;
        }

        for (i = 0; i < n; i++) {
            if ((i % 16) == 0) {
                print(screen, "\n");
            }

            print_hex_8(screen, buffer[i]);
            print(screen, " ");
        }

        print(screen, "\n\n");

        // if (kbdmo_getc_poll()) break;
    }
}

/*
 * Test file creation
 */
short cli_test_create(short screen, int argc, char * argv[]) {
    short n;

    if (argc > 1) {
        short channel = fsys_open(argv[1], FA_CREATE_NEW | FA_WRITE);
        if (channel >= 0) {
            char * message = "Hello, world!\n";
            n = chan_write(channel, message, strlen(message));
            if (n <= 0) {
                err_print(screen, "Unable to write to file", n);
            }

            fsys_close(channel);
            return 0;

        } else {
            err_print(screen, "Unable to open to file", channel);
            return -1;
        }

    } else {
        print(screen, "USAGE: TEST CREATE <path>\n");
        return -1;
    }
}

static t_cli_test_feature cli_test_features[] = {
    {"CREATE", "CREATE <path>: test creating a file", cli_test_create},
    {"IDE", "IDE: test reading the MBR of the IDE drive", cli_test_ide},
    {"PANIC", "PANIC: test the kernel panic mechanism", cli_test_panic},
    {"RTC", "RTC: test the real time clock periodic interrupt", cli_test_rtc},
    {"MEM", "MEM: test reading and writing memory", cli_mem_test},
    {"UART", "UART: test the serial port", cli_test_uart},
    {0, 0}
};

void test_help(short screen) {
    p_cli_test_feature f;

    print(screen, "USAGE: TEST <feature>\nFeatures supported...\n");

    for (f = cli_test_features; f->name != 0; f++) {
        print(screen, f->help);
        print(screen, "\n");
    }
}

/*
 * Test command
 */
short cmd_test(short screen, int argc, char * argv[]) {
    short i;
    p_cli_test_feature f;

    if (argc > 1) {
        char feature_upcase[80];

        /* Make an upper case copy of the feature to test */
        strncpy(feature_upcase, argv[1], 80);
        for (i = 0; i < strlen(feature_upcase); i++) {
            feature_upcase[i] = toupper(feature_upcase[i]);
        }

        for (f = cli_test_features; f->name != 0; f++) {
            if (strcmp(f->name, feature_upcase) == 0) {
                f->handler(screen, argc - 1, &argv[1]);
                return 0;
            }
        }

        test_help(screen);
        return -1;
    } else {
        test_help(screen);
        return -1;
    }
}
