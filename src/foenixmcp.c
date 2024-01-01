/*
 * Startup file for the Foenix/MCP
 */

#include "log_level.h"
#ifndef DEFAULT_LOG_LEVEL
    #define DEFAULT_LOG_LEVEL LOG_TRACE
#endif

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
#include "libfoenix/include/kbd_mo.h"
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
#include "libfoenix/include/bitmap.h"
#include "memory.h"
#include "dev/block.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "libfoenix/include/fdc.h"
#include "libfoenix/include/lpt.h"
#include "libfoenix/include/midi.h"
#include "dev/hdd.h"
#include "libfoenix/include/ps2.h"
#include "libfoenix/include/rtc.h"
#include "libfoenix/include/sdc.h"
#include "dev/txt_screen.h"

#include "libfoenix/include/uart.h"
#include "snd/codec.h"
#include "snd/psg.h"
#include "snd/sid.h"
#include "snd/yamaha.h"
#include "variables.h"
#include "vicky_general.h"
#include "fatfs/ff.h"
#include "cli/cli.h"
#include "rsrc/font/MSX_CP437_8x8.h"
//#include "rsrc/bitmaps/image.h"

/* Performs all hardware initializations */
static void hw_initialize(void);

/* Install OS drivers */
static void os_devices_initialize(void);


void mcp_init(void) {
    hw_initialize();
	os_devices_initialize();
}


int main(int argc, char * argv[]) {
    short result;
    short i;

	mcp_init();
	buzzer_off();

    // At this point, we should be able to call into to console to print to the screens
    /* Play the SID test bong on the Gideon SID implementation */
    sid_test_internal();
    TRACE("Internal SID tested");

    if (result = cli_init()) {
        ERROR1("FAILED: CLI initialization (%d)", result);
    } else {
        INFO("CLI initialized.");
    }
	
#if MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS || MODEL == MODEL_FOENIX_A2560X
    // Make sure the command path is set to the default before we get started
    cli_command_set("");
TRACE("Calling boot_screen()");
    // Display the splash screen and wait for user input
    short boot_dev = boot_screen();
TRACE1("Booting from device %d",boot_dev);
    // Start the boot process
    boot_from_bdev(boot_dev);

    INFO("Stopping.");
#elif MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS || MODEL == MODEL_FOENIX_FMX
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
#endif

    /* Infinite loop... TODO: we could STOP the processor. */
    while (1) {
	};
}


const char* VolumeStr[FF_VOLUMES] = { "sd", "fd", "hd" };

t_sys_info info;

/*
 * Initialize the hardware.
 */
static void hw_initialize(void) {
    int i;
    short res;
	
    /* Setup logging early to facilitate troubleshooting. This may require the UART to be
	 * functional (e.g. if debugging to the serial port) so it may call uart_init. */

#if HAS_SUPERIO
    /* Initialize the SuperIO chip. We do this early so to have the UARTs available for debug output */
    superio_init();
    INFO("SuperIO initialized");
#endif
    log_init();

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

    /* Initialize the memory system.
     * TODO: The amount of RAM should depend on the machine! E.g; U only has 2MB. */
    mem_init(0x3d0000);

    /* Initialize the indicators */
    ind_init();
    INFO("Indicators initialized");

    /* Initialize the interrupt system */
    int_init();
    INFO("Interrupts initialized");

    /* Initialize the timers the MCP uses */
    timers_init();
	INFO("Timers initialized");

    /* Initialize the real time clock */
    rtc_init();
	INFO("Real time clock initialized");

	/* Mute the PSG */
    psg_mute_all();
    INFO("PSGs muted");

    /* Initialize and mute the SID chips */
    sid_init_all();
    INFO("SIDs initialized");

    /* Initialize the Yamaha sound chips (well, turn their volume down at least) */
    ym_init();
    INFO("Yamaha soundchips initialized");

    /* Initialize the CODEC (mixer chip) */
    init_codec();

	/* PS/2 keyboard and mouse */
    if ((res = ps2_init())) {
        ERROR1("FAILED: PS/2 keyboard initialization (%d)", res);
    } else {
        INFO("PS/2 keyboard initialized.");
    }

#if MODEL == MODEL_FOENIX_A2560K
	/* A2560K's special keyboard controller */
    if ((res = kbdmo_init())) {
        ERROR1("FAILED: A2560K built-in keyboard initialization (%d)", res);
    } else {
        INFO("A2560K built-in keyboard initialized.");
    }
#endif

#if MODEL == MODEL_FOENIX_C256U || MODEL == MODEL_FOENIX_C256U_PLUS || MODEL == MODEL_FOENIX_FMX
	// Initialize the bitmap system
	bm_init();
	INFO("Bitmap system initialized...");
#endif

	/* Initialize the text channels */
    txt_init();

    /* Enable all interrupts */
    int_enable_all();
    TRACE("Interrupts enabled");

	/* Hide the mouse */
    mouse_set_visible(0);

	/* Other devices are not directly initialized. They are initialized through their OS "device",
	 * either eagerly or lazyly (i.e whenever the device opened). */	
}


/* Install OS devices */
static void os_devices_initialize(void) {
	short res;

#if HAS_PARALLEL_PORT
	/* Parallel port */
    if ((res = lpt_install())) {
        ERROR1("FAILED: LPT installation (%d)", res);
    } else {
        INFO("LPT installed.");
    }
#endif

#if HAS_MIDI_PORTS
	/* MIDI ports */
    if ((res = midi_install())) {
        ERROR1("FAILED: MIDI installation (%d)", res);
    } else {
        INFO("MIDI installed.");
    }
#endif

    if (res = uart_install()) {
        ERROR1("FAILED: serial port initialization (%d)", res);
    } else {
        INFO("Serial ports initialized.");
    }

    /* Initialize the variable system */
    var_init();
	
    /* Initialize the text channels: register the driver for the screens, then use txt_init_screen. */
#if HAS_DUAL_SCREEN
    txt_a2560k_a_install();
    txt_a2560k_b_install();
    INFO("Initializing screens...");
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

    cdev_init_system();   // Initialize the channel device system
    INFO("Channel device system ready.");

    bdev_init_system();   // Initialize the block device system
    INFO("Block device system ready.");

    if ((res = con_install())) {
        ERROR1("FAILED: Console installation (%d)", res);
    } else {
        INFO("Console installed.");
    }

    if ((res = hdd_install())) {
        ERROR1("FAILED: PATA driver installation (%d)", res);
    } else {
        INFO("PATA driver installed.");
    }

    if ((res = sdc_install())) {
        ERROR1("FAILED: SDC driver installation (%d)", res);
    } else {
        INFO("SDC driver installed.");
    }

#if HAS_FLOPPY
    if ((res = fdc_install())) {
        ERROR1("FAILED: Floppy drive initialization (%d)", res);
    } else {
        INFO("Floppy drive initialized.");
    }
#endif

    if ((res = fsys_init())) {
        ERROR1("FAILED: file system initialization (%d)", res);
    } else {
        INFO("File system initialized.");
    }
}
