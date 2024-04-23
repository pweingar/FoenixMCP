/*
 * Gather and return information about the system
 */

#ifndef __SYS_GENERAL_H
#define __SYS_GENERAL_H

#include "hardware_ids.h"
#include "version.h"
#include "types.h"

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
    uint32_t fpga_date;             /* YYYYMMDD */
    uint32_t fpga_model;       		/* FPGA model number */
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
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
extern void sys_get_information(p_sys_info info);

#endif
