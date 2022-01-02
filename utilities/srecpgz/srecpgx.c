/*
 * A simple utility to convert Motorola SREC to the Foenix PGZ file format
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER 128

enum {
    STAT_ADDRESS_OVERFLOW = -3, /* The address field was too big for 24-bit format */
    STAT_COUNT_OVERFLOW = -2,   /* The count field was too big for 24-bit format */
    STAT_GOOD = 0,              /* The record is good and may be used */
    STAT_NO_RECORD,             /* The line parsed contained no record or an ignored record type */
    STAT_EOF,                   /* There is no more data in the file */
    STAT_BAD_ADDRESS,           /* There was a parse error in the address field */
    STAT_BAD_DATA,              /* There was a parse error in the data field */
    STAT_BAD_CHKSUM,            /* There was a parse error in the checksum field */
    STAT_MISMATCH_CHKSUM,       /* The checksum does not match the line */
    STAT_ERROR                  /* General file read error */
};

/*
 * Structure to contain a record from the SREC file
 */
typedef struct s_srecord {
    short status;
    short type;
    short count;
    short binary_count;
    long address;
    char data[MAX_BUFFER];
    short checksum;
} t_srecord, *p_srecord;

/* Size of the address and count fields: 0 = 24-bit, 1 = 32-bit */
short use_32bits = 0;

/*
 * Convert a hex digit to a binary number
 */
short hex_convert(char c) {
    if ((c >= '0') && (c <= '9')) {
        return (c - '0');
    } else if ((c >= 'a') && (c <= 'f')) {
        return (c - 'a' + 10);
    } else if ((c >= 'A') && (c <= 'F')) {
        return (c - 'A' + 10);
    } else {
        /* There was a problem... return -1 as the result */
        return -1;
    }
}

/*
 * Convert a string of hex digits to a binary number
 *
 * Inputs:
 * raw = the source string
 * count = the number of characters to parse
 *
 * Returns:
 * the binary number read, -1 if there was an error
 */
long parse_hex(char * raw, short count) {
    int i;
    long result = 0;

    for (i = 0; i < count; i++) {
        short n = hex_convert(raw[i]);
        if (n == -1) {
            return -1;
        } else {
            result = result * 16 + n;
        }
    }
    return result;
}

/*
 * Parse the data field
 *
 * Inputs:
 * raw = the source string of hex digits (must be the start of the data field)
 * data = the buffer to fill with the binary data
 * count = the number of chars to put into data
 */
short parse_data(char * raw, char * data, short count) {
    int i;
    for (i = 0; i < count; i++) {
        char * digits = &raw[2*i];
        long n = parse_hex(digits, 2);
        if (n == -1) {
            return -1;
        } else {
            data[i] = (char)(n & 0xff);
        }
    }

    return 0;
}

/*
 * Read a record from the input stream and fill it out
 *
 * Inputs:
 * fd = FILE pointer for the SREC file to read
 * r = pointer to the record structure to fill out
 */
void read_record(FILE * fd, p_srecord r) {
    char buffer[MAX_BUFFER];
    short n;
    int i;

    r->status = STAT_NO_RECORD;
    char * result = fgets(buffer, MAX_BUFFER, fd);
    if (result == 0) {;
        r->status = STAT_EOF;

    } else if (strlen(buffer) >= 10) {
        if ((buffer[0] == 'S') && isdigit(buffer[1]) && (buffer[1] >= '0') && (buffer[1] <= '9')) {
            r->status = STAT_GOOD;
            r->type = buffer[1] - '0';
            r->count = (short)parse_hex(&buffer[2], 2);
            r->checksum = (short)parse_hex(&buffer[strlen(buffer)-2], 2);

            for (i = 0; i < MAX_BUFFER; i++) {
                r->data[i] = 0;
            }

            switch (r->type) {
                case 1:
                    /* Data record, 16-bit address */
                    r->binary_count = r->count - 3;
                    r->address = parse_hex(&buffer[4], 4);
                    n = parse_data(&buffer[8], r->data, r->binary_count);
                    if (n == -1) {
                        r->status = STAT_BAD_DATA;
                    }
                    break;

                case 2:
                    /* Data record, 24-bit address */
                    r->binary_count = r->count - 4;
                    r->address = parse_hex(&buffer[4], 6);
                    n = parse_data(&buffer[10], r->data, r->binary_count);
                    if (n == -1) {
                        r->status = STAT_BAD_DATA;
                    }
                    break;

                case 3:
                    /* Data record, 32-bit address */
                    r->binary_count = r->count - 5;
                    r->address = parse_hex(&buffer[4], 8);
                    n = parse_data(&buffer[12], r->data, r->binary_count);
                    if (n == -1) {
                        r->status = STAT_BAD_DATA;
                    }
                    break;

                case 7:
                    /* Start address, 32-bit */
                    r->address = parse_hex(&buffer[4], 8);
                    r->binary_count = 0;
                    break;

                case 8:
                    /* Start address, 24-bit */
                    r->address = parse_hex(&buffer[4], 6);
                    r->binary_count = 0;
                    break;

                case 9:
                    /* Start address, 16-bit */
                    r->address = parse_hex(&buffer[4], 4);
                    r->binary_count = 0;
                    break;

                default:
                    /* Unsupported type, return null */
                    r->status = STAT_NO_RECORD;
                    break;
            }
        }
    }
}

/*
 * Format the record for binary output
 *
 * Inputs:
 * out = FILE pointer for the output binary file
 * r = record to be output
 */
short write_record(FILE * out, p_srecord r) {
    int i;
    char buffer[MAX_BUFFER];
    short count_idx;                /* offset to count field in binary output */
    short data_idx;                 /* offset to data field in binary output */

    fprintf(stderr, "{type=%d, addr=%08x, count=%08x}\n", r->type, (int)r->address, (int)r->binary_count);

    if (use_32bits) {
        count_idx = 4;
        data_idx = 8;
    } else {
        count_idx = 3;
        data_idx = 6;
    }

    /* Clear out the buffer */
    for (i = 0; i < MAX_BUFFER; i++) {
        buffer[i] = 0;
    }

    /* Set the address of the record */
    if (use_32bits) {
        buffer[3] = (char)((r->address >> 24) & 0xff);
    } else if ((r->address >> 24) != 0) {
        return STAT_ADDRESS_OVERFLOW;
    }
    buffer[2] = (char)((r->address >> 16) & 0xff);
    buffer[1] = (char)((r->address >> 8) & 0xff);
    buffer[0] = (char)((r->address) & 0xff);

    /* Set the count of the record */
    if (use_32bits) {
        buffer[count_idx+3] = (char)((r->binary_count >> 24) & 0xff);
    } else if ((r->binary_count >> 24) != 0) {
        return STAT_COUNT_OVERFLOW;
    }
    buffer[count_idx+2] = (char)((r->binary_count >> 16) & 0xff);
    buffer[count_idx+1] = (char)((r->binary_count >> 8) & 0xff);
    buffer[count_idx] = (char)((r->binary_count) & 0xff);

    /* Fill out the data */
    for (i = 0; i < r->binary_count; i++) {
        buffer[data_idx+i] = r->data[i];
    }

    int result = write(fileno(out), buffer, r->binary_count + data_idx);
    if (result == -1) {
        perror("Error writing file");
        return -1;
    }

    return 0;
}

int main(int argc, char * argv[]) {
    FILE * in_file;
    FILE * out_file;
    short in_file_arg;
    short out_file_arg;
    char signature[1];
    t_srecord r;
    int line_number = 0;
    int keep_going = 1;
    short n;

    switch (argc) {
        case 3:
            in_file_arg = 1;
            out_file_arg = 2;
            use_32bits = 0;
            break;

        case 4:
            if (strcmp(argv[1], "--large") == 0) {
                in_file_arg = 2;
                out_file_arg = 3;
                use_32bits = 1;
            } else {
                fprintf(stderr, "Usage: srecpgx [--large] <inputfile> <outputfile>\n");
                exit(5);
            }
            break;

        default:
            fprintf(stderr, "Usage: srecpgx [--large] <inputfile> <outputfile>\n");
            exit(5);
    }

    in_file = fopen(argv[in_file_arg], "r");
    if (!in_file) {
        fprintf(stderr, "Could not open input file (%d).\n", errno);
        exit(6);
    }

    out_file = fopen(argv[out_file_arg], "w");
    if (!out_file) {
        perror("Could not open output file");
        exit(7);
    }

    /* Write the signature... use a lower case 'z' to distinguish */
    if (use_32bits) {
        signature[0] = 'z';
    } else {
        signature[0] = 'Z';
    }
    if (write(fileno(out_file), signature, 1) == -1) {
        perror("Error writing the output file");
        exit(9);
    }

    while (keep_going) {
        read_record(in_file, &r);
        line_number++;
        switch (r.status) {
            case STAT_GOOD:
                n = write_record(out_file, &r);
                if (n == -1) {
                    fprintf(stderr, "Error writing the output file on line %d", line_number);
                    exit(10);
                } else if (n == STAT_ADDRESS_OVERFLOW) {
                    fprintf(stderr, "Address too big for 24-bit binary on line %d", line_number);
                    exit(11);
                } else if (n == STAT_COUNT_OVERFLOW) {
                    fprintf(stderr, "Count too big for 24-bit binary on line %d", line_number);
                    exit(12);
                }
                break;

            case STAT_ERROR:
                /* There was an error reading the file */
                fprintf(stderr, "Error reading the input file on line %d.\n", line_number);
                exit(8);

            case STAT_EOF:
                /* End of file... stop looping, close files, and quit */
                fprintf(stderr, "d");
                keep_going = 0;
                break;

            case STAT_BAD_ADDRESS:
                fprintf(stderr, "Bad address on line %d.\n", line_number);
                exit(1);

            case STAT_BAD_DATA:
                fprintf(stderr, "Bad data on line %d.\n", line_number);
                exit(2);

            case STAT_BAD_CHKSUM:
                fprintf(stderr, "Bad checksum on line %d.\n", line_number);
                exit(3);

            case STAT_MISMATCH_CHKSUM:
                fprintf(stderr, "Checksum did not match on line %d.\n", line_number);
                exit(4);

            default:
                break;
        }
    }

    fclose(in_file);
    fclose(out_file);
    return 0;
}
