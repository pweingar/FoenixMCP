/*
 * Declarations for Mo, the built-in keyboard of the A2560K
 */

#include <ctype.h>
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K

#include "log.h"
#include "interrupt.h"
#include "kbd_mo.h"
#include "ring_buffer.h"
#include "gabe_reg.h"
#include "simpleio.h"

#define KBD_MO_LEDMATRIX    ((volatile unsigned short *)0xFEC01000) /* 6x16 array of 16-bit words: ARGB */
#define KBD_MO_LED_ROWS     6
#define KBD_MO_LED_COLUMNS  16
#define KBD_MO_DATA         ((volatile unsigned int *)0xFEC00040)   /* Data register for the keyboard (scan codes will be here) */
#define KBD_MO_EMPTY        0x8000                                  /* Status flag that will be set if the keyboard buffer is empty */
#define KBD_MO_FULL         0x4000                                  /* Status flag that will be set if the keyboard buffer is full */

/*
 * Modifier bit flags
 */

#define KBD_LOCK_SCROLL     0x01
#define KBD_LOCK_NUM        0x02
#define KBD_LOCK_CAPS       0x04
#define KBD_MOD_SHIFT       0x08
#define KBD_MOD_ALT         0x10
#define KBD_MOD_CTRL        0x20
#define KBD_MOD_OS          0x40
#define KBD_MOD_MENU        0x80

/*
 * Status codes
 */
#define KBD_STAT_BREAK      0x80        /* BREAK has been pressed recently */

/*
 * Structure to track the keyboard input
 */
struct s_kdbmo_kbd {
    unsigned char control;      /* Bits to control how the keyboard processes things */
    unsigned char status;       /* Status of the keyboard */
    t_word_ring sc_buf;         /* Buffer containing scancodes that have been processed */
    t_word_ring char_buf;       /* Buffer containing characters to be read */
    unsigned char modifiers;    /* State of the modifier keys (CTRL, ALT, SHIFT) and caps lock */

    /* Scan code to character lookup tables */

    char * keys_unmodified;
    char * keys_shift;
    char * keys_control;
    char * keys_control_shift;
    char * keys_caps;
    char * keys_caps_shift;
};

/*
 * Driver global variables
 */

struct s_kdbmo_kbd g_kbdmo_control;
static short kbdmo_leds = 0;

/*
 * Mapping of "codepoints" 0x80 - 0x98 (function keys, etc)
 * to ANSI escape codes
 */
const char * ansi_keys[] = {
    "1",    /* HOME */
    "2",    /* INS */
    "3",    /* DELETE */
    "4",    /* END */
    "5",    /* PgUp */
    "6",    /* PgDn */
    "A",    /* Up */
    "B",    /* Left */
    "C",    /* Right */
    "D",    /* Down */
    "11",   /* F1 */
    "12",   /* F2 */
    "13",   /* F3 */
    "14",   /* F4 */
    "15",   /* F5 */
    "17",   /* F6 */
    "18",   /* F7 */
    "19",   /* F8 */
    "20",   /* F9 */
    "21",   /* F10 */
    "23",   /* F11 */
    "24",   /* F12 */
    "30",   /* MONITOR */
    "31",   /* CTX SWITCH */
    "32"    /* MENU HELP */
};

/*
 * US keyboard layout scancode translation tables
 */

static char g_us_sc_unmodified[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5', '6',           /* 0x00 - 0x07 */
    '7', '8', '9', '0', '-', '=', 0x08, 0x09,           /* 0x08 - 0x0F */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',             /* 0x10 - 0x17 */
    'o', 'p', '[', ']', 0x0D, 0x00, 'a', 's',           /* 0x18 - 0x1F */
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',             /* 0x20 - 0x27 */
    0x27, '`', 0x00, '\\', 'z', 'x', 'c', 'v',          /* 0x28 - 0x2F */
    'b', 'n', 'm', ',', '.', '/', 0x00, '*',            /* 0x30 - 0x37 */
    0x00, ' ', 0x00, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,      /* 0x38 - 0x3F */
    0x8F, 0x90, 0x91, 0x92, 0x93, 0x00, 0x00, 0x80,     /* 0x40 - 0x47 */
    0x86, 0x84, '-', 0x89, '5', 0x88, '+', 0x83,        /* 0x48 - 0x4F */
    0x87, 0x85, 0x81, 0x82, 0x96, 0x97, 0x98, 0x94,     /* 0x50 - 0x57 */
    0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x58 - 0x5F */
    0x00, 0x00, 0x81, 0x80, 0x84, 0x82, 0x83, 0x85,     /* 0x60 - 0x67 */
    0x86, 0x89, 0x87, 0x88, '/', 0x0D, 0x00, 0x00,      /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* 0x78 - 0x7F */
};

static char g_us_sc_shift[] = {
    0x00, 0x1B, '!', '@', '#', '$', '%', '^',           /* 0x00 - 0x07 */
    '&', '*', '(', ')', '_', '+', 0x08, 0x09,           /* 0x08 - 0x0F */
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',             /* 0x10 - 0x17 */
    'O', 'P', '{', '}', 0x0A, 0x00, 'A', 'S',           /* 0x18 - 0x1F */
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',             /* 0x20 - 0x27 */
    0x22, '~', 0x00, '|', 'Z', 'X', 'C', 'V',           /* 0x28 - 0x2F */
    'B', 'N', 'M', '<', '>', '?', 0x00, 0x00,           /* 0x30 - 0x37 */
    0x00, ' ', 0x00, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,      /* 0x38 - 0x3F */
    0x8F, 0x90, 0x91, 0x92, 0x93, 0x00, 0x00, 0x80,     /* 0x40 - 0x47 */
    0x86, 0x84, '-', 0x89, '5', 0x88, '+', 0x83,        /* 0x48 - 0x4F */
    0x87, 0x85, 0x81, 0x82, 0x96, 0x97, 0x98, 0x94,     /* 0x50 - 0x57 */
    0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x58 - 0x5F */
    0x00, 0x00, 0x81, 0x80, 0x84, 0x82, 0x83, 0x85,     /* 0x60 - 0x67 */
    0x86, 0x89, 0x87, 0x88, '/', 0x0D, 0x00, 0x00,      /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* 0x78 - 0x7F */
};

static char g_us_sc_ctrl[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5', 0x1E,          /* 0x00 - 0x07 */
    '7', '8', '9', '0', 0x1F, '=', 0x08, 0x09,          /* 0x08 - 0x0F */
    0x11, 0x17, 0x05, 0x12, 0x14, 0x19, 0x15, 0x09,     /* 0x10 - 0x17 */
    0x0F, 0x10, 0x1B, 0x1D, 0x0A, 0x00, 0x01, 0x13,     /* 0x18 - 0x1F */
    0x04, 0x06, 0x07, 0x08, 0x0A, 0x0B, 0x0C, ';',      /* 0x20 - 0x27 */
    0x22, '`', 0x00, '\\', 0x1A, 0x18, 0x03, 0x16,      /* 0x28 - 0x2F */
    0x02, 0x0E, 0x0D, ',', '.', 0x1C, 0x00, 0x00,       /* 0x30 - 0x37 */
    0x00, ' ', 0x00, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,      /* 0x38 - 0x3F */
    0x8F, 0x90, 0x91, 0x92, 0x93, 0x00, 0x00, 0x80,     /* 0x40 - 0x47 */
    0x86, 0x84, '-', 0x89, '5', 0x88, '+', 0x83,        /* 0x48 - 0x4F */
    0x87, 0x85, 0x81, 0x82, 0x96, 0x97, 0x98, 0x94,     /* 0x50 - 0x57 */
    0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x58 - 0x5F */
    0x00, 0x00, 0x81, 0x80, 0x84, 0x82, 0x83, 0x85,     /* 0x60 - 0x67 */
    0x86, 0x89, 0x87, 0x88, '/', 0x0D, 0x00, 0x00,      /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* 0x78 - 0x7F */
};

static char g_us_sc_lock[] = {
    0x00, 0x1B, '1', '2', '3', '4', '5', '6',           /* 0x00 - 0x07 */
    '7', '8', '9', '0', '-', '=', 0x08, 0x09,           /* 0x08 - 0x0F */
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',             /* 0x10 - 0x17 */
    'O', 'P', '[', ']', 0x0D, 0x00, 'A', 'S',           /* 0x18 - 0x1F */
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',             /* 0x20 - 0x27 */
    0x27, '`', 0x00, '\\', 'Z', 'X', 'C', 'V',          /* 0x28 - 0x2F */
    'B', 'N', 'M', ',', '.', '/', 0x00, 0x00,           /* 0x30 - 0x37 */
    0x00, ' ', 0x00, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,      /* 0x38 - 0x3F */
    0x8F, 0x90, 0x91, 0x92, 0x93, 0x00, 0x00, '7',      /* 0x40 - 0x47 */
    '8', '9', '-', '4', '5', '6', '+', '1',             /* 0x48 - 0x4F */
    '2', '3', '0', '.', 0x96, 0x97, 0x98, 0x94,         /* 0x50 - 0x57 */
    0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x58 - 0x5F */
    0x00, 0x00, 0x81, 0x80, 0x84, 0x82, 0x83, 0x85,     /* 0x60 - 0x67 */
    0x86, 0x89, 0x87, 0x88, '/', 0x0D, 0x00, 0x00,      /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* 0x78 - 0x7F */
};

static char g_us_sc_lock_shift[] = {
    0x00, 0x1B, '!', '@', '#', '$', '%', '^',           /* 0x00 - 0x07 */
    '&', '*', '(', ')', '_', '+', 0x08, 0x09,           /* 0x08 - 0x0F */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',             /* 0x10 - 0x17 */
    'o', 'p', '{', '}', 0x0A, 0x00, 'a', 's',           /* 0x18 - 0x1F */
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ':',             /* 0x20 - 0x27 */
    0x22, '~', 0x00, '|', 'z', 'x', 'c', 'v',           /* 0x28 - 0x2F */
    'b', 'n', 'm', '<', '>', '?', 0x00, 0x00,           /* 0x30 - 0x37 */
    0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      /* 0x38 - 0x3F */
    0x8F, 0x90, 0x91, 0x92, 0x93, 0x00, 0x00, '7',      /* 0x40 - 0x47 */
    '8', '9', '-', '4', '5', '6', '+', '1',             /* 0x48 - 0x4F */
    '2', '3', '0', '.', 0x96, 0x97, 0x98,  0x94,         /* 0x50 - 0x57 */
    0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x58 - 0x5F */
    0x00, 0x00, 0x81, 0x80, 0x84, 0x82, 0x83, 0x85,     /* 0x60 - 0x67 */
    0x86, 0x89, 0x87, 0x88, '/', 0x0D, 0x00, 0x00,      /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* 0x78 - 0x7F */
};

static char g_us_sc_ctrl_shift[] = {
    0x00, 0x1B, '!', '@', '#', '$', '%', '^',           /* 0x00 - 0x07 */
    '&', '*', '(', ')', '_', '+', 0x08, 0x09,           /* 0x08 - 0x0F */
    0x11, 0x17, 0x05, 0x12, 0x14, 0x19, 0x15, 0x09,     /* 0x10 - 0x17 */
    0x0F, 0x10, 0x1B, 0x1D, 0x0A, 0x00, 0x01, 0x13,     /* 0x18 - 0x1F */
    0x04, 0x06, 0x07, 0x08, 0x0A, 0x0B, 0x0C, ';',      /* 0x20 - 0x27 */
    0x22, '`', 0x00, '\\', 0x1A, 0x18, 0x03, 0x16,      /* 0x28 - 0x2F */
    0x02, 0x0E, 0x0D, ',', '.', 0x1C, 0x00, 0x00,       /* 0x30 - 0x37 */
    0x00, ' ', 0x00, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E,      /* 0x38 - 0x3F */
    0x8F, 0x90, 0x91, 0x92, 0x93, 0x00, 0x00, 0x80,     /* 0x40 - 0x47 */
    0x86, 0x84, '-', 0x89, '5', 0x88, '+', 0x83,        /* 0x48 - 0x4F */
    0x87, 0x85, 0x81, 0x82, 0x96, 0x97, 0x98, 0x94,     /* 0x50 - 0x57 */
    0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x58 - 0x5F */
    0x00, 0x00, 0x81, 0x80, 0x84, 0x82, 0x83, 0x85,     /* 0x60 - 0x67 */
    0x86, 0x89, 0x87, 0x88, '/', 0x0D, 0x00, 0x00,      /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* 0x78 - 0x7F */
};

/**
 * Set the color of the A2560K keyboard LED matrix
 *
 * @param row the number of the row to set (0 - 5)
 * @param color the color for the LEDs: ARGB
 */
void kbdmo_set_led_matrix_row(unsigned char row, unsigned short color) {
    int column;
    for (column = 0; column < KBD_MO_LED_COLUMNS; column++) {
        KBD_MO_LEDMATRIX[row * KBD_MO_LED_COLUMNS + column] = color;
    }
}

/**
 * Set all the LEDs to the same color
 *
 * @param color the color for the LEDs: ARGB
 */
void kbdmo_set_led_matrix_fill(unsigned short color) {
    unsigned char row;
    for (row = 0; row < KBD_MO_LED_ROWS; row++) {
        kbdmo_set_led_matrix_row(row, color);
    }
}

/*
 * Make sure everything is removed from Mo's input buffer
 */
void kbdmo_flush_out() {
    long data;

    TRACE("kbdmo_flush_out");

    /* While there is data in the buffer ... */
    do {
        data = *KBD_MO_DATA;
    } while ((data & 0x00ff0000) != 0);
}

/*
 * Check to see if a BREAK code has been pressed recently
 * If so, return 1 and reset the internal flag.
 *
 * BREAK will be F-ESC on the A2560K
 *
 * Returns:
 * 1 if a BREAK has been pressed since the last check
 */
short kbdmo_break() {
    if (g_kbdmo_control.status & KBD_STAT_BREAK) {
        /* BREAK was pressed: clear the flag and return a 1 */
        g_kbdmo_control.status &= ~KBD_STAT_BREAK;
        return 1;
    } else {
        /* BREAK was not pressed: return a 0 */
        return 0;
    }
}

/*
 * Initialize the PS2 controller and any attached devices
 * Enable keyboard and mouse interrupts as appropriate.
 *
 * Returns:
 *  Status code indicating if either the mouse or the keyboard is missing.
 */
short kbdmo_init() {
    TRACE("kbdmo_init");

    int_disable(INT_KBD_A2560K);

    /* Turn off the LEDs */
    kbdmo_set_led_matrix_fill(0);

    /* Set up the ring buffers */

    rb_word_init(&g_kbdmo_control.sc_buf);        /* Scan-code ring buffer is empty */
    rb_word_init(&g_kbdmo_control.char_buf);      /* Character ring buffer is empty */

    /* Set the default keyboard layout to US */

    g_kbdmo_control.keys_unmodified = g_us_sc_unmodified;
    g_kbdmo_control.keys_shift = g_us_sc_shift;
    g_kbdmo_control.keys_control = g_us_sc_ctrl;
    g_kbdmo_control.keys_control_shift = g_us_sc_ctrl_shift;
    g_kbdmo_control.keys_caps = g_us_sc_lock;
    g_kbdmo_control.keys_caps_shift = g_us_sc_lock_shift;

    g_kbdmo_control.status = 0;
    g_kbdmo_control.modifiers = 0;

    /* Make sure everything is read */
    kbdmo_flush_out();

    /* Turn off the LEDs */
    kbdmo_leds = 0;
    *GABE_MO_LEDS = kbdmo_leds;

    /* Clear out any pending interrupt */
    int_clear(INT_KBD_A2560K);

#ifndef KBD_POLLED
    /* Register a handler for the keyboard */
    int_register(INT_KBD_A2560K, kbdmo_handle_irq);

    /* Enable the interrupt for the keyboard */
    int_enable(INT_KBD_A2560K);
#endif

    return 0;
}

/*
 * Toggle the lock bit based on the flag.
 */
void kbdmo_toggle_modifier(short flag) {
    g_kbdmo_control.modifiers ^= flag;

    if (flag == KBD_LOCK_CAPS) {
        if (g_kbdmo_control.modifiers & flag) {
            /* CAPS is on... set it to purple */
            kbdmo_set_caps_led(5);

        } else {
            /* CAPS is off... turn off the LED */
            kbdmo_set_caps_led(0);
        }
    }
}

/*
 * Set or clear the modifier flag depending on if the scan code is a make or break code.
 */
void kbdmo_makebreak_modifier(short flag, short is_break) {
    if (is_break) {
        g_kbdmo_control.modifiers &= ~flag;
    } else {
        g_kbdmo_control.modifiers |= flag;
    }
}

/*
 * Add the scan code to the queue of scan codes
 */
void kbdmo_enqueue_scan(unsigned char scan_code) {
    // Make sure the scan code isn't 0 or 128, which are invalid make/break codes
    if ((scan_code != 0) && (scan_code != 0x80)) {
        unsigned char is_break = scan_code & 0x80;

        // Check the scan code to see if it's a modifier key or a lock key
        // update the modifier and lock variables accordingly...
        switch (scan_code & 0x7f) {
            case 0x01:
                /* ESC key pressed... check to see if it's a press with the Foenix key */
                if (((g_kbdmo_control.modifiers & KBD_MOD_OS) != 0) && (is_break == 0)) {
                    /* ESC pressed with Foenix key... flag a BREAK. */
                    g_kbdmo_control.status |= KBD_STAT_BREAK;
                }
                break;

            case 0x2A:
            case 0x36:
                kbdmo_makebreak_modifier(KBD_MOD_SHIFT, is_break);
                break;

            case 0x1D:
            case 0x5E:
                kbdmo_makebreak_modifier(KBD_MOD_CTRL, is_break);
                break;

            case 0x38:
            case 0x5C:
                kbdmo_makebreak_modifier(KBD_MOD_ALT, is_break);
                break;

            case 0x5D:
                kbdmo_makebreak_modifier(KBD_MOD_MENU, is_break);
                break;

            case 0x5B:
                kbdmo_makebreak_modifier(KBD_MOD_OS, is_break);
                break;

            case 0x3A:
                if (!is_break) kbdmo_toggle_modifier(KBD_LOCK_CAPS);
                break;

            case 0x45:
                if (!is_break) kbdmo_toggle_modifier(KBD_LOCK_NUM);
                break;

            case 0x46:
                if (!is_break) kbdmo_toggle_modifier(KBD_LOCK_SCROLL);
                break;

            default:
                break;
        }

        rb_word_put(&g_kbdmo_control.sc_buf, g_kbdmo_control.modifiers << 8 | scan_code);
    }
}

/*
 * IRQ handler for the keyboard... read a scan code and queue it
 */
void kbdmo_handle_irq() {
    unsigned long data;
    /* We got an interrupt for MO.
     * While there is data in the input queue...
     */

    int_clear(INT_KBD_A2560K);

    /* While there is data in the buffer ... */
    do {
        data = *KBD_MO_DATA;

        /* Read and throw out the scan codes */
        unsigned short scan_code = data & 0xffff;
        if ((scan_code & 0x7fff) != 0) {
            /* TODO: beep if the input was full or the ring buffer is full */

            /* Process it and enqueue it */
            kbdmo_enqueue_scan((unsigned char)(scan_code & 0x00ff));
        }
    } while ((data & 0x00ff0000) != 0);
}


/*
 * Try to retrieve the next scancode from the keyboard.
 *
 * Returns:
 *      The next scancode to be processed, 0 if nothing.
 */
unsigned short kbdmo_get_scancode() {
    unsigned long data;
    unsigned short scan_code = rb_word_get(&g_kbdmo_control.sc_buf);
    if (scan_code != 0) {
        /* Got a result... return it */
        return scan_code;

    } else {
        return 0;
    }
}

/*
 * Catch special keys and convert them to their ANSI terminal codes
 *
 * Characters 0x80 - 0x98 are reserved for function keys, arrow keys, etc.
 * This function maps them to the ANSI escape codes
 *
 * Inputs:
 * modifiers = the current modifier bit flags (ALT, CTRL, META, etc)
 * c = the character found from the scan code.
 */
static char kbd_to_ansi(unsigned char modifiers, unsigned char c) {
    if ((c >= 0x80) && (c <= 0x98)) {
        /* The key is a function key or a special control key */
        const char * ansi_key = ansi_keys[c - 0x80];
        const char * sequence;
        short modifiers_after = 0;

        // Figure out if the modifiers come before or after the sequence code
        if (isdigit(ansi_key[0])) {
            // Sequence is numeric, modifiers come after
            modifiers_after = 1;
        }

        // After ESC, all sequences have [
        rb_word_put(&g_kbdmo_control.char_buf, '[');

        if (modifiers_after) {
            // Sequence is numberic, get the expanded sequence and put it in the queue
            for (sequence = ansi_keys[c - 0x80]; *sequence != 0; sequence++) {
                rb_word_put(&g_kbdmo_control.char_buf, *sequence);
            }
        }

        // Check to see if we need to send a modifier sequence
        if (modifiers & (KBD_MOD_SHIFT | KBD_MOD_CTRL | KBD_MOD_ALT | KBD_MOD_OS)) {
            unsigned char code_bcd;
            short modifier_code = 0;
            short i;

            if (modifiers_after) {
                // Sequence is numeric, so put modifiers after the sequence and a semicolon
                rb_word_put(&g_kbdmo_control.char_buf, ';');
            }

            modifier_code = ((modifiers >> 3) & 0x1F) + 1;
            code_bcd = i_to_bcd(modifier_code);

            if (code_bcd & 0xF0) {
                rb_word_put(&g_kbdmo_control.char_buf, ((code_bcd & 0xF0) >> 4) + '0');
            }
            rb_word_put(&g_kbdmo_control.char_buf, (code_bcd & 0x0F) + '0');
        }

        if (!modifiers_after) {
            // Sequence is a letter code
            rb_word_put(&g_kbdmo_control.char_buf, ansi_key[0]);
        } else {
            // Sequence is numeric, close it with a tilda
            rb_word_put(&g_kbdmo_control.char_buf, '~');
        }

        return 0x1B;    /* Start the sequence with an escape */

    } else if (c == 0x1B) {
        /* ESC should be doubled, to distinguish from the start of an escape sequence */
        rb_word_put(&g_kbdmo_control.char_buf, 0x1B);
        return c;

    } else {
        /* Not a special key: return the character unmodified */
        return c;
    }
}

/*
 * Try to get a character from the keyboard...
 *
 * Returns:
 *      the next character to be read from the keyboard (0 if none available)
 */
char kbdmo_getc() {
    if (!rb_word_empty(&g_kbdmo_control.char_buf)) {
        // If there is a character waiting in the character buffer, return it...
        return (char)rb_word_get(&g_kbdmo_control.char_buf);

    } else {
        // Otherwise, we need to check the scan code queue...
        unsigned short raw_code = kbdmo_get_scancode();
        while (raw_code != 0) {
            if ((raw_code & 0x80) == 0) {
                // If it's a make code, let's try to look it up...
                unsigned char modifiers = (raw_code >> 8) & 0xff;    // Get the modifiers
                unsigned char scan_code = raw_code & 0x7f;           // Get the base code for the key

                // Check the modifiers to see what we should lookup...

                if ((modifiers & (KBD_MOD_SHIFT | KBD_MOD_CTRL | KBD_LOCK_CAPS)) == 0) {
                    // No modifiers... just return the base character
                    return kbd_to_ansi(modifiers, g_kbdmo_control.keys_unmodified[scan_code]);

                } else if (modifiers & KBD_MOD_CTRL) {
                    // If CTRL is pressed...
                    if (modifiers & KBD_MOD_SHIFT) {
                        // If SHIFT is also pressed, return CTRL-SHIFT form
                        return kbd_to_ansi(modifiers, g_kbdmo_control.keys_control_shift[scan_code]);

                    } else {
                        // Otherwise, return just CTRL form
                        return kbd_to_ansi(modifiers, g_kbdmo_control.keys_control[scan_code]);
                    }

                } else if (modifiers & KBD_LOCK_CAPS) {
                    // If CAPS is locked...
                    if (modifiers & KBD_MOD_SHIFT) {
                        // If SHIFT is also pressed, return CAPS-SHIFT form
                        return kbd_to_ansi(modifiers, g_kbdmo_control.keys_caps_shift[scan_code]);

                    } else {
                        // Otherwise, return just CAPS form
                        return kbd_to_ansi(modifiers, g_kbdmo_control.keys_caps[scan_code]);
                    }

                } else {
                    // SHIFT is pressed, return SHIFT form
                    return kbd_to_ansi(modifiers, g_kbdmo_control.keys_shift[scan_code]);
                }
            }

            // If we reach this point, it wasn't a useful scan-code...
            // So try to fetch another
            raw_code = kbdmo_get_scancode();
        }

        // If we reach this point, there are no useful scan codes
        return 0;
    }
}

/*
 * Use polling to fetch a key
 */
char kbdmo_getc_poll() {
    kbdmo_handle_irq();
    return kbdmo_getc();
}

/*
 * Use polling to fetch a scan code
 */
unsigned short kbdmo_get_scancode_poll() {
    kbdmo_handle_irq();
    return kbdmo_get_scancode();
}

/*
 * Set the color of the LED for the capslock
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
void kbdmo_set_caps_led(short colors) {
    kbdmo_leds = (kbdmo_leds & 0xF1FF) | ((colors & 0x07) << 9);
    *GABE_MO_LEDS = kbdmo_leds;
}

/*
 * Set the color of the LED for the floppy drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
void kbdmo_set_fdc_led(short colors) {
    kbdmo_leds = (kbdmo_leds & 0xFFF8) | (colors & 0x07);
    *GABE_MO_LEDS = kbdmo_leds;
}

/*
 * Set the color of the LED for the SD card slot
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
void kbdmo_set_sdc_led(short colors) {
    kbdmo_leds = (kbdmo_leds & 0xFFC7) | ((colors & 0x07) << 3);
    *GABE_MO_LEDS = kbdmo_leds;
}

/*
 * Set the color of the LED for the IDE hard drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
void kbdmo_set_hdc_led(short colors)  {
    kbdmo_leds = (kbdmo_leds & 0xFE3F) | ((colors & 0x07) << 6);
    *GABE_MO_LEDS = kbdmo_leds;
}

/*
 * Set the keyboard translation tables
 *
 * The translation tables provided to the keyboard consist of eight
 * consecutive tables of 128 characters each. Each table maps from
 * the MAKE scan code of a key to its appropriate 8-bit character code.
 *
 * The tables included must include, in order:
 * - UNMODIFIED: Used when no modifier keys are pressed or active
 * - SHIFT: Used when the SHIFT modifier is pressed
 * - CTRL: Used when the CTRL modifier is pressed
 * - CTRL-SHIFT: Used when both CTRL and SHIFT are pressed
 * - CAPSLOCK: Used when CAPSLOCK is down but SHIFT is not pressed
 * - CAPSLOCK-SHIFT: Used when CAPSLOCK is down and SHIFT is pressed
 * - ALT: Used when only ALT is presse
 * - ALT-SHIFT: Used when ALT is pressed and either CAPSLOCK is down
 *   or SHIFT is pressed (but not both)
 *
 * Inputs:
 * tables = pointer to the keyboard translation tables
 */
short kbdmo_layout(const char * tables) {
    return 0;
}

#endif
