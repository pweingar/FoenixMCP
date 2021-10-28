/*
 * Declarations for Mo, the built-in keyboard of the A2560K
 */

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K

#include "log.h"
#include "interrupt.h"
#include "kbd_mo.h"
#include "ring_buffer.h"
#include "gabe_reg.h"

#define KBD_MO_DATA     ((volatile unsigned short *)0x00C00040)     /* Data register for the keyboard (scan codes will be here) */
#define KBD_MO_STAT     ((volatile unsigned short *)0x00C00042)     /* Status register for the keyboard */
#define KBD_MO_EMPTY    0x8000                                      /* Status flag that will be set if the keyboard buffer is empty */
#define KBD_MO_FULL     0x4000                                      /* Status flag that will be set if the keyboard buffer is full */

/*
 * Modifier bit flags
 */

#define KBD_LOCK_SCROLL     0x01
#define KBD_LOCK_NUM        0x02
#define KBD_LOCK_CAPS       0x04
#define KBD_MOD_SHIFT       0x08
#define KBD_MOD_CTRL        0x10
#define KBD_MOD_ALT         0x20
#define KBD_MOD_OS          0x40
#define KBD_MOD_MENU        0x80

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
    0x87, 0x85, 0x81, 0x82, 0x00, 0x00, 0x00, 0x94,     /* 0x50 - 0x57 */
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
    0x87, 0x85, 0x81, 0x82, 0x00, 0x00, 0x00, 0x94,     /* 0x50 - 0x57 */
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
    0x87, 0x85, 0x81, 0x82, 0x00, 0x00, 0x00, 0x94,     /* 0x50 - 0x57 */
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
    '2', '3', '0', '.', 0x00, 0x00, 0x00, 0x94,         /* 0x50 - 0x57 */
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
    '2', '3', '0', '.', 0x00, 0x00, 0x00, 0x94,         /* 0x50 - 0x57 */
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
    0x87, 0x85, 0x81, 0x82, 0x00, 0x00, 0x00, 0x94,     /* 0x50 - 0x57 */
    0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x58 - 0x5F */
    0x00, 0x00, 0x81, 0x80, 0x84, 0x82, 0x83, 0x85,     /* 0x60 - 0x67 */
    0x86, 0x89, 0x87, 0x88, '/', 0x0D, 0x00, 0x00,      /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* 0x78 - 0x7F */
};

/*
 * Make sure everything is removed from Mo's input buffer
 */
void kbdmo_flush_out() {
    TRACE("kbdmo_flush_out");

    /* While there is data in the buffer ... */
    while ((*KBD_MO_STAT & 0x00ff) != 0) {
        /* Read and throw out the scan codes */
        unsigned short dummy = *KBD_MO_DATA;
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
    /* We got an interrupt for MO.
     * While there is data in the input queue...
     */

    int_clear(INT_KBD_A2560K);

    while ((*KBD_MO_STAT & 0x00ff) != 0) {
        /* Get a scan code from the input buffer */
        unsigned short scan_code = *KBD_MO_DATA;

        if ((scan_code & 0x7fff) != 0) {
            /* TODO: beep if the input was full or the ring buffer is full */

            /* Process it and enqueue it */
            kbdmo_enqueue_scan((unsigned char)(scan_code & 0x00ff));
        }
    }
}


/*
 * Try to retrieve the next scancode from the keyboard.
 *
 * Returns:
 *      The next scancode to be processed, 0 if nothing.
 */
unsigned short kbdmo_get_scancode() {
    unsigned short scan_code = rb_word_get(&g_kbdmo_control.sc_buf);
    if (scan_code != 0) {
        /* Got a result... return it */
        return scan_code;

    } else {
        /* Nothing in the queue... let's make sure we haven't lost an interrupt */
        if ((*KBD_MO_STAT & 0x00ff) != 0) {
            /* Something is pending, process it as if an interrupt occurred */
            kbdmo_handle_irq();
            return rb_word_get(&g_kbdmo_control.sc_buf);
        } else {
            return 0;
        }
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
                    return g_kbdmo_control.keys_unmodified[scan_code];

                } else if (modifiers & KBD_MOD_CTRL) {
                    // If CTRL is pressed...
                    if (modifiers & KBD_MOD_SHIFT) {
                        // If SHIFT is also pressed, return CTRL-SHIFT form
                        return g_kbdmo_control.keys_control_shift[scan_code];

                    } else {
                        // Otherwise, return just CTRL form
                        return g_kbdmo_control.keys_control[scan_code];
                    }

                } else if (modifiers & KBD_LOCK_CAPS) {
                    // If CAPS is locked...
                    if (modifiers & KBD_MOD_SHIFT) {
                        // If SHIFT is also pressed, return CAPS-SHIFT form
                        return g_kbdmo_control.keys_caps_shift[scan_code];

                    } else {
                        // Otherwise, return just CAPS form
                        return g_kbdmo_control.keys_caps[scan_code];
                    }

                } else {
                    // SHIFT is pressed, return SHIFT form
                    return g_kbdmo_control.keys_shift[scan_code];
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

#endif
