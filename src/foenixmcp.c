/*
 * Startup file for the Foenix/MCP
 */

#include "syscalls_m68k.h"

int main(int argc, char * argv[]) {
  bconout('A');
  bconout('B');
  bconout('C');

  /* Infinite loop... */
  while (1) {};
}
