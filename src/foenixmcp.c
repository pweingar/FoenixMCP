/*
 * Startup file for the Foenix/MCP
 */

#include <string.h>

#include "sys_general.h"
#include "m68k/syscalls_m68k.h"
#include "text_screen.h"

void print(short screen, char * message) {
  int i;
  for (i = 0; i < strlen(message); i++) {
    text_put_raw(screen, message[i]);
  }
}

int main(int argc, char * argv[]) {
  text_init();

  print(0, "Hello from Screen A!");
  print(1, "Hello from Screen B!");
  
  /* Infinite loop... */
  while (1) {};
}
