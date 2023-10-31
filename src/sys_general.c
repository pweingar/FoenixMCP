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
    unsigned short gabe_id_Genx = 0;    // Needs to be a 16bits for access to the CPLD registers. (0xFEC005xx Region)
    unsigned short genx_model_id = 0;
    unsigned short clock_speed = 0;
    unsigned short cpu = 0;
    unsigned short machine_id = 0;

    info->mcp_version = VER_MAJOR;
    info->mcp_rev = VER_MINOR;
    info->mcp_build = VER_BUILD;

    info->has_floppy = 0;

    info->system_ram_size = 0x400000;

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
    gabe_id = *GABE_SUBVER_ID;
    clock_speed = (gabe_id & 0xE0) >> 5;
    cpu = (gabe_id & 0xF000) >> 12;

#if ( MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X )
    gabe_id_Genx = *GABE_SUB_MODEL_ID;  // diffent register inside the CPLD - not the FPGA - The FPGA will report a reserved code 0101
    machine_id = (gabe_id_Genx & 0x000F);
#else 
    machine_id = (gabe_id & 0x0F);
#endif 

    info->has_floppy = 1;
    info->has_expansion_card = 0;
    info->has_hard_drive = 1;
    info->has_ethernet = 1;
    info->screens = 2;


#if MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
    genx_model_id = *GABE_SUB_MODEL_FF_ID; // This gets the MID on the board that tells us if it is a PB/LB/CU
                                        // Now, that doesn't tell us if it is stuffed with audio chip or not
                                        // However, those will tell us about if there is a Floppy or not
                                        // This of course is good also for the A2560X
    genx_model_id = genx_model_id & GABE_SUB_FF_MASK;  // This isolate 2 bits, 
    // 00 = PB
    // 01 = LB
    // 10 = CU
    info->sub_model = genx_model_id;
#else 
    info->sub_model = 0;
#endif

    // Get the FPGA information
    info->fpga_date = /* YYYYMMDD */
        ((((*GABE_FIRMWARE_DATE) & GABE_FIRMWARE_DATE_YEAR_MASK) + 2000) << 16) |
        ((*GABE_FIRMWARE_DATE) & GABE_FIRMWARE_DATE_MONTH_MASK) |
        (((*GABE_FIRMWARE_DATE) & GABE_FIRMWARE_DATE_DAY_MASK) >> 16 );

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

        case CPU_MC68060:
            info->cpu_name = "MC68xx060"
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
            switch (genx_model_id) {
                case 0: info->model_name = "GenX32 - PB"; break;
                case 1: info->model_name = "GenX32 - LB"; break;
                case 2: info->model_name = "GenX32 - CUBE"; break;
                default: info->model_name = "GenX32"; break;
            }        
            break;

        case MODEL_FOENIX_C256U_PLUS:
            info->model_name = "C256 U+";
            break;

        case MODEL_FOENIX_A2560U_PLUS:
            info->model_name = "A2560 U+";
            break;

        case MODEL_FOENIX_A2560X:
            switch (genx_model_id) {
                case 0: info->model_name = "A2560X - PB"; break;
                case 1: info->model_name = "A2560X - LB"; break;
                case 2: info->model_name = "A2560X - CUBE"; break;
                default: info->model_name = "A2560 X"; break;
            }
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

#if MODEL == MODEL_FOENIX_GENX || MODEL == MODEL_FOENIX_A2560X
static short genx_leds = 0;

/*
 * Set the color of the LED for the floppy drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
void genx_set_fdc_led(short colors) {
    genx_leds = (genx_leds & 0xFFF8) | (colors & 0x07);
    *GABE_GENX_STAT_LEDS = genx_leds;
}

/*
 * Set the color of the LED for the SD card slot
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
void genx_set_sdc_led(short colors) {
    genx_leds = (genx_leds & 0xFFC7) | ((colors & 0x07) << 3);
    *GABE_GENX_STAT_LEDS = genx_leds;
}

/*
 * Set the color of the LED for the IDE hard drive
 *
 * Inputs:
 * colors = color specification, three bits: 0x_____RGB
 */
void genx_set_hdc_led(short colors)  {
    genx_leds = (genx_leds & 0xFE3F) | ((colors & 0x07) << 6);
    *GABE_GENX_STAT_LEDS = genx_leds;
}
#endif
