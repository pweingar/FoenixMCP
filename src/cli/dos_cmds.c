#include <ctype.h>
#include <string.h>

#include "syscalls.h"
#include "log.h"
#include "simpleio.h"
#include "cli/dos_cmds.h"
#include "dev/block.h"
#include "dev/fsys.h"
#include "fatfs/ff.h"

/*
 * Fetch and display the MBR for the specified device:
 *
 * Inputs:
 * drive = "@S" for SDC, "@H" for hard drive
 */
short cmd_dump_mbr(short screen, char * drive) {

    return 0;
}

short cmd_dir(short screen, int argc, char * argv[]) {
    t_file_info my_file;
    char * path = "";

    log_num(LOG_INFO, "cmd_dir: ", argc);

    if (argc > 1) {
        path = argv[1];
    }

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

/*
 * Print the contents of a file to the screen
 */
short cmd_type(short screen, int argc, char * argv[]) {
    if (argc > 1) {
        unsigned char buffer[128];

        log3(LOG_INFO, "Attempting to type [", argv[1], "]");
        short fd = fsys_open(argv[1], FA_READ);
        if (fd >= 0) {
            log(LOG_INFO, "File open");
            while (1) {
                short n = chan_read(fd, buffer, 128);
                log_num(LOG_INFO, "chan_read: ", n);
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
    } else {
        log(LOG_ERROR, "Usage: TYPE <path>");
    }
}

/*
 * Load a binary file into memory
 * parameters: path [address]
 */
short cmd_load(short screen, int argc, char * argv[]) {
    if (argc > 1) {
        long start = 0;
        short result = fsys_load(argv[1], 0x30000, &start);
        if (result == 0) {
            if (start != 0) {
                log(LOG_INFO, "Loaded file with a start adddress.");
            } else {
                log(LOG_INFO, "File loaded.");
            }
        } else {
            log_num(LOG_ERROR, "Unable to open file: ", result);
        }

        return result;
    } else {
        log(LOG_ERROR, "Usage: LOAD <path>");
    }
}
