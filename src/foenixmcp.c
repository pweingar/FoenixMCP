/*
 * Startup file for the Foenix/MCP
 */

#include "syscalls_m68k.h"

int main(int argc, char * argv[]) {

  char * greet = "Foenix/MCP";

  for (char * c = greet; c != 0; c++) {
    bconout(*c);
  }

  /* Infinite loop... */
  while (1) {};
}
