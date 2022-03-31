#include "sys_general.h"
#include "gabe_reg.h"

/*
 * Fill out a s_MODEL_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_MODEL_info structure to fill out
 */
void sys_get_information(p_sys_info info) {

    info->mcp_version = VER_MAJOR;
    info->mcp_rev = VER_MINOR;
    info->mcp_build = VER_BUILD;

    info->has_floppy = 0;

    info->system_ram_size = 0x400000;

#if MODEL == MODEL_FOENIX_A2560K
    info->model = MODEL_FOENIX_A2560K;
    info->model_name = "Foenix A2560K";
    info->cpu = CPU_M68040;
    info->cpu_name = "M68040V";
    info->has_floppy = 1;
    info->has_expansion_card = 0;
    info->has_hard_drive = 1;
    info->has_ethernet = 1;
    info->screens = 2;

    info->pcb_version[0] = '?';
    info->pcb_version[1] = '?';
    info->pcb_version[2] = '?';
    info->pcb_version[3] = 0;

    // Get the FPGA number and version/sub-version
    info->fpga_date = 0;
    info->fpga_model = (*GABE_CHIP_VERSION & GABE_CHIP_N_MASK) >> 16;
    info->fpga_version = *GABE_CHIP_VERSION & GABE_CHIP_V_MASK;
    info->fpga_subver = (*GABE_SUBVER_ID & GABE_CHIP_SV_MASK) >> 16;

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
    // All A2560Us made are actually A2560U+s
    info->model = MODEL_FOENIX_A2560U_PLUS;
    info->model_name = "Foenix A2560U+";
    info->cpu = CPU_M68000;
    info->cpu_name = "M68000";
    info->has_floppy = 0;
    info->has_expansion_card = 0;
    info->has_hard_drive = 1;
    info->has_ethernet = 0;
    info->screens = 1;

    info->pcb_version[0] = '?';
    info->pcb_version[1] = '?';
    info->pcb_version[2] = '?';
    info->pcb_version[3] = 0;

    info->fpga_date = *FPGA_DATE_YEAR << 16 | *FPGA_DATE_MONTHDAY;
    info->fpga_model = *FPGA_MODEL_H << 16 | *FPGA_MODEL_L;
    info->fpga_version = *FPGA_VER;
    info->fpga_subver = *FPGA_SUBVER;

#else
    info->model = MODEL_FOENIX_A2560U_PLUS;
    info->model_name = "Unknown";
    info->cpu = CPU_M68000;
    info->cpu_name = "Unknown";
    info->has_floppy = 0;
    info->has_expansion_card = 0;
    info->has_hard_drive = 0;
    info->has_ethernet = 0;
    info->screens = 1;

    info->pcb_version[0] = '?';
    info->pcb_version[1] = '?';
    info->pcb_version[2] = '?';
    info->pcb_version[3] = 0;


    info->fpga_date = 0
    info->fpga_model = 0;
    info->fpga_version = 0;
    info->fpga_subver = 0;
#endif
}
