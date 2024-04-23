/*
 * Parallel port printer driver
 */

#include "features.h"
#if HAS_PARALLEL_PORT

#include <stdint.h>
#include <stdbool.h>
#include "errors.h"
#include "log.h"
#include "lpt_reg.h"
#include "simpleio.h"
#include "rtc.h"
#include "libfoenix/include/lpt.h"

/* How many jiffies should laptse when we wait */
static int16_t delay_jiffies;

/**
 * Initialize the printer... assert the INIT pin to trigger a reset on the printer
 */
int16_t lpt_init(int16_t delay) {
    delay_jiffies = delay;

    /* Set the outputs to start the initialization process */
    *LPT_CTRL_PORT = LPT_CTRL_SELECT;
    lpt_delay();

    /* Set the outputs to stop the initialization process */
    *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT;

    return 0;
}

/**
 * Wait a little bit...
 */
void lpt_delay() {
    int32_t target_jiffies = rtc_get_jiffies() + 1;
    while (target_jiffies > rtc_get_jiffies()) ;
}


void lpt_select(bool selected) {
    if (selected)
        *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT;
    else 
        *LPT_CTRL_PORT = LPT_CTRL_mINIT;
}

int16_t lpt_busy() {
    return (*LPT_STAT_PORT & LPT_STAT_nBUSY) != LPT_STAT_nBUSY;
}

/*
 * Sets the output data lines.
 * THIS DOESN'T CHECK IF THE PRINTER IS BUSY OR NOT.
 */
void lpt_data(uint8_t b) {
    *LPT_DATA_PORT = b;
}

void lpt_strobe() {
    *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT;
    lpt_delay();
    *LPT_CTRL_PORT = LPT_CTRL_mINIT | LPT_CTRL_SELECT | LPT_CTRL_STROBE;
}

/**
 * Return the status of the printer
 */
int16_t lpt_status() {
    // Get the status
    uint8_t stat = *LPT_STAT_PORT;
    int16_t result;

    // Conver the status bits to be consistent with MCP's channels
    if ((stat & LPT_STAT_nERROR) == 0)
        result |= LPT_STATUS_ERROR;
    
    if (stat & LPT_STAT_PO)
        result |= LPT_STATUS_PAPER;
    
    if (stat & LPT_STAT_SELECT)
        result |= LPT_STATUS_ONLINE;
    if ((stat & (LPT_STAT_nERROR | LPT_STAT_PO | LPT_STAT_nBUSY | LPT_STAT_SELECT)) == LPT_STAT_nERROR | LPT_STAT_nBUSY | LPT_STAT_SELECT)
        // Online, there's paper, not busy, and not in error
        result |= LPT_STATUS_WRITABLE;

    return result;
}

#endif
