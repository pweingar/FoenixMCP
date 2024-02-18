/**
 * Implmentation of support low level channel device drivers
 *
 * A channel or stream is a sequence of bytes that may be read or written to.
 *
 * Examples include: console, serial port, an open file, etc.
 */

#include "log_level.h"
#ifdef DEFAULT_LOG_LEVEL
    //#undef DEFAULT_LOG_LEVEL
#endif
#ifndef DEFAULT_LOG_LEVEL
    #define DEFAULT_LOG_LEVEL LOG_TRACE
#endif

#include <string.h>

#include "dev/channel.h"
#include "errors.h"
#include "simpleio.h"
#include "sys_types.h"
#include "log.h"

t_dev_chan g_channel_devs[CDEV_DEVICES_MAX];
t_channel g_channels[CHAN_MAX];

//
// Initialize the channel driver system
//
void cdev_init_system() {
    int i;

    TRACE("cdev_init_system");

    // Clear out all the channel device records...
    for (i = 0; i < CDEV_DEVICES_MAX; i++) {
        g_channel_devs[i].number = 0;
        g_channel_devs[i].name = 0;
    }

    // Clear out all the channel records
    for (i = 0; i < CHAN_MAX; i++) {
        g_channels[i].number = -1;
        g_channels[i].dev = -1;
    }
}

//
// Register a channel device driver
//
short cdev_register(const p_dev_chan device) {

    TRACE1("cdev_register %s", device->name);
    short dev;

    dev = device->number;
    if (dev < CDEV_DEVICES_MAX) {
        // Copy the device description into the master table
        memcpy(&g_channel_devs[dev], device, sizeof(t_dev_chan));
        return E_OK;
    } else {
        return DEV_ERR_BADDEV;
    }
}

/*
 * Get a free channel
 *
 * Inputs:
 * the device to associate with the channel
 *
 * Returns:
 * A pointer to the free channel, 0 if none are available.
 */
p_channel chan_alloc(short dev) {
    int i;

    TRACE1("chan_alloc(%d)", (int)dev);

    if ((dev >= CDEV_CONSOLE) && (dev < CDEV_FILE)) {
        /* For most devices (all but files): the channel is always the same number as the device */
        g_channels[dev].number = dev;
        g_channels[dev].dev = dev;
        return &g_channels[dev];

    } else {
        for (i = CDEV_FILE; i < CHAN_MAX; i++) {
            if (g_channels[i].number != i) {
                g_channels[i].number = i;
                g_channels[i].dev = dev;
                return &g_channels[i];
            }
        }
    }

    return 0;
}

//
// Return a pointer to the channel record for a given channel handle.
//
// Inputs:
// c = the number of the channel
//
// Returns:
// a pointer to the channel record.
//
p_channel chan_get_record(short c) {
    return &g_channels[c];
}

//
// Return a channel to the pool of unused channels
//
// Inputs:
// chan = a pointer to the channel record to return to the kernel
//
void chan_free(p_channel chan) {
    log_num(LOG_INFO, "chan_free: ", chan->number);

    chan->number = -1;
    chan->dev = -1;
}

//
// Find the records for the channel and the channel's device, given the channel number
//
// Inputs:
//  channel = the number of the channel to look up
//  chan = pointer to the channel structure pointer to set
//  cdev = pointer to the channel device structure pointer to set
//
//  Returns:
//   0 on success, a negative number on error
//
short chan_get_records(short channel, p_channel * chan, p_dev_chan * cdev) {
    if (channel < CHAN_MAX) {
        *chan = &g_channels[channel];
        if ((*chan)->number == channel) {
            if ((*chan)->dev < CDEV_DEVICES_MAX) {
                *cdev = &g_channel_devs[(*chan)->dev];
                return 0;
            } else {
                log_num(LOG_ERROR, "chan_get_records 1: ", (*chan)->dev);
                return DEV_ERR_BADDEV;
            }

        } else {
            log_num(LOG_ERROR, "chan_get_records 2: ", channel);
            return DEV_ERR_BADDEV;
        }

    } else {
        return ERR_BADCHANNEL;
    }
}

//
// Initialize the device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
short cdev_init(short dev) {
    short ret = DEV_ERR_BADDEV;
    if (dev < CDEV_DEVICES_MAX) {
        p_dev_chan cdev = &g_channel_devs[dev];
        if (cdev->number == dev)
            ret = cdev->init ? cdev->init() : 0;
    }
    return ret;
}

/*
 * Open a channel
 *
 * Inputs:
 * dev = the device number to have a channel opened
 * path = a "path" describing how the device is to be open
 * mode = is the device to be read, written, both?
 *
 * Returns:
 * the number of the channel opened, negative number on error
 */
SYSTEMCALL short chan_open(short dev, const uint8_t * path, short mode) {
    short result;
    p_channel chan;
    p_dev_chan cdev;

    TRACE("chan_open");
    log_num(LOG_DEBUG, "dev = ", dev);

    if (dev < CDEV_DEVICES_MAX) {
        /* Get the device record */
        cdev = &g_channel_devs[dev];
        if (cdev->number != dev) {
            /* Double check we have a real device */
            return DEV_ERR_BADDEV;
        }

        /* Grab a channel */
        chan = chan_alloc(dev);
        if (chan == 0) {
            return ERR_OUT_OF_HANDLES;
        }
        if (chan->dev != dev) {
            return ERR_BADCHANNEL;
        }

        /* Open the channel */
        if (cdev->open) {
            result = cdev->open(chan, path, mode);
            if (result == 0) {
                /* Success: return the channel number */
                return chan->number;
            } else {
                return result;
            }
        } else {
            // There's no actual open routine... just return the channel number
            return chan->number;
        }
    } else {
        return DEV_ERR_BADDEV;
    }
}

/*
 * Close a channel
 *
 * Inputs:
 * chan = the number of the channel to close
 *
 * Returns:
 * nothing useful
 */
SYSTEMCALL short chan_close(short channel) {
    TRACE1("chan_close(%d)", channel);
    p_channel chan;
    p_dev_chan cdev;
    if (chan_get_records(channel, &chan, &cdev) == 0) {
        if (cdev->close) {
            cdev->close(chan);
        }
        chan_free(chan);
    }

    return 0;
}

//
// Read bytes from the channel
//
// Inputs:
//  channel = the number of the channel
//  buffer = the buffer into which to copy the channel data
//  size = the size of the buffer.
//
// Returns:
//  number of bytes read, any negative number is an error code
//
SYSTEMCALL short chan_read(short channel, uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    TRACE3("chan_read(%d,%p,%d)", (int)channel, buffer, (int)size);

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        DEBUG1("chan_read: %s", cdev->name);
        res = cdev->read ? cdev->read(chan, buffer, size) : 0;
    } else {
        DEBUG1("Couldn't get channel: %d", (int)res);
    }
    return res;
}

//
// Read a line from the channel
//
// Inputs:
//  channel = the number of the channel
//  buffer = the buffer into which to copy the channel data
//  size = the size of the buffer.
//
// Returns:
//  number of bytes read, any negative number is an error code
//
SYSTEMCALL short chan_readline(short channel, uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        if (cdev->readline) {
            return cdev->readline(chan, buffer, size);
        } else {
            return 0;
        }
    } else {
        return res;
    }
}


//
// Read a single uint8_t from the channel
//
// Inputs:
//  channel = the number of the channel
//
// Returns:
//  the value read (if negative, error)
//
SYSTEMCALL short chan_read_b(short channel) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        if (cdev->read_b) {
            return cdev->read_b(chan);
        } else {
            return 0;
        }
    } else {
        return res;
    }
}

//
// Write a bytes to the channel
//
// Inputs:
//  channel = the number of the channel
//  buffer = the buffer containing the data to write
//  size = the size of the buffer.
//
// Returns:
//  number of bytes written, any negative number is an error code
//
SYSTEMCALL short chan_write(short channel, const uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    log(LOG_TRACE,"chan_write(%d,%p,%x)", channel, buffer, (int)size);

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0)
        res = cdev->write ? cdev->write(chan, buffer, size) : 0;
    else
        ERROR1("chan_write error: %d", res);

    return res;
}

//
// Write a single uint8_t to the device
//
// Inputs:
//  channel = the number of the channel
//  b = the uint8_t to write
//
// Returns:
//  0 on success, a negative value on error
//
SYSTEMCALL short chan_write_b(short channel, uint8_t b) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    // TRACE("chan_write_b");
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0)
        res = cdev->write_b ? cdev->write_b(chan, b) : 0;
    else
        ERROR1("chan_write_b error: %d", res);

    return res;
}


//
// Return the status of the channel device
//
// Inputs:
//  channel = the number of the channel
//
// Returns:
//  the status of the device
//
SYSTEMCALL short chan_status(short channel) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0)
        res = cdev->status ? cdev->status(chan) : 0;
    else
        ERROR1("chan_status error: %d", res);

    return res;
}

//
// Ensure that any pending writes to teh device have been completed
//
// Inputs:
//  channel = the number of the channel
//
// Returns:
//  0 on success, any negative number is an error code
//
SYSTEMCALL short chan_flush(short channel) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0)
        res = cdev->flush ? cdev->flush(chan) : 0;
    else
        ERROR1("flush error: %d", res);

    return res;
}

/*
 * Attempt to set the position of the channel cursor (if supported)
 *
 * Inputs:
 *  channel = the number of the channel
 *  position = the position of the cursor
 *  base = whether the position is from the beginning of the channel, relative to the current position,
 *         or relative to the end of the channel
 *
 * Returns:
 *  0 = success, a negative number is an error.
 */
SYSTEMCALL short chan_seek(short channel, long position, short base) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0)
        res = cdev->seek ? cdev->seek(chan, position, base) : 0;
    else
        ERROR1("chan_seek error: %d", res);

    return res;
}

//
// Issue a control command to the device
//
// Inputs:
//  channel = the number of the channel
//  command = the number of the command to send
//  buffer = pointer to bytes of additional data for the command
//  size = the size of the buffer
//
// Returns:
//  0 on success, any negative number is an error code
//
SYSTEMCALL short chan_ioctrl(short channel, short command, uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;

    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0)
        res = cdev->ioctrl ? cdev->ioctrl(chan, command, buffer, size) : 0;
    else
        ERROR1("chan_seek error: %d", res);

    return res;
}

/**
 * Return the device associated with the channel
 *
 * @param channel the ID of the channel to query
 * @return the ID of the device associated with the channel, negative number for error
 */
short chan_device(short channel) {
    if (channel >= CHAN_MAX) {
        // If either channel ID is bad...
        return ERR_BADCHANNEL;

    } else {
        if (g_channels[channel].number != channel) {
            // Channel is closed
            return ERR_BADCHANNEL;

        } else {
            return g_channels[channel].dev;
        }
    }
}

/**
 * Swap the channel ID assignments for two channels
 *
 * Before call: channel1 = "Channel A", channel2 = "Channel B"
 * After call: channel1 = "Channel B", channel2 = "Channel A"
 *
 * @param channel1 the ID of one of the channels
 * @param channel2 the ID of the other channel
 * @return 0 on success, any other number is an error
 */
short chan_swap(short channel1, short channel2) {
    if ((channel1 >= CHAN_MAX) || (channel2 >= CHAN_MAX)) {
        // If either channel ID is bad...
        return ERR_BADCHANNEL;

    } else {
        uint8_t tmp_data[CHAN_DATA_SIZE];
        p_channel chan1 = 0, chan2 = 0;
        short i = 0, tmp_dev = 0;

        chan1 = &g_channels[channel1];
        chan2 = &g_channels[channel2];

        // Swap the devices
        tmp_dev = chan1->dev;
        chan1->dev = chan2->dev;
        chan2->dev = tmp_dev;

        // Swap the data blocks
        memcpy(tmp_data, chan1->data, CHAN_DATA_SIZE);
        memcpy(chan1->data, chan2->data, CHAN_DATA_SIZE);
        memcpy(chan2->data, tmp_data, CHAN_DATA_SIZE);

        return 0;
    }
}
