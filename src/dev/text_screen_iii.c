/*
 * Driver for VICKY III text screens, both channel A and channel B
 */

#include "constants.h"
#include "vicky_general.h"
#include "text_screen_iii.h"
#include "simpleio.h"
#include "rsrc/font/MSX_CP437_8x8.h"

#define MAX_TEXT_CHANNELS 2

/*
 * Structure to hold pointers to the text channel's registers and memory
 */
typedef struct s_text_channel {
    unsigned char current_color;

    volatile unsigned long * master_control;
    volatile char * text_cells;
    volatile char * color_cells;
    volatile unsigned long * cursor_settings;
    volatile unsigned long * cursor_position;
    volatile unsigned long * border_control;

    short columns_max;
    short rows_max;
    short columns_visible;
    short rows_visible;

    short x;
    short y;
    volatile char * text_cursor_ptr;
    volatile unsigned char * color_cursor_ptr;
} t_text_channel, *p_text_channel;

static t_text_channel text_channel[MAX_TEXT_CHANNELS];

//  0xHHLL, 0xHHLL
//  0xGGBB, 0xAARR
const unsigned short fg_color_lut [32] = {
	0x0000, 0xFF00,	// Black (transparent)
	0x0000, 0xFF80, // Mid-Tone Red
	0x8000, 0xFF00, // Mid-Tone Green
	0x8000, 0xFF80, // Mid-Tone Yellow
	0x0080, 0xFF00, // Mid-Tone Blue
	0x5500, 0xFFAA, // Mid-Tone Orange
	0x8080, 0xFF00, // Mid-Tone Cian
	0x8080, 0xFF80, // 50% Grey
	0x5555, 0xFF55, // Dark Grey
    0x5555, 0xFFFF, // Bright Red
	0xFF55, 0xFF55, // Bright Green
	0xFF55, 0xFFFF, // Bright Yellow
	0x55FF, 0xFF55, // Bright Blue
	0x7FFF, 0xFFFF, // Bright Orange
	0xFFFF, 0xFF55, // Bright Cyan
	0xFFFF, 0xFFFF 	// White
};

const unsigned short bg_color_lut [32] = {
    0x0000, 0xFF00,	// Black (transparent)
	0x0000, 0xFF80, // Mid-Tone Red
	0x8000, 0xFF00, // Mid-Tone Green
	0x8000, 0xFF80, // Mid-Tone Yellow
	0x0080, 0xFF00, // Mid-Tone Blue
	0x8000, 0xFF80, // Mid-Tone Orange
	0x8080, 0xFF00, // Mid-Tone Cian
	0x8080, 0xFF80, // 50% Grey
	0x5555, 0xFF55, // Dark Grey
    0x5555, 0xFFFF, // Bright Red
	0xFF55, 0xFF55, // Bright Green
	0xFF55, 0xFFFF, // Bright Yellow
	0x55FF, 0xFF55, // Bright Blue
	0x7FFF, 0xFFFF, // Bright Orange
	0xFFFF, 0xFF55, // Bright Cyan
	0xFFFF, 0xFFFF 	// White
};

/*
 * Initialize the text screen driver
 */
int text_init() {
    short need_hires = 0;
    int i, x;
    p_text_channel chan_a = &text_channel[0];

#if MODEL == MODEL_FOENIX_A2560K
    p_text_channel chan_b = &text_channel[1];
#endif

    for (i = 0; i < MAX_TEXT_CHANNELS; i++) {
        text_channel[i].master_control = 0xffffffff;
        text_channel[i].text_cells = 0xffffffff;
        text_channel[i].color_cells = 0xffffffff;
        text_channel[i].cursor_settings = 0xffffffff;
        text_channel[i].cursor_position = 0xffffffff;
        text_channel[i].border_control = 0xffffffff;
        text_channel[i].text_cursor_ptr = 0xffffffff;
        text_channel[i].color_cursor_ptr = 0xffffffff;
        text_channel[i].current_color = 0;
        text_channel[i].columns_max = 0;
        text_channel[i].rows_max = 0;
        text_channel[i].columns_visible = 0;
        text_channel[i].rows_visible = 0;
        text_channel[i].x = 0;
        text_channel[i].y = 0;
    }

	// Init CLUT for the Color Memory
	for (i = 0; i<32; i++) {
		FG_CLUT_A[i] = fg_color_lut[i];
		BG_CLUT_A[i] = bg_color_lut[i];
#if MODEL == MODEL_FOENIX_A2560K
		FG_CLUT_B[i] = fg_color_lut[i];
		BG_CLUT_B[i] = bg_color_lut[i];
#endif
	}

    /* Initialize everything... only do a screen if it's present */

    need_hires = ((*VKY3_DIP_REG & VKY3_DIP_HIRES) == 0) ? 1 : 0;

    chan_a->master_control = MasterControlReg_A;
    chan_a->text_cells = ScreenText_A;
    chan_a->color_cells = ColorText_A;
    chan_a->cursor_settings = CursorControlReg_L_A;
    chan_a->cursor_position = CursorControlReg_H_A;
    chan_a->border_control = BorderControlReg_L_A;

    if (need_hires) {
        *chan_a->master_control = VKY3_MCR_800x600 | VKY3_MCR_TEXT_EN;      /* Set to text only mode: 800x600 */
    } else {
        *chan_a->master_control = VKY3_MCR_640x480 | VKY3_MCR_TEXT_EN;      /* Set to text only mode: 640x480 */
    }

	chan_a->border_control[0] = 0x00102001;	// Enable
	chan_a->border_control[1] = 0x00000040;	//Dark Blue

    /* Set the font for channel A */

    for (i = 0; i < 0x800; i++) {
        unsigned char b = MSX_CP437_8x8_bin[i];
        VICKY_TXT_FONT_A[i] = b;
    }

    text_set_border(0, 1, 0x20, 0x10, 0x00008080);
    text_setsizes(0);
    text_set_color(0, 0xf, 4);
    text_set_cursor(0, 0xF3, 0x7F, 1, 1);
    text_set_xy(0, 0, 0);
    text_clear(0, 2);

#if MODEL == MODEL_FOENIX_A2560K

    chan_b->master_control = MasterControlReg_B;
    chan_b->text_cells = ScreenText_B;
    chan_b->color_cells = ColorText_B;
    chan_b->cursor_settings = CursorControlReg_L_B;
    chan_b->cursor_position = CursorControlReg_H_B;
    chan_b->border_control = BorderControlReg_L_B;

    if (need_hires) {
        *chan_b->master_control = VKY3_MCR_800x600 | VKY3_MCR_TEXT_EN;      /* Set to text only mode: 800x600 */
    } else {
        *chan_b->master_control = VKY3_MCR_640x480 | VKY3_MCR_TEXT_EN;      /* Set to text only mode: 640x480 */
    }

    chan_b->border_control[0] = 0x00102000;	// Enable
	chan_b->border_control[1] = 0x00400000;	//Dark Red

    text_setsizes(1);
    text_set_color(1, 4, 3);
    text_clear(1, 2);
    text_set_cursor(1, 0xF3, 0x7F, 1, 1);
    text_set_xy(1, 0, 0);

    /* Set the font for channel B */

    for (i = 0; i < 0x800; i++) {
        unsigned char b = MSX_CP437_8x8_bin[i];
        VICKY_TXT_FONT_B[i] = b;
    }

#endif

    return 0;
}

/*
 * Set the border
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * visible = 0 to hide, any other number to show
 * width = the horizontal thickness of the border in pixels
 * height = the vertical thickness of the border in pixels
 * color = the RGB color (xxRRGGBB)
 */
void text_set_border(short screen, short visible, short width, short height, unsigned long color) {
    if (screen < MAX_TEXT_CHANNELS) {
        p_text_channel chan = &text_channel[screen];

        if (visible) {
            /* Set the width and color */
            chan->border_control[0] = ((height & 0xff) << 16) | ((width & 0xff) << 8) | 1;
            chan->border_control[1] = (color & 0x00ff0000) | ((color & 0xff) << 8) | ((color & 0xff00) >> 8);

        } else {
            /* Hide the border and make it 0 width */
            chan->border_control[0] = 0;
            chan->border_control[1] = 0;
        }
    }
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

        *(chan->cursor_position) = ((unsigned long)y << 16) | (unsigned long)x;
        short offset = y * chan->columns_max + x;
        chan->text_cursor_ptr = &chan->text_cells[offset];
        chan->color_cursor_ptr = &chan->color_cells[offset];
    }
}

/*
 * Get the position of the cursor on the screen.
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * x = pointer to the location to store the column (0 is left most)
 * y =  pointer to the location to store the row (0 is right most)
 */
void text_get_xy(short screen, unsigned short * x, unsigned short * y) {
    p_text_channel chan = &text_channel[screen];

    *x = chan->x;
    *y = chan->y;
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
 * Get the foreground and background color for printing
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * foreground = pointer to the foreground color number
 * background = pointer to the background color number
 */
void text_get_color(short screen, short * foreground, short * background) {
    if (screen < MAX_TEXT_CHANNELS) {
        p_text_channel chan = &text_channel[screen];
        *foreground = (chan->current_color >> 4) & 0x0f;
        *background = chan->current_color & 0x0f;
    }
}

/*
 * Clear the screen of data
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * mode = 0: erase from the cursor to the end of the screen,
          1: erase from start of the screen to the cursor,
          2: erase entire screen
 */
void text_clear(short screen, short mode) {
    if (screen < MAX_TEXT_CHANNELS) {
        int i;
        int sos_index = 0;
        p_text_channel chan = &text_channel[screen];
        int eos_index = chan->columns_max * chan->rows_max;
        int cursor_index = chan->y * chan->columns_max + chan->x;

        // switch (mode) {
        //     case 0:
        //         /* Clear from cursor to the end of the screen */
        //         for (i = cursor_index; i < eos_index; i++) {
        //             chan->text_cells[i] = ' ';
        //             chan->color_cells[i] = chan->current_color;
        //         }
        //         break;
        //
        //     case 1:
        //         /* Clear from (0, 0) to cursor */
        //         for (i = sos_index; i <= cursor_index; i++) {
        //             chan->text_cells[i] = ' ';
        //             chan->color_cells[i] = chan->current_color;
        //         }
        //         break;
        //
        //     case 2:
                /* Clear entire screen */
                for (i = 0; i < 0x2000; i++) {
                    chan->text_cells[i] = ' ';
                    chan->color_cells[i] = chan->current_color;
                }

        //         break;
        //
        //     default:
        //         break;
        // }
    }
}

/*
 * Clear part or all of the current line
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * mode = 0: erase from the cursor to the end of the line
 *        1: erase from the start of the line to the cursor
 *        2: erase entire line
 */
void text_clear_line(short screen, short mode) {
    if (screen < MAX_TEXT_CHANNELS) {
        int i;
        p_text_channel chan = &text_channel[screen];
        int sol_index = chan->y * chan->columns_max;
        int eol_index = (chan->y + 1) * chan->columns_max;
        int cursor_index = chan->y * chan->columns_max + chan->x;

        switch (mode) {
            case 0:
                /* Clear from cursor to the end of the line */
                for (i = cursor_index; i < eol_index; i++) {
                    chan->text_cells[i] = ' ';
                    chan->color_cells[i] = chan->current_color;
                }
                break;

            case 1:
                /* Clear from (0, y) to cursor */
                for (i = sol_index; i <= cursor_index; i++) {
                    chan->text_cells[i] = ' ';
                    chan->color_cells[i] = chan->current_color;
                }
                break;

            case 2:
                /* Clear entire screen */
                for (i = sol_index; i < eol_index; i++) {
                    chan->text_cells[i] = ' ';
                    chan->color_cells[i] = chan->current_color;
                }
                break;

            default:
                break;
        }
    }
}

/*
 * Insert a number of characters at the cursor position
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * count = the number of characters to insert
 */
void text_insert(short screen, short count) {
    int i;
    p_text_channel chan = &text_channel[screen];
    int eol_index = (chan->y + 1) * chan->columns_max - 1;
    int cursor_index = chan->y * chan->columns_max + chan->x;

    for (i = cursor_index; i < eol_index; i++) {
        chan->text_cells[i+1] = chan->text_cells[i];
        chan->color_cells[i+1] = chan->color_cells[i];
    }

    chan->text_cells[cursor_index] = ' ';
    chan->color_cells[cursor_index] = chan->current_color;
}

/*
 * Delete a number of characters at the cursor position
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * count = the number of characters to delete
 */
void text_delete(short screen, short count) {
    int i;
    p_text_channel chan = &text_channel[screen];
    int eol_index = (chan->y + 1) * chan->columns_max - 1;
    int cursor_index = chan->y * chan->columns_max + chan->x;

    for (i = cursor_index; i < eol_index; i++) {
        chan->text_cells[i] = chan->text_cells[i+1];
        chan->color_cells[i] = chan->color_cells[i+1];
    }

    chan->text_cells[eol_index] = ' ';
    chan->color_cells[eol_index] = chan->current_color;
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
            *color_dest++ = color << 8 | color;
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
        short x, y;
        p_text_channel chan = &text_channel[screen];

        switch (c) {
        case CHAR_NL:
            text_set_xy(screen, 0, chan->y + 1);
            break;

        case CHAR_CR:
            break;

        case CHAR_BS:
            if (chan->x > 0) {
                text_set_xy(screen, chan->x - 1, chan->y);
                *chan->text_cursor_ptr = ' ';
                *chan->color_cursor_ptr = chan->current_color;
            }
            break;

        case CHAR_TAB:
            text_get_xy(screen, &x, &y);
            x = x + 8;
            text_set_xy(screen, x, y);
            break;

        default:
            *chan->text_cursor_ptr++ = c;
            *chan->color_cursor_ptr++ = chan->current_color;
            text_set_xy(screen, chan->x + 1, chan->y);
            break;
        }
    }
}
