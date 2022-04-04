/**
 * @file txt_screen.c
 *
 * Uniform routines to manage the text screens
 *
 * The console code will call this layer, which will dispatch those calls
 * to the low level drivers (e.g. txt_a2560k_a) registered with it.
 */

#include "constants.h"
#include "log.h"
#include "dev/txt_screen.h"

/**
 * The array of device drivers.
 *
 * If the number of the driver record is 0, no driver is registered for that device
 */
static t_txt_device txt_device_driver[TXT_CNT_SCREENS];

/**
 * Initialize the text system.
 */
void txt_init() {
    int i;

    /* Initialize all the drivers to blank... */
    for (i = 0; i < TXT_CNT_SCREENS; i++) {
        txt_device_driver[i].number = 0;
        txt_device_driver[i].name = 0;

        txt_device_driver[i].init = 0;
        txt_device_driver[i].get_capabilities = 0;
        txt_device_driver[i].set_mode = 0;
        txt_device_driver[i].set_sizes = 0;
        txt_device_driver[i].set_resolution = 0;
        txt_device_driver[i].set_border = 0;
        txt_device_driver[i].set_border_color = 0;
        txt_device_driver[i].set_font = 0;
        txt_device_driver[i].set_cursor = 0;
        txt_device_driver[i].get_region = 0;
        txt_device_driver[i].set_region = 0;
        txt_device_driver[i].get_color = 0;
        txt_device_driver[i].set_color = 0;
        txt_device_driver[i].set_xy = 0;
        txt_device_driver[i].get_xy = 0;
        txt_device_driver[i].put = 0;
        txt_device_driver[i].scroll = 0;
        txt_device_driver[i].fill = 0;
    }
}

/**
 * Register a device driver for a text screen
 *
 * The data in the provided structure will be copied to the kernel's
 * internal structures. Also, the driver data provided will over-write
 * any previous driver data for the device number, if one was previously
 * registered.
 *
 * @param device the pointer to the device driver
 *
 * @return 0 on success, any other number is an error
 */
short txt_register(p_txt_device device) {
    if (device->number < TXT_CNT_SCREENS) {
        int i = device->number;

        txt_device_driver[i].number = device->number;
        txt_device_driver[i].name = device->name;

        txt_device_driver[i].init = device->init;
        txt_device_driver[i].get_capabilities = device->get_capabilities;
        txt_device_driver[i].set_mode = device->set_mode;
        txt_device_driver[i].set_sizes = device->set_sizes;
        txt_device_driver[i].set_resolution = device->set_resolution;
        txt_device_driver[i].set_border = device->set_border;
        txt_device_driver[i].set_border_color = device->set_border_color;
        txt_device_driver[i].set_font = device->set_font;
        txt_device_driver[i].set_cursor = device->set_cursor;
        txt_device_driver[i].get_region = device->get_region;
        txt_device_driver[i].set_region = device->set_region;
        txt_device_driver[i].get_color = device->get_color;
        txt_device_driver[i].set_color = device->set_color;
        txt_device_driver[i].set_xy = device->set_xy;
        txt_device_driver[i].get_xy = device->get_xy;
        txt_device_driver[i].put = device->put;
        txt_device_driver[i].scroll = device->scroll;
        txt_device_driver[i].fill = device->fill;

        return 0;
    } else {
        return -1;
    }
}

/**
 * Find the device driver for the screen, if installed
 *
 * @param device the pointer to the device driver
 *
 * @return pointer to the device driver structure, 0 if invalid or not found
 */
p_txt_device txt_get_device(short screen) {
    if (screen < TXT_CNT_SCREENS) {
        p_txt_device device = &(txt_device_driver[screen]);
        if (device->number == screen) {
            return device;
        } else {
            log_num(LOG_ERROR, "txt_get_device: number mismatch ", screen);
        }
    }

    return 0;
}

/**
 * Initialize a screen
 *
 * @param screen the number of the text device
 */
void txt_init_screen(short screen) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->init) {
            device->init();
        }
    } else {
        log_num(LOG_ERROR, "Could not find screen ", screen);
    }
}

/**
 * Gets the description of a screen's capabilities
 *
 * @param screen the number of the text device
 *
 * @return a pointer to the read-only description (0 on error)
 */
const p_txt_capabilities txt_get_capabilities(short screen) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->get_capabilities) {
            return device->get_capabilities();
        }
    }
    return 0;
}

/**
 * Set the display mode for the screen
 *
 * @param mode a bitfield of desired display mode options
 *
 * @return 0 on success, any other number means the mode is invalid for the screen
 */
short txt_set_mode(short screen, short mode) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_mode) {
            return device->set_mode(mode);
        }
    }
    return -1;
}

/**
 * Recalculate the size of the text screen
 *
 * @return 0 on success, any other number means the mode is invalid for the screen
 */
short txt_setsizes(short screen) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_sizes) {
            device->set_sizes();
            return 0;
        }
    }
    return -1;
}

/**
 * Set the display resolution of the screen
 *
 * @param screen the number of the text device
 * @param width the desired horizontal resolution in pixels
 * @param height the desired veritical resolution in pixels
 *
 * @return 0 on success, any other number means the resolution is unsupported
 */
short txt_set_resolution(short screen, short width, short height) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_resolution) {
            return device->set_resolution(width, height);
        }
    }
    return -1;
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param screen the number of the text device
 * @param width the horizontal size of one side of the border (0 - 32 pixels)
 * @param height the vertical size of one side of the border (0 - 32 pixels)
 */
void txt_set_border(short screen, short width, short height) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_border) {
            device->set_border(width, height);
        }
    }
}

/**
 * Set the size of the border of the screen (if supported)
 *
 * @param screen the number of the text device
 * @param red the red component of the color (0 - 255)
 * @param green the green component of the color (0 - 255)
 * @param blue the blue component of the color (0 - 255)
 */
void txt_set_border_color(short screen, unsigned char red, unsigned char green, unsigned char blue) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_border_color) {
            device->set_border_color(red, green, blue);
        }
    }
}

/**
 * Load a font as the current font for the screen
 *
 * @param screen the number of the text device
 * @param width width of a character in pixels
 * @param height of a character in pixels
 * @param data pointer to the raw font data to be loaded
 */
short txt_set_font(short screen, short width, short height, unsigned char * data) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_font) {
            return device->set_font(width, height, data);
        }
    }
    return -1;
}

/**
 * Set the appearance of the cursor
 *
 * @param screen the number of the text device
 * @param enable 0 to hide, any other number to make visible
 * @param rate the blink rate for the cursor (0=1s, 1=0.5s, 2=0.25s, 3=1/5s)
 * @param c the character in the current font to use as a cursor
 */
void txt_set_cursor(short screen, short enable, short rate, char c) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_cursor) {
            return device->set_cursor(enable, rate, c);
        }
    }
}

/**
 * Get the current region.
 *
 * @param screen the number of the text device
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
short txt_get_region(short screen, p_rect region) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->get_region) {
            return device->get_region(region);
        }
    }
    return -1;
}

/**
 * Set a region to restrict further character display, scrolling, etc.
 * Note that a region of zero size will reset the region to the full size of the screen.
 *
 * @param screen the number of the text device
 * @param region pointer to a t_rect describing the rectangular region (using character cells for size and size)
 *
 * @return 0 on success, any other number means the region was invalid
 */
short txt_set_region(short screen, p_rect region) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_region) {
            return device->set_region(region);
        }
    }
    return -1;
}

/**
 * Set the default foreground and background colors for printing
 *
 * @param screen the number of the text device
 * @param foreground the Text LUT index of the new current foreground color (0 - 15)
 * @param background the Text LUT index of the new current background color (0 - 15)
 */
short txt_set_color(short screen, unsigned char foreground, unsigned char background) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_color) {
            return device->set_color(foreground, background);
        }
    }
    return -1;
}

/*
 * Get the foreground and background color for printing
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * foreground = pointer to the foreground color number
 * background = pointer to the background color number
 */
void txt_get_color(short screen, unsigned char * foreground, unsigned char * background) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->get_color) {
            return device->get_color(foreground, background);
        }
    }
}

/**
 * Set the position of the cursor to (x, y) relative to the current region
 * If the (x, y) coordinate is outside the region, it will be clipped to the region.
 * If y is greater than the height of the region, the region will scroll until that relative
 * position would be within view.
 *
 * @param screen the number of the text device
 * @param x the column for the cursor
 * @param y the row for the cursor
 */
void txt_set_xy(short screen, short x, short y) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->set_xy) {
            device->set_xy(x, y);
        }
    }
}

/**
 * Get the position of the cursor (x, y) relative to the current region
 *
 * @param screen the number of the text device
 * @param position pointer to a t_point record to fill out
 */
void txt_get_xy(short screen, p_point position) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->get_xy) {
            device->get_xy(position);
        }
    }
}

/**
 * Print a character to the current cursor position in the current color
 *
 * Most character codes will result in a glyph being displayed at the current
 * cursor position, advancing the cursor one spot. There are some exceptions that
 * will be treated as control codes:
 *
 * 0x08 - BS - Move the cursor back one position, erasing the character underneath
 * 0x09 - HT - Move forward to the next TAB stop
 * 0x0A - LF - Move the cursor down one line (line feed)
 * 0x0D - CR - Move the cursor to column 0 (carriage return)
 *
 * @param screen the number of the text device
 * @param c the character to print
 */
void txt_put(short screen, char c) {
    t_point cursor;

    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->put) {
            switch (c) {
                case CHAR_BS:
                    /* Backspace */
                    txt_get_xy(screen, &cursor);
                    if (cursor.x > 0) {
                        txt_set_xy(screen, cursor.x - 1, cursor.y);
                        device->put(' ');
                        txt_set_xy(screen, cursor.x - 1, cursor.y);
                    }
                    break;

                case CHAR_TAB:
                    /* horizontal tab */
                    txt_get_xy(screen, &cursor);
                    txt_set_xy(screen, ((cursor.x >> 3) + 1) << 3, cursor.y);
                    break;

                case CHAR_NL:
                    /* line feed */
                    txt_get_xy(screen, &cursor);
                    txt_set_xy(screen, 0, cursor.y + 1);
                    break;

                case CHAR_CR:
                    /* carriage return */
                    break;

                default:
                    device->put(c);
                    break;
            }
        }
    }
}

/*
 * Send a character to the screen without any escape code interpretation
 *
 * Deprecated legacy function
 *
 * @param screen the screen number 0 for channel A, 1 for channel B
 * @param c the character to print
 */
void text_put_raw(short screen, char c) {
    txt_put(screen, c);
}

/**
 * Print an ASCII Z string to the screen
 *
 * @param screen the number of the text device
 * @param c the ASCII Z string to print
 */
void txt_print(short screen, const char * message) {
    const char * x = message;

    while (*x) {
        txt_put(screen, *x++);
    }
}

/**
 * Scroll the text in the current region
 *
 * @param screen the number of the text device
 * @param horizontal the number of columns to scroll (negative is left, positive is right)
 * @param vertical the number of rows to scroll (negative is down, positive is up)
 */
void txt_scroll(short screen, short horizontal, short vertical) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->scroll) {
            device->scroll(horizontal, vertical);
        }
    }
}

/**
 * Fill the current region with a character in the current color
 *
 * @param screen the number of the text device
 * @param c the character to fill the region with
 */
void txt_fill(short screen, char c) {
    p_txt_device device = txt_get_device(screen);
    if (device) {
        if (device->fill) {
            device->fill(c);
        }
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
void txt_clear(short screen, short mode) {
    t_point cursor;
    t_rect old_region, region;

    txt_get_xy(screen, &cursor);
    txt_get_region(screen, &old_region);

    switch (mode) {
        case 0:
            // Erase from cursor to end of region

            // Clear the end of the line
            region.origin.x = old_region.origin.x + cursor.x;
            region.origin.y = old_region.origin.y + cursor.y;
            region.size.width = old_region.size.width - cursor.x;
            region.size.height = 1;
            txt_set_region(screen, &region);
            txt_fill(screen, ' ');

            // Clear the region after the cursor
            region.origin.x = old_region.origin.x;
            region.origin.y = old_region.origin.y + cursor.y + 1;
            region.size.width = old_region.size.width;
            region.size.height = old_region.size.height - cursor.y - 1;
            txt_set_region(screen, &region);
            txt_fill(screen, ' ');

            // Restore the original region
            txt_set_region(screen, &old_region);
            break;

        case 1:
            // Erase from start of region to cursor

            // Clear the region to the cursor line
            region.origin.x = old_region.origin.x;
            region.origin.y = old_region.origin.y;
            region.size.width = old_region.size.width;
            region.size.height = cursor.y;
            txt_set_region(screen, &region);
            txt_fill(screen, ' ');

            // Clear the end of the line
            region.origin.x = old_region.origin.x;
            region.origin.y = old_region.origin.y + cursor.y;
            region.size.width = cursor.x + 1;
            region.size.height = 1;
            txt_set_region(screen, &region);
            txt_fill(screen, ' ');

            // Restore the original region
            txt_set_region(screen, &old_region);
            break;

        case 2:
            // Erase entire region
            txt_fill(screen, ' ');
            break;

        default:
            break;
    }
}

/*
 * Clear part or all of the current line
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * mode = 0: erase from the start of the line to the cursor,
 *        1: erase from cursor to end of the line,
 *        2: erase entire line
 */
void txt_clear_line(short screen, short mode) {
    t_point cursor;
    t_rect old_region, region;

    txt_get_xy(screen, &cursor);
    txt_get_region(screen, &old_region);

    switch (mode) {
        case 0:
            // Erase from cursor to end of line

            // Clear the end of the line
            region.origin.x = old_region.origin.x + cursor.x;
            region.origin.y = old_region.origin.y + cursor.y;
            region.size.width = old_region.size.width - cursor.x;
            region.size.height = 1;
            txt_set_region(screen, &region);
            txt_fill(screen, ' ');

            // Restore the original region
            txt_set_region(screen, &old_region);
            break;

        case 1:
            // Erase from start of line to cursor

            // Clear the end of the line
            region.origin.x =old_region.origin.x;
            region.origin.y = old_region.origin.y + cursor.y;
            region.size.width = cursor.x;
            region.size.height = 1;
            txt_set_region(screen, &region);
            txt_fill(screen, ' ');

            // Restore the original region
            txt_set_region(screen, &old_region);
            break;

        case 2:
            // Clear the line
            region.origin.x = old_region.origin.x;
            region.origin.y = old_region.origin.y + cursor.y;
            region.size.width = old_region.size.width;
            region.size.height = 1;
            txt_set_region(screen, &region);
            txt_fill(screen, ' ');

            // Restore the original region
            txt_set_region(screen, &old_region);
            break;

        default:
            break;
    }
}

/*
 * Insert a number of characters at the cursor position
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * count = the number of characters to insert
 */
void txt_insert(short screen, short count) {
    t_point cursor;
    t_rect old_region, region;

    if (count > 0) {
        txt_get_xy(screen, &cursor);
        txt_get_region(screen, &old_region);

        region.origin.x = old_region.origin.x + cursor.x;
        region.origin.y = old_region.origin.y + cursor.y;
        region.size.width = old_region.size.width - cursor.x;
        region.size.height = 1;
        txt_set_region(screen, &region);
        txt_scroll(screen, 0 - count, 0);
        txt_set_region(screen, &old_region);
    }
}

/*
 * Delete a number of characters at the cursor position
 *
 * Inputs:
 * screen = the screen number 0 for channel A, 1 for channel B
 * count = the number of characters to delete
 */
void txt_delete(short screen, short count) {
    t_point cursor;
    t_rect old_region, region;
    short left;

    if (count > 0) {
        txt_get_xy(screen, &cursor);
        txt_get_region(screen, &old_region);

        if (count > cursor.x) {
            count = cursor.x;
        }

        region.origin.x = old_region.origin.x + cursor.x;
        region.origin.y = old_region.origin.y + cursor.y;
        region.size.width = old_region.size.width - cursor.x;
        region.size.height = 1;
        txt_set_region(screen, &region);
        txt_scroll(screen, count, 0);
        txt_set_region(screen, &old_region);
    }
}
