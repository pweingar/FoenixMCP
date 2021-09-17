/**
 * Implementation of 68000 specific syscall routines.
 *
 * NOTE: these routines are not called directly but are instead called through TRAP#13
 */

#include "types.h"
#include "syscalls.h"
#include "log.h"
#include "dev/channel.h"

/*
 * Determine the correct system function implementation and call it.
 */
int32_t syscall_dispatch(int32_t function, int32_t param0, int32_t param1, int32_t param2, int32_t param3, int32_t param4, int32_t param5) {
    switch (function) {
        case KFN_CHAN_WRITE_B:
            return chan_write_b((short)param0, (uint8_t)param1);

        case KFN_CHAN_WRITE:
            return chan_write((short)param0, (const uint8_t *)param1, (short)param2);

        default:
            DEBUG("syscall unknown function\n");
            return -1;
    }
}
