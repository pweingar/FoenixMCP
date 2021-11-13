#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "syscalls.h"
#include "log.h"
#include "simpleio.h"
#include "cli.h"
#include "proc.h"
#include "cli/dos_cmds.h"
#include "dev/block.h"
#include "dev/fsys.h"
#include "dev/kbd_mo.h"
#include "fatfs/ff.h"

/*
 * Read a sector off a drive
 *
 * DISKREAD <drive #> <sector #>
 */
short cmd_diskread(short screen, int argc, char * argv[]) {
    unsigned char buffer[512];
    short bdev_number = 0;
    long lba = 0;
    short result;
    short i;

    if (argc < 3) {
        print(screen, "USAGE: DISKREAD <drive #> <sector #>\n");
        return -1;
    }

    bdev_number = (short)cli_eval_number(argv[1]);
    lba = cli_eval_number(argv[2]);

    sprintf(buffer, "Reading drive #%d, sector 0x%X\n", bdev_number, lba);
    print(screen, buffer);

    result = bdev_read(bdev_number, lba, buffer, 512);
    if (result < 512) {
        err_print(screen, "Unable to read sector", result);
        return -2;
    }

    dump_buffer(screen, buffer, 512, 1);

    return 0;
}

/*
 * Fill a sector of a drive with a byte value
 *
 * DISKFILL <drive #> <sector #> <value>
 */
short cmd_diskfill(short screen, int argc, char * argv[]) {
    unsigned char buffer[512];
    unsigned char value;
    short bdev_number = 0;
    long lba = 0;
    short result;
    short i;

    if (argc < 4) {
        print(screen, "USAGE: DISKFILL <drive #> <sector #> <byte value>\n");
        return -1;
    }

    bdev_number = (short)cli_eval_number(argv[1]);
    lba = cli_eval_number(argv[2]);
    value = (unsigned char)cli_eval_number(argv[3]);

    sprintf(buffer, "Filling drive #%d, sector 0x%X with 0x%02X\n", bdev_number, lba, value);
    print(screen, buffer);

    for (i = 0; i < 512; i++) {
        buffer[i] = value;
    }

    result = bdev_write(bdev_number, lba, buffer, 512);
    if (result < 512) {
        err_print(screen, "Unable to write sector", result);
        return -2;
    }

    /* Read the sector back for verification */
    return cmd_diskread(screen, argc, argv);
}


/*
 * Try to run a command from storage.
 *
 * Command name is in argv[0].
 */
short cmd_run(short screen, int argc, char * argv[]) {
    TRACE("cmd_run");

    short result = proc_run(argv[0], argc, argv);
    if (result < 0) {
        err_print(screen, "Unable to execute file", result);
        return -1;
    }

    return result;
}

/*
 * Create a directory
 */
short cmd_mkdir(short screen, int argc, char * argv[]) {

    TRACE("cmd_mkdir");

    if (argc > 1) {
        short result = fsys_mkdir(argv[1]);
        if (result < 0) {
            err_print(screen, "Unable to create directory", result);
            return result;
        }
    } else {
        print(screen, "USAGE: MKDIR <path>\n");
        return -1;
    }
}

/*
 * Delete a file
 */
short cmd_del(short screen, int argc, char * argv[]) {

    TRACE("cmd_del");

    if (argc > 1) {
        short result = fsys_delete(argv[1]);
        if (result < 0) {
            err_print(screen, "Unable to delete file", result);
            return result;
        }
    } else {
        print(screen, "USAGE: DEL <path>\n");
        return -1;
    }
}

/*
 * Change the directory
 */
short cmd_cd(short screen, int argc, char * argv[]) {

    TRACE("cmd_cd");

    if (argc > 1) {
        short result = fsys_set_cwd(argv[1]);
        if (result < 0) {
            err_print(screen, "Unable to change directory", result);
            return result;
        } else {
            print(screen, "Changed to: ");
            print(screen, argv[1]);
            print(screen, "\n");
        }
    } else {
        print(screen, "USAGE: CD <path>\n");
        return -1;
    }

    return 0;
}

/*
 * Change the directory
 */
short cmd_pwd(short screen, int argc, char * argv[]) {
    char buffer[128];

    TRACE("cmd_pwd");

    short result = fsys_get_cwd(buffer, 128);
    if (result < 0) {
        err_print(screen, "Unable to get current directory", result);
        return result;
    } else {
        print(screen, buffer);
        return 0;
    }
}

/*
 * Rename a file or directory
 */
short cmd_rename(short screen, int argc, char * argv[]) {

    TRACE("cmd_rename");

    if (argc > 2) {
        short result = fsys_rename(argv[1], argv[2]);
        if (result < 0) {
            err_print(screen, "Unable to rename file", result);
            return result;
        }
    }

    return 0;
}

short cmd_dir(short screen, int argc, char * argv[]) {
    short result;
    char buffer[80];
    t_file_info my_file;
    char * path = "";
    char label[40];

    if (argc > 1) {
        path = argv[1];
    }

    short dir = fsys_opendir(path);
    if (dir >= 0) {
        result = fsys_getlabel(path, label);
        if ((result == 0) && (strlen(label) > 0)) {
            sprintf(buffer, "Drive: %s\n", label);
            chan_write(screen, buffer, strlen(buffer));
        }

        while (1) {
            short result = fsys_readdir(dir, &my_file);
            if ((result == 0) && (my_file.name[0] != 0)) {

                if ((my_file.attributes & AM_HID) == 0) {
                    if (my_file.attributes & AM_DIR) {
                        sprintf(buffer, "%s/\n", my_file.name);
                        chan_write(screen, buffer, strlen(buffer));

                    } else {
                        if (my_file.size < 1024) {
                            sprintf(buffer, "%-20.20s %d\n", my_file.name, (int)my_file.size);
                        } else if (my_file.size < 1024*1024) {
                            sprintf(buffer, "%-20.20s %d KB\n", my_file.name, (int)my_file.size / 1024);
                        } else {
                            sprintf(buffer, "%-29.20s %d MB\n", my_file.name, (int)my_file.size / (1024*1024));
                        }
                        chan_write(screen, buffer, strlen(buffer));
                    }
                }
            } else {
                break;
            }
        }

        fsys_closedir(dir);
    } else {
        err_print(screen, "Unable to open directory", dir);
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
            log_num(LOG_INFO, "File open: ", fd);
            while (1) {
                short n = chan_read(fd, buffer, 128);
                log_num(LOG_INFO, "cmd_type chan_read: ", n);
                if (n > 0) {
                    log(LOG_INFO, "cmd_type chan_write: ");
                    chan_write(screen, buffer, n);
                    log(LOG_INFO, "/cmd_type chan_write: ");
                } else {
                    break;
                }
            }

            fsys_close(fd);
            return 0;

        } else {
            err_print(screen, "Unable to open file", fd);
            return fd;
        }
    } else {
        log(LOG_ERROR, "Usage: TYPE <path>");
        return -1;
    }
}

/*
 * Load a binary file into memory
 * parameters: path [address]
 */
short cmd_load(short screen, int argc, char * argv[]) {
    if (argc > 1) {
        long start = 0;
        long destination = 0;

        if (argc > 2) {
            destination = cli_eval_number(argv[2]);
        }

        short result = fsys_load(argv[1], destination, &start);
        if (result == 0) {
            if (start != 0) {
                log(LOG_INFO, "Loaded file with a start adddress.");
            } else {
                log(LOG_INFO, "File loaded.");
            }
        } else {
            err_print(screen, "Unable to open file", result);
            return -1;
        }

        return result;
    } else {
        log(LOG_ERROR, "Usage: LOAD <path> [<destination>]");
        return -1;
    }
}

/*
 * Set the label of a drive
 *
 * LABEL <drive #> <label>
 *
 */
short cmd_label(short screen, int argc, char * argv[]) {
    if (argc > 2) {
        short drive = cli_eval_number(argv[1]);
        short result = fsys_setlabel(drive, argv[2]);
        if (result != 0) {
            err_print(screen, "Unable to set volume label", result);
            return -1;
        }

    } else {
        print(screen, "USAGE: LABEL <drive #> <label>\n");
        return -1;
    }
}

/*
 * Format a drive
 *
 * FORMAT <drive #>
 */
short cmd_format(short screen, int argc, char * argv[]) {
    if (argc > 1) {
        short drive = cli_eval_number(argv[1]);
        short result = fsys_mkfs(drive, "");
        if (result != 0) {
            err_print(screen, "Unable to format volume", result);
            return -1;
        }

    } else {
        print(screen, "USAGE: FORMAT <drive #>\n");
        return -1;
    }
}
