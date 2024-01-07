/*
 * Startup file for the Foenix/MCP
 */

#include "log_level.h"
#define DEFAULT_LOG_LEVEL LOG_DEBUG
#define LOG_CHANNEL LOG_CHANNEL_CHANNEL_A

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "features.h"
#include "sys_general.h"
#include "simpleio.h"
#include "log.h"
#include "indicators.h"
#include "interrupt.h"
#include "gabe_reg.h"

#if HAS_SUPERIO
#include "superio.h"
#endif

#if MODEL == MODEL_FOENIX_A2560K
#include "dev/kbd_mo.h"
#endif
#if HAS_DUAL_SCREEN
#include "dev/txt_a2560k_a.h"
#include "dev/txt_a2560k_b.h"
#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
#include "dev/txt_a2560u.h"
#elif MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS || MODEL == MODEL_FOENIX_FMX
#include "dev/txt_c256.h"
#include "dev/txt_evid.h"
#endif

#include "syscalls.h"
#include "timers.h"
#include "boot.h"
#include "dev/bitmap.h"
#include "memory.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/fdc.h"
#include "dev/lpt.h"
#include "dev/midi.h"
#include "dev/pata.h"
#include "dev/ps2.h"
#include "dev/rtc.h"
#include "dev/sdc.h"
#include "dev/txt_screen.h"

#include "dev/uart.h"
#include "snd/codec.h"
#include "snd/psg.h"
#include "snd/sid.h"
#include "snd/yamaha.h"
#include "variables.h"
#include "vicky_general.h"
#include "fatfs/ff.h"
#include "cli/cli.h"
#include "rsrc/font/MSX_CP437_8x8.h"
#include "rsrc/bitmaps/splash_c256_u.h"

const char* VolumeStr[FF_VOLUMES] = { "sd", "fd", "hd" };

extern unsigned long __memory_start;

#if HAS_SUPERIO
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
	// TODO: bring back...
    // print(channel, message);
    // print(channel, ": ");
    // print_hex_16(channel, code);
    // print(channel, "\n");
}

t_sys_info info;

/*
 * Initialize the kernel systems.
 */
void initialize() {
    long target_jiffies;
    int i;
    short res;

    /* Setup logging early */
    log_init();
	log_setlevel(DEFAULT_LOG_LEVEL);

	/* Fill out the system information */
	sys_get_information(&info);

    #if 0
    char msg[] = "This is some text to test that the debug to UART works ok\r\n";
    {
        char *c = (char*)msg;
        while (*c) {
            uart_put(1, *c++);
        }
    }

    // The text below gets corrupted. VBCC libc's not being properly initialized if we didn't call ___main ?
    //DEBUG("This is some text to test that the debug to UART works ok");
    #endif

    /* Initialize the memory system */
    mem_init(0x3d0000);

    /* Hide the mouse */
    mouse_set_visible(0);

    /* Initialize the variable system */
    var_init();

    /* Initialize the text channels */
    txt_init();
#if HAS_DUAL_SCREEN
    txt_a2560k_a_install();
    txt_a2560k_b_install();
    log(LOG_INFO, "Initializing screens...");
    txt_init_screen(TXT_SCREEN_A2560K_A);
    txt_init_screen(TXT_SCREEN_A2560K_B);

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
    txt_a2560u_install();
    txt_init_screen(TXT_SCREEN_A2560U);

#elif MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS || MODEL == MODEL_FOENIX_FMX
	// Install and initialize the main screen text driver
	txt_c256_install();
	txt_init_screen(TXT_SCREEN_C256);

	// If the EVID card is plugged in, install and initialize the EVID driver
	if (info.screens > 1) {
		short result = txt_evid_install();
		txt_init_screen(TXT_SCREEN_EVID);
	}

#else
#error Cannot identify screen setup
#endif

	INFO("Text system initialized...");

	// Initialize the bitmap system
	bm_init();
	INFO("Bitmap system initialized...");

    /* Initialize the indicators */
    ind_init();
    INFO("Indicators initialized");

    /* Initialize the interrupt system */
    int_init();
	INFO("Interrupts initialized");

    /* Mute the PSG */
    psg_mute_all();

    /* Initialize and mute the SID chips */
    sid_init_all();

//     /* Initialize the Yamaha sound chips (well, turn their volume down at least) */
//     ym_init();

    /* Initialize the CODEC */
    init_codec();

    cdev_init_system();   // Initialize the channel device system
    INFO("Channel device system ready.");

    bdev_init_system();   // Initialize the channel device system
    INFO("Block device system ready.");

    if ((res = con_install())) {
        log_num(LOG_ERROR, "FAILED: Console installation", res);
    } else {
        INFO("Console installed.");
    }

    /* Initialize the timers the MCP uses */
    timers_init();
	INFO("Timers initialized");

    /* Initialize the real time clock */
    rtc_init();
	INFO("Real time clock initialized");

    target_jiffies = sys_time_jiffies() + 300;     /* 5 seconds minimum */
    DEBUG1("target_jiffies assigned: %d", target_jiffies);

    /* Enable all interrupts */
    int_enable_all();
    TRACE("Interrupts enabled");

//     /* Play the SID test bong on the Gideon SID implementation */
//     sid_test_internal();

    if ((res = pata_install())) {
        log_num(LOG_ERROR, "FAILED: PATA driver installation", res);
    } else {
        INFO("PATA driver installed.");
    }

    if ((res = sdc_install())) {
        ERROR1("FAILED: SDC driver installation %d", res);
    } else {
        INFO("SDC driver installed.");
    }

#if HAS_FLOPPY
    if ((res = fdc_install())) {
        ERROR1("FAILED: Floppy drive initialization %d", res);
    } else {
        INFO("Floppy drive initialized.");
    }
#endif

    // At this point, we should be able to call into to console to print to the screens

    if ((res = ps2_init())) {
        print_error(0, "FAILED: PS/2 keyboard initialization", res);
    } else {
        log(LOG_INFO, "PS/2 keyboard initialized.");
    }

#if MODEL == MODEL_FOENIX_A2560K
    if ((res = kbdmo_init())) {
        log_num(LOG_ERROR, "FAILED: A2560K built-in keyboard initialization", res);
    } else {
        log(LOG_INFO, "A2560K built-in keyboard initialized.");
    }
#endif

#if HAS_PARALLEL_PORT
    if ((res = lpt_install())) {
        log_num(LOG_ERROR, "FAILED: LPT installation", res);
    } else {
        log(LOG_INFO, "LPT installed.");
    }
#endif

#if HAS_MIDI_PORTS
    if ((res = midi_install())) {
        log_num(LOG_ERROR, "FAILED: MIDI installation", res);
    } else {
        log(LOG_INFO, "MIDI installed.");
    }
#endif

//     if (res = uart_install()) {
//         log_num(LOG_ERROR, "FAILED: serial port initialization", res);
//     } else {
//         log(LOG_INFO, "Serial ports initialized.");
//     }

//     if (res = cli_init()) {
//         log_num(LOG_ERROR, "FAILED: CLI initialization", res);
//     } else {
//         INFO("CLI initialized.");
//     }

    if ((res = fsys_init())) {
        log_num(LOG_ERROR, "FAILED: file system initialization", res);
    } else {
        INFO("File system initialized.");
    }
}

#define BOOT_DEFAULT    -1  // User chose default, or the time to over-ride has passed

t_file_info file;

int main(int argc, char * argv[]) {
    short result;
    short i;
	char message[256];

    initialize();

    // // Make sure the command path is set to the default before we get started
    // cli_command_set("");

    // // Display the splash screen and wait for user input
    // short boot_dev = boot_screen();

    // // Start the boot process
    // boot_from_bdev(boot_dev);

    // log(LOG_INFO, "Stopping.");

	printf("\n\nSDC directory:\n");
	int directory = fsys_opendir("/sd/");
	while (fsys_readdir(directory, &file) == 0) {
		if (file.name[0] == 0) {
			break;
		}
		printf("%s\n", file.name);
	}
	fsys_closedir(directory);

	printf("\n\nHDC directory:\n");
	directory = fsys_opendir("/hd/");
	while (fsys_readdir(directory, &file) == 0) {
		if (file.name[0] == 0) {
			break;
		}
		printf("%s\n", file.name);
	}
	fsys_closedir(directory);

	txt_clear(0, 2);
	bm_fill((uint8_t *)0xb00000, 0, 640, 480);
	bm_load_clut(0, splashscreen_lut);
	bm_load_rle((uint8_t *)0xb00000, splashscreen_pix, 640, 480);
	bm_set_data(0, (uint8_t *)0xb00000);
	bm_set_visibility(0, 0, 1);

#ifdef _CALYPSI_MCP_DEBUGGER
	extern int CalypsiDebugger(void);
	CalypsiDebugger(); // This will not return
#endif

    /* Infinite loop... */
    while (1) {
	};
}
