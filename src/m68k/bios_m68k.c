/**
 * Implementation of 68000 specific syscall routines.
 *
 * NOTE: these routines are not called directly but are instead called through TRAP#13
 */

#include "log.h"
#include "types.h"
#include "syscalls.h"
#include "interrupt.h"
#include "dev/channel.h"
#include "dev/block.h"
#include "dev/fsys.h"

/*
 * Determine the correct system function implementation and call it.
 */
int32_t syscall_dispatch(int32_t function, int32_t param0, int32_t param1, int32_t param2, int32_t param3, int32_t param4, int32_t param5) {
    switch (function & 0x00f0) {
        case 0x00:
            /* Core System Calls */
            switch (function) {
                case KFN_EXIT:
                    proc_exit((int)param0);
                    break;

                case KFN_WARMBOOT:
                    break;

                case KFN_INT_REGISTER:
                    return (int32_t)int_register((unsigned short)param0, (p_int_handler)param1);

                case KFN_INT_ENABLE:
                    int_enable((unsigned short)param0);
                    return;

                case KFN_INT_DISABLE:
                    int_disable((unsigned short)param0);
                    return;

                case KFN_INT_ENABLE_ALL:
                    int_enable_all();
                    return;

                case KFN_INT_DISABLE_ALL:
                    int_disable_all();
                    return;

                case KFN_INT_CLEAR:
                    int_clear((unsigned short)param0);
                    return;

                case KFN_INT_PENDING:
                    return int_pending((unsigned short)param0);

                default:
                    break;
            }
            break;

        case 0x10:
            /* Channel system calls */
            switch (function) {
                case KFN_CHAN_WRITE_B:
                    return chan_write_b((short)param0, (uint8_t)param1);

                case KFN_CHAN_WRITE:
                    return chan_write((short)param0, (const uint8_t *)param1, (short)param2);

                case KFN_CHAN_READ_B:
                    return chan_read_b((short)param0);

                case KFN_CHAN_READ:
                    return chan_read((short)param0, (const uint8_t *)param1, (short)param2);

                case KFN_CHAN_READ_LINE:
                    return chan_readline((short)param0, (const uint8_t *)param1, (short)param2);

                case KFN_CHAN_STATUS:
                    return chan_status((short)param0);

                case KFN_CHAN_FLUSH:
                    return chan_flush((short)param0);

                case KFN_CHAN_SEEK:
                    return chan_seek((short)param0, (long)param1, (short)param2);

                case KFN_CHAN_IOCTRL:
                    return chan_ioctrl((short)param0, (short)param1, (unsigned char *)param2, (short)param3);

                case KFN_CHAN_REGISTER:
                    return cdev_register((p_dev_chan)param0);

                default:
                    break;
            }
            break;

        case 0x20:
            /* Block device system calls */
            switch (function) {
                case KFN_BDEV_GETBLOCK:
                    return bdev_read((short)param0, (long)param1, (unsigned char *)param2, (short)param3);

                case KFN_BDEV_PUTBLOCK:
                    return bdev_write((short)param0, (long)param1, (unsigned char *)param2, (short)param3);

                case KFN_BDEV_FLUSH:
                    return bdev_flush((short)param0);

                case KFN_BDEV_STATUS:
                    return bdev_status((short)param0);

                case KFN_BDEV_IOCTRL:
                    return bdev_ioctrl((short)param0, (short)param1, (unsigned char *)param2, (short)param3);

                case KFN_BDEV_REGISTER:
                    return bdev_register((p_dev_block)param0);

                default:
                    break;
            }
            break;

        case 0x30:
            /* File System Calls */
            switch (function) {
                case KFN_OPEN:
                    return fsys_open((const char *)param0, (short)param1);

                case KFN_CLOSE:
                    return fsys_close((short)param0);

                case KFN_OPENDIR:
                    return fsys_opendir((const char *)param0);

                case KFN_CLOSEDIR:
                    return fsys_closedir((short)param0);

                case KFN_READDIR:
                    return fsys_readdir((short)param0, (p_file_info)param1);

                case KFN_FINDFIRST:
                    return fsys_findfirst((const char *)param0, (const char *)param1, (p_file_info)param2);

                case KFN_FINDNEXT:
                    return fsys_findnext((short)param0, (p_file_info)param1);

                case KFN_DELETE:
                    return fsys_delete((const char *)param0);

                case KFN_RENAME:
                    return fsys_rename((const char *)param0, (const char *)param1);

                default:
                    break;
            }

            break;

        default:
            break;
    }

    DEBUG("syscall unknown function\n");
    do {} while (1);
    return -1;
}