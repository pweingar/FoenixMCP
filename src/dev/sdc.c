/* SD card driver for MCP */

#include <stdint.h>
#include "block.h"
#include "log.h"
#include "dev/sdc.h"
#include "libfoenix/include/sdc.h"


static int16_t sdc_ioctrl(int16_t command, uint8_t * buffer, int16_t size);

//
// Install the SDC driver
//
int16_t sdc_install() {
    t_dev_block dev;   // bdev_register copies the data, so we'll allocate this on the stack

    TRACE("sdc_install");

    sdc_init();

    dev.number = BDEV_SDC;
    dev.name = "SDC";
    dev.init = sdc_init;
    dev.read = sdc_read;
    dev.write = sdc_write;
    dev.flush = sdc_flush;
    dev.status = sdc_status;
    dev.ioctrl = sdc_ioctrl;

    return bdev_register(&dev);
}


//
// Issue a control command to the device
//
// Inputs:
//  command = the number of the command to send
//  buffer = pointer to bytes of additional data for the command
//  size = the size of the buffer
//
// Returns:
//  0 on success, any negative number is an error code
//
static int16_t sdc_ioctrl(int16_t command, uint8_t * buffer, int16_t size) {
    uint32_t *p_dword;
    uint16_t *p_word;
    uint32_t *p_lba_word;

    switch (command) {
        case SDC_GET_SECTOR_COUNT:
            p_lba_word = (uint32_t *)buffer;
            *p_lba_word = sdc_sector_count();
            break;

        case SDC_GET_SECTOR_SIZE:
            // Return the size of a sector... always 512
            p_word = (uint16_t *)buffer;
            *p_word = SDC_SECTOR_SIZE;
            break;

        case SDC_GET_BLOCK_SIZE:
            // We don't know what the block size is... return 1
            p_dword = (uint32_t *)buffer;
            *p_dword = 1;
            break;

        default:
            break;
    }

	return 0;
}