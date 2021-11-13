/*
 * Gather and return information about the system
 */

#ifndef __SYS_GENERAL_H
#define __SYS_GENERAL_H

#include "types.h"

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

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
extern void sys_get_information(p_sys_info info);

#endif
