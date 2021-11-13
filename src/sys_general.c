#include "sys_general.h"
#include "gabe_reg.h"

/*
 * Fill out a s_MODEL_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_MODEL_info structure to fill out
 */
void sys_get_information(p_sys_info info) {
    unsigned short machine_id = *GABE_MACHINE_ID;

    info->mcp_version = VER_MAJOR;
    info->mcp_rev = VER_MINOR;
    info->mcp_build = VER_BUILD;

    info->has_floppy = 0;

    info->system_ram_size = 0x200000;

#if MODEL == MODEL_FOENIX_A2560K
    info->model = MODEL_FOENIX_A2560K;
    info->model_name = "Foenix A2560K";
    info->cpu_name = "m68000";
    info->has_floppy = 1;
#else

    /* Model, CPU, and the presence of the floppy are set at compile time */
    switch (machine_id & 0x000f) {
        case 0x00:
            info->model = MODEL_FOENIX_FMX;
            info->model_name = "Foenix FMX";
            info->cpu_name = "WDC 65816";
            info->has_floppy = 1;
            break;

        case 0x01:
            info->model = MODEL_FOENIX_C256U;
            info->model_name = "Foenix C256U";
            info->cpu_name = "WDC 65816";
            break;

        case 0x05:
            info->model = MODEL_FOENIX_C256U_PLUS;
            info->model_name = "Foenix C256U+";
            info->cpu_name = "WDC 65816";
            break;

        case 0x09:
            info->cpu_name = "MC68SEC000";
            if ((machine_id & GABE_MEMORY_BANKS) == 0xC0) {
                info->system_ram_size = 0x400000;
                info->model_name = "Foenix A2560U+";
                info->model = MODEL_FOENIX_A2560U;
            } else {
                info->system_ram_size = 0x200000;
                info->model_name = "Foenix A2560U";
                info->model = MODEL_FOENIX_A2560U_PLUS;
            }
            break;

        default:
            /* Model is unknown */
            info->model = 0xffff;
            info->model_name = "Unknown";
            info->cpu_name = "Unknown";
            break;
    }

    switch ((machine_id & 0xf000) >> 12) {
        case 0x00:
            info->cpu = CPU_M68000;
            info->cpu_name = "MC68SEC000";
            break;

        default:
            /* Unknown CPU */
            info->cpu = 0xffff;
            info->cpu_name = "Unknown";
            break;
    }
#endif

    info->fpga_date = *FPGA_DATE_YEAR << 16 | *FPGA_DATE_MONTHDAY;
    info->fpga_model = *FPGA_MODEL_H << 16 | *FPGA_MODEL_L;
    info->fpga_version = *FPGA_VER;
    info->fpga_subver = *FPGA_SUBVER;

    info->pcb_version[0] = (*PCB_REV_1 & 0xFF00) >> 8;
    info->pcb_version[1] = *PCB_REV_1 & 0xFF;
    info->pcb_version[2] = (*PCB_REV_2 & 0xFF00) >> 8;
    info->pcb_version[3] = 0;

    info->vicky_rev = 0x0000;       /* TODO: get this from VICKY */

    info->has_expansion_card = 0;   /* TODO: figure this out by checking with GABE */

    info->has_hard_drive = 0;       /* TODO: figure this out by checking with GABE */

    /* Set the number of screens and the presence of ethernet based on the model and expansion card */
    switch (info->model) {
        case MODEL_FOENIX_A2560K:
        case MODEL_FOENIX_A2560X:
        case MODEL_FOENIX_GENX:
            /* These systems are built with 2 screens and ethernet */
            info->screens = 2;
            info->has_ethernet = 1;
            break;

        default:
            /* Otherwise, we need the correct expansion card */
            info->screens = 1;
            info->has_ethernet = 0;

            // if (info->has_expansion_card) {
            //     /* TODO: detect card and set screen number and/or ethernet accordingly */
            //     ;
            // }
            break;
    }
}
