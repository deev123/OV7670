#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"


/**
 * Store the uint8_t `num` as binary in the c string `string`. `string` must be 9 indexes to fit the \0 null terminator. returns pointer to `string`
*/
char* helpers_byte_to_binary(uint8_t num, char* string);

void helpers_sub_special_chars(signed char* string, uint8_t len);