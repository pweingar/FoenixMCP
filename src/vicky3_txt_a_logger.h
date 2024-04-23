#ifndef VICKY3_TXT_A_LOGGER
#define VICKY3_TXT_A_LOGGER

#include "sys_general.h"

#if MODEL == MODEL_FOENIX_A2560K || MODEL == MODEL_FOENIX_A2560X || MODEL == MODEL_FOENIX_GENX

void channel_A_logger_init(void);
void channel_A_cls();
void channel_A_write(const char *c);

#endif

#endif
