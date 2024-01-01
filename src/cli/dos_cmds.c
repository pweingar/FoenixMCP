#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "syscalls.h"
#include "log.h"
#include "simpleio.h"
#include "cli.h"
#include "proc.h"
#include "boot.h"
#include "cli/dos_cmds.h"
#include "dev/block.h"
#include "dev/console.h"
#include "dev/fsys.h"
#include "libfoenix/include/kbd_mo.h"
#include "fatfs/ff.h"

/*
 * Read a sector off a drive
 *
 * DISKREAD <drive #> <sector #>
 */
short cmd_diskread(short screen, int argc, const char * argv[]) {
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

    sprintf(buffer, "Reading drive #%d, sector %p\n", bdev_number, (void*)lba);
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
short cmd_diskfill(short screen, int argc, const char * argv[]) {
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

    sprintf(buffer, "Filling drive #%d, sector %p with 0x%02X\n", bdev_number, (void*)lba, (short)value);
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
short cmd_run(short screen, int argc, const char * argv[]) {
    TRACE("cmd_run");

    sys_chan_ioctrl(screen, CON_IOCTRL_ECHO_ON, 0, 0);
    short result = proc_run(argv[0], argc, argv);
    if (result < 0) {
        err_print(screen, "Unable to execute file", result);
        return -1;
    }

    sys_chan_ioctrl(screen, CON_IOCTRL_ECHO_OFF, 0, 0);
    return result;
}

/*
 * Create a directory
 */
short cmd_mkdir(short screen, int argc, const char * argv[]) {

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
short cmd_del(short screen, int argc, const char * argv[]) {

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
short cmd_cd(short screen, int argc, const char * argv[]) {

    TRACE("cmd_cd");

    if (argc > 1) {
        short result = fsys_set_cwd(argv[1]);
        if (result < 0) {
            err_print(screen, "Unable to change directory", result);
            return result;
        } else {
            cli_flag_cwd();
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
short cmd_pwd(short screen, int argc, const char * argv[]) {
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
short cmd_rename(short screen, int argc, const char * argv[]) {

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

/**
 * Structure to hold file and directory information for sorting
 */
typedef struct s_dir_entry {
    t_file_info info;
    struct s_dir_entry *next, *prev;
} t_dir_entry, *p_dir_entry;

/**
 * Add a directory entry to a list of directory entries using simple insertion sort
 *
 * @param list_pointer a pointer to a pointer to a list of directory entries
 * @param entry a pointer to a directory entry to add
 */
void dir_entry_insert(p_dir_entry * list_pointer, p_dir_entry entry) {
    p_dir_entry list = *list_pointer;

    if (list == 0) {
        *list_pointer = entry;
        entry->next = 0;
        entry->prev = 0;

    } else {
        p_dir_entry x = list;
        while (x != 0) {
            if (strcmp(x->info.name, entry->info.name) >= 0) {
                if (x->prev == 0) {
                    *list_pointer = entry;
                    entry->next = x;
                    entry->prev = 0;
                    x->prev = entry;
                } else {
                    entry->prev = x->prev;
                    entry->next = x;
                    x->prev->next = entry;
                    x->prev = entry;
                }
                return;

            } else {
                if (x->next == 0) {
                    x->next = entry;
                    entry->prev = x;
                    entry->next = 0;
                    return;
                } else {
                    x = x->next;
                }
            }
        }
    }
}

/**
 * Examine the optional path/pattern argument and separate it into a path and a pattern
 *
 * @param arg the string describing the directory and pattern to search
 * @param path a pointer to a pointer to a string... set to the path portion or 0 if no path
 * @param pattern a pointer to a pointer to a string... set to the pattern poriton, or 0 if none provided
 */
void dir_parse_pattern(char * arg, char ** path, char ** pattern) {
    *path = 0;
    *pattern = 0;
    if ((arg != 0) && (strlen(arg) > 0)) {
        // And argument was provided... see if there is a pattern
        if ((strchr(arg, '*') == 0) && (strchr(arg, '?') == 0)) {
            // No pattern found... return the whole thing as path
            *path = arg;

        } else {
            // Found a pattern... do we specify a folder?
            char * x = strrchr(arg, '/');
            if (x == 0) {
                // No... it's all pattern
                *pattern = arg;

            } else {
                // Yes... split the string into path and pattern at x
                *x = 0;
                *path = arg;
                *pattern = x + 1;
            }
        }
    }
}

short cmd_dir(short screen, int argc, const char * argv[]) {
    short result = 0, dir = -1;
    char buffer[80];
    char arg[128];
    t_file_info my_file;
    p_dir_entry directories = 0, files = 0, entry = 0, prev = 0;
    char *path=0, *pattern = 0;
    char label[40];

    if (argc > 1) {
        strcpy(arg, argv[1]);
        dir_parse_pattern(arg, &path, &pattern);
    }

    if (path == 0) {
        // Make the path the empty string if it was not provided
        path = "";
    }

    if (pattern != 0) {
        // A pattern was provided
        dir = sys_fsys_findfirst(path, pattern, &my_file);

    } else {
        // No pattern... just open the path as a directory
        dir = sys_fsys_opendir(path);
    }

    if (dir >= 0) {
        result = fsys_getlabel(path, label);
        if ((result == 0) && (strlen(label) > 0)) {
            sprintf(buffer, "Drive: %s\n", label);
            chan_write(screen, buffer, strlen(buffer));
        }

        while (1) {
            if (pattern == 0) {
                result = sys_fsys_readdir(dir, &my_file);
                if (result != 0) break;
            }

            if (my_file.name[0] != 0) {
                if ((my_file.attributes & AM_HID) == 0) {
                    if (my_file.attributes & AM_DIR) {
                        entry = (p_dir_entry)malloc(sizeof(t_dir_entry));
                        if (entry) {
                            memcpy(&entry->info, &my_file, sizeof(t_file_info));
                            dir_entry_insert(&directories, entry);
                        } else {
                            print(screen, "Unable to display directory... out of memory.\n");
                            return -1;
                        }

                    } else {
                        entry = (p_dir_entry)malloc(sizeof(t_dir_entry));
                        if (entry) {
                            memcpy(&entry->info, &my_file, sizeof(t_file_info));
                            dir_entry_insert(&files, entry);
                        } else {
                            print(screen, "Unable to display directory... out of memory.\n");
                            return -1;
                        }
                    }
                }
            } else {
                break;
            }

            if (pattern != 0) {
                result = fsys_findnext(dir, &my_file);
                if (result != 0) {
                    break;
                }
            }
        }

        fsys_closedir(dir);

        // Print the directories
        entry = directories;
        while (entry != 0) {
            sprintf(buffer, "%s/\n", entry->info.name);
            print(screen, buffer);
            prev = entry;
            entry = entry->next;
            free(prev);
        }

        // Print the files
        entry = files;
        while (entry != 0) {
            if (entry->info.size < 1024) {
                sprintf(buffer, "%-20.20s %d B\n", entry->info.name, (int)entry->info.size);
            } else if (my_file.size < 1024*1024) {
                sprintf(buffer, "%-20.20s %d KB\n", entry->info.name, (int)entry->info.size / 1024);
            } else {
                sprintf(buffer, "%-29.20s %d MB\n", entry->info.name, (int)entry->info.size / (1024*1024));
            }
            print(screen, buffer);
            prev = entry;
            entry = entry->next;
            free(prev);
        }

    } else {
        err_print(screen, "Unable to open directory", dir);
        return dir;
    }

    return 0;
}

/*
 * Print the contents of a file to the screen
 */
short cmd_type(short screen, int argc, const char * argv[]) {
    if (argc > 1) {
        unsigned char buffer[128];

        log3(LOG_INFO, "Attempting to type [", (char*)(argv[1]), "]");
        short fd = fsys_open(argv[1], FA_READ);
        if (fd >= 0) {
            log_num(LOG_INFO, "File open: ", fd);
            while (1) {
                short n = chan_read(fd, buffer, 128);
                log_num(LOG_INFO, "cmd_type chan_read: ", n);
                if (n > 0) {
                    logmsg(LOG_INFO, "cmd_type chan_write: ");
                    chan_write(screen, buffer, n);
                    logmsg(LOG_INFO, "/cmd_type chan_write: ");
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
        logmsg(LOG_ERROR, "Usage: TYPE <path>");
        return -1;
    }
}

/*
 * Load a binary file into memory
 * parameters: path [address]
 */
short cmd_load(short screen, int argc, const char * argv[]) {
    if (argc > 1) {
        long start = 0;
        long destination = 0;

        if (argc > 2) {
            destination = cli_eval_number(argv[2]);
        }

        short result = fsys_load(argv[1], destination, &start);
        if (result == 0) {
            if (start != 0) {
                logmsg(LOG_INFO, "Loaded file with a start adddress.");
            } else {
                logmsg(LOG_INFO, "File loaded.");
            }
        } else {
            err_print(screen, "Unable to open file", result);
            return -1;
        }

        return result;
    } else {
        logmsg(LOG_ERROR, "Usage: LOAD <path> [<destination>]");
        return -1;
    }
}

/*
 * Set the label of a drive
 *
 * LABEL <drive #> <label>
 *
 */
short cmd_label(short screen, int argc, const char * argv[]) {
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

/**
 * Command to make a device bootable by writing to the MBR or VBR
 *
 * MKBOOT <drive #> -r --- removes boot record
 * MKBOOT <drive #> -b <boot record path> --- installs boot record
 * MKBOOT <drive #> -s <start file path> --- defines a startup file
 */
short cmd_mkboot(short screen, int argc, const char * argv[]) {
    const char * usage = "USAGE: MKBOOT <drive #> -r\n       MKBOOT <drive #> -b <boot record path>\n       MKBOOT <drive #> -s <start file path>\n";
    short mode = 0;
    unsigned char * boot_sector = 0;
    unsigned char * new_boot_sector = 0;
    char message[80];
    short dev = 0;
    short i = 0;
    short result = 0;

    // Parse the inputs...
    if (argc == 3) {
        // Must be -r
        if (strcmp("-r", argv[2]) == 0) {
            mode = 0;
            dev = cli_eval_number(argv[1]);
        } else {
            print(screen, usage);
            return -1;
        }

    } else if (argc == 4) {
        // Can be either -b or -s
        dev = cli_eval_number(argv[1]);

        if (strcmp("-b", argv[2]) == 0) {
            // -b
            mode = 1;

        } else if (strcmp("-s", argv[2]) == 0) {
            // -s
            mode = 2;

        } else {
            print(screen, usage);
            return -1;
        }

    } else {
        // Bad arguments...
        print(screen, usage);
        return -1;
    }

    switch (mode) {
        case 0:
            // Clear out the boot record
            result = boot_non_booting(dev);
            if (result != 0) {
                sprintf(message, "Could not change boot record: %s\n", err_message(result));
                print(screen, message);
            } else {
                print(screen, "Boot record updated.\n");
            }
            break;

        case 2:
            // Write a boot sector that loads and runs a file
             result = boot_set_file(dev, argv[3]);
             if (result != 0) {
                 sprintf(message, "Could not change boot record: %s\n", err_message(result));
                 print(screen, message);
             } else {
                 print(screen, "Boot record updated.\n");
             }
             break;

        default:
            print(screen, "Unknown MKBOOT operation.\n");
            result = -1;
            break;
    }

    return result;
}

/*
 * Format a drive
 *
 * FORMAT <drive #>
 */
short cmd_format(short screen, int argc, const char * argv[]) {
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
