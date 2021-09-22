#include "sys_general.h"
#include "gabe_reg.h"

/*
 * Fill out a s_MODEL_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_MODEL_info structure to fill out
 */
void sys_get_info(p_sys_info info) {
    unsigned short machine_id = *GABE_MACHINE_ID;

    info->has_floppy = 0;

    /* Model, CPU, and the presence of the floppy are set at compile time */
    switch (machine_id & 0x000f) {
        case 0x00:
            info->model = MODEL_FOENIX_FMX;
            info->has_floppy = 1;
            break;

        case 0x01:
            info->model = MODEL_FOENIX_C256U;
            break;

        case 0x05:
            info->model = MODEL_FOENIX_C256U_PLUS;
            break;

        case 0x0B:
            info->model = MODEL_FOENIX_A2560K;
            info->has_floppy = 1;
            break;

        case 0x09:
            info->model = MODEL_FOENIX_A2560U;
            break;

        default:
            /* Model is unknown */
            info->model = 0xffff;
            break;
    }

    switch ((machine_id & 0xf000) >> 12) {
        case 0x00:
            info->cpu = CPU_M68000;
            break;

        default:
            /* Unknown CPU */
            info->cpu = 0xffff;
            break;
    }

    info->gabe_number = *GABE_CHIP_NUMBER;
    info->gabe_version = *GABE_CHIP_VERSION;
    info->gabe_subrev = *GABE_CHIP_SUBREV;

    info->vicky_rev = 0x0000;       /* TODO: get this from VICKY */

    info->system_ram_size = 0;      /* TODO: compute this by testing RAM */

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
