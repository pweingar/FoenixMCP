//
// Types used in FoenixMCP system calls
//

#ifndef _sys_types_h_
#define _sys_types_h_

#include <stdbool.h>
#include <stdint.h>
#include "constants.h"

/**
 * @struct s_extent
 *
 * An extent or size of a rectangular area
 */
typedef struct s_extent {
    short width;        /**< The width of the region */
    short height;       /**< The height of the region */
} t_extent, *p_extent;

/**
 * @struct s_point
 *
 * A point on a plane
 */
typedef struct s_point {
    short x;                /**< The column of the point */
    short y;                /**< The row of the point */
} t_point, *p_point;

/**
 * @struct s_rect
 *
 * A rectangle on the screen
 */
typedef struct s_rect {
    t_point origin;         /**< The upper-left corner of the rectangle */
    t_extent size;          /**< The size of the rectangle */
} t_rect, *p_rect;

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

typedef void  (*FUNC_V_2_V)();
typedef short (*FUNC_V_2_S)();
typedef short (*FUNC_S_2_S)(char *);
typedef short (*FUNC_BS_2_S)(unsigned char *, short);
typedef short (*FUNC_cBS_2_S)(const unsigned char *, short);
typedef short (*FUNC_B_2_S)(const unsigned short);
typedef short (*FUNC_LBS_2_S)(long, unsigned char *, short);
typedef short (*FUNC_LcBS_2_S)(long, const unsigned char *, short);
typedef short (*FUNC_SBS_2_S)(short, unsigned char *, short);
typedef short (*FUNC_LB_2_S)(long, short);

/*
 * Type declaration for an interrupt handler
 */

typedef void (*p_int_handler)();

/*
 * Structure to describe the hardware
 */

typedef struct s_sys_info {
    uint16_t mcp_version;     		/* Current version of the MCP kernel */
    uint16_t mcp_rev;         		/* Current revision, or sub-version of the MCP kernel */
    uint16_t mcp_build;       		/* Current vuild # of the MCP kernel */
    uint16_t model;           		/* Code to say what model of machine this is */
    uint16_t sub_model;         	/* 0x00 = PB, 0x01 = LB, 0x02 = CUBE */
    const char * model_name;        /* Human readable name of the model of the computer */
    uint16_t cpu;             		/* Code to say which CPU is running */
    const char * cpu_name;          /* Human readable name for the CPU */
    uint32_t cpu_clock_khz;     	/* Speed of the CPU clock in KHz */
    unsigned long fpga_date;        /* YYYYMMDD */    
    uint16_t fpga_model;       		/* FPGA model number */
    uint16_t fpga_version;    		/* FPGA version */
    uint16_t fpga_subver;     		/* FPGA sub-version */
    uint32_t system_ram_size;		/* The number of bytes of system RAM on the board */
    bool has_floppy;                /* TRUE if the board has a floppy drive installed */
    bool has_hard_drive;            /* TRUE if the board has a PATA device installed */
    bool has_expansion_card;        /* TRUE if an expansion card is installed on the device */
    bool has_ethernet;              /* TRUE if an ethernet port is present */
    uint16_t screens;         		/* How many screens are on this computer */
} t_sys_info, *p_sys_info;

/*
 * Structure defining a block device's functions
 */

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

/*
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

/*
 * Type to describe the current time
 */

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

/*
 * A description of a screen's capabilities
 */
 
typedef struct s_txt_capabilities {
    short number;               /**< The unique ID of the screen */
    short supported_modes;      /**< The display modes supported on this screen */
    short font_size_count;      /**< The number of supported font sizes */
    p_extent font_sizes;        /**< Pointer to a list of t_extent listing all supported font sizes */
    short resolution_count;     /**< The number of supported display resolutions */
    p_extent resolutions;       /**< Pointer to a list of t_extent listing all supported display resolutions (in pixels) */
} t_txt_capabilities, *p_txt_capabilities;

#endif