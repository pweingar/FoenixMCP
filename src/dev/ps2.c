/**
 * Definitions for the PS/2 interface devices... mouse and keyboard
 */

#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "log.h"
#include "types.h"
#include "ring_buffer.h"
#include "interrupt.h"
#include "vicky_general.h"
#include "dev/ps2.h"
#include "dev/rtc.h"
#include "dev/text_screen_iii.h"
#include "rsrc/bitmaps/mouse_pointer.h"

#define PS2_TIMEOUT_JF          10          /* Timeout in jiffies: 1/60 second units */
#define PS2_RESEND_MAX          50          /* Number of times we'll repeat a command on receiving a 0xFE reply */
#define KBD_XLATE_TABLE_SIZE    128*8       /* Number of characters in the keyboard layout tables */

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
 * Special scan codes
 */

#define KBD_SC_PIVOT        0x38

/*
 * Structures
 */

/*
 * States the keyboard driver can be in
 */
typedef enum kbd_state {
    KBD_ST_IDLE = 0,
    KBD_ST_E0,
    KBD_ST_E02A,
    KBD_ST_E02AE0,
    KBD_ST_E1,
    KBD_ST_E11D,
    KBD_ST_E11D45,
    KBD_ST_E11D45E1,
    KBD_ST_E11D45E19D,
    KBD_ST_E0B7,
    KBD_ST_E0B7E0
} t_kbd_state;

/*
 * Structure to track the keyboard input
 */
struct s_ps2_kbd {
    t_kbd_state state;          /* State of the scan code processing state machine */
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
    char * keys_r_alt;
    char * keys_r_alt_shift;

    char * translation_table;
};

/*
 * Driver global variables
 */

struct s_ps2_kbd g_kbd_control;

short g_mouse_state = 0;                /* Mouse packet state machine's state */

/*
 * Mapping of "codepoints" 0x80 - 0x95 (function keys, etc)
 * to ANSI escape codes
 */
const char * ansi_keys[] = {
    "1~",      /* HOME */
    "2~",      /* INS */
    "3~",      /* DELETE */
    "4~",      /* END */
    "5~",      /* PgUp */
    "6~",      /* PgDn */
    "A",       /* Up */
    "B",       /* Left */
    "C",       /* Right */
    "D",       /* Down */
    "11~",     /* F1 */
    "12~",     /* F2 */
    "13~",     /* F3 */
    "14~",     /* F4 */
    "15~",     /* F5 */
    "17~",     /* F6 */
    "18~",     /* F7 */
    "19~",     /* F8 */
    "20~",     /* F9 */
    "21~",     /* F10 */
    "23~",     /* F11 */
    "24~",     /* F12 */
};

// Translation table from base set1 make scan codes to Foenix scan codes
const unsigned char g_kbd_set1_base[128] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, /* 0x00 - 0x07 */
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, /* 0x08 - 0x0F */
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, /* 0x10 - 0x17 */
    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, /* 0x18 - 0x1F */
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, /* 0x20 - 0x27 */
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, /* 0x28 - 0x2F */
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, /* 0x30 - 0x37 */
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, /* 0x38 - 0x3F */
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, /* 0x40 - 0x47 */
    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, /* 0x48 - 0x4F */
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, /* 0x50 - 0x57 */
    0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x58 - 0x5F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x60 - 0x67 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /* 0x78 - 0x7F */
};

// Translation table from E0 prefixed set1 make scan codes to Foenix scan codes
const unsigned char g_kbd_set1_e0[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x00 - 0x07 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x08 - 0x0F */
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x10 - 0x17 */
    0x00, 0x71, 0x00, 0x00, 0x6D, 0x5E, 0x00, 0x00, /* 0x18 - 0x1F */
    0x72, 0x00, 0x6E, 0x00, 0x6F, 0x00, 0x00, 0x00, /* 0x20 - 0x27 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x00, /* 0x28 - 0x2F */
    0x74, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x00, 0x00, /* 0x30 - 0x37 */
    0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x38 - 0x3F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x63, /* 0x40 - 0x47 */
    0x68, 0x64, 0x00, 0x69, 0x00, 0x6B, 0x00, 0x66, /* 0x48 - 0x4F */
    0x6A, 0x67, 0x62, 0x65, 0x00, 0x00, 0x00, 0x00, /* 0x50 - 0x57 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x5D, 0x00, 0x00, /* 0x58 - 0x5F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x60 - 0x67 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x68 - 0x6F */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70 - 0x77 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /* 0x78 - 0x7F */
};

/*
 * US keyboard layout scancode translation tables
 */

const char g_us_sc_unmodified[] = {
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

const char g_us_sc_shift[] = {
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

const char g_us_sc_ctrl[] = {
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

const char g_us_sc_lock[] = {
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

const char g_us_sc_lock_shift[] = {
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

const char g_us_sc_ctrl_shift[] = {
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

/* Right Alt */
const char g_us_sc_alt[] = {
    0x00, 0x1B, '1', '2', 0x9C, 0x9E, '5', '6',         /* 0x00 - 0x07 ... British Pound, and Euro */
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

/* Right Alt and Shift/CAPS */
const char g_us_sc_alt_shift[] = {
    0x00, 0x1B, '1', '2', 0x9C, 0x9E, '5', '6',         /* 0x00 - 0x07 */
    '7', '8', '9', '0', '-', '=', 0x08, 0x09,           /* 0x08 - 0x0F */
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',             /* 0x10 - 0x17 */
    'O', 'P', '[', ']', 0x0D, 0x00, 'A', 'S',           /* 0x18 - 0x1F */
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',             /* 0x20 - 0x27 */
    0x27, '`', 0x00, '\\', 'Z', 'X', 'C', 'V',          /* 0x28 - 0x2F */
    'B', 'N', 'M', ',', '.', '/', 0x00, '*',            /* 0x30 - 0x37 */
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
 * Code...
 */

/*
 * Wait for the output buffer of the PS/2 controller to have data.
 *
 * Returns:
 *  0 if successful, -1 if there was no response after PS2_RETRY_MAX tries
 */
short ps2_wait_out() {
    long target_ticks;

    log(LOG_TRACE, "ps2_wait_out");

    target_ticks = rtc_get_jiffies() + PS2_TIMEOUT_JF;
    while ((*PS2_STATUS & PS2_STAT_OBF) == 0) {
        if (rtc_get_jiffies() > target_ticks) {
            return -1;
        }
    }

    return 0;
}

/*
 * Wait for the input buffer of the PS/2 controller to be clear.
 *
 * Returns:
 *  0 if successful, -1 if there was no response after PS2_RETRY_MAX tries
 */
short ps2_wait_in() {
    long target_ticks;

    log(LOG_TRACE, "ps2_wait_in");

    target_ticks = rtc_get_jiffies() + PS2_TIMEOUT_JF;
    while ((*PS2_STATUS & PS2_STAT_IBF) != 0) {
        if (rtc_get_jiffies() > target_ticks) {
            return -1;
        }
    }

    return 0;
}

/*
 * Send a command to the controller and wait for a response.
 *
 * Returns:
 *  The response from the PS/2 controller, -1 if there was a timeout
 */
short ps2_controller_cmd(unsigned char cmd) {
    if (ps2_wait_in()) return -1;
    *PS2_CMD_BUF = cmd;

    if (ps2_wait_out()) return -1;
    return (short)*PS2_DATA_BUF;
}

/*
 * Send a command with parameter to the controller and wait for a response.
 *
 * Returns:
 *  The response from the PS/2 controller, -1 if there was a timeout
 */
short ps2_controller_cmd_param(unsigned char cmd, unsigned char parameter) {
    if (ps2_wait_in()) return -1;
    *PS2_CMD_BUF = cmd;

    if (ps2_wait_in()) return -1;
    *PS2_DATA_BUF = parameter;

    return 0;
}

/*
 * Send a command with parameter to the keyboard and wait for a response.
 *
 * Returns:
 *  The response from the PS/2 controller, -1 if there was a timeout
 */
short ps2_kbd_cmd_p(unsigned char cmd, unsigned char parameter) {
    if (ps2_wait_in()) return -1;
    *PS2_DATA_BUF = cmd;

    // May need a delay here

    if (ps2_wait_in()) return -1;
    *PS2_DATA_BUF = parameter;

    // Return 0 by default... maybe read DATA?
    return 0;
}

/*
 * Send a command to the keyboard and wait for a response.
 *
 * Inputs:
 *  cmd = the command byte to send to the keyboard
 *  delay = an indication of how much to wait before checking for output
 *
 * Returns:
 *  The response from the PS/2 controller, -1 if there was a timeout
 */
short ps2_kbd_cmd(unsigned char cmd, short delay) {
    if (ps2_wait_in()) return -1;
    *PS2_DATA_BUF = cmd;

    // A delay may be needed here
    while (delay-- > 0) {
        ;
    }

    if (ps2_wait_out()) return -1;
    return (short)*PS2_DATA_BUF;
}

/*
 * Read from the PS/2 data port until there are no more bytes ready.
 */
void ps2_flush_out() {
    while (*PS2_STATUS & PS2_STAT_OBF) {
        unsigned char x = *PS2_DATA_BUF;
    }
}

/*
 * Toggle the lock bit based on the flag.
 */
void kbd_toggle_modifier(short flag) {
    g_kbd_control.modifiers ^= flag;
}

/*
 * Set or clear the modifier flag depending on if the scan code is a make or break code.
 */
void kbd_makebreak_modifier(short flag, short is_break) {
    if (is_break) {
        g_kbd_control.modifiers &= ~flag;
    } else {
        g_kbd_control.modifiers |= flag;
    }
}

/*
 * Add the scan code to the queue of scan codes
 */
void kbd_enqueue_scan(unsigned char scan_code) {
    // Make sure the scan code isn't 0 or 128, which are invalid make/break codes
    if ((scan_code != 0) && (scan_code != 0x80)) {
        unsigned char is_break = scan_code & 0x80;

        // Check the scan code to see if it's a modifier key or a lock key
        // update the modifier and lock variables accordingly...
        switch (scan_code & 0x7f) {
            case 0x2A:
            case 0x36:
                kbd_makebreak_modifier(KBD_MOD_SHIFT, is_break);
                break;

            case 0x1D:
            case 0x5E:
                kbd_makebreak_modifier(KBD_MOD_CTRL, is_break);
                break;

            case 0x38:
            case 0x5C:
                kbd_makebreak_modifier(KBD_MOD_ALT, is_break);
                break;

            case 0x5D:
                kbd_makebreak_modifier(KBD_MOD_MENU, is_break);
                break;

            case 0x5B:
                kbd_makebreak_modifier(KBD_MOD_OS, is_break);
                break;

            case 0x3A:
                if (!is_break) kbd_toggle_modifier(KBD_LOCK_CAPS);
                break;

            case 0x45:
                if (!is_break) kbd_toggle_modifier(KBD_LOCK_NUM);
                break;

            case 0x46:
                if (!is_break) kbd_toggle_modifier(KBD_LOCK_SCROLL);
                break;

            default:
                break;
        }

        rb_word_put(&g_kbd_control.sc_buf, g_kbd_control.modifiers << 8 | scan_code);
    }
}

/*
 * Try to retrieve the next scancode from the keyboard.
 *
 * Returns:
 *      The next scancode to be processed, 0 if nothing.
 */
unsigned short kbd_get_scancode() {
    return rb_word_get(&g_kbd_control.sc_buf);
}

/*
 * IRQ handler for the keyboard... read a scan code and queue it
 */
void kbd_handle_irq() {
    volatile unsigned char *data = (unsigned char *)PS2_DATA_BUF;
    unsigned char scan_code = *data;
    unsigned char translated_code;

    /* Clear the pending flag */
    int_clear(INT_KBD_PS2);

    if (scan_code) {
        // Make sure the scan code isn't 0 or 128, which are invalid make/break codes
        if ((scan_code != 0) && (scan_code != 0x80)) {
            unsigned char is_break = scan_code & 0x80;

            // Process the byte according to the state machine...
            switch (g_kbd_control.state) {
            case KBD_ST_IDLE:
                switch (scan_code) {
                    case 0xE0:
                        g_kbd_control.state = KBD_ST_E0;
                        break;

                    case 0xE1:
                        g_kbd_control.state = KBD_ST_E1;
                        break;

                    default:
                        // Lookup the translation of the code...
                        translated_code = g_kbd_set1_base[scan_code & 0x7f];
                        if (translated_code != 0) {
                            // If we got a translation, enqueue it
                            kbd_enqueue_scan(translated_code | is_break);
                        }
                        break;
                }
                break;

            case KBD_ST_E0:
                switch (scan_code) {
                    case 0x2A:
                        g_kbd_control.state = KBD_ST_E02A;
                        break;

                    case 0xB7:
                        g_kbd_control.state = KBD_ST_E0B7;
                        break;

                    default:
                        // Lookup the translation of the E0 prefixed code...
                        translated_code = g_kbd_set1_e0[scan_code & 0x7f];
                        if (translated_code != 0) {
                            // If we got a translation, enqueue it
                            kbd_enqueue_scan(translated_code | is_break);
                        }
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E02A:
                switch (scan_code) {
                    case 0xE0:
                        g_kbd_control.state = KBD_ST_E02AE0;
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E02AE0:
                switch (scan_code) {
                    case 0x37:
                        g_kbd_control.state = KBD_ST_IDLE;
                        kbd_enqueue_scan(0x60);           // Return our PrintScreen key
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E0B7:
                switch (scan_code) {
                    case 0xE0:
                        g_kbd_control.state = KBD_ST_E0B7E0;
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E0B7E0:
                switch (scan_code) {
                    case 0xAA:
                        g_kbd_control.state = KBD_ST_IDLE;
                        kbd_enqueue_scan(0xD0);           // Return our PrintScreen key break
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E1:
                switch (scan_code) {
                    case 0x1D:
                        g_kbd_control.state = KBD_ST_E11D;
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E11D:
                switch (scan_code) {
                    case 0x45:
                        g_kbd_control.state = KBD_ST_E11D45;
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E11D45:
                switch (scan_code) {
                    case 0xE1:
                        g_kbd_control.state = KBD_ST_E11D45E1;
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E11D45E1:
                switch (scan_code) {
                    case 0x9D:
                        g_kbd_control.state = KBD_ST_E11D45E19D;
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            case KBD_ST_E11D45E19D:
                switch (scan_code) {
                    case 0xC5:
                        g_kbd_control.state = KBD_ST_IDLE;
                        kbd_enqueue_scan(0x61);           // return our Break key break
                        break;

                    default:
                        // Bad scan-code... reset the machine and just eat the code
                        g_kbd_control.state = KBD_ST_IDLE;
                        break;
                }
                break;

            default:
                // TODO: kernel panic?
                ;
        }
        }
    }
}

/*
 * Catch special keys and convert them to their ANSI terminal codes
 *
 * Characters 0x80 - 0x95 are reserved for function keys, arrow keys, etc.
 * This function maps them to the ANSI escape codes
 *
 * Inputs:
 * modifiers = the current modifier bit flags (ALT, CTRL, META, etc)
 * c = the character found from the scan code.
 */
char kbd_to_ansi(unsigned char modifiers, unsigned char c) {
    if ((c >= 0x80) && (c <= 0x95)) {
        /* The key is a function key or a special control key */
        const char * sequence;

        /* After ESC, all sequences have [ */
        rb_word_put(&g_kbd_control.char_buf, '[');

        /* Check to see if we need to send a modifier sequence */
        if (modifiers & (KBD_MOD_SHIFT | KBD_MOD_CTRL | KBD_MOD_ALT | KBD_MOD_OS)) {
            unsigned char code_bcd;
            short modifier_code = 0;
            short i;

            modifier_code = (modifiers >> 2) & 0x0E;
            code_bcd = i_to_bcd(modifier_code);

            if (code_bcd & 0xF0) {
                rb_word_put(&g_kbd_control.char_buf, ((code_bcd & 0xF0) >> 4) + '0');
            }
            rb_word_put(&g_kbd_control.char_buf, (code_bcd & 0x0F) + '0');
            rb_word_put(&g_kbd_control.char_buf, ';');
        }

        /* Get the expanded sequence and put it in the queue */
        for (sequence = ansi_keys[c - 0x80]; *sequence != 0; sequence++) {
            rb_word_put(&g_kbd_control.char_buf, *sequence);
        }

        return 0x1B;    /* Start the sequence with an escape */

    } else if (c == 0x1B) {
        /* ESC should be doubled, to distinguish from the start of an escape sequence */
        rb_word_put(&g_kbd_control.char_buf, 0x1B);
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
char kbd_getc() {
    if (!rb_word_empty(&g_kbd_control.char_buf)) {
        // If there is a character waiting in the character buffer, return it...
        return (char)rb_word_get(&g_kbd_control.char_buf);

    } else {
        // Otherwise, we need to check the scan code queue...
        unsigned short raw_code = kbd_get_scancode();
        while (raw_code != 0) {
            if ((raw_code & 0x80) == 0) {
                // If it's a make code, let's try to look it up...
                unsigned char modifiers = (raw_code >> 8) & 0xff;    // Get the modifiers
                unsigned char scan_code = raw_code & 0x7f;           // Get the base code for the key

                if (scan_code < KBD_SC_PIVOT) {
                    // It's on the left side of the keyboard, use modifiers to determine lookup table
                    // including SHIFT, CONTROL, CAPS

                    // Check the modifiers to see what we should lookup...

                    if ((modifiers & (KBD_MOD_SHIFT | KBD_MOD_CTRL | KBD_LOCK_CAPS | KBD_MOD_ALT)) == 0) {
                        // No modifiers... just return the base character
                        return kbd_to_ansi(modifiers, g_kbd_control.keys_unmodified[scan_code]);

                    } else if (modifiers & KBD_MOD_ALT) {
                        if ((( (modifiers & KBD_MOD_SHIFT) == 0) && ((modifiers & KBD_LOCK_CAPS) != 0)) ||
                            (( (modifiers & KBD_MOD_SHIFT) != 0) && ((modifiers & KBD_LOCK_CAPS) == 0))) {
                                /* Either SHIFT or CAPSLOCK is active, but not both */
                                return g_kbd_control.keys_r_alt_shift[scan_code];

                            } else {
                                /* No shift, or both SHIFT and CAPS are active */
                                return g_kbd_control.keys_r_alt[scan_code];
                            }

                    } else if (modifiers & KBD_MOD_CTRL) {
                        // If CTRL is pressed...
                        if (modifiers & KBD_MOD_SHIFT) {
                            // If SHIFT is also pressed, return CTRL-SHIFT form
                            return kbd_to_ansi(modifiers, g_kbd_control.keys_control_shift[scan_code]);

                        } else {
                            // Otherwise, return just CTRL form
                            return kbd_to_ansi(modifiers, g_kbd_control.keys_control[scan_code]);
                        }

                    } else if (modifiers & KBD_LOCK_CAPS) {
                        // If CAPS is locked...
                        if (modifiers & KBD_MOD_SHIFT) {
                            // If SHIFT is also pressed, return CAPS-SHIFT form
                            return kbd_to_ansi(modifiers, g_kbd_control.keys_caps_shift[scan_code]);

                        } else {
                            // Otherwise, return just CAPS form
                            return kbd_to_ansi(modifiers, g_kbd_control.keys_caps[scan_code]);
                        }

                    } else {
                        // SHIFT is pressed, return SHIFT form
                        return kbd_to_ansi(modifiers, g_kbd_control.keys_shift[scan_code]);
                    }

                } else {
                    // It's on the right side of the keyboard, NUMLOCK determines lock value

                    // TODO: flesh this out...
                    return kbd_to_ansi(modifiers, g_kbd_control.keys_unmodified[scan_code]);
                }
            }

            // If we reach this point, it wasn't a useful scan-code...
            // So try to fetch another
            raw_code = kbd_get_scancode();
        }

        // If we reach this point, there are no useful scan codes
        return 0;
    }
}

/*
 * Handle an interrupt from the PS/2 mouse port
 */
void mouse_handle_irq() {
    //unsigned char status = *PS2_STATUS;
    unsigned char mouse_byte = *PS2_DATA_BUF;

    /* Clear the pending interrupt flag for the mouse */
    int_clear(INT_MOUSE);

    if ((g_mouse_state == 0) && ((mouse_byte & 0x08) != 0x08)) {
        /*
         * If this is the first byte in the packet, bit 4 must be set
         * If it is not, ignore the byte... we're out of synch
         */
        return;

    } else {
        /* Send the byte to Vicky */
        MousePtr_A_Mouse0[g_mouse_state++] = (unsigned short)mouse_byte;

        /* After three bytes, return to state 0 */
        if (g_mouse_state > 2) {
            g_mouse_state = 0;
        }
    }
}

/*
 * Send a command to the mouse
 *
 * Inputs:
 * cmd = the mouse command byte to send
 *
 * Returns:
 * -1 on timeout, otherwise the result of the command
 */
short ps2_mouse_command(unsigned char cmd) {

    short result;

    if (ps2_wait_in()) return -1;
    *PS2_CMD_BUF = MOUSE_CMD_PREFIX;

    // log_num(LOG_VERBOSE, "ps_mouse_command command: ", cmd);

    if (ps2_wait_in()) return -1;
    *PS2_DATA_BUF = cmd;

    if (ps2_wait_out()) return -1;
    result = *PS2_DATA_BUF;

    // log_num(LOG_VERBOSE, "ps_mouse_command result: ", result);

    return (short)result;
}

short ps2_mouse_command_repeatable(unsigned char cmd) {
    int sends = 0;
    short result = 0;

    while ((result != PS2_RESP_ACK) && (sends < PS2_RESEND_MAX)) {
        result = ps2_mouse_command(cmd);
        sends++;
    }

    return result;
}

/*
 * Query the mouse for an update packet (use if we aren't using interrupts)
 *
 * Returns:
 * 0 on success, any other number is an error
 */
short ps2_mouse_get_packet() {
    short result;
    short i;

    TRACE("ps2_mouse_get_packet");

    result = ps2_mouse_command(MOUSE_CMD_REQPACK);
    if (result == -1) {
        log_num(LOG_INFO, "MOUSE_CMD_REQPACK: ", result);
        return result;
    }

    for (i = 0; i < 3; i++) {
        if (ps2_wait_out()) return -1;
        unsigned char data = *PS2_DATA_BUF;

        /* Send the byte to Vicky */
        MousePtr_A_Mouse0[i] = (unsigned short)data;
    }

    return 0;
}

/*
 * Set the visibility of the VICKY mouse pointer
 *
 * Input:
 * is_visible = 0 for hide, any other value to show
 */
void mouse_set_visible(short is_visible) {
    short i;

    if (is_visible != 0) {
        *MousePtr_A_CTRL_Reg = MousePtr_En;
    } else {
        *MousePtr_A_CTRL_Reg = 0;

        for (i = 0; i < 256; i++) {
            short dest_offset = 2*i;

            MousePointer_Mem_A[dest_offset] = 0;
            MousePointer_Mem_A[dest_offset+1] = 0;
        }
    }
}

/*
 * Attempt to initialize the PS/2 mouse
 *
 * Returns:
 * 0 on success, any other number is an error
 */
short mouse_init() {
    short i, retries;
    unsigned short low_components;
    unsigned short hi_components;
    short result;

    TRACE("mouse_init");

    /* Set the state machine to the initial state */

    g_mouse_state = 0;

    /* Send a mouse reset command, and wait for the mouse to reply with 0xAA */

    result = ps2_mouse_command(MOUSE_CMD_RESET);
    if (result == -1) {
        log_num(LOG_INFO, "MOUSE_CMD_RESET: ", result);
        return result;
    }

    /* Disable streaming for the moment */

    result = ps2_mouse_command_repeatable(MOUSE_CMD_DISABLE);
    if (result != PS2_RESP_ACK) {
        log_num(LOG_INFO, "MOUSE_CMD_DISABLE: ", result);
        return result;
    }

    /* Set the mouse to default settings */

    result = ps2_mouse_command_repeatable(MOUSE_CMD_DEFAULTS);
    if (result != PS2_RESP_ACK) {
        log_num(LOG_INFO, "MOUSE_CMD_DEFAULTS: ", result);
        return result;
    }

    /* Set resolution to be lowest for 640x480 */

    result = ps2_mouse_command_repeatable(MOUSE_CMD_SETRES);
    if (result != PS2_RESP_ACK) {
        log_num(LOG_INFO, "MOUSE_CMD_SETRES: ", result);
        return result;
    }

    result = ps2_mouse_command_repeatable(0x00);
    if (result != PS2_RESP_ACK) {
        log_num(LOG_INFO, "MOUSE_CMD_SETRES resolution: ", result);
        return result;
    }

    /* Enable packet streaming */

    result = ps2_mouse_command_repeatable(MOUSE_CMD_ENABLE);
    if (result != PS2_RESP_ACK) {
        log_num(LOG_INFO, "MOUSE_CMD_ENABLE: ", result);
        return result;
    }

    /* Set up the mouse pointer */

    short src_offset = 0;
    short dest_offset = 0;
    for (i = 0; i < 256; i++) {
        low_components = (Color_Pointer_bin[src_offset+1] << 8) + Color_Pointer_bin[src_offset];
        hi_components = Color_Pointer_bin[src_offset+2];
        MousePointer_Mem_A[dest_offset] = low_components;
        MousePointer_Mem_A[dest_offset+1] = hi_components;

        src_offset += 3;
        dest_offset += 2;
    }

    /* Enable the mouse pointer on channel A */
    mouse_set_visible(1);

    return 0;
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
 * tables = pointer to the keyboard translation tables (0 to reset to default)
 *
 * Returns:
 * 0 on success, a negative number if there was an error
 */
short kbd_layout(const char * tables) {
    short i;

    /* Are we resetting the tables? */
    if (tables == 0) {
        /* Yes... free the old table if needed */
        if (g_kbd_control.translation_table != 0) {
            free(g_kbd_control.translation_table);
        }

        /* Reset the translation tables to their default values */
        g_kbd_control.keys_unmodified = g_us_sc_unmodified;
        g_kbd_control.keys_shift = g_us_sc_shift;
        g_kbd_control.keys_control = g_us_sc_ctrl;
        g_kbd_control.keys_control_shift = g_us_sc_ctrl_shift;
        g_kbd_control.keys_caps = g_us_sc_lock;
        g_kbd_control.keys_caps_shift = g_us_sc_lock_shift;
        g_kbd_control.keys_r_alt = g_us_sc_alt;
        g_kbd_control.keys_r_alt_shift = g_us_sc_alt_shift;

    } else {
        /* No: we're setting new tables */

        /* Allocate a space for all the tables in the kernel's memory, if we don't have one already */
        if (g_kbd_control.translation_table == 0) {
            g_kbd_control.translation_table = (char *)malloc(KBD_XLATE_TABLE_SIZE);
            if (g_kbd_control.translation_table == 0) {
                /* We couldn't allocate... return out of memory */
                return ERR_OUT_OF_MEMORY;
            }
        }

        /* Copy the tables into kernel memory */
        for (i = 0; i < KBD_XLATE_TABLE_SIZE; i++) {
            g_kbd_control.translation_table[i] = tables[i];
        }

        /* Set the lookup tables to the individual tables in the collection */
        g_kbd_control.keys_unmodified = g_kbd_control.translation_table;
        g_kbd_control.keys_shift = g_kbd_control.keys_unmodified + 128;
        g_kbd_control.keys_control = g_kbd_control.keys_shift + 128;
        g_kbd_control.keys_control_shift = g_kbd_control.keys_control + 128;
        g_kbd_control.keys_caps = g_kbd_control.keys_control_shift + 128;
        g_kbd_control.keys_caps_shift = g_kbd_control.keys_caps + 128;
        g_kbd_control.keys_r_alt = g_kbd_control.keys_caps_shift + 128;
        g_kbd_control.keys_r_alt_shift = g_kbd_control.keys_r_alt + 128;
    }

    return 0;
}

/*
 * Initialize the PS2 controller and any attached devices
 * Enable keyboard and mouse interrupts as appropriate.
 *
 * Returns:
 *  Status code indicating if either the mouse or the keyboard is missing.
 */
short ps2_init() {
    unsigned char x;
    short mouse_present;
    short mouse_error;
    short res;

    // Initialize the keyboard controller variables

    g_kbd_control.state = KBD_ST_IDLE;          // Initial state for the scan code state machine
    rb_word_init(&g_kbd_control.sc_buf);        // Scan-code ring buffer is empty
    rb_word_init(&g_kbd_control.char_buf);      // Character ring buffer is empty

    // Set the default keyboard layout to US

    g_kbd_control.keys_unmodified = g_us_sc_unmodified;
    g_kbd_control.keys_shift = g_us_sc_shift;
    g_kbd_control.keys_control = g_us_sc_ctrl;
    g_kbd_control.keys_control_shift = g_us_sc_ctrl_shift;
    g_kbd_control.keys_caps = g_us_sc_lock;
    g_kbd_control.keys_caps_shift = g_us_sc_lock_shift;
    g_kbd_control.keys_r_alt = g_us_sc_alt;
    g_kbd_control.keys_r_alt_shift = g_us_sc_alt_shift;
    g_kbd_control.translation_table = 0;

    // Disable the PS/2 interrupts...

    int_disable(INT_MOUSE);     /* Disable mouse interrupts */
    int_disable(INT_KBD_PS2);   /* Disable keyboar interrupts */

    // Prevent the keyboard and mouse from sending events
    ps2_controller_cmd(PS2_CTRL_DISABLE_1);
    ps2_controller_cmd(PS2_CTRL_DISABLE_2);

    // Read and clear out the controller's output buffer
    ps2_flush_out();

    // // Controller selftest...
    if (ps2_controller_cmd(PS2_CTRL_SELFTEST) != PS2_RESP_OK) {
        ; // return PS2_FAIL_SELFTEST;
    }

    // Keyboard test
    if (ps2_controller_cmd(PS2_CTRL_KBDTEST) != 0) {
        ; // return PS2_FAIL_KBDTEST;
    }

    /* Test if the mouse is working */
    if (ps2_controller_cmd(PS2_CTRL_MOUSETEST) == 0) {
        mouse_present = 1;
    } else {
        mouse_present = 0;
    }

    // Set scancode translation to set1, enable interrupts on mouse and keyboard
    ps2_controller_cmd_param(PS2_CTRL_WRITECMD, 0x43);  /* %01000011 */

    // Enable the keyboard, don't check response
    ps2_wait_in();
    *PS2_CMD_BUF = PS2_CTRL_ENABLE_1;

    // Reset the keyboard... waiting a bit before we check for a result
    ps2_kbd_cmd(KBD_CMD_RESET, 1000);

    // Ideally, we would attempt a re-enable several times, but this doesn't work on the U/U+ for some keyboards
    ps2_kbd_cmd(KBD_CMD_ENABLE, 0);

    // TODO: set the keyboard LEDs

    if (mouse_present) {
        /* Initialize the mouse */
        if (mouse_error = mouse_init()) {
            log_num(LOG_INFO, "Unable to initialize mouse", res);
        }
    }

    ps2_wait_in();
    *PS2_CMD_BUF = PS2_CTRL_ENABLE_2;

    // Make sure everything is read
    ps2_flush_out();

    // Register the interrupt handler for the keyboard
    int_register(INT_KBD_PS2, kbd_handle_irq);

    // Clear any pending keyboard interrupts
    int_clear(INT_KBD_PS2);

    // Enable the keyboard interrupt
    int_enable(INT_KBD_PS2);

    if (mouse_present && (mouse_error == 0)) {
        log(LOG_TRACE, "mouse enabled");

        // Register the interrupt handler for the mouse
        int_register(INT_MOUSE, mouse_handle_irq);

        // Clear any pending mouse interrupts
        int_clear(INT_MOUSE);

        // Enable the mouse interrupt
        int_enable(INT_MOUSE);
    }

    return(0);
}
