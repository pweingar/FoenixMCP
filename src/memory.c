/*
 * Memory mangament system
 *
 * Memory will be managed in 4KB "pages". A program can allocate blocks of these pages.
 * While a program my request so many bytes of storage, it will be allocated complete pages.
 * 
 * Memory can be allocated, in which case, the kernel will find and return the memory,
 * or memory can be reserved, in which case the program specifies which memory pages it is using.
 */

#include "memory.h"

#define MEM_PAGE_SIZE   4096                /* The size of a page in bytes */
#define MEM_MAX_PAGES   0x40                /* The maximum number of pages of system RAM on this computer */
#define MEM_OWN_KERNEL  1                   /* "PID" of the kernel */
#define MEM_OWN_NULL    2                   /* "PID" of memory not in the system */
#define MEM_TAG_VECTORS 1                   /* Tag for the vector block */

/*
 * Structure to track who owns a page of memory
 */
typedef struct s_mem_ownership {
    unsigned short pid;
    unsigned short tag;
} t_mem_ownership, * p_mem_ownership;

t_mem_ownership mem_pages[MEM_MAX_PAGES];   /* Array containing the information about who owns a page */

/*
 * Convert an address to a page number
 */
short mem_addr_to_page(uint32_t address) {
    return (address >> 12) & 0xffff;
}

/*
 * Convert a page number to the address of its first byte
 */
uint32_t mem_page_to_addr(short page) {
    return ((page & 0xffff) << 12);
}

/*
 * Initialize the memory management system
 */
void mem_init() {
    int page;

    /* Initialize the page ownership for all pages to "unowned" */
    for (page = 0; page < MEM_MAX_PAGES; page++) {
        mem_pages[page].pid = 0;
        mem_pages[page].tag = 0;
    }

    /* The kernel should now claim the memory it needs to operate */
    mem_reserve(MEM_OWN_KERNEL, MEM_TAG_VECTORS, 0, mem_page_to_addr(1));   /* Reserve the first page for system vectors */

    /* TODO: scan the system for memory or set it based on the machine ID */
    /* TODO: reserve the kernel's working memory */
}

/*
 * Fill out an s_memory_info structure detailing how much memory is available.
 *
 * Inputs:
 * info = pointer to an s_memory_info structure to be filled out
 */
void mem_statistics(p_memory_info info) {
    short page;
    short contiguous_free_count = 0;
    short first_free = -1;

    info->total_pages = 0;
    info->allocated_pages = 0;
    info->free_pages = 0;
    info->max_contiguous_free = 0;

    for (page = 0; page < MEM_MAX_PAGES; page++) {
        if (mem_pages[page].pid == MEM_OWN_NULL) {
            /* If we've reached a non-existant page, we're done. */
            break;
        }

        info->total_pages++;
        if (mem_pages[page].pid == 0) {
            /* We have a free page... count it and see if it's the first of a block */
            info->free_pages++;
            contiguous_free_count++;
            if (first_free == -1) {
                first_free = page;
            }

        } else {
            /* We have an allocated page... count it and see if we need to close out a contiguous free block */
            info->allocated_pages++;

            if (first_free != -1) {
                /* The previous page was free... check to see if the free block before it is largest */
                if (contiguous_free_count > info->max_contiguous_free) {
                    info->max_contiguous_free = contiguous_free_count;
                }

                contiguous_free_count = 0;
                first_free = -1;
            }
        }
    }

    /* If the end of memory is free... check if it's the biggest free block */
    if (contiguous_free_count > info->max_contiguous_free) {
        info->max_contiguous_free = contiguous_free_count;
    }
}

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
uint32_t mem_alloc(unsigned short pid, unsigned short tag, uint32_t bytes) {
    short page;
    short i;
    short first_free = -1;
    short free_count = 0;

    for (page = 0; page < MEM_MAX_PAGES; page++) {
        if (mem_pages[page].pid != 0) {
            /* Page is not free... reset our tracking information */
            if (first_free != -1) {
                free_count = 0;
                first_free = -1;
            }

        } else {
            /* Page is free... */
            if (first_free == -1) {
                /* If the last one was taken, this is the first free page */
                first_free = page;
            }

            free_count++;

            if (free_count * MEM_PAGE_SIZE >= bytes) {
                /* We have found enough pages to hold the requested amount of memory... */

                /* Allocate the memory to this process */
                for (i = first_free; i < free_count + first_free - 1; i++) {
                    mem_pages[i].pid = pid;
                    mem_pages[i].tag = tag;
                }

                /* Return the starting address for the block */
                return mem_page_to_addr(first_free);
            }
        }
    }

    /* We did not find a block big enough... return 0 */
    return 0;
}

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
int mem_reserve(unsigned short pid, unsigned short tag, uint32_t start_addr, uint32_t end_addr) {
    short page;
    short start_page = mem_addr_to_page(start_addr);
    short end_page = mem_addr_to_page(end_addr);

    /* Check to see if all the pages are free */
    for (page = start_page; page <= end_page; page++) {
        if (mem_pages[page].pid != 0) {
            /* If not, return an error */
            /* TODO: return a real error number */
            return -1;
        }
    }

    /* Reserve the pages */
    for (page = start_page; page <= end_page; page++) {
        mem_pages[page].pid = 0;
        mem_pages[page].tag = 0;
    }  

    return 0;
}

/*
 * Return a block of memory to the kernel.
 *
 * Inputs:
 * pid = the ID of the process that will own this memory
 * address = the address of the first byte to return
 */
void mem_free(unsigned short pid, uint32_t address) {
    short tag, p, start_page, end_page;
    short page = mem_addr_to_page(address);

    if (mem_pages[page].pid == pid) {
        /* If this page is owned by the calling process, get the tag for this block */
        tag = mem_pages[page].tag;

        /* Scan the previous pages until we find a page not in this block */
        for (p = page; (mem_pages[p].pid == pid) && (mem_pages[p].tag == tag); p--) {
            start_page = p;
        }

        /* Scan the next pages until we find a page not in this block */
        for (p = page; (mem_pages[p].pid == pid) && (mem_pages[p].tag == tag); p++) {
            end_page = p;
        }

        /* Mark all pages from start to end as unowned */
        for (p = start_page; p <= end_page; p++) {
            mem_pages[p].pid = 0;
            mem_pages[p].tag = 0;
        }
    }
}

/*
 * Return all memory allocated by a process.
 *
 * Inputs:
 * pid = the ID of the process that will own this memory
 */
void mem_free_all(unsigned short pid) {
    int page;

    /* Reset all pages owned by this PID to unowned */
    for (page = 0; page < MEM_MAX_PAGES; page++) {
        if (mem_pages[page].pid = pid) {
            mem_pages[page].pid = 0;
            mem_pages[page].tag = 0;
        }
    }
}
