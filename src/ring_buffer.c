/**
 * Definitions for the ring buffers
 */

#include "ring_buffer.h"

//
// Initialize the word ring buffer
//
void rb_word_init(p_word_ring r) {
    r->head = 0;
    r->tail = 0;
}

//
// Return true if the ring buffer is full
//
unsigned short rb_word_full(p_word_ring r) {
    return(r->head + 1 == r->tail);
}

//
// Return true if the ring buffer is empty
//
unsigned short rb_word_empty(p_word_ring r) {
    return(r->head == r->tail);
}

//
// Add a word to the ring buffer... data is lost if buffer is full
//
void rb_word_put(p_word_ring r, unsigned short data) {
    if (!rb_word_full(r)) {
        r->buffer[r->head++] = data;
        if (r->head >= MAX_BUFFER_SIZE) {
            r->head = 0;
        }
    }
}

//
// Get a word from the ring buffer... returns 0 if the buffer is empty
//
unsigned short rb_word_get(p_word_ring r) {
    if (!rb_word_empty(r)) {
        unsigned short data = r->buffer[r->tail++];
        if (r->tail >= MAX_BUFFER_SIZE) {
            r->tail = 0;
        }

        return data;
    } else {
        return 0;
    }
}
