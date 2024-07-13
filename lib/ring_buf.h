#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"

typedef struct ring_buf_t
{
    uint8_t size;
    uint8_t tail;
    uint8_t head;
    signed char* buf;      // opted for a pointer instead of array so a custom size can be used
}  ring_buf_t;

/**
 * Initialise and assign the fields of a ring_buf_t struct
 */
void ring_buf_init(ring_buf_t* buf, signed char* buffer, uint8_t buffer_size);

/**
 * Add a character to the circular buffer buf and increment tail, echo the character c, or return -1 if the buffer has overflowed
 * in the case of an overflow the buffer still function as normal except it will behave as if it were reset due to the tail overtaking the head
 */
signed char ring_buf_push(ring_buf_t* buf, signed char c);

/**
 * Pop a character from the head of the ring buffer buf and increment the head
 * return -1 and do nothing if the buffer is empty
 */
signed char ring_buf_pop(ring_buf_t* buf);

/**
 * increment an index in the given ring buffer
 */
uint8_t ring_buf_index_incr(ring_buf_t* buf, uint8_t i);

/**
 * decrement an index in the given ring buffer
 */
uint8_t ring_buf_index_decr(ring_buf_t* buf, uint8_t i);