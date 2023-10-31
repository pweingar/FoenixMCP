/*
 * Definitions of types used by the kernel
 */

#ifndef __MCP_TYPES_H
#define __MCP_TYPES_H

#include <stdint.h>

/*
 * Function types
 */

/*
 * Integer types in their standard sizes, signed and unsigned.
 */

typedef unsigned char bool;

//
// A color (BGR)
//
typedef struct s_color3 {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} t_color3;

//
// A color entry for a color lookup table (BGRA)
//
typedef struct s_color4 {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} t_color4;

/*
 * Function types
 */

typedef short (*FUNC_V_2_S)();
typedef short (*FUNC_S_2_S)(char *);
typedef short (*FUNC_BS_2_S)(unsigned char *, short);
typedef short (*FUNC_cBS_2_S)(const unsigned char *, short);
typedef short (*FUNC_B_2_S)(const unsigned short);
typedef short (*FUNC_LBS_2_S)(long, unsigned char *, short);
typedef short (*FUNC_LcBS_2_S)(long, const unsigned char *, short);
typedef short (*FUNC_SBS_2_S)(short, unsigned char *, short);
typedef short (*FUNC_LB_2_S)(long, short);

/*=========================================
 * dev/channel
 *========================================*/

/*
 * Preset channel device numbers
 */

#define CDEV_DEVICES_MAX    8       // The maximum number of channel devices we will support
#define CHAN_MAX            16      // The maximum number of open channels we will support
#define CHAN_DATA_SIZE      32      // The number of bytes in the channel's data area

#define CDEV_CONSOLE 0
#define CDEV_EVID 1
#define CDEV_COM1 2
#define CDEV_COM2 3
#define CDEV_LPT 4
#define CDEV_MIDI 5
#define CDEV_FILE 6

/*
 * Channel status bits
 */

#define CDEV_STAT_EOF       0x01    // The channel has reached the end of the data
#define CDEV_STAT_ERROR     0x02    // The channel has encountered some error
#define CDEV_STAT_READABLE  0x04    // The channel has data to read (read will not block)
#define CDEV_STAT_WRITABLE  0x08    // The channel can accept data (write will not block)

#define CDEV_SEEK_ABSOLUTE  0
#define CDEV_SEEK_RELATIVE  1

/*
 * Structure defining a channel
 */

typedef struct s_channel {
    short number;                   // The number of the channel
    short dev;                      // The number of the channel's device
    uint8_t data[CHAN_DATA_SIZE];   // A block of state data that the channel code can use for its own purposes
} t_channel, *p_channel;

typedef short (*FUNC_CBS_2_S)(p_channel, uint8_t *, short);
typedef short (*FUNC_C_2_S)(p_channel);
typedef short (*FUNC_CcBS_2_S)(p_channel, const uint8_t *, short);
typedef short (*FUNC_CB_2_S)(p_channel, uint8_t);
typedef short (*FUNC_CLS_2_S)(p_channel, long, short);
typedef short (*FUNC_CSBS_2_S)(p_channel, short, uint8_t *, short);

/*
 * Structure defining a channel device's functions
 */

typedef struct s_dev_chan {
    short number;           // The number of the device (assigned by registration)
    char * name;            // The name of the device
    FUNC_V_2_S init;        // short init() -- Initialize the device
    FUNC_CBS_2_S open;      // short open(t_channel * chan, uint8_t * path, short mode) -- open a channel for the device
    FUNC_V_2_S close;       // short close(t_channel * chan) -- called when a channel is closed
    FUNC_CBS_2_S read;      // short read(t_channel *, uint8_t * buffer, short size) -- Read a a buffer from the device
    FUNC_CBS_2_S readline;  // short readline(t_channel *, uint8_t * buffer, short size) -- Read a line of text from the device
    FUNC_C_2_S read_b;      // short read_b(t_channel *) -- read a single uint8_t from the device
    FUNC_CcBS_2_S write;    // short write(t_channel *, uint8_t * buffer, short size) -- Write a buffer to the device
    FUNC_CB_2_S write_b;    // short write_b(t_channel *, const uint8_t b) -- Write a single uint8_t to the device
    FUNC_C_2_S status;      // short status(t_channel *) -- Get the status of the device
    FUNC_C_2_S flush;       // short flush(t_channel *) -- Ensure that any pending writes to teh device have been completed
    FUNC_CLS_2_S seek;      // short cdev_seek(t_channel *, long position, short base) -- attempt to move the "cursor" position in the channel
    FUNC_CSBS_2_S ioctrl;   // short ioctrl(t_channel *, short command, uint8_t * buffer, short size)) -- Issue a control command to the device
} t_dev_chan, *p_dev_chan;

/*=========================================
 * dev/blocks
 *========================================*/

//
// Preset block device numbers
//

#define BDEV_DEVICES_MAX 8

#define BDEV_SDC 0
#define BDEV_FDC 1
#define BDEV_HDC 2

//
// Structure defining a block device's functions
//

typedef struct s_dev_block {
    short number;           // The number of the device (assigned by registration)
    char * name;            // The name of the device
    FUNC_V_2_S init;        // short init() -- Initialize the device
    FUNC_LBS_2_S read;      // short read(long lba, byte * buffer, short size) -- Read a block from the device
    FUNC_LcBS_2_S write;    // short write(long lba, byte * buffer, short size) -- Write a block to the device
    FUNC_V_2_S status;      // short status() -- Get the status of the device
    FUNC_V_2_S flush;       // short flush() -- Ensure that any pending writes to teh device have been completed
    FUNC_SBS_2_S ioctrl;    // short ioctrl(short command, byte * buffer, short size)) -- Issue a control command to the device
} t_dev_block, *p_dev_block;

/*=========================================
 * dev/fsys
 *========================================*/

#define MAX_PATH_LEN        256
#define DEFAULT_CHUNK_SIZE  256

/**
 * Type for directory information about a file
 */
typedef struct s_file_info {
    long size;
    unsigned short date;
    unsigned short time;
    unsigned char attributes;
    char name[MAX_PATH_LEN];
} t_file_info, * p_file_info;

/*
 * Pointer type for file loaders
 *
 * short loader(short chan, destination, start);
 */

typedef short (*p_file_loader)(short chan, long destination, long * start);


/*=========================================
 * dev/rtc
 *========================================*/

typedef struct s_time {
    short year;
    short month;
    short day;
    short hour;
    short minute;
    short second;
    short is_pm;
    short is_24hours;
} t_time, *p_time;

/*=========================================
 * sys_info
 *========================================*/

/* IDs for the various Foenix machines supported */

#define MODEL_FOENIX_FMX            0
#define MODEL_FOENIX_C256U          1
#define MODEL_FOENIX_GENX           4
#define MODEL_FOENIX_C256U_PLUS     5
#define MODEL_FOENIX_A2560U_PLUS    6
#define MODEL_FOENIX_A2560X         8
#define MODEL_FOENIX_A2560U         9
#define MODEL_FOENIX_A2560K         13

/* IDs for the CPUs supported */

#define CPU_WDC65816                0x16  /* CPU code for the Western Design Center 65816 */
#define CPU_M68000                  0x20  /* CPU code for the Motorola 68000 */
#define CPU_M68010                  0x21  /* CPU code for the Motorola 68010 */
#define CPU_M68020                  0x22  /* CPU code for the Motorola 68020 */
#define CPU_M68030                  0x23  /* CPU code for the Motorola 68030 */
#define CPU_M68040                  0x24  /* CPU code for the Motorola 68040 */
#define CPU_I486DX                  0x34  /* CPU code for the Intel 486DX */

/*
 * Structure to describe the hardware
 */
typedef struct s_sys_info {
    unsigned short mcp_version;     /* Current version of the MCP kernel */
    unsigned short mcp_rev;         /* Current revision, or sub-version of the MCP kernel */
    unsigned short mcp_build;       /* Current vuild # of the MCP kernel */
    unsigned short model;           /* Code to say what model of machine this is */
    const char * model_name;        /* Human readable name of the model of the computer */
    unsigned short cpu;             /* Code to say which CPU is running */
    const char * cpu_name;          /* Human readable name for the CPU */
    unsigned long fpga_date;        /* BCD date describing the date of the FPGA built: YYYYMMDD */
    char pcb_version[4];            /* PCB version (string) */
    unsigned long fpga_model;       /* FPGA model number */
    unsigned short fpga_version;    /* FPGA version */
    unsigned short fpga_subver;     /* FPGA sub-version */
    unsigned short vicky_rev;       /* Code for the VICKY revision number */
    long system_ram_size;           /* The number of bytes of system RAM on the board */
    bool has_floppy;                /* TRUE if the board has a floppy drive installed */
    bool has_hard_drive;            /* TRUE if the board has a PATA device installed */
    bool has_expansion_card;        /* TRUE if an expansion card is installed on the device */
    bool has_ethernet;              /* TRUE if an ethernet port is present */
    unsigned short screens;         /* How many screens are on this computer */
} t_sys_info, *p_sys_info;

#endif
