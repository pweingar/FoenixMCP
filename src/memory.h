/*
 * Memory mangament system
 *
 * Memory will be managed in 4KB "pages". A program can allocate blocks of these pages.
 * While a program my request so many bytes of storage, it will be allocated complete pages.
 * 
 * Memory can be allocated, in which case, the kernel will find and return the memory,
 * or memory can be reserved, in which case the program specifies which memory pages it is using.
 */

#ifndef __MEMORY_H
#define __MEMORY_H

#include "types.h"

typedef struct s_memory_info {
    short total_pages;
    short allocated_pages;
    short free_pages;
    short max_contiguous_free;
} t_memory_info, * p_memory_info;

/*
 * Initialize the memory management system
 */
extern void mem_init();

/*
 * Fill out an s_memory_info structure detailing how much memory is available.
 *
 * Inputs:
 * info = pointer to an s_memory_info structure to be filled out
 */
extern void mem_statistics(p_memory_info info);

/*
 * Allocate a block of memory for a program.
 *
 * Inputs:
 * pid = the ID of the process that will own this memory
 * tag = a number that must be unique per allocated block in a process
 * bytes = the number of bytes to allocate
 * 
 * Returns:
 * the address of the first byte of the allocated block, 0 for failure
 */
extern uint32_t mem_alloc(unsigned short pid, unsigned short tag, uint32_t bytes);

/*
 * Reserve a block of memory for a program.
 *
 * NOTE: although the caller specifies the starting and ending addresses
 *       needed, these addresses may not be page aligned. The FULL pages
 *       containing the addresses will be reserved.
 *
 * Inputs:
 * pid = the ID of the process that will own this memory
 * tag = a number that must be unique per allocated block in a process
 * start_addr = the address of the first byte to reserve
 * end_addr = the address of the last byte to reserve
 * 
 * Returns:
 * 0 on success, any other number is a failure
 */
extern int mem_reserve(unsigned short pid, unsigned short tag, uint32_t start_addr, uint32_t end_addr);

/*
 * Return a block of memory to the kernel.
 *
 * Inputs:
 * pid = the ID of the process that will own this memory
 * address = the address of the first byte to return
 */
extern void mem_free(unsigned short pid, uint32_t address);

/*
 * Return all memory allocated by a process.
 *
 * Inputs:
 * pid = the ID of the process that will own this memory
 */
extern void mem_free_all(unsigned short pid);

#endif