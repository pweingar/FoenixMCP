/*
 * Test commands
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "cli.h"
#include "cli/test_cmds.h"
#include "cli/sound_cmds.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/fdc.h"
#include "dev/fsys.h"
#include "dev/lpt.h"
#include "dev/rtc.h"
#include "dev/txt_screen.h"
#include "dev/uart.h"
#include "fatfs/ff.h"
#include "interrupt.h"
#include "log.h"
#include "lpt_reg.h"
#include "dev/ps2.h"
#include "rtc_reg.h"
#include "simpleio.h"
#include "syscalls.h"
#include "sys_general.h"
#include "uart_reg.h"
#include "vicky_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "dev/kbd_mo.h"
#endif

typedef struct s_cli_test_feature {
    const char * name;
    const char * help;
    cli_cmd_handler handler;
} t_cli_test_feature, * p_cli_test_feature;

/*
 * Tests...
 */

/*
 * Return true if the BREAK key has been pressed
 */
short kbd_break() {
    /* Channel 0, CON_IOCTRL_BREAK */
    return sys_chan_ioctrl(0, 5, 0, 0);
}

/*
 * Test the PS/2 keyboard
 */
short cli_test_ps2(short channel, int argc, const char * argv[]) {
    char message[80];
    unsigned short scancode = 0;

    sprintf(message, "Press keys on a PS/2 keyboard... CTRL-C to quit.\n");
    sys_chan_write(channel, message, strlen(message));

    do {
        if (scancode != 0) {
            sprintf(message, "Scan code: %04x\n", scancode);
            sys_chan_write(channel, message, strlen(message));
        }
    } while (sys_chan_ioctrl(channel, 0x05, 0, 0) == 0);

    return 0;
}

/*
 * Test the joystick ports
 */
short cli_test_joystick(short channel, int argc, const char * argv[]) {
    char message[80];
    volatile unsigned int * joystick_port = (volatile unsigned int *)0xFEC00500;
    volatile unsigned int * game_ctrl_port = (volatile unsigned int *)0xFEC00504;
    unsigned int joy_state = 0, old_joy_state = 0xffffffff;
    unsigned short scancode = 0;

    sprintf(message, "Plug a joystick into either port 0 or port 1... ESC to quit.\n");
    sys_chan_write(channel, message, strlen(message));

    /* Make sure we're in Atari joystick mode */
    *game_ctrl_port = 0;

    do {
        joy_state = *joystick_port;
        if (joy_state != old_joy_state) {
            old_joy_state = joy_state;
            sprintf(message, "Joystick: %08X\n", joy_state);
            sys_chan_write(channel, message, strlen(message));
        }

        scancode = sys_kbd_scancode();
    } while (sys_chan_ioctrl(channel, 5, 0, 0) == 0);

    return 0;
}

/*
 * Test SNES gamepads
 */
short cli_test_gamepad(short channel, int argc, const char * argv[]) {
    char message[80];
    volatile unsigned int * game_ctrl_port = (volatile unsigned int *)0xFEC00504;
    volatile unsigned int * game_0_0_port = (volatile unsigned int *)0xFEC00508;
    volatile unsigned int * game_0_1_port = (volatile unsigned int *)0xFEC0050C;
    volatile unsigned int * game_1_0_port = (volatile unsigned int *)0xFEC00510;
    volatile unsigned int * game_1_1_port = (volatile unsigned int *)0xFEC00514;
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

    sprintf(message, "Testing SNES gamepad port %d... ESC to quit.\n", (short)port);
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
    short scan_code;

    // Switch to bitmap mode
    sys_txt_set_mode(0, TXT_MODE_BITMAP);

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

    do {
        scan_code = sys_kbd_scancode();
    } while (scan_code != 0x01);

    // Go back to text mode
    sys_txt_set_mode(0, TXT_MODE_TEXT);

    return 0;
}

/**
 * Test for the serial ports
 *
 * TEST UART [1 | 2]
 */
short cli_test_uart(short channel, int argc, const char * argv[]) {
    char c, c_out;
    short scan_code;
    char buffer[80];
    short cdev = CDEV_COM1;
    short uart = -1;
    short uart_index = 0;
    unsigned long uart_address = 0;

    if (argc > 1) {
        // Get the COM port number
        short port = (short)cli_eval_number(argv[1]);
        if (port <= 1) cdev = CDEV_COM1;
        if (port >= 2) cdev = CDEV_COM2;
    }

    uart_index = cdev - CDEV_COM1;
    uart_address = (unsigned long)uart_get_base(uart_index);

    sprintf(buffer, "Serial port loopback test of COM%d at 0x%08X...\n", cdev - CDEV_COM1 + 1, uart_address);
    print(channel, buffer);

    uart = sys_chan_open(cdev, "9600,8,1,NONE", 0);
    if (uart >= 0) {
        sprintf(buffer, "COM%d: 9600, no parity, 1 stop bit, 8 data bits\nPress ESC to finish.\n", cdev - CDEV_COM1 + 1);
        print(channel, buffer);

        c_out = ' ';
        do {
            sys_chan_write_b(uart, c_out++);
            if (c_out > '}') {
                c_out = ' ';
                sys_chan_write_b(uart, '\r');
                sys_chan_write_b(uart, '\n');
            }

            if (sys_chan_status(uart) & CDEV_STAT_READABLE) {
                c = sys_chan_read_b(uart);
                if (c != 0) {
                    sys_chan_write_b(channel, c);
                }
            }

            scan_code = sys_kbd_scancode();
        } while (scan_code != 0x01);
    } else {
        sprintf(buffer, "Unable to open the serial port: %d\n", uart);
        print(channel, buffer);
    }

    return 0;
}

/**
 * Do a simple test of a kernel panic using division by zero
 */
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
 * MEM [SYS|MERA]
 */
short cli_mem_test(short channel, int argc, const char * argv[]) {
    volatile unsigned char * memory = 0x00000000;
    t_sys_info sys_info;
    unsigned long mem_start = 0x00010000;
    unsigned long mem_end = sys_mem_get_ramtop();
    char message[80];
    unsigned long i;

#if MODEL == MODEL_FOENIX_A2560K
    if (argc > 1) {
        if ((strcmp(argv[1], "MERA") == 0) || (strcmp(argv[1], "mera") == 0)) {
            mem_start = 0x02000000;
            mem_end = 0x06000000;

            print(channel, "\x1B[H\x1B[2JTesting MERA memory...");
        }
    }
#endif

    sprintf(message, "\x1B[H\x1B[2JTesting memory from 0x%08X to 0x%08X\n", (unsigned long)mem_start, (unsigned long)mem_end);
    print(channel, message);

    for (i = mem_start; i < mem_end; i++) {
        memory[i] = 0x55; /* Every other bit starting with 1 */
        if (memory[i] != 0x55) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0x55... read %02X at %p\n\n", memory[i], (void*)i);
            print(channel, message);
            return ERR_GENERAL;
        }

        memory[i] = 0xAA; /* Every other bit starting with 0 */
        if (memory[i] != 0xAA) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0xAA... read %02X at %p\n\n", memory[i], (void*)i);
            print(channel, message);
            return ERR_GENERAL;
        }

        memory[i] = 0x00;
        if (memory[i] != 0x00) {
            sprintf(message, "\x1B[1;2H\x1B[KFailed to write 0x00... read %02X at %p\n\nX", memory[i], (void*)i);
            print(channel, message);
            return ERR_GENERAL;
        }

        if ((i % 1024) == 0) {
            sprintf(message, "\x1B[1;2H\x1B[0KMemory tested: %p", (void*)i);
            print(channel, message);
        }
    }

    print(channel, "\x1B[1;3H\x1B[0KMemory passed basic tests.\n\n");

    return 0;
}

#if MODEL == MODEL_FOENIX_A2560K
short cli_test_recalibrate(short screen, int argc, const char * argv[]) {
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

short cli_test_seek(short screen, int argc, const char * argv[]) {
    unsigned char buffer[512];
    short i;
    unsigned char cylinder;
    short result;

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_ON, 0, 0);

    cylinder = (unsigned char)cli_eval_number(argv[1]);

    sprintf(buffer, "Seeking to %d\n", (short)cylinder);
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
 *
 * TEST FDC [<lba> [WRITE <data>]]
 */
short cli_test_fdc(short screen, int argc, const char * argv[]) {
    unsigned char buffer[512];
    char message[80];
    unsigned long lba = 0;
    short i;
    short scancode;
    short n = 0;
    short result;
    short is_write = 0;
    unsigned char data = 0xAA;

    if (argc > 1) {
        lba = (unsigned long)cli_eval_number(argv[1]);
        if (argc > 2) {
            print(screen, "Will attempt to write before reading...\n");
            is_write = 1;
            data = (unsigned long)cli_eval_number(argv[3]);
        }
    }

    bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_ON, 0, 0);

    result = bdev_init(BDEV_FDC);
    if (result != 0) {
        sprintf(buffer, "Could not initialize FDC: %s\n", sys_err_message(result));
        sys_chan_write(screen, buffer, strlen(buffer));
        return result;
    }

    for (i = 0; i < 512; i++) {
        buffer[i] = data;
    }

    if (is_write) {
        n = bdev_write(BDEV_FDC, lba, buffer, 512);
        if (n < 0) {
            dump_buffer(screen, buffer, 512, 1);
            sprintf(message, "Unable to write sector %d: %s\n", lba, err_message(n));
            print(screen, message);
            bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_OFF, 0, 0);
            return n;
        }
    }

    for (i = 0; i < 512; i++) {
        buffer[i] = 0xAA;
    }

    n = bdev_read(BDEV_FDC, lba, buffer, 512);
    if (n < 0) {
        dump_buffer(screen, buffer, 512, 1);
        sprintf(message, "Unable to read sector %d: %s\n", lba, err_message(n));
        print(screen, message);
        bdev_ioctrl(BDEV_FDC, FDC_CTRL_MOTOR_OFF, 0, 0);
        return n;
    }

    dump_buffer(screen, buffer, 512, 1);

    print(screen, "\n\n");

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

    sprintf(message, "Test parallel port:\nF1: DATA='B'  F2: DATA='A'  F3: STRB=1  F4: STRB=0\n");
    sys_chan_write(screen, message, strlen(message));
    sprintf(message, "F5: INIT=1  F6: INIT=0  F7: SEL=1  F8: SEL=0\nESC: Quit\n");
    sys_chan_write(screen, message, strlen(message));

    unsigned char ctrl = 0;
    *LPT_CTRL_PORT = ctrl;

    while (1) {
        scancode = sys_kbd_scancode();
        switch (scancode) {
            case 0x3B:      /* F1 */
                print(0, "DATA = 'B'\n");
                *LPT_DATA_PORT = 'B';
                break;

            case 0x3C:      /* F2 */
                print(0, "DATA = 'A'\n");
                *LPT_DATA_PORT = 'A';
                break;

            case 0x3D:      /* F3 */
                ctrl |= LPT_CTRL_STROBE;
                *LPT_CTRL_PORT = ctrl;
                sprintf(message, "STROBE = TRUE [%02X]\n", ctrl);
                print(0, message);
                break;

            case 0x3E:      /* F4 */
                ctrl &= ~LPT_CTRL_STROBE;
                *LPT_CTRL_PORT = ctrl;
                sprintf(message, "STROBE = FALSE [%02X]\n", ctrl);
                print(0, message);
                break;

            case 0x3F:      /* F5 */
                ctrl |= LPT_CTRL_mINIT;
                *LPT_CTRL_PORT = ctrl;
                sprintf(message, "INIT = TRUE [%02X]\n", ctrl);
                print(0, message);
                break;

            case 0x40:      /* F6 */
                ctrl &= ~LPT_CTRL_mINIT;
                *LPT_CTRL_PORT = ctrl;
                sprintf(message, "INIT = FALSE [%02X]\n", ctrl);
                print(0, message);
                break;

            case 0x41:      /* F7 */
                ctrl |= LPT_CTRL_SELECT;
                *LPT_CTRL_PORT = ctrl;
                sprintf(message, "SELECT = TRUE [%02X]\n", ctrl);
                print(0, message);
                break;

            case 0x42:      /* F8 */
                ctrl &= ~LPT_CTRL_SELECT;
                *LPT_CTRL_PORT = ctrl;
                sprintf(message, "SELECT = FALSE [%02X]\n", ctrl);
                print(0, message);
                break;

            case 0x01:      /* ESC */
                return 0;

            case 0x02:      /* 1 */
                *LPT_DATA_PORT = '\r';
                break;

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

    print(screen, "Initializing printer...\n");

    short lpt = sys_chan_open(CDEV_LPT, 0, 0);
    if (lpt > 0) {
        print(screen, "Sending test patterns to printer (ESC to quit)...\n");

        while (scancode != 0x01) {
            short result = sys_chan_write(lpt, test_pattern, strlen(test_pattern));
            if (result != 0) {
                sprintf(message, "Unable to print: %s\n", err_message(result));
                print(screen, message);

                sprintf(message, "Printer status: %02X\n", sys_chan_status(lpt));
                print(screen, message);
                break;
            }
            scancode = sys_kbd_scancode();
        }

        sys_chan_close(lpt);
    } else if (lpt == 0) {
        print(screen, "Unable to print: got a bad channel number.\n");

    } else {
        sprintf(message, "Could not open channel to printer: %s\n", err_message(lpt));
        print(screen, message);
    }
#endif
    return 0;
}

/**
 * Test the ANSI escape codes
 */
short cmd_test_ansi(short screen, int argc, const char * argv[]) {
    t_rect old_region, region;
    char buffer[255];

    // Clear the screen and home the cursor
    print(screen, "\x1b[2J\x1b[H0_________1_________2_________3_________\n");
    print(screen, "0123456789012345678901234567890123456789\n");

    // Test some positioning
    print(screen, "\x1b[21;3H20\x1b[50;1H\x1b[31m\x03\x1b[B\x1b[30m\x05\x1b[3D\x06\x1b[1B\x1b[31m\x04\n");

    // Test color bars
    for (int i = 0; i < 8; i++) {
        char * color_name;
        switch (i) {
            case 0: color_name = "BLACK"; break;
            case 1: color_name = "RED"; break;
            case 2: color_name = "GREEN"; break;
            case 3: color_name = "YELLOW"; break;
            case 4: color_name = "BLUE"; break;
            case 5: color_name = "ORANGE"; break;
            case 6: color_name = "CYAN"; break;
            case 7: color_name = "GREY"; break;
            default: color_name = "???"; break;
        }

        sprintf(buffer, "\x1b[%dm%10s\x1b[37;44m \x1b[30;%dm%10s\x1b[37;44m", 30 + i, color_name, 40 + i, color_name);
        sys_chan_write(screen, buffer, strlen(buffer));

        sprintf(buffer, "\t\x1b[%dm%10s\x1b[37;44m \x1b[30;%dm%10s\x1b[37;44m\n", 90 + i, color_name, 100 + i, color_name);
        sys_chan_write(screen, buffer, strlen(buffer));
    }

    print(screen, "\x1b[1;13H0123456789ABCDEF\x1b[6;13H\x1b[2@^^\x1b[20;13H<--2 characters inserted");
    print(screen, "\x1b[1;14H0123456789ABCDEF\x1b[6;14H\x1b[2P\x1b[20;14H<--2 characters deleted");
    print(screen, "\x1b[1;15H0123456789ABCDEF\x1b[8;15H\x1b[0K\x1b[20;15H<--2nd half of line deleted");
    print(screen, "\x1b[1;16H0123456789ABCDEF\x1b[8;16H\x1b[1K\x1b[20;16H<--1st half of line deleted");
    print(screen, "\x1b[1;17H0123456789ABCDEF\x1b[8;17H\x1b[2K\x1b[20;17H<-- Whole line deleted...\n");

    print(screen, "\x1b[1;20HDelete 2nd Half\x1b[22;20HDelete 1st Half\x1b[43;20HDelete All");

    txt_get_region(screen, &old_region);

    region.origin.x = 0;
    region.origin.y = 20;
    region.size.width = 20;
    region.size.height = 10;
    txt_set_region(screen, &region);
    txt_fill(screen, 'X');
    region.origin.x = 1;
    region.origin.y = 21;
    region.size.width = 18;
    region.size.height = 8;
    txt_set_region(screen, &region);
    print(screen, "\x1b[10;4H\x1b[0J");

    region.origin.x = 21;
    region.origin.y = 20;
    region.size.width = 20;
    region.size.height = 10;
    txt_set_region(screen, &region);
    txt_fill(screen, 'X');
    region.origin.x = 22;
    region.origin.y = 21;
    region.size.width = 18;
    region.size.height = 8;
    txt_set_region(screen, &region);
    print(screen, "\x1b[10;4H\x1b[1J");

    region.origin.x = 42;
    region.origin.y = 20;
    region.size.width = 20;
    region.size.height = 10;
    txt_set_region(screen, &region);
    txt_fill(screen, 'X');
    region.origin.x = 43;
    region.origin.y = 21;
    region.size.width = 18;
    region.size.height = 8;
    txt_set_region(screen, &region);
    print(screen, "\x1b[1;4H\x1b[2J");

    txt_set_region(screen, &old_region);
    print(screen, "\x1b[1;35H\x1b[0;44;37mDone!\n");

    return 0;
}

const t_cli_test_feature cli_test_features[] = {
    {"ANSI", "ANSI: test the ANSI escape codes", cmd_test_ansi},
    {"BITMAP", "BITMAP: test the bitmap screen", cli_test_bitmap},
    {"CREATE", "CREATE <path>: test creating a file", cli_test_create},
    {"IDE", "IDE: test reading the MBR of the IDE drive", cli_test_ide},
#if MODEL == MODEL_FOENIX_A2560K
    {"FDC", "FDC: test reading the MBR from the floppy drive", cli_test_fdc},
    {"GAMEPAD", "GAMEPAD [0 | 1]: test SNES gamepads", cli_test_gamepad},
    {"JOY", "JOY: test the joystick", cli_test_joystick},
#endif
    {"LPT", "LPT: test the parallel port", cli_test_lpt},
    {"MEM", "MEM [SYS|MERA]: test reading and writing of system or MERA memory", cli_mem_test},
    {"MIDILOOP", "MIDILOOP: perform a loopback test on the MIDI ports", midi_loop_test},
    {"MIDIRX", "MIDIRX: perform a receive test on the MIDI ports", midi_rx_test},
    {"MIDITX", "MIDITX: send a note to a MIDI keyboard", midi_tx_test},
    {"OPL3", "OPL3: test the OPL3 sound chip", opl3_test},
    {"OPN", "OPN [EXT|INT]: test the OPN sound chip", opn_test},
    {"OPM", "OPM [EXT|INT]: test the OPM sound chip", opm_test},
    {"PANIC", "PANIC: test the kernel panic mechanism", cli_test_panic},
    {"PS2", "PS2: test the PS/2 keyboard", cli_test_ps2},
    {"PSG", "PSG [EXT|INTL|INTR|INTS]: test the PSG sound chip", psg_test},
    {"PRINT", "PRINT: sent text to the printer", cmd_test_print},
#if MODEL == MODEL_FOENIX_A2560K
    {"RECALIBRATE", "RECALIBRATE: recalibrate the floppy drive", cli_test_recalibrate},
    {"SEEK", "SEEK <track>: move the floppy drive head to a track", cli_test_seek},
    {"SID", "SID [EXT|INT]: test the SID sound chips", sid_test},
#endif
    {"UART","UART [1|2]: test the serial port",cli_test_uart},
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
