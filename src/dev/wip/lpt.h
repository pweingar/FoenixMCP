/*
 * Parallel printer port driver
 */

#ifndef __LPT_H
#define __LPT_H

#include "dev/channel.h"

/*
 * Install the LPT driver
 */
extern short lpt_install();

extern void lpt_initialize();

/*
 * Write a character to the parallel port
 */
extern short lpt_write_b(p_channel chan, unsigned char b);

/*
 * Write a buffer of bytes to the parallel port
 */
extern short lpt_write(p_channel chan, unsigned char * buffer, short size);

#endif
