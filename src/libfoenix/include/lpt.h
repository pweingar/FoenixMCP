/*
 * Parallel printer port driver
 */

#ifndef __LPT_H
#define __LPT_H

#include <stdint.h>
#include <stdbool.h>

#define LPT_STATUS_ERROR    0x02    /** The printer has encountered some error */
#define LPT_STATUS_WRITABLE 0x08    /** The printer can accept data (online, no error, has paper, not busy) */
#define LPT_STATUS_PAPER    0x10    /** The printer is out of paper */
#define LPT_STATUS_ONLINE   0x20    /** The printer is selected/online */

int16_t lpt_init(int16_t delay);
void    lpt_delay();
void    lpt_select(bool selected);
void    lpt_strobe();
void    lpt_data(uint8_t b);
int16_t lpt_status();
int16_t lpt_busy();

#endif
