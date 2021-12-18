/*
 * Test commands
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

#include "cli.h"
#include "cli/test_cmds.h"
#include "cli/sound_cmds.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/fdc.h"
#include "dev/fsys.h"
#include "dev/lpt.h"
#include "dev/rtc.h"
#include "dev/uart.h"
#include "fatfs/ff.h"
#include "interrupt.h"
#include "log.h"
#include "dev/ps2.h"
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

/*
 * Test the PS/2 keyboard
 */
int cli_test_ps2(short channel, int argc, char * argv[]) {
    char message[80];
    unsigned short scancode = 0;

    sprintf(message, "Press keys on a PS/2 keyboard... ESC to quit.\n");
    sys_chan_write(channel, message, strlen(message));

    while (scancode != 0x01) {
        scancode = kbd_get_scancode();
        if (scancode != 0) {
            sprintf(message, "Scan code: %04x\n", scancode);
            sys_chan_write(channel, message, strlen(message));
        }
    }

    return 0;
}

/*
 * Test the joystick ports
 */
int cli_test_joystick(short channel, int argc, char * argv[]) {
    char message[80];
    volatile unsigned int * joystick_port = 0xFEC00500;
    volatile unsigned int * game_ctrl_port = 0xFEC00504;
    unsigned int joy_state = 0, old_joy_state = 0xffffffff;
    unsigned short scancode = 0;

    sprintf(message, "Plug a joystick into either port 0 or port 1... ESC to quit.\n");
    sys_chan_write(channel, message, strlen(message));

    /* Make sure we're in Atari joystick mode */
    *game_ctrl_port = 0;

    while (scancode != 0x01) {
        joy_state = *joystick_port;
        if (joy_state != old_joy_state) {
            old_joy_state = joy_state;
            sprintf(message, "Joystick: %08X\n", joy_state);
            sys_chan_write(channel, message, strlen(message));
        }

        scancode = sys_kbd_scancode();
    }

    return 0;
}

/*
 * Test SNES gamepads
 */
int cli_test_gamepad(short channel, int argc, char * argv[]) {
    char message[80];
    volatile unsigned int * game_ctrl_port = 0xFEC00504;
    volatile unsigned int * game_0_0_port = 0xFEC00508;
    volatile unsigned int * game_0_1_port = 0xFEC0050C;
    volatile unsigned int * game_1_0_port = 0xFEC00510;
    volatile unsigned int * game_1_1_port = 0xFEC00514;
    unsigned int game_ctrl;
    unsigned int game_status;
    unsigned int game_state_0 = 0;
    unsigned int game_state_1 = 0;
    unsigned int old_game_state_0 = 0xffffffff;
    unsigned int old_game_state_1 = 0xffffffff;
    unsigned short scancode = 0;
    unsigned short port = 0;

    if (argc > 1) {
        port = (unsigned short)(cli_eval_number(argv[1]) & 0x0000ffff);
        if (port > 1) {
            port = 1;
        }
    }

    sprintf(message, "Testing SNES gamepad port %d... ESC to quit.\n", port);
    sys_chan_write(channel, message, strlen(message));

    /* Make sure we're in SNES mode */
    if (port == 0) {
        /* Port #0 is SNES */
        game_ctrl = 0x00000005;
    } else {
        /* Port #1 is SNES */
        game_ctrl = 0x0000000A;
    }

    *game_ctrl_port = game_ctrl;

    while (scancode != 0x01) {
        /* Start transferring the data and wait for completion */
        *game_ctrl_port = game_ctrl | 0x00000080;
        do {
            game_status = *game_ctrl_port;
        } while ((game_status & 0x00000040) == 0) ;

        if (port == 0) {
            game_state_0 = *game_0_0_port;
            game_state_1 = *game_0_1_port;
        } else {
            game_state_0 = *game_1_0_port;
            game_state_1 = *game_1_1_port;
        }

        if ((game_state_0 != old_game_state_0) || (game_state_1 != old_game_state_1)) {
            old_game_state_0 = game_state_0;
            old_game_state_1 = game_state_1;
            sprintf(message, "Gamepads: %08X %08X\n", game_state_0, game_state_1);
            sys_chan_write(channel, message, strlen(message));
        }

        scancode = sys_kbd_scancode();
    }

    *game_ctrl_port = 0;
    return 0;
}

short cli_test_bitmap(short channel, int argc, const char * argv[]) {
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

    return 0;
}

short cli_test_uart(short channel, int argc, const char * argv[]) {
    char c;
    char buffer[80];

    uart_init(0);
    uart_setbps(0, UART_115200);
    uart_setlcr(0, LCR_DATABITS_8 | LCR_STOPBIT_1 | LCR_PARITY_NONE);

    sprintf(buffer, "COM1: 115200, no parity, 1 stop bit, 8 data bits\nPress ESC to finish (%d).\n", UART_115200);
    sys_chan_write(0, buffer, strlen(buffer));

    for (;;) {
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

    return 0;
}

short cli_test_panic(short channel, int argc, const char * argv[]) {
    volatile int x = 0;
    return argc / x;
}

/*
 * Try using the RTC periodic interrupt in polled mode
 */
short cli_test_rtc(short channel, int argc, const char * argv[]) {
    char buffer[80];
    char * spinner = "|/-\\";
    short count = 0;
    long ticks;

    *RTC_RATES = 0x0e;          /* Periodic interrupt rate: 250 ms */
    *RTC_ENABLES = RTC_PIE;     /* Turn on the periodic interrupt */
    int_enable(INT_RTC);

    ticks = sys_time_jiffies();

    sprintf(buffer, "Waiting for updated ticks starting from %ld\n", ticks);
    sys_chan_write(channel, buffer, strlen(buffer));

    for (;;) {
        if (ticks < sys_time_jiffies()) {
            /* We got the periodic interrupt */

            sprintf(buffer, "Tick! %ld\n", ticks);
            sys_chan_write(channel, buffer, strlen(buffer));

            ticks = sys_time_jiffies();
        }
    }

    return 0;
}

/*
 * Test the memory
 */
short cli_mem_test(short channel, int argc, const char * argv[]) {
    volatile unsigned char * memory = 0x00000000;
    t_sys_info sys_info;
    const long mem_start = 0x00050000; /* TODO find out better where the kernel stop */
    long mem_end;
    char message[80];
    long i;

    sys_get_info(&sys_info);
    mem_end = sys_info.system_ram_size;

    sprintf(message, "\x1B[H\x1B[2JTesting memory...");
    sys_chan_write(channel, message, strlen(message));

    for (i = mem_start; i < mem_end; i++) {
        memory[i] = 0x55; /* Every other bit starting with 1 */
        if (memory[i] != 0x55) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0x55... read %02X at %p\n\n", memory[i], (void*)i);
            sys_chan_write(channel, message, strlen(message));
            return ERR_GENERAL;
        }

        memory[i] = 0xAA; /* Every other bit starting with 0 */
        if (memory[i] != 0xAA) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0xAA... read %02X at %p\n\n", memory[i], (void*)i);
            sys_chan_write(channel, message, strlen(message));
            return ERR_GENERAL;
        }

        memory[i] = 0x00;
        if (memory[i] != 0x00) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0x00... read %02X at %p\n\nX", memory[i], (void*)i);
            sys_chan_write(channel, message, strlen(message));
            return ERR_GENERAL;
        }

        if ((i % 1024) == 0) {
            sprintf(message, "\x1B[H\x1B[0KMemory tested: %p", (void*)i);
            sys_chan_write(channel, message, strlen(message));
        }
    }

    sprintf(message, "\x1B[H\x1B[2JMemory passed basic tests.\n\n");
    sys_chan_write(channel, message, strlen(message));

    return 0;
}

#if MODEL == MODEL_FOENIX_A2560K
int cli_test_recalibrate(short screen, int argc, char * argv[]) {
    unsigned char buffer[512];
    short i;
    short result;

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_ON, 0, 0);

    sprintf(buffer, "Recalibrating the floppy drive\n");
    sys_chan_write(screen, buffer, strlen(buffer));

    if (fdc_recalibrate() == 0) {
        sprintf(buffer, "Success\n");
        sys_chan_write(screen, buffer, strlen(buffer));
    } else {
        sprintf(buffer, "Failed\n");
        sys_chan_write(screen, buffer, strlen(buffer));
    }

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_OFF, 0, 0);
    return 0;
}

int cli_test_seek(short screen, int argc, char * argv[]) {
    unsigned char buffer[512];
    short i;
    unsigned char cylinder;
    short result;

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_ON, 0, 0);

    cylinder = (unsigned char)cli_eval_number(argv[1]);

    sprintf(buffer, "Seeking to %d\n", cylinder);
    sys_chan_write(screen, buffer, strlen(buffer));

    if (fdc_seek(cylinder) == 0) {
        sprintf(buffer, "Success\n");
        sys_chan_write(screen, buffer, strlen(buffer));
    } else {
        sprintf(buffer, "Failed\n");
        sys_chan_write(screen, buffer, strlen(buffer));
    }

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_OFF, 0, 0);
    return 0;
}

/*
 * Test the FDC interface by reading the MBR
 */
int cli_test_fdc(short screen, int argc, char * argv[]) {
    unsigned char buffer[512];
    short i;
    short scancode;
    short n = 0;
    short result;

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_ON, 0, 0);

    result = fdc_init();

    //result = bdev_init(BDEV_FDC);
    if (result != 0) {
        sprintf(buffer, "Could not initialize FDC: %s\n", sys_err_message(result));
        sys_chan_write(screen, buffer, strlen(buffer));
        return result;
    }

    while (1) {
        for (i = 0; i < 512; i++) {
            buffer[i] = 0xAA;
        }

        n = bdev_read(BDEV_FDC, 0, buffer, 512);
        if (n < 0) {
            dump_buffer(screen, buffer, 512, 1);
            err_print(screen, "Unable to read MBR", n);
            bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_OFF, 0, 0);
            return n;
        }

        dump_buffer(screen, buffer, 512, 1);

        print(screen, "\n\n");

        scancode = sys_kbd_scancode();
        if (scancode == 0x01) {
            break;
        }
    }

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_OFF, 0, 0);
}
#endif

/*
 * Test the IDE interface by reading the MBR
 */
short cli_test_ide(short screen, int argc, const char * argv[]) {
    unsigned char buffer[512];
    short i;
    short scancode;
    short n = 0;

    for (;;) {
        n = bdev_read(BDEV_HDC, 0, buffer, 512);
        if (n <= 0) {
            err_print(screen, "Unable to read MBR", n);
            return n;
        }

        dump_buffer(screen, buffer, 512, 1);

        print(screen, "\n\n");

        return 0;

        scancode = sys_kbd_scancode();
        if (scancode == 0x01) {
            break;
        }
    }

    return 0;
}

/*
 * Test file creation
 */
short cli_test_create(short screen, int argc, const char * argv[]) {
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
            return ERR_GENERAL;
        }

    } else {
        print(screen, "USAGE: TEST CREATE <path>\n");
        return ERR_GENERAL;
    }
}

short cli_test_lpt(short screen, int argc, const char * argv[]) {
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

short cmd_test_print(short screen, int argc, const char * argv[]) {
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

const t_cli_test_feature cli_test_features[] = {
    {"BITMAP", "BITMAP: test the bitmap screen", cli_test_bitmap},
    {"CREATE", "CREATE <path>: test creating a file", cli_test_create},
    {"IDE", "IDE: test reading the MBR of the IDE drive", cli_test_ide},
#if MODEL == MODEL_FOENIX_A2560K
    {"FDC", "FDC: test reading the MBR from the floppy drive", cli_test_fdc},
    {"GAMEPAD", "GAMEPAD [0 | 1]: test SNES gamepads", cli_test_gamepad},
    {"JOY", "JOY: test the joystick", cli_test_joystick},
#endif
    {"LPT", "LPT: test the parallel port", cli_test_lpt},
    {"MEM", "MEM: test reading and writing memory", cli_mem_test},
    {"MIDILOOP", "MIDILOOP: perform a loopback test on the MIDI ports", midi_loop_test},
    {"MIDIRX", "MIDIRX: perform a receive test on the MIDI ports", midi_rx_test},
    {"MIDITX", "MIDITX: send a note to a MIDI keyboard", midi_tx_test},
    {"OPL3", "OPL3: test the OPL3 sound chip", opl3_test},
    {"OPN", "OPN [EXT|INT]: test the OPN sound chip", opm_test},
    {"OPM", "OPM [EXT|INT]: test the OPM sound chip", opm_test},
    {"PANIC", "PANIC: test the kernel panic mechanism", cli_test_panic},
    {"PS2", "PS2: test the PS/2 keyboard", cli_test_ps2},
    {"PSG", "PSG: test the PSG sound chip", psg_test},
    {"PRINT", "PRINT: sent text to the printer", cmd_test_print},
#if MODEL == MODEL_FOENIX_A2560K
    {"RECALIBRATE", "RECALIBRATE: recalibrate the floppy drive", cli_test_recalibrate},
    {"SEEK", "SEEK <track>: move the floppy drive head to a track", cli_test_seek},
    {"SID", "SID [EXT|INT]: test the SID sound chips", sid_test},
#endif
    {"UART","UART: test the serial port",cli_test_uart},
    {"END", "END", 0}
};

void test_help(short screen) {
    p_cli_test_feature f;
    int i;
    int count;

    print(screen, "USAGE: TEST <feature>\nFeatures supported...\n");

    count = sizeof(cli_test_features) / sizeof(t_cli_test_feature);
    for (i = 0; i < count - 1; i++) {
        if (cli_test_features[i].help != 0) {
            print(screen, cli_test_features[i].help);
            print(screen, "\n");
        }
    }
}

/*
 * Test command
 */
short cmd_test(short screen, int argc, const char * argv[]) {
    short i;
    p_cli_test_feature f;

    if (argc > 1) {
        char feature_upcase[80];

        /* Make an upper case copy of the feature to test */
        strncpy(feature_upcase, argv[1], 80);
        for (i = 0; i < strlen(feature_upcase); i++) {
            feature_upcase[i] = toupper(feature_upcase[i]);
        }

        for (f = (p_cli_test_feature)cli_test_features; f->name != 0; f++) {
            if (strcmp(f->name, feature_upcase) == 0) {
                f->handler(screen, argc - 1, &argv[1]);
                return 0;
            }
        }

        test_help(screen);
        return ERR_GENERAL;
    } else {
        test_help(screen);
        return ERR_GENERAL;
    }

    return 0;
}
