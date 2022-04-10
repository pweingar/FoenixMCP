#include "sys_general.h"
#include "gabe_reg.h"

/*
 * Fill out a s_MODEL_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_MODEL_info structure to fill out
 */
void sys_get_information(p_sys_info info) {
    unsigned long gabe_id = 0;
    unsigned short clock_speed = 0;
    unsigned short cpu = 0;
    unsigned short machine_id = 0;

    info->mcp_version = VER_MAJOR;
    info->mcp_rev = VER_MINOR;
    info->mcp_build = VER_BUILD;

    info->has_floppy = 0;

    info->system_ram_size = 0x400000;

#if MODEL == MODEL_FOENIX_A2560K
    gabe_id = *GABE_SUBVER_ID;
    clock_speed = (gabe_id & 0xE0) >> 5;
    cpu = (gabe_id & 0xF000) >> 12;
    machine_id = (gabe_id & 0x0F);

    info->has_floppy = 1;
    info->has_expansion_card = 0;
    info->has_hard_drive = 1;
    info->has_ethernet = 1;
    info->screens = 2;

    // Get the FPGA number and version/sub-version
    info->fpga_model = (*GABE_CHIP_VERSION & GABE_CHIP_N_MASK) >> 16;
    info->fpga_version = *GABE_CHIP_VERSION & GABE_CHIP_V_MASK;
    info->fpga_subver = (gabe_id & GABE_CHIP_SV_MASK) >> 16;

#elif MODEL == MODEL_FOENIX_A2560U || MODEL == MODEL_FOENIX_A2560U_PLUS
    // All A2560Us made are actually A2560U+s
    // TODO: actually get this from GABE
    machine_id = MODEL_FOENIX_A2560U_PLUS;
    cpu = CPU_M68000;
    clock_speed = SYSCLK_20MHZ;

    info->has_floppy = 0;
    info->has_expansion_card = 0;
    info->has_hard_drive = 1;
    info->has_ethernet = 0;
    info->screens = 1;

    info->fpga_date = *FPGA_DATE_YEAR << 16 | *FPGA_DATE_MONTHDAY;
    info->fpga_model = *FPGA_MODEL_H << 16 | *FPGA_MODEL_L;
    info->fpga_version = *FPGA_VER;
    info->fpga_subver = *FPGA_SUBVER;

#else
    machine_id = 0xFF;
    cpu = CPU_M68000;
    clock_speed = 0xFF;

    info->has_floppy = 0;
    info->has_expansion_card = 0;
    info->has_hard_drive = 0;
    info->has_ethernet = 0;
    info->screens = 1;

    info->fpga_model = 0;
    info->fpga_version = 0;
    info->fpga_subver = 0;
#endif

    info->model = machine_id;
    info->cpu = cpu;

    switch (clock_speed) {
        case SYSCLK_14MHZ:
            /* 14.318 MHz */
            info->cpu_clock_khz = 14318;
            break;

        case SYSCLK_20MHZ:
            info->cpu_clock_khz = 20000;
            break;

        case SYSCLK_25MHZ:
            info->cpu_clock_khz = 25000;
            break;

        case SYSCLK_33MHZ:
            info->cpu_clock_khz = 33000;
            break;

        case SYSCLK_40MHZ:
            info->cpu_clock_khz = 40000;
            break;

        case SYSCLK_50MHZ:
            info->cpu_clock_khz = 50000;
            break;

        case SYSCLK_66MHZ:
            info->cpu_clock_khz = 66000;
            break;

        case SYSCLK_80MHZ:
            info->cpu_clock_khz = 80000;
            break;

        default:
            info->cpu_clock_khz = 0;
            break;
    }

    switch (info->cpu) {
        case CPU_WDC65816:
            info->cpu_name = "WDC65816";
            break;

        case CPU_M68000:
            info->cpu_name = "M68SEC000";
            break;

        case CPU_M68020:
            info->cpu_name = "M68020";
            break;

        case CPU_M68EC020:
            info->cpu_name = "M68EC020";
            break;

        case CPU_M68030:
            info->cpu_name = "M68030";
            break;

        case CPU_M680EC30:
            info->cpu_name = "M680EC30";
            break;

        case CPU_M68040:
            info->cpu_name = "M68040";
            break;

        case CPU_M68040V:
            info->cpu_name = "M68040V";
            break;

        case CPU_M680EC40:
            info->cpu_name = "M68EC040";
            break;

        case CPU_486DX2_50:
            info->cpu_name = "i486DX2 50";
            break;

        case CPU_486DX2_60:
            info->cpu_name = "i486DX2 60";
            break;

        case CPU_486DX4:
            info->cpu_name = "i486DX4";
            break;

        default:
            info->cpu_name = "UNKNOWN";
            break;
    }

    switch (info->model) {
        case MODEL_FOENIX_FMX:
            info->model_name = "C256 FMX";
            break;

        case MODEL_FOENIX_C256U:
            info->model_name = "C256 U";
            break;

        case MODEL_FOENIX_GENX:
            info->model_name = "A2560 GenX";
            break;

        case MODEL_FOENIX_C256U_PLUS:
            info->model_name = "C256 U+";
            break;

        case MODEL_FOENIX_A2560U_PLUS:
            info->model_name = "A2560 U+";
            break;

        case MODEL_FOENIX_A2560X:
            info->model_name = "A2560 X";
            break;

        case MODEL_FOENIX_A2560U:
            info->model_name = "A2560 U";
            break;

        case MODEL_FOENIX_A2560K:
            info->model_name = "A2560 K";
            break;

        default:
            info->model_name = "UNKNOWN";
            break;
    }
}
