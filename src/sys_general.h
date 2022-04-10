/*
 * Gather and return information about the system
 */

#ifndef __SYS_GENERAL_H
#define __SYS_GENERAL_H

#include "version.h"
#include "types.h"

/* IDs for the various Foenix machines supported */

#define MODEL_FOENIX_FMX            0
#define MODEL_FOENIX_C256U          1
#define MODEL_FOENIX_GENX           4
#define MODEL_FOENIX_C256U_PLUS     5
#define MODEL_FOENIX_A2560U_PLUS    6
#define MODEL_FOENIX_A2560X         8
#define MODEL_FOENIX_A2560U         9
#define MODEL_FOENIX_A2560K         11

/* IDs for the CPUs supported */

#define CPU_WDC65816                0xFF  /* CPU code for the Western Design Center 65816 */
#define CPU_M68000                  0x00  /* Motorola 680X0 chips... */
#define CPU_M68020                  0x01
#define CPU_M68EC020                0x02
#define CPU_M68030                  0x03
#define CPU_M680EC30                0x04
#define CPU_M68040                  0x05
#define CPU_M68040V                 0x06
#define CPU_M680EC40                0x07
#define CPU_486DX2_50               0x08  /* Intel 486 chips... */
#define CPU_486DX2_60               0x09
#define CPU_486DX4                  0x0A

/* Clock speeds */

#define SYSCLK_14MHZ                0x00    /* 14.318 MHz */
#define SYSCLK_20MHZ                0x01    /* 20 MHz */
#define SYSCLK_25MHZ                0x02    /* 25 MHz */
#define SYSCLK_33MHZ                0x03    /* 33 MHz */
#define SYSCLK_40MHZ                0x04    /* 40 MHz */
#define SYSCLK_50MHZ                0x05    /* 50 MHz */
#define SYSCLK_66MHZ                0x06    /* 66 MHz */
#define SYSCLK_80MHZ                0x07    /* 88 MHz */

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
    unsigned int cpu_clock_khz;     /* Speed of the CPU clock in KHz */
    unsigned long fpga_model;       /* FPGA model number */
    unsigned short fpga_version;    /* FPGA version */
    unsigned short fpga_subver;     /* FPGA sub-version */
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
