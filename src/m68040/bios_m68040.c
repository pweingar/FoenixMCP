/**
 * Implementation of 68000 specific syscall routines.
 *
 * NOTE: these routines are not called directly but are instead called through TRAP#13
 */

#include "log.h"
#include "types.h"
#include "syscalls.h"
#include "interrupt.h"
#include "memory.h"
#include "proc.h"
#include "dev/channel.h"
#include "dev/block.h"
#include "dev/fsys.h"
#include "dev/rtc.h"
#include "dev/txt_screen.h"
#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K
#include "dev/kbd_mo.h"
#else
#include "dev/ps2.h"
#endif

/*
 * Determine the correct system function implementation and call it.
 */
unsigned long syscall_dispatch(int32_t function, int32_t param0, int32_t param1, int32_t param2, int32_t param3, int32_t param4, int32_t param5) {
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
                    return 0;

                case KFN_INT_DISABLE:
                    int_disable((unsigned short)param0);
                    return 0;

                case KFN_INT_ENABLE_ALL:
                    return int_enable_all();

                case KFN_INT_DISABLE_ALL:
                    return int_disable_all();

                case KFN_INT_CLEAR:
                    int_clear((unsigned short)param0);
                    return 0;

                case KFN_INT_PENDING:
                    return int_pending((unsigned short)param0);

                case KFN_SYS_GET_INFO:
                    sys_get_information((p_sys_info)param0);
                    return 0;

                default:
                    return ERR_GENERAL;
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
                    return chan_read((short)param0, (unsigned char *)param1, (short)param2);

                case KFN_CHAN_READ_LINE:
                    return chan_readline((short)param0, (unsigned char *)param1, (short)param2);

                case KFN_CHAN_STATUS:
                    return chan_status((short)param0);

                case KFN_CHAN_FLUSH:
                    return chan_flush((short)param0);

                case KFN_CHAN_SEEK:
                    return chan_seek((short)param0, (long)param1, (short)param2);

                case KFN_CHAN_IOCTRL:
                    return chan_ioctrl((short)param0, (short)param1, (unsigned char *)param2, (short)param3);

                case KFN_CHAN_OPEN:
                    return chan_open((short)param0, (const char *)param1, (short)param2);

                case KFN_CHAN_CLOSE:
                    return chan_close((short)param0);

                case KFN_CHAN_REGISTER:
                    return cdev_register((p_dev_chan)param0);

                case KFN_CHAN_SWAP:
                    return chan_swap((short)param0, (short)param1);

                case KFN_CHAN_DEVICE:
                    return chan_device((short)param0);

                default:
                    return ERR_GENERAL;
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

                case KFN_STAT:
                    return fsys_stat((const char *)param0, (p_file_info)param1);

                default:
                    return ERR_GENERAL;
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

                case KFN_LOAD:
                    return fsys_load((char *)param0, (long)param1, (long *)param2);

                case KFN_GET_LABEL:
                    return fsys_getlabel((char *)param0, (char *)param1);

                case KFN_SET_LABEL:
                    return fsys_setlabel((short)param0, (char *)param1);

                case KFN_GET_CWD:
                    return fsys_get_cwd((char *)param0, (short)param1);

                case KFN_SET_CWD:
                    return fsys_set_cwd((char *)param0);

                case KFN_LOAD_REGISTER:
                    return fsys_register_loader((char *)param0, (p_file_loader)param1);

                default:
                    return ERR_GENERAL;
            }

            break;

        case 0x40:
            /* Process and Memory functions */
            switch (function) {
                case KFN_RUN:
                    return proc_run((char *)param0, (int)param1, (char **)param2);

                case KFN_MEM_GET_RAMTOP:
                    return mem_get_ramtop();

                case KFN_MEM_RESERVE:
                    return mem_reserve((unsigned long)param0);

                default:
                    return ERR_GENERAL;
            }
            break;

        case 0x50:
            /* Misc functions */
            switch (function) {
                case KFN_TIME_JIFFIES:
                    return rtc_get_jiffies();

                case KFN_TIME_SETRTC:
                    rtc_set_time((p_time)param0);
                    return 0;

                case KFN_TIME_GETRTC:
                    rtc_get_time((p_time)param0);
                    return 0;

                case KFN_KBD_SCANCODE:
#if MODEL == MODEL_FOENIX_A2560K
                    return kbdmo_get_scancode();
#else
                    return kbd_get_scancode();
#endif

                case KFN_ERR_MESSAGE:
                    return (unsigned long)err_message((short)param0);

                case KFN_KBD_LAYOUT:
#if MODEL == MODEL_FOENIX_A2560K
                    return kbdmo_layout((const char *)param0);
#else
                    return kbd_layout((const char *)param0);
#endif

                default:
                    return ERR_GENERAL;
            }
            break;

        case 0x60:
            /* Text mode operations */
            switch (function) {
                case KFN_TEXT_INIT_SCREEN:
                    /* Reset a screen to its default mode */
                    txt_init_screen((short)param0);
                    return 0;

                case KFN_TXT_GET_CAPS:
                    /* Get the capabilities of a screen */
                    return (unsigned long)txt_get_capabilities((short)param0);

                case KFN_TXT_SET_MODE:
                    /* Set the display mode of a screen */
                    return txt_set_mode((short)param0, (short)param1);

                case KFN_TEXT_SETSIZES:
                    /* Adjusts the screen size based on the current graphics mode */
                    return txt_setsizes((short)param0);

                case KFN_TXT_SET_RESOLUTION:
                    /* Set the base display resolution for a screen */
                    return txt_set_resolution((short)param0, (short)param1, (short)param2);

                case KFN_TXT_SET_BORDER:
                    /* Set the size of the border */
                    txt_set_border((short)param0, (short)param1, (short)param2);
                    return 0;

                case KFN_TXT_SET_BORDERCOLOR:
                    /* Set the border color */
                    txt_set_border_color((short)param0, (unsigned char)param1, (unsigned char)param2, (unsigned char)param3);
                    return 0;

                case KFN_TXT_SET_FONT:
                    /* Set the font for the screen's text mode (if applicable) */
                    return txt_set_font((short)param0, (short)param1, (short)param2, (unsigned char *)param3);

                case KFN_TXT_SET_CURSOR:
                    /* Set the text-mode cursor look */
                    txt_set_cursor((short)param0, (short)param1, (short)param2, (char)param3);
                    return 0;

                case KFN_TXT_SET_REGION:
                    /* Sets the clipping/scrolling region for further text operations */
                    return txt_set_region((short)param0, (p_rect)param1);

                case KFN_TXT_GET_REGION:
                    /* Gets the current clipping/scrolling region */
                    return txt_get_region((short)param0, (p_rect)param1);

                case KFN_TXT_SET_COLOR:
                    /* Sets the foreground and background text colors */
                    return txt_set_color((short)param0, (unsigned char)param1, (unsigned char)param2);

                case KFN_TXT_GET_COLOR:
                    /* Gets the foreground and background text colors */
                    txt_get_color((short)param0, (unsigned char *)param1, (unsigned char *)param2);
                    return 0;

                case KFN_TXT_SET_XY:
                    /* Sets the cursor's position */
                    txt_set_xy((short)param0, (short)param1, (short)param2);
                    return 0;

                case KFN_TXT_GET_XY:
                    /* Gets the cursor's position */
                    txt_get_xy((short)param0, (p_point)param1);
                    return 0;

                case KFN_TXT_SCROLL:
                    /* Scroll the current region */
                    txt_scroll((short)param0, (short)param1, (short)param2);
                    return 0;

                default:
                    return ERR_GENERAL;
            }
            break;

        case 0x70:
            /* Text calls #2 */
            switch (function) {
                case KFN_TXT_SET_CURSOR_VIS:
                    /* Set the cursor visibility */
                    txt_set_cursor_visible((short)param0, (short)param1);
                    break;

                default:
                    return ERR_GENERAL;
            }
            break;

        default:
            break;
    }

    return ERR_GENERAL;
}
