/*
 * Startup file for the Foenix/MCP
 */

#include <string.h>
#include "sys_general.h"
#include "syscalls.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/text_screen_iii.h"
#include "dev/pata.h"
#include "dev/sdc.h"
#include "log.h"

/*
 * Initialize the kernel systems.
 */
void initialize() {
  text_init();          // Initialize the text channels
  DEBUG("Foenix/MCP starting up...");
  cdev_init_system();   // Initialize the channel device system
  DEBUG("Channel device system ready.");
  if (con_install()) {
    DEBUG("FAILED: Console installation.");
  } else {
    DEBUG("Console installed.");
  }

  // At this point, we should be able to call into to console to print to the screens

  // if (pata_init()) {
  //     DEBUG("Error initializing the PATA drive.");
  // }

  if (sdc_init()) {
      DEBUG("Error initializing the PATA drive.");
  }
}

void print(short channel, char * message) {
  syscall(SYS_CHAN_WRITE, channel, message, strlen(message));
}

void print_hex(short channel, unsigned short x) {
    char number[3];
    short digit;
    char hex_digits[] = "0123456789ABCDEF";

    digit = (x & 0xf0) >> 4;
    number[0] = hex_digits[digit];

    digit = (x & 0x0f);
    number[1] = hex_digits[digit];

    number[2] = 0;

    print(channel, number);
}

int main(int argc, char * argv[]) {
    short x;
    short result;
    char buffer[512];
    initialize();

    for (x = 0; x < 512; x++) {
        buffer[x] = 0;
    }

    print(CDEV_CONSOLE, "Hello from Screen A!\n");
    print(CDEV_EVID, "Hello from Screen B!\n");

    print(1, "Hard drive sector 0:\n")
    result = sdc_read(0L, buffer, 512);
    if (result > 0) {
        for (x = 0; x < result; x++) {
            if (x % 16 == 0) {
                print(1, "\n");
            }
            print_hex(1, buffer[x]);
            print(1, " ");
        }
        print(1, "\n");
    } else if (result < 0) {
        DEBUG("IDE returned an error.");
    } else {
        DEBUG("IDE returned nothing.");
    }

    DEBUG("Stopping.");

    /* Infinite loop... */
    while (1) {};
}
