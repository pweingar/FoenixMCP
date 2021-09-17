/*
 * Driver for VICKY III text screens, both channel A and channel B
 */

#include "constants.h"
#include "vicky_general.h"
#include "text_screen_iii.h"

#define MAX_TEXT_CHANNELS 2

/*
 * Structure to hold pointers to the text channel's registers and memory
 */
typedef struct s_text_channel {
    volatile uint32_t * master_control;
    volatile char * text_cells;
    volatile uint8_t * color_cells;
    volatile uint32_t * cursor_settings;
    volatile uint32_t * cursor_position;
    volatile uint32_t * border_control;

    short columns_max;
    short rows_max;
    short columns_visible;
    short rows_visible;

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

    chan_a->master_control = MasterControlReg_A;
    chan_a->text_cells = ScreenText_A;
    chan_a->color_cells = ColorText_A;
    chan_a->cursor_settings = CursorControlReg_L_A;
    chan_a->cursor_position = CursorControlReg_H_A;
    chan_a->border_control = BorderControlReg_L_A;

    *chan_a->master_control = VKY3_MCR_TEXT_EN;     /* Set to text only mode: 640x480 */
    // *chan_a->border_control = 0;                    /* Set to no border */

    text_setsizes(0);
    text_set_color(0, 15, 0);
    text_clear(0);
    text_set_xy(0, 0, 0);

    chan_b->master_control = MasterControlReg_B;
    chan_b->text_cells = ScreenText_B;
    chan_b->color_cells = ColorText_B;
    chan_b->cursor_settings = CursorControlReg_L_B;
    chan_b->cursor_position = CursorControlReg_H_B;
    chan_b->border_control = BorderControlReg_L_B;

    *chan_b->master_control = VKY3_MCR_TEXT_EN;     /* Set to text only mode: 640x480 */
    // *chan_b->border_control = 0;                    /* Set to no border */

    text_setsizes(1);
    text_set_color(1, 15, 0);
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

        if (x >= chan->columns_visible) {
            x = 0;
            y++;
        }

        if (y >= chan->rows_visible) {
            y = chan->rows_visible - 1;
            text_scroll(screen);
        }

        chan->x = x;
        chan->y = y;
        *(chan->cursor_position) = y << 16 | x;
        short offset = y * chan->columns_max + x;
        chan->text_cursor_ptr = &chan->text_cells[offset];
        chan->color_cursor_ptr = &chan->color_cells[offset];
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
        uint32_t border = 0;
        short pixel_double = 0;
        short resolution = 0;
        p_text_channel chan = &text_channel[screen];

        border = *chan->border_control;
        pixel_double = *chan->master_control & VKY3_MCR_DOUBLE_EN;
        resolution = (*chan->master_control & VKY3_MCR_RESOLUTION_MASK) >> 8;

        /* Set number of maximum rows and columns based on the base resolution */
        switch (resolution) {
            case 0: /* 640x480 */
                chan->columns_max = 80;
                chan->rows_max = 60;
                break;

            case 1: /* 800x600 */
                chan->columns_max = 100;
                chan->rows_max = 75;
                break;

            case 2: /* 1024x768 */
                chan->columns_max = 128;
                chan->rows_max = 96;
                break;

            case 3: /* 640x400 */
                chan->columns_max = 80;
                chan->rows_max = 50;
                break;

            default:
                break;
        }

        /* If we are pixel doubling, characters are twice as big */
        if (pixel_double) {
            chan->columns_max /= 2;
            chan->rows_max /= 2;
        }

        /* Calculate visible rows and columns assuming no border */
        chan->rows_visible = chan->rows_max;
        chan->columns_visible = chan->columns_max;

        /* If the border is enabled, subtract it from the visible rows and columns */
        if (border & VKY3_BRDR_EN) {
            short border_width = (border & VKY3_X_SIZE_MASK) >> 8;
            short border_height = (border & VKY3_Y_SIZE_MASK) >> 16;

            short columns_reduction = border_width / 4;
            short rows_reduction = border_height / 4;

            if (pixel_double) {
                columns_reduction /= 2;
                rows_reduction /= 2;
            }

            chan->columns_visible -= columns_reduction;
            chan->rows_visible -= rows_reduction;
        }
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
        for (i = 0; i < chan->columns_max * chan->rows_max; i++) {
            chan->text_cells[i] = ' ';
            chan->color_cells[i] = chan->current_color;
        }
    }
}

/*
 * Scroll the text screen up one row
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 */
void text_scroll(short screen) {
    if (screen < MAX_TEXT_CHANNELS) {
        short row, column;
        p_text_channel chan = &text_channel[screen];

        for (row = 0; row < chan->rows_visible - 1; row++) {
            short offset1 = row * chan->columns_max;
            short offset2 = (row + 1) * chan->columns_max;
            volatile short * text_dest = &chan->text_cells[offset1];
            volatile short * color_dest = &chan->color_cells[offset1];
            volatile short * text_src = &chan->text_cells[offset2];
            volatile short * color_src = &chan->color_cells[offset2];

            for (column = 0; column < chan->columns_max; column += 2) {
                *text_dest++ = *text_src++;
                *color_dest++ = *color_src++;
            }
        }

        short offset3 = (chan->rows_visible - 1) * chan->columns_max;
        volatile short * text_dest = &chan->text_cells[offset3];
        volatile short * color_dest = &chan->color_cells[offset3];
        uint8_t color = chan->current_color;
        for (column = 0; column < chan->columns_max; column += 2) {
            *text_dest++ = ' ';
            *color_dest++ = color;
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

        switch (c) {
        case CHAR_NL:
            text_set_xy(screen, 0, chan->y + 1);
            break;

        case CHAR_CR:
            break;

        default:
            *chan->text_cursor_ptr++ = c;
            *chan->color_cursor_ptr++ = chan->current_color;
            text_set_xy(screen, chan->x + 1, chan->y);
            break;
        }
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
