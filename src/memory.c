/**
 * @file memory.h
 *
 * Memory managament system: memory in Foenix/MCP is handled very simply.
 * The system will keep track of the top of available system RAM.
 * User programs can do whatever they want with system RAM from $400 (end of 68K vectors area)
 * to the top of system RAM. Memory above top of system RAM is reserved for
 * the kernel and any terminate-stay-resident code the user cares to install.
 *
 * NOTE: this code does not manage video RAM or DRAM (on the A2560K)... only
 * system RAM, where code may be executed.
 *
 */

#include <stdint.h>
#include "memory.h"

uint32_t mem_top_of_ram = 0;

/*
 * Initialize the memory management system
 *
 * @param top_of_ram initial value for the top of system RAM
 */
void mem_init(uint32_t top_of_ram) {
    mem_top_of_ram = top_of_ram;
}

/**
 * Return the top of system RAM... the user program must not use any
 * system memory from this address and above.
 *
 * @return the address of the first byte of reserved system RAM (one above the last byte the user program can use)
 */
uint32_t mem_get_ramtop() {
    return mem_top_of_ram;
}

/**
 * Reserve a block of memory at the top of system RAM.
 *
 * @param bytes the number of bytes to reserve
 * @return address of the first byte of the reserved block
 */
uint32_t mem_reserve(uint32_t bytes) {
    mem_top_of_ram -= bytes;
    return mem_top_of_ram;
}
