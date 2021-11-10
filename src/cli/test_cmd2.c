/*
 * Test commands
 */

#include <ctype.h>
#include <string.h>

#include "cli.h"
#include "cli/test_cmds.h"
#include "cli/sound_cmds.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/fsys.h"
#include "dev/lpt.h"
#include "dev/rtc.h"
#include "dev/uart.h"
#include "fatfs/ff.h"
#include "interrupt.h"
#include "rtc_reg.h"
#include "simpleio.h"
#include "syscalls.h"
#include "sys_general.h"
#include "uart_reg.h"
#include "vicky_general.h"

#define LPT_DATA_PORT   ((volatile unsigned char *)0x00C02378)

#define LPT_STAT_PORT   ((volatile unsigned char *)0x00C02379)
#define LPT_STAT_BUSY   0x80
#define LPT_STAT_ACK    0x40
#define LPT_STAT_PO     0x20
#define LPT_STAT_SELECT 0x10
#define LPT_STAT_ERROR  0x08
#define LPT_STAT_IRQ    0x04

#define LPT_CTRL_PORT   ((volatile unsigned char *)0x00C0237A)
#define LPT_CTRL_STROBE 0x01
#define LPT_CTRL_AL     0x02
#define LPT_CTRL_INIT   0x04
#define LPT_CTRL_SELECT 0x08
#define LPT_CTRL_IRQE   0x10
#define LPT_CTRL_BI     0x20

#define LPT_INIT_ON     0x08            /* Start the printer initialization process */
#define LPT_INIT_OFF    0x0C            /* Stop the printer initialization process */
#define LPT_STROBE_ON   0x0F            /* Strobe the printer */
#define LPT_STROBE_OFF  0x0E            /* Drop the strobe to the printer */

typedef struct s_cli_test_feature {
    const char * name;
    const char * help;
    cli_cmd_handler handler;
} t_cli_test_feature, * p_cli_test_feature;

/*
 * Tests...
 */

short cli_test_bitmap(short channel, int argc, char * argv[]) {
    int i,m,p;
    unsigned char j;
    unsigned short k;

    *MasterControlReg_A = VKY3_MCR_BITMAP_EN | VKY3_MCR_GRAPH_EN;   // Enable bitmap graphics
    *BM0_Control_Reg = 0x00000001;      // Enable BM0
    *BM0_Addy_Pointer_Reg = 0x00000000; //Pointing to Starting of VRAM Bank A
    // *BorderControlReg_L = 0x00000800;   // Enable
    k = 0;
    for (i = 0; i< 256; i++) {
        LUT_0[4*i] = i;
        LUT_0[4*i+1] = i;
        LUT_0[4*i+2] = i;
        LUT_0[4*i+3] = 0;
    }

    for (i = 0; i< 640 * 480; i++) {
        VRAM_Bank0[i] = i & 0xff;
    }
}

short cli_test_uart(short channel, int argc, char * argv[]) {
    char c;
    char buffer[80];

    uart_init(0);
    uart_setbps(0, UART_115200);
    uart_setlcr(0, LCR_DATABITS_8 | LCR_STOPBIT_1 | LCR_PARITY_NONE);

    sprintf(buffer, "COM1: 115200, no parity, 1 stop bit, 8 data bits\nPress ESC to finish (%d).\n", UART_115200);
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

    ticks = sys_time_jiffies();

    sprintf(buffer, "Waiting for updated ticks starting from %d\n", ticks);
    sys_chan_write(channel, buffer, strlen(buffer));

    while (1) {
        if (ticks < sys_time_jiffies()) {
            /* We got the periodic interrupt */

            sprintf(buffer, "Tick! %d\n", ticks);
            sys_chan_write(channel, buffer, strlen(buffer));

            ticks = sys_time_jiffies();
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

        dump_buffer(screen, buffer, 512, 1);

        print(screen, "\n\n");

        scancode = sys_kbd_scancode();
        if (scancode == 0x01) {
            break;
        }
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

short cli_test_lpt(short screen, int argc, char * argv[]) {
#if MODEL == MODEL_FOENIX_A2560K
    char message[80];
    unsigned char scancode;

    sprintf(message, "Test parallel port:\nF1: DATA=00  F2: DATA=FF  F3: STRB=1  F4: STRB=0\n");
    sys_chan_write(screen, message, strlen(message));
    sprintf(message, "F5: INIT=1  F6: INIT=0  F7: SEL=1  F8: SEL=0\nESC: Quit");
    sys_chan_write(screen, message, strlen(message));

    while (1) {
        scancode = sys_kbd_scancode();
        switch (scancode) {
            case 0x3B:      /* F1 */
                *LPT_DATA_PORT = 0;
                break;

            case 0x3C:      /* F2 */
                *LPT_DATA_PORT = 0xff;
                break;

            case 0x3D:      /* F3 */
                *LPT_CTRL_PORT = LPT_CTRL_STROBE;
                break;

            case 0x3E:      /* F4 */
                *LPT_CTRL_PORT = 0;
                break;

            case 0x3F:      /* F5 */
                *LPT_CTRL_PORT = 0;
                break;

            case 0x40:      /* F6 */
                *LPT_CTRL_PORT = LPT_CTRL_INIT;
                break;

            case 0x41:      /* F7 */
                *LPT_CTRL_PORT = LPT_CTRL_SELECT;
                break;

            case 0x42:      /* F8 */
                *LPT_CTRL_PORT = 0;
                break;

            case 0x1B:      /* ESC */
                return 0;

            default:
                break;
        }
    }
#endif
    return 0;
}

short cmd_test_print(short screen, int argc, char * argv[]) {
#if MODEL == MODEL_FOENIX_A2560K
    const char * test_pattern = "0123456789ABCDEFGHIJKLMNOPQRTSUVWZXYZ\r\n";

    char message[80];
    unsigned short scancode = 0;

    sprintf(message, "Initializing printer...\n");
    sys_chan_write(screen, message, strlen(message));

    lpt_initialize();

    sprintf(message, "Sending test patterns to printer (ESC to quit)...\n");
    sys_chan_write(screen, message, strlen(message));

    while (scancode != 0x01) {
        scancode = sys_kbd_scancode();
        lpt_write(0, test_pattern, strlen(test_pattern));
    }
#endif
    return 0;
}

static t_cli_test_feature cli_test_features[] = {
    {"BITMAP", "BITMAP: test the bitmap screen", cli_test_bitmap},
    {"CREATE", "CREATE <path>: test creating a file", cli_test_create},
    {"IDE", "IDE: test reading the MBR of the IDE drive", cli_test_ide},
    {"PANIC", "PANIC: test the kernel panic mechanism", cli_test_panic},
    {"LPT", "LPT: test the parallel port", cli_test_lpt},
    {"MEM", "MEM: test reading and writing memory", cli_mem_test},
    {"MIDILOOP", "MIDILOOP: perform a loopback test on the MIDI ports", midi_loop_test},
    {"MIDIRX", "MIDIRX: perform a receive test on the MIDI ports", midi_rx_test},
    {"MIDITX", "MIDITX: send a note to a MIDI keyboard", midi_tx_test},
    {"OPL2", "OPL2: test the OPL2 sound chip", opl2_test},
    {"OPL3", "OPL3: test the OPL3 sound chip", opl3_test},
    {"PSG", "PSG: test the PSG sound chip", psg_test},
    {"PRINT", "PRINT: sent text to the printer", cmd_test_print},
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
