#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "ring_buf.h"


void ring_buf_init(ring_buf_t* buf, signed char* buffer, uint8_t buffer_size)
{
    buf->size = buffer_size;
    buf->tail = 0;
    buf->head = 0;
    buf->buf = buffer;
}


signed char ring_buf_push(ring_buf_t* buf, signed char c)
{
    buf->buf[buf->tail] = c;
    buf->tail++;
    if(buf->tail >= buf->size) buf->tail = 0;
    // return -1 if overflowed, otherwise echo c
    if(buf->tail == buf->head) return -1;
    else return c;
}


signed char ring_buf_pop(ring_buf_t* buf)
{
    if(buf->head != buf->tail)
    {
        char ret = buf->buf[buf->head];
        buf->head++;
        if(buf->head >= buf->size) buf->head = 0;
        return ret;
    }
    else return -1;
}


uint8_t ring_buf_index_incr(ring_buf_t* buf, uint8_t i)
{
    i++;
    if(i >= buf->size) i = 0;
    return i;
}


uint8_t ring_buf_index_decr(ring_buf_t* buf, uint8_t i)
{
    if(i <= 0) i = buf->size - 1;
    else i--;
    return i;
}
