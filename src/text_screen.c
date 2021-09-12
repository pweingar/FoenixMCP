/*
 * Driver for VICKY III text screens, both channel A and channel B
 */

#include "vicky_general.h"
#include "text_screen.h"

#define MAX_TEXT_CHANNELS 2

/*
 * Structure to hold pointers to the text channel's registers and memory
 */
typedef struct s_text_channel {
    volatile char * text_cells;
    volatile uint8_t * color_cells;
    volatile uint32_t * cursor_settings;
    volatile uint32_t * cursor_position;

    short columns;
    short rows;
    short x;
    short y;
    volatile char * text_cursor_ptr;
    volatile uint8_t * color_cursor_ptr;
    uint8_t current_color;
} t_text_channel, *p_text_channel;

static t_text_channel text_channel[MAX_TEXT_CHANNELS];

/*
 * Initialize the text screen driver
 */
int text_init() {
    int x;
    p_text_channel chan_a = &text_channel[0];
    p_text_channel chan_b = &text_channel[1];

    /* TODO: initialize everything... only do a screen if it's present */

    chan_a->text_cells = ScreenText_A;
    chan_a->color_cells = ColorText_A;
    chan_a->cursor_settings = CursorControlReg_L_A;
    chan_a->cursor_position = CursorControlReg_H_A;
    text_setsizes(0);
    text_set_color(0, 1, 0);
    text_clear(0);
    text_set_xy(0, 0, 0);

    chan_b->text_cells = ScreenText_B;
    chan_b->color_cells = ColorText_B;
    chan_b->cursor_settings = CursorControlReg_L_B;
    chan_b->cursor_position = CursorControlReg_H_B;
    text_setsizes(1);
    text_set_color(1, 4, 0);
    text_clear(1);
    text_set_xy(1, 0, 0);

    return 0;
}

/*
 * Set the cursor properties
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * color = the color index for the cursor
 * character = the character to display for the cursor
 * rate = the rate of blinking (0 = 1 per sec, 1 = 2 per sec, 2 = 4 per sec, 3 = 5 per sec)
 * enable = 1 to display the cursor, 0 to disable
 */
void text_set_cursor(short screen, short color, char character, short rate, short enable) {
    if (screen < MAX_TEXT_CHANNELS) {
        p_text_channel chan = &text_channel[screen];
        *(chan->cursor_settings) = ((color & 0xff) << 24) | (character << 16) | ((rate & 0x02) << 1) | (enable & (0x01));
    }
}

/*
 * Set the position of the cursor on the screen. Adjusts internal pointers used for printing the characters
 *
 * If the x and y coordinates are out of bounds of the display, the routine will attempt to handle wrapping and
 * scrolling accordingly.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * x = the column of the cursor (0 is left most)
 * y = the row of the cursor (0 is right most)
 */
void text_set_xy(short screen, unsigned short x, unsigned short y) {
    if (screen < MAX_TEXT_CHANNELS) {
        /* TODO: add in wrapping and scrolling */
        p_text_channel chan = &text_channel[screen];
        chan->x = x;
        chan->y = y;
        *(chan->cursor_position) = y << 16 | x;
        chan->text_cursor_ptr = &chan->text_cells[y * chan->columns + x];
        chan->color_cursor_ptr = &chan->color_cells[y * chan->columns + x];
    }
}

/*
 * Compute the size information for the text screen based on the current settings in VICKY
 * These settings are needed to correctly position text on the screen.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
void text_setsizes(short screen) {
    if (screen < MAX_TEXT_CHANNELS) {
        /* TODO: compute sizes based on master control register settings */
        p_text_channel chan = &text_channel[screen];
        chan->rows = (short)((480 - 32) / 8);
        chan->columns = (short)((640 - 32) / 8);
    }
}

/*
 * Set the foreground and background color for printing
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * foreground = the foreground color number to use (0 - 15)
 * background = the background color number to use (0 - 15)
 */
void text_set_color(short screen, short foreground, short background) {
    if (screen < MAX_TEXT_CHANNELS) {
        p_text_channel chan = &text_channel[screen];
        chan->current_color = ((foreground & 0x0f) << 4) | (background & 0x0f);
    }
}

/*
 * Clear the screen of data
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
void text_clear(short screen) {
    if (screen < MAX_TEXT_CHANNELS) {
        int i;
        p_text_channel chan = &text_channel[screen];
        for (i = 0; i < chan->columns * chan->rows; i++) {
            chan->text_cells[i] = ' ';
            chan->color_cells[i] = chan->current_color;
        }
    }
}

/*
 * Send a character to the screen without any escape code interpretation
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * c = the character to print
 */
void text_put_raw(short screen, char c) {
    if (screen < MAX_TEXT_CHANNELS) {
        p_text_channel chan = &text_channel[screen];
        *chan->text_cursor_ptr++ = c;
        *chan->color_cursor_ptr++ = chan->current_color;
        text_set_xy(screen, chan->x + 1, chan->y);
    }
}

/*
 * Send a character to the screen... but handle ANSI escape codes and process accordingly.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * c = the character to print
 */
void text_put_ansi(short screen, char c) {
    if (screen < MAX_TEXT_CHANNELS) {
        /* TODO: magic! */
    }
}