/**
 * Implmentation of support low level channel device drivers
 * 
 * A channel or stream is a sequence of bytes that may be read or written to.
 * 
 * Examples include: console, serial port, an open file, etc.
 */

#include "dev/channel.h"
#include "errors.h"
#include "types.h"
#include "log.h"

t_dev_chan g_channel_devs[CDEV_DEVICES_MAX];
t_channel g_channels[CHAN_MAX];

//
// Initialize the channel driver system
//
void cdev_init_system() {
    int i;

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

    // Pre-open a channel for the console and EVID
    g_channels[0].number = 0;
    g_channels[0].dev = 0;

    g_channels[1].number = 1;
    g_channels[1].dev = 1;
}

//
// Register a channel device driver
//
short cdev_register(p_dev_chan device) {
    short dev;

    dev = device->number;
    if (dev < CDEV_DEVICES_MAX) {
        // Copy the device description into the master table

        p_dev_chan cdev = &g_channel_devs[dev];
        cdev->number = device->number;
        cdev->name = device->name;
        cdev->init = device->init;
        cdev->read = device->read;
        cdev->readline = device->readline;
        cdev->read_b = device->read_b;
        cdev->write = device->write;
        cdev->write_b = device->write_b;
        cdev->status = device->status;
        cdev->seek = device->seek;
        cdev->flush = device->flush;
        cdev->ioctrl = device->ioctrl;
        return 0;
    } else {
        return DEV_ERR_BADDEV;
    }
}

//
// Get a free channel
//
// Returns:
// A pointer to the free channel, 0 if none are available.
//
p_channel chan_alloc() {
    int i;

    for (i = 0; i < CHAN_MAX; i++) {
        if (g_channels[i].number < 0) {
            g_channels[i].number = i;
            return &g_channels[i];
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
    chan->number = -1;
    chan->dev = 0;
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
    if (dev < CDEV_DEVICES_MAX) {
        p_dev_chan cdev = &g_channel_devs[dev];
        if (cdev->number == dev) {
            return cdev->init();
        } else {
            return DEV_ERR_BADDEV;
        }
    }
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
                return DEV_ERR_BADDEV;
            }

        } else {
            return DEV_ERR_BADDEV;
        }

    } else {
        return ERR_BADCHANNEL;
    }
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
short chan_read(short channel, uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->read(chan, buffer, size);
    } else {
        return res;
    }
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
short chan_readline(short channel, uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->readline(chan, buffer, size);
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
short chan_read_b(short channel) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->read_b(chan);
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
short chan_write(short channel, const uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->write(chan, buffer, size);
    } else {
        DEBUG("chan_write error\n");
        return res;
    }
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
short chan_write_b(short channel, uint8_t b) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->write_b(chan, b);
    } else {
        return res;
    }  
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
short chan_status(short channel) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->status(chan);
    } else {
        return res;
    }  
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
short chan_flush(short channel) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->flush(chan);
    } else {
        return res;
    }  
}

//
// Attempt to set the position of the channel cursor (if supported)
//
// Inputs:
//  channel = the number of the channel
//  position = the position of the cursor
//  base = whether the position is absolute or relative to the current position
//
// Returns:
//  0 = success, a negative number is an error.
//
short chan_seek(short channel, long position, short base) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->seek(chan, position, base);
    } else {
        return res;
    }  
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
short chan_ioctrl(short channel, short command, uint8_t * buffer, short size) {
    p_channel chan;
    p_dev_chan cdev;
    short res;
    
    res = chan_get_records(channel, &chan, &cdev);
    if (res == 0) {
        return cdev->ioctrl(chan, command, buffer, size);
    } else {
        return res;
    }  
}
