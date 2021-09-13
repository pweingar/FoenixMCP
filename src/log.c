/*
 * A logging utility
 */

#include "log.h"
#include "dev/text_screen_iii.h"

/*
 * Send a message to the debugging channel
 */
void DEBUG(char * message) {
    int i;
    for (i = 0; i < strlen(message); i++) {
        text_put_raw(0, message[i]);
    }
}