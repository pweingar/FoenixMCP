#include <ctype.h>
#include <string.h>

#include "syscalls.h"
#include "log.h"
#include "cli/dos_cmds.h"
#include "dev/block.h"
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


DIR my_dir;
FILINFO my_file;
FATFS my_fs;

short dos_cmd_dir(short screen, char * path) {
    FRESULT fres;
    char line[255];

    print(screen, "Attempting to read directory for [");
    print(screen, path);
    print(screen, "]\n");

    fres = f_mount(&my_fs, path, 0);
    if (fres == FR_OK) {
        fres = f_opendir(&my_dir, "/");
        if (fres == FR_OK) {
            do {
                fres = f_readdir(&my_dir, &my_file);
                if ((fres == FR_OK) && (my_file.fname[0] != 0)) {
                    if ((my_file.fattrib & AM_HID) == 0) {
                        sys_chan_write(screen, my_file.fname, strlen(my_file.fname));
                        if (my_file.fattrib & AM_DIR) {
                            sys_chan_write_b(screen, '/');
                        }
                        sys_chan_write(screen, "\n", 1);
                    }
                } else {
                    break;
                }
            } while(1);

            f_closedir(&my_dir);
        } else {
            char * err = "Could not open directory: ";
            sys_chan_write(screen, err, strlen(err));
            //print_hex_16(screen, fres);
            sys_chan_write_b(screen, '\n');
        }

        f_mount(0, "", 0);
    } else {
        char * err = "Could not mount drive: ";
        //sys_chan_write(screen, err, strlen(err));
        print_hex_16(screen, fres);
    }

    return 0;
}

/*
 * List the directory at the given path
 */
short cmd_dir(short screen, char * path) {
    return dos_cmd_dir(screen, path);
}
