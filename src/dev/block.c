/**
 * Implementation of block device routines
 */

#include "log_level.h"
#ifndef DEFAULT_LOG_LEVEL
    #define DEFAULT_LOG_LEVEL LOG_ERROR
#endif

#include "log.h"
#include "block.h"

t_dev_block g_block_devs[BDEV_DEVICES_MAX];

//
// Initialize the block driver system
//
void bdev_init_system() {
    int i;

    TRACE("bdev_init_system");

    for (i = 0; i < BDEV_DEVICES_MAX; i++) {
        g_block_devs[i].number = 0;
        g_block_devs[i].name = 0;
    }
}

//
// Register a block device driver
//
short bdev_register(p_dev_block device) {
    short dev;

    TRACE1("bdev_register(%s)", device->name);

    dev = device->number;
    if (dev < BDEV_DEVICES_MAX) {
        // Copy the device description into the master table

        p_dev_block bdev = &g_block_devs[dev];
        bdev->number = device->number;
        bdev->name = device->name;
        bdev->init = device->init;
        bdev->read = device->read;
        bdev->write = device->write;
        bdev->status = device->status;
        bdev->flush = device->flush;
        bdev->ioctrl = device->ioctrl;
        TRACE("bdev_register returning 0");
        return 0;
    } else {
        TRACE("bdev_register returning DEV_ERR_BADDEV");
        return DEV_ERR_BADDEV;
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
short bdev_init(short dev) {
    TRACE1("bdev_init(%d)", (int)dev);

    short ret = DEV_ERR_BADDEV;

    if (dev < BDEV_DEVICES_MAX) {
        p_dev_block bdev = &g_block_devs[dev];
        if (bdev->number == dev)
            ret = bdev->init();
    }

    TRACE1("bdev_init returning %d", (int)ret);
    return ret;
}

//
// Read a block from the device
//
// Inputs:
//  dev = the number of the device
//  lba = the logical block address of the block to read
//  buffer = the buffer into which to copy the block data
//  size = the size of the buffer.
//
// Returns:
//  number of bytes read, any negative number is an error code
//
short bdev_read(short dev, long lba, unsigned char * buffer, short size) {
    TRACE4("bdev_read(%d,%ld,%p,%d)", (int)dev, lba, buffer, (int)size);

    short ret = DEV_ERR_BADDEV;

    if (dev < BDEV_DEVICES_MAX) {
        p_dev_block bdev = &g_block_devs[dev];
        if (bdev->number == dev)
            ret = bdev->read(lba, buffer, size);
    }

    TRACE1("bdev_read returning %d", (int)ret);
    return ret;
}

//
// Write a block from the device
//
// Inputs:
//  dev = the number of the device
//  lba = the logical block address of the block to write
//  buffer = the buffer containing the data to write
//  size = the size of the buffer.
//
// Returns:
//  number of bytes written, any negative number is an error code
//
short bdev_write(short dev, long lba, const unsigned char * buffer, short size) {
    TRACE4("bdev_write(%d,%ld,%p,%d)", (int)dev, lba, buffer, (int)size);

    short ret = DEV_ERR_BADDEV;

    if (dev < BDEV_DEVICES_MAX) {
        p_dev_block bdev = &g_block_devs[dev];
        if (bdev->number == dev)
            ret = bdev->write(lba, buffer, size);
    }

    TRACE1("bdev_write returning %d", (int)ret);
    return ret;
}

//
// Return the status of the block device
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  the status of the device
//
short bdev_status(short dev) {
    TRACE1("bdev_status(%d)", dev);

    short ret = DEV_ERR_BADDEV;

    if (dev < BDEV_DEVICES_MAX) {
        p_dev_block bdev = &g_block_devs[dev];
        if (bdev->number == dev)
            ret = bdev->status();
    }

    TRACE2("bdev_status returning %d (0x%x)", (int)ret, ret);
    return ret;
}

//
// Ensure that any pending writes to teh device have been completed
//
// Inputs:
//  dev = the number of the device
//
// Returns:
//  0 on success, any negative number is an error code
//
short bdev_flush(short dev) {
    TRACE1("bdev_flush(%d)", (int)dev);

    short ret = DEV_ERR_BADDEV;

    if (dev < BDEV_DEVICES_MAX) {
        p_dev_block bdev = &g_block_devs[dev];
        if (bdev->number == dev)
            return bdev->flush();
    }

    TRACE1("bdev_flush returning %d", (int)ret);
    return ret;
}

//
// Issue a control command to the device
//
// Inputs:
//  dev = the number of the device
//  command = the number of the command to send
//  buffer = pointer to bytes of additional data for the command
//  size = the size of the buffer
//
// Returns:
//  0 on success, any negative number is an error code
//
short bdev_ioctrl(short dev, short command, unsigned char * buffer, short size) {
    TRACE4("bdev_ioctrl(%d, %d, %p, %d)", (int)dev, command, buffer, (int)size);

    short ret = DEV_ERR_BADDEV;

    if (dev < BDEV_DEVICES_MAX) {
        p_dev_block bdev = &g_block_devs[dev];
        if (bdev->number == dev)
            ret =  bdev->ioctrl(command, buffer, size);
    }

    TRACE1("bdev_ioctrl returning %d", (int)ret);
    return ret;
}
