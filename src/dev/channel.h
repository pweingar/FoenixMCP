/**
 * Definitions support low level channel device drivers
 *
 * A channel or stream is a sequence of bytes that may be read or written to.
 *
 * Examples include: console, serial port, an open file, etc.
 */

#ifndef __CHANNEL_H
#define __CHANNEL_H

#include "types.h"

/*
 * Preset channel device numbers
 */

#define CDEV_DEVICES_MAX    8       // The maximum number of channel devices we will support
#define CHAN_MAX            16      // The maximum number of open channels we will support
#define CHAN_DATA_SIZE      32      // The number of bytes in the channel's data area

#define CDEV_CONSOLE 0
#define CDEV_EVID 1
#define CDEV_COM1 2
#define CDEV_COM2 3
#define CDEV_LPT 4
#define CDEV_MIDI 5
#define CDEV_FILE 6

/*
 * Channel status bits
 */

#define CDEV_STAT_EOF       0x01    // The channel has reached the end of the data
#define CDEV_STAT_ERROR     0x02    // The channel has encountered some error
#define CDEV_STAT_READABLE  0x04    // The channel has data to read (read will not block)
#define CDEV_STAT_WRITABLE  0x08    // The channel can accept data (write will not block)

#define CDEV_SEEK_START     0       /* Seek from the start of the file */
#define CDEV_SEEK_RELATIVE  1       /* Seek from the current position */
#define CDEV_SEEK_END       2       /* Seek from teh end of the file */

/*
 * Structure defining a channel
 */

typedef struct s_channel {
    short number;                   // The number of the channel
    short dev;                      // The number of the channel's device
    uint8_t data[CHAN_DATA_SIZE];   // A block of state data that the channel code can use for its own purposes
} t_channel, *p_channel;

typedef short (*FUNC_CBS_2_S)(p_channel, uint8_t *, short);
typedef short (*FUNC_C_2_S)(p_channel);
typedef short (*FUNC_CcBS_2_S)(p_channel, const uint8_t *, short);
typedef short (*FUNC_CB_2_S)(p_channel, uint8_t);
typedef short (*FUNC_CLS_2_S)(p_channel, long, short);
typedef short (*FUNC_CSBS_2_S)(p_channel, short, uint8_t *, short);

/*
 * Structure defining a channel device's functions
 */

typedef struct s_dev_chan {
    short number;           // The number of the device (assigned by registration)
    char * name;            // The name of the device
    FUNC_V_2_S init;        // short init() -- Initialize the device
    FUNC_CcBS_2_S open;     // short open(t_channel * chan, const uint8_t * path, short mode) -- open a channel for the device
    FUNC_V_2_S close;       // short close(t_channel * chan) -- called when a channel is closed
    FUNC_CBS_2_S read;      // short read(t_channel *, uint8_t * buffer, short size) -- Read a a buffer from the device
    FUNC_CBS_2_S readline;  // short readline(t_channel *, uint8_t * buffer, short size) -- Read a line of text from the device
    FUNC_C_2_S read_b;      // short read_b(t_channel *) -- read a single uint8_t from the device
    FUNC_CcBS_2_S write;    // short write(t_channel *, const uint8_t * buffer, short size) -- Write a buffer to the device
    FUNC_CB_2_S write_b;    // short write_b(t_channel *, const uint8_t b) -- Write a single uint8_t to the device
    FUNC_C_2_S status;      // short status(t_channel *) -- Get the status of the device
    FUNC_C_2_S flush;       // short flush(t_channel *) -- Ensure that any pending writes to teh device have been completed
    FUNC_CLS_2_S seek;      // short cdev_seek(t_channel *, long position, short base) -- attempt to move the "cursor" position in the channel
    FUNC_CSBS_2_S ioctrl;   // short ioctrl(t_channel *, short command, uint8_t * buffer, short size)) -- Issue a control command to the device
} t_dev_chan, *p_dev_chan;

/*
 * Initialize the channel driver system
 */
extern void cdev_init_system();

/*
 * Register a channel device driver
 *
 * Inputs:
 * p_dev_chan = pointer to the description of the channel device
 */
extern short cdev_register(p_dev_chan device);

/*
 * Get a free channel
 *
 * Returns:
 * A pointer to the free channel, 0 if none are available.
 */
extern p_channel chan_alloc();

/*
 * Return a channel to the pool of unused channels
 *
 * Inputs:
 * chan = a pointer to the channel record to return to the kernel
 */
extern void chan_free(p_channel chan);

/*
 * Return a pointer to the channel record for a given channel handle.
 *
 * Inputs:
 * c = the number of the channel
 *
 * Returns:
 * a pointer to the channel record.
 */
extern p_channel chan_get_record(short c);

/*
 * Initialize the device
 *
 * Inputs:
 *  dev = the number of the device
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short cdev_init(short dev);

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
extern short chan_open(short dev, const uint8_t * path, short mode);

/*
 * Close a channel
 *
 * Inputs:
 * chan = the number of the channel to close
 *
 * Returns:
 * nothing useful
 */
extern short chan_close(short chan);

/*
 * Read bytes from the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *  buffer = the buffer into which to copy the channel data
 *  size = the size of the buffer.
 *
 * Returns:
 *  number of bytes read, any negative number is an error code
 */
extern short chan_read(short channel, uint8_t * buffer, short size);

/*
 * Read a line from the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *  buffer = the buffer into which to copy the channel data
 *  size = the size of the buffer.
 *
 * Returns:
 *  number of bytes read, any negative number is an error code
 */
extern short chan_readline(short channel, uint8_t * buffer, short size);

/*
 * Read a single uint8_t from the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  the value read (if negative, error)
 */
extern short chan_read_b(short channel);

/*
 * Write a bytes to the channel
 *
 * Inputs:
 *  channel = the number of the channel
 *  buffer = the buffer containing the data to write
 *  size = the size of the buffer.
 *
 * Returns:
 *  number of bytes written, any negative number is an error code
 */
extern short chan_write(short channel, const uint8_t * buffer, short size);

/*
 * Write a single uint8_t to the device
 *
 * Inputs:
 *  channel = the number of the channel
 *  b = the uint8_t to write
 *
 * Returns:
 *  0 on success, a negative value on error
 */
extern short chan_write_b(short channel, uint8_t b);

/*
 * Return the status of the channel device
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  the status of the device
 */
extern short chan_status(short channel);

/*
 * Ensure that any pending writes to teh device have been completed
 *
 * Inputs:
 *  channel = the number of the channel
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short chan_flush(short channel);

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
extern short chan_seek(short channel, long position, short base);

/*
 * Issue a control command to the device
 *
 * Inputs:
 *  channel = the number of the channel
 *  command = the number of the command to send
 *  buffer = pointer to bytes of additional data for the command
 *  size = the size of the buffer
 *
 * Returns:
 *  0 on success, any negative number is an error code
 */
extern short chan_ioctrl(short channel, short command, uint8_t * buffer, short size);

/**
 * Return the device associated with the channel
 *
 * @param channel the ID of the channel to query
 * @return the ID of the device associated with the channel, negative number for error
 */
extern short chan_device(short channel);

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
extern short chan_swap(short channel1, short channel2);

#endif
