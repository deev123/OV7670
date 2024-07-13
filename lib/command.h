#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "ring_buf.h"


#define COMMAND_BUF_SIZE 50

/**
 * Call this to initialise the command buffer
 */
void command_buf_init();


/**
 * Go through the uart buffer until a command delimiter <CR><LF> is reaches indicating a whole command is in the buffer
 */
void command_process();

/**
 * UNSAFE! output_buf should be 1 index larger than command_buf to ensure it can never overflow (to accommodate string \0 terminator)
 */
void command_parse(char* output_buf);