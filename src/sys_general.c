#include "sys_general.h"

/*
 * Fill out a s_sys_info structure with the information about the current system
 *
 * Inputs:
 * info = pointer to a s_sys_info structure to fill out
 */
void sys_get_info(p_sys_info info) {
    /* Model, CPU, and the presence of the floppy are set at compile time */
    info->model = MODEL;
    info->cpu = CPU;
#if MODEL==SYS_FOENIX_FMX || MODEL==SYS_FOENIX_A2560K || MODEL==SYS_FOENIX_A2560X || MODEL==SYS_FOENIX_GENX
    info->has_floppy = 1;
#else
    info->has_floppy = 0;
#endif

    info->gabe_rev = 0x0000;        /* TODO: get this from GABE */

    info->vicky_rev = 0x0000;       /* TODO: get this from VICKY */
    
    info->system_ram_size = 0;      /* TODO: compute this by testing RAM */
    
    info->has_expansion_card = 0;   /* TODO: figure this out by checking with GABE */
    
    info->has_hard_drive = 0;       /* TODO: figure this out by checking with GABE */

    /* Set the number of screens and the presence of ethernet based on the model and expansion card */
    switch (info->model) {
        case SYS_FOENIX_A2560K:
        case SYS_FOENIX_A2560X:
        case SYS_FOENIX_GENX:
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
