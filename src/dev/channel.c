/**
 * Implementation of support low-level byte-oriented device drivers
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
    #define DEFAULT_LOG_LEVEL LOG_DEBUG
#endif

#include <stdint.h>
#include <string.h>

#include "errors.h"
#include "log.h"
#include "simpleio.h"
#include "types.h"
#include "dev/channel.h"

static t_dev_chan devices[CDEV_DEVICES_MAX];
static t_channel channels[CHAN_MAX];


//
// Initialize the channel driver system
//
void cdev_init_system() {
    int i;

    TRACE("cdev_init_system");

    // Clear out all the channel device records...
    for (i = 0; i < CDEV_DEVICES_MAX; i++) {
        devices[i].number = 0;
        devices[i].name = 0;
    }

    // Clear out all the channel records
    for (i = 0; i < CHAN_MAX; i++) {
        channels[i].number = -1;
        channels[i].dev = -1;
    }
}

//
// Register a channel device driver
//
int16_t cdev_register(const p_dev_chan device) {
    int16_t dev;

    TRACE1("cdev_register %s", device->name);

    dev = device->number;
    if (dev < CDEV_DEVICES_MAX) {
        // Copy the device description into the master table
        memcpy(&devices[dev], device, sizeof(t_dev_chan));
        return E_OK;
    } else {
        ERROR2("cdev_register failed to register %s (%d)", device->name, device->number);
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
p_channel chan_alloc(int16_t dev) {
    int i;

    TRACE1("chan_alloc(%d)", (int)dev);

    if ((dev >= CDEV_CONSOLE) && (dev < CDEV_FILE)) {
        /* For most devices (all but files): the channel is always the same number as the device */
        channels[dev].number = dev;
        channels[dev].dev = dev;
        return &channels[dev];

    } else {
        for (i = CDEV_FILE; i < CHAN_MAX; i++) {
            if (channels[i].number != i) {
                channels[i].number = i;
                channels[i].dev = dev;
                return &channels[i];
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
p_channel chan_get_record(int16_t c) {
    return &channels[c];
}

//
// Return a channel to the pool of unused channels
//
// Inputs:
// chan = a pointer to the channel record to return to the kernel
//
void chan_free(p_channel chan) {
    TRACE1("chan_free: %d", chan->number);

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
int16_t chan_get_records(int16_t channel, p_channel * chan, p_dev_chan * cdev) {
    if (channel < CHAN_MAX) {
        *chan = &channels[channel];
        //TRACE3("chan_get_records(%d,%d,%d)", channel, (*chan)->number, (*chan)->dev);
        if ((*chan)->number == channel) {
            if ((*chan)->dev < CDEV_DEVICES_MAX) {
                *cdev = &devices[(*chan)->dev];
                return 0;
            } else {
                ERROR1("chan_get_records 1: %d", (*chan)->dev);
                return DEV_ERR_BADDEV;
            }

        } else {
            ERROR1("chan_get_records 2: %d", channel);
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
int16_t cdev_init(int16_t dev) {
    TRACE1("cdev_init(%d)", dev);
    int16_t ret = DEV_ERR_BADDEV;
    if (dev < CDEV_DEVICES_MAX) {
        p_dev_chan cdev = &devices[dev];
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
int16_t chan_open(int16_t dev, const uint8_t * path, int16_t mode) {
    int16_t result;
    p_channel chan;
    p_dev_chan cdev;

    TRACE3("chan_open(%d,%s,%d)",dev,path?path:"",mode);

    if (dev < CDEV_DEVICES_MAX) {
        /* Get the device record */
        cdev = &devices[dev];
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
int16_t chan_close(int16_t channel) {
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
int16_t chan_read(int16_t channel, uint8_t * buffer, int16_t size) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_readline(int16_t channel, uint8_t * buffer, int16_t size) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_read_b(int16_t channel) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_write(int16_t channel, const uint8_t * buffer, int16_t size) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;
    //logmsg(LOG_TRACE,"chan_write(%d,%p,%x)", channel, buffer, (int)size);

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
int16_t chan_write_b(int16_t channel, uint8_t b) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_status(int16_t channel) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_flush(int16_t channel) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_seek(int16_t channel, long position, int16_t base) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_ioctrl(int16_t channel, int16_t command, uint8_t * buffer, int16_t size) {
    p_channel chan;
    p_dev_chan cdev;
    int16_t res;

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
int16_t chan_device(int16_t channel) {
    if (channel >= CHAN_MAX) {
        // If either channel ID is bad...
        return ERR_BADCHANNEL;

    } else {
        if (channels[channel].number != channel) {
            // Channel is closed
            return ERR_BADCHANNEL;

        } else {
            return channels[channel].dev;
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
int16_t chan_swap(int16_t channel1, int16_t channel2) {
    if ((channel1 >= CHAN_MAX) || (channel2 >= CHAN_MAX)) {
        // If either channel ID is bad...
        return ERR_BADCHANNEL;

    } else {
        uint8_t tmp_data[CHAN_DATA_SIZE];
        p_channel chan1 = 0, chan2 = 0;
        int16_t i = 0, tmp_dev = 0;

        chan1 = &channels[channel1];
        chan2 = &channels[channel2];

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
