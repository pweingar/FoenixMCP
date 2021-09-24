#include <ctype.h>
#include <string.h>

#include "syscalls.h"
#include "log.h"
#include "simpleio.h"
#include "cli/dos_cmds.h"
#include "dev/block.h"
#include "dev/fsys.h"
#include "fatfs/ff.h"

void test_get_mbr(short screen, short device) {
    short x;
    short result;
    char buffer[512];

    for (x = 0; x < 512; x++) {
        buffer[x] = 0;
    }

    bdev_init(device);

    print(1, "Master Boot Record:\n");
    result = bdev_read(device, 0, buffer, 512);
    if (result > 0) {
        for (x = 0; x < result; x++) {
            if (x % 16 == 0) {
                print(screen, "\n");
            }
            print_hex(screen, buffer[x]);
            print(screen, " ");
        }
        print(screen, "\n");
    } else if (result < 0) {
        DEBUG("Drive returned an error.");
    } else {
        DEBUG("Drive returned nothing.");
    }
}

/*
 * Fetch and display the MBR for the specified device:
 *
 * Inputs:
 * drive = "@S" for SDC, "@H" for hard drive
 */
short cmd_dump_mbr(short screen, char * drive) {
    int i;
    for (i = 0; i < strlen(drive); i++) {
        if (islower(drive[i])) {
            drive[i] = toupper(drive[i]);
        }
    }

    if (strcmp(drive, "@S:") == 0) {
        test_get_mbr(screen, BDEV_SDC);

    } else if (strcmp(drive, "@H:") == 0) {
        test_get_mbr(screen, BDEV_HDC);

    } else {
        const char * bad_spec = "Error: Unknown specification.";
        sys_chan_write(screen, bad_spec, strlen(bad_spec));
        return -1;
    }
    return 0;
}

short dos_cmd_dir(short screen, char * path) {
    t_file_info my_file;

    log3(LOG_INFO, "Attempting to read directory for [", path, "]\n");
    short dir = fsys_opendir(path);
    if (dir >= 0) {
        while (1) {
            short result = fsys_readdir(dir, &my_file);
            if ((result == 0) && (my_file.name[0] != 0)) {
                if ((my_file.attributes & AM_HID) == 0) {
                    print(screen, my_file.name);
                    if (my_file.attributes & AM_DIR) {
                        print(screen, "/");
                    }
                    print(screen, "\n");
                }
            } else {
                break;
            }
        }

        fsys_closedir(dir);
    } else {
        log_num(LOG_ERROR, "Could not open directory: ", dir);
        return dir;
    }

    return 0;
}

short cmd_type(short screen, char * path) {
    unsigned char buffer[128];

    log3(LOG_INFO, "Attempting to type [", path, "]");
    short fd = fsys_open(path, FA_READ);
    if (fd >= 0) {
        log(LOG_INFO, "File open");
        while (1) {
            short n = chan_read(fd, buffer, 128);
            log_num(LOG_INFO, "chan_read: " n);
            if (n > 0) {
                chan_write(screen, buffer, n);
            } else {
                break;
            }
        }

    } else {
        log_num(LOG_ERROR, "Could not open file for reading: ", fd);
        return fd;
    }
}

/*
 * List the directory at the given path
 */
short cmd_dir(short screen, char * path) {
    return dos_cmd_dir(screen, path);
}
