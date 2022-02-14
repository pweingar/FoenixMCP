/*
 * Text screen driver framework
 */

#include "errors.h"
#include "dev/text_device.h"

/* How many text devices do we support */
#define MAX_TXT_DEVICES 4

/* The list of known text devices */
t_txt_device txt_devices[MAX_TXT_DEVICES];

/*
 * Initialize the display system
 */
void txt_init() {
    int i;
    for (i = 0; i < MAX_TXT_DEVICES; i++) {
        p_txt_device dev = &txt_devices[i];
        dev.number = 0;
        dev.name = 0;
    }
}

/*
 * Install the device driver for a text display
 *
 * Inputs:
 * driver = pointer to the t_txt_device record for the text display
 *
 * Returns:
 * 0 on success, any other number is an error code
 */
short txt_install(p_txt_device driver) {
    if (driver->number < MAX_TXT_DEVICES) {
        p_txt_device dev = &txt_devices[driver->number];

        // Try to make a copy of the name
        dev->name = (char *)malloc(strlen(driver->name) + 1);
        if (dev->name != 0) {
            // Copy the driver data
            dev->number = driver->number;
            strcpy(dev->name, driver->name);
            dev->init = driver->init;
            dev->get_caps = driver->get_caps;
            dev->set_mode = driver->set_mode;
            dev->set_resolution = driver->set_resolution;
            dev->set_font = driver->set_font;
            dev->set_border = driver->set_border;
            dev->set_sizes = driver->set_sizes;
            dev->put = driver->put;
            dev->move = driver->move;
            dev->fill = driver->fill;

            // Initialize the text display
            return dev->init();
        } else {
            // Could not allocate a copy of the device name
            return ERR_OUT_OF_MEMORY;
        }
    } else {
        // The device number was out of range
        return DEV_ERR_BADDEV;
    }
}

/**
 * Find the driver for device
 *
 * Inputs:
 * device = the number of the device
 *
 * Returns:
 * pointer to the device driver for that device, 0 if not found
 */
p_txt_device txt_find_device(short device) {
    p_txt_device dev = 0;

    if (device < MAX_TXT_DEVICES) {
        dev = &txt_devices[device];
        if (dev != 0) {
            if (dev->number == 0) {
                // Found a record, but no driver was installed for it...
                return 0;
            }
        }
    }

    return dev;
}

/*
 * Get the list of supported display resolutions
 *
 * Inputs:
 * display = the number of the display
 *
 * Returns:
 * a pointer to a table describing the supported resolutions (w, h) in pixels of the display
 * A sentinel entry with w = h = 0 marks the end of the list.
 */
const p_rect txt_get_resolutions(short display) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        p_disp_caps capabilities = dev->get_caps();
        return capabilities->resolutions;
    } else {
        return 0;
    }
}

/*
 * Set the base resolution of the display.
 * It is an error if the requested resolution is not supported.
 *
 * Inputs:
 * display = the number of the display
 * width = the number of pixels across
 * height = the number of pixels down
 *
 * Returns:
 * 0 on success, any other number means the requested resolution is not supported
 */
short txt_set_resolution(short display, short width, short height) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->set_resolution(width, height);
        return 0;
    } else {
        return DEV_ERR_BADDEV;
    }
}

/*
 * Get the list of supported font sizes
 *
 * Inputs:
 * display = the number of the display
 *
 * Returns:
 * a pointer to a table describing the supported font sizes (w, h) in pixels
 * A sentinel entry with w = h = 0 marks the end of the list.
 */
const p_rect txt_get_font_sizes(short display) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        p_disp_caps capabilities = dev->get_caps();
        return capabilities->fonts;
    } else {
        return 0;
    }
}

/*
 * Set the font (size and bitmap) for display
 * It is an error if the font size is not supported.
 *
 * Inputs:
 * display = the number of the display
 * width = the number of pixels across per character
 * height = the number of pixels down per character
 * data = pointer to the raw pixel data for the font
 *
 * Returns:
 * 0 on success, any other number means the requested resolution is not supported
 */
short txt_set_font(short display, short width, short height, unsigned char * data) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->set_font(width, height, data);
        return 0;
    } else {
        return DEV_ERR_BADDEV;
    }
}

/*
 * Set the border of the display
 *
 * Inputs:
 * display = the number of the display
 * visible = 0 for disabled, 1 for enabled (visible)
 * horizontal = the width of the border in pixels
 * vertical = the height of the border in pixels
 * color = pointer to the color structure specifying the color of the border
 */
void txt_set_border(short display, short visible, short horizontal, short vertical, p_color3 color) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->set_border(visible, width, height, color);
    }
}

/*
 * Set the display region to limit all character operations
 * Changes to the characters, scrolling, deleting, cursor positions, etc.
 * will be limited to the rectangular region provided.
 *
 * Setting a width or height of 0 in the region will reset the region to the
 * full screen, which is the default.
 *
 * Inputs:
 * display = the number of the display
 * region = pointer to rectangle describing the region (in character positions)
 */
void txt_set_region(short display, p_rect region) {

}

/*
 * Set the color of the text for future puts
 *
 * Inputs:
 * display = the number of the display
 * foreground = the color of the character
 * background = the color of the background
 */
void txt_set_colors(short display, short foreground, short background) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->foreground = foreground;
        dev->background = background;
    }
}

/*
 * Setup the text cursor
 *
 * Inputs:
 * display = the number of the display
 * c = the character to display for the cursor
 * visible = 0 for invisible, any other number for visible
 * foreground = the color of the cursor foreground
 * background = the color of the cursor background
 * rate = the flash rate for the cursor
 */
void txt_set_cursor(short display, short visible, char c, short foreground, short background, short rate) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->set_cursor(visible, c, foreground, background, rate);
    }
}

/*
 * Recompute shorternal size information about the display based on the hardware settings.
 *
 * This call allows the user program to alter VICKY registers itself and still allow the kernel text drivers
 * to display data correctly.
 *
 * Inputs:
 * display = the number of the display
 */
void txt_set_sizes(short display) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->set_sizes();
    }
}

/*
 * Set the position of the cursor within the current region
 *
 * Inputs:
 * display = the number of the display
 * column = the horizontal position of the cursor (within the region)
 * row = the vertical position of the cursor (within the region)
 */
void txt_set_position(short display, short column, short row) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->set_cursor(column, row);
    }
}

/*
 * Put a character on the screen at the cursor position
 *
 * Inputs:
 * display = the number of the display
 * c = the character to put on the display
 */
void txt_put(short display, char c) {
    p_txt_device dev = txt_find_device(display);
    if (dev != 0) {
        dev->set_put(c, dev->foreground, dev->background);
    }
}

/*
 * Clear all or part of the line the cursor is on (within the current region)
 *
 * Inputs:
 * display = the number of the display
 * mode = 2 = all, 1 = from cursor to end of line, 0 = from beginning of line to cursor
 */
void txt_clear_line(short display, short mode) {

}

/*
 * Clear all or part of the region
 *
 * Inputs:
 * display = the number of the display
 * mode = 2 = all, 1 = from cursor to end of region, 0 = from beginning of region to cursor
 */
void txt_clear(short display, short mode) {

}

/*
 * Insert a number of blanks at the cursor position
 *
 * Inputs:
 * display = the number of the display
 * n = number of blanks to insert
 */
void txt_insert(short display, short n) {

}

/*
 * Delete a number of blanks at the cursor position
 *
 * Inputs:
 * display = the number of the display
 * n = number of blanks to delete
 */
void txt_delete(short display, short n) {

}

/*
 * Scroll the current region horizontally or vertically
 *
 * Inputs:
 * display = the number of the display
 * horizontal = columns to scroll left (negative) or right (positive)
 * vertical = rows to scroll up (negative) or right (positive)
 */
void txt_scroll(short display, short horizontal, short vertical) {

}
