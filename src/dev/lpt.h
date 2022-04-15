/*
 * Parallel printer port driver
 */

#ifndef __LPT_H
#define __LPT_H

#include "dev/channel.h"

#define LPT_STAT_ERROR      0x02    /** The printer has encountered some error */
#define LPT_STAT_WRITABLE   0x08    /** The printer can accept data (online, no error, has paper, not busy) */
#define LPT_STAT_PAPER      0x10    /** The printer is out of paper */
#define LPT_STAT_ONLINE     0x20    /** The printer is selected/online */

/*
 * Install the LPT driver
 */
extern short lpt_install();

#endif
