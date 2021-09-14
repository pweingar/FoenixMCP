/**
 * Definitions of ring buffers
 */

#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include "types.h"

#define MAX_BUFFER_SIZE 128

// A ring buffer of unsigned words
typedef struct s_word_ring {
    unsigned short buffer[MAX_BUFFER_SIZE];
    unsigned short head;
    unsigned short tail;
} t_word_ring, *p_word_ring;

//
// Initialize the byte ring buffer
//
extern void rb_word_init(p_word_ring r);

//
// Return true if the ring buffer is full
//
extern unsigned short rb_word_full(p_word_ring r);

//
// Return true if the ring buffer is empty
//
extern unsigned short rb_word_empty(p_word_ring r);

//
// Add a byte to the ring buffer... data is lost if buffer is full
//
extern void rb_word_put(p_word_ring r, unsigned short data);

//
// Get a byte from the ring buffer... returns 0 if the buffer is empty
//
extern unsigned short rb_word_get(p_word_ring r);

#endif
