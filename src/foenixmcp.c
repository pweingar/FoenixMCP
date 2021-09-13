/*
 * Startup file for the Foenix/MCP
 */

#include <string.h>

#include "sys_general.h"
#include "syscalls.h"
#include "dev/channel.h"
#include "dev/console.h"
#include "dev/text_screen_iii.h"
#include "log.h"

/*
 * Initialize the kernel systems.
 */
void initialize() {
  text_init();          // Initialize the text channels
  DEBUG("Foenix/MCP starting up...\n");
  cdev_init_system();   // Initialize the channel device system
  DEBUG("Channel device system ready.\n");
  if (con_install()) {
    DEBUG("FAILED: Console installation.\n");
  } else {
    DEBUG("Console installed.\n");
  }

  // At this point, we should be able to call into to console to print to the screens

}

void print(short channel, char * message) {
  syscall(SYS_CHAN_WRITE, channel, message, strlen(message));
}

int main(int argc, char * argv[]) {
  initialize();

  print(CDEV_CONSOLE, "Hello from Screen A!\n");
  print(CDEV_EVID, "Hello from Screen B!\n");

  DEBUG("Stopping.\n");
  
  /* Infinite loop... */
  while (1) {};
}
