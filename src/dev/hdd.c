/* HDD device driver, based on the IDE/PATA driver
 */

#include <stdint.h>
#include "log.h"
#include "dev/block.h"
#include "libfoenix/include/pata.h"

static int16_t hdd_ioctrl(int16_t command, uint8_t * buffer, int16_t size);

//
// Install the PATA driver
//
// Returns:
//  0 on success, any negative number is an error code
//
int16_t hdd_install() {
    t_dev_block bdev;

    TRACE("hdd_install");

    // Note: any error code is ignored here.
    pata_init();

    // Check if drive is installed
    // if ((*DIP_BOOTMODE & HD_INSTALLED) == 0) {
        bdev.number = BDEV_HDC;
        bdev.name = "HDD";
        bdev.init = pata_init;
        bdev.read = pata_read;
        bdev.write = pata_write;
        bdev.status = pata_status;
        bdev.flush = pata_flush;
        bdev.ioctrl = hdd_ioctrl;

        return bdev_register(&bdev);
    // } else {
    //     return 0;
    // }
}

//
// Issue a control command to the PATA hard drive
//
// Inputs:
//  command = the number of the command to send
//  buffer = pointer to chars of additional data for the command
//  size = the size of the buffer
//
// Returns:
//  0 on success, any negative number is an error code
//
static int16_t hdd_ioctrl(int16_t command, uint8_t * buffer, int16_t size) {
    int16_t result;
    int32_t *p_long;
    uint16_t *p_word;
    int32_t *p_lba_word;
    t_drive_info drive_info;
    p_drive_info p_info;

    TRACE("hdd_ioctrl");

    switch (command) {
        case PATA_GET_SECTOR_COUNT:
            p_lba_word = (int32_t *)buffer;
            result = pata_identity(&drive_info);
            if (result != 0) {
                return result;
            }

            *p_lba_word = drive_info.l.lba_default;
            break;

        case PATA_GET_SECTOR_SIZE:
            // Return the size of a sector... always 512
            p_word = (uint16_t *)buffer;
            *p_word = PATA_SECTOR_SIZE;
            break;

        case PATA_GET_BLOCK_SIZE:
            // This isn't a flash device... return 1
            p_long = (int32_t *)buffer;
            *p_long = 1;
            break;

        case PATA_GET_DRIVE_INFO:
            p_info = (p_drive_info)buffer;
            result = pata_identity(p_info);
            if (result != 0) {
                return result;
            }
            break;

        default:
            return 0;
    }

    return 0;
}