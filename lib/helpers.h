#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"


/**
 * Store the uint8_t `num` as binary in the c string `string`. `string` must be 9 indexes to fit the \0 null terminator. returns pointer to `string`
*/
char* helpers_byte_to_binary(uint8_t num, char* string);


/**
 * Substitute some special characters such ad \r etc to a different character to make easier to read when printed in a terminal
 */
void helpers_sub_special_chars(signed char* string, uint8_t len);


/**
 * Turn any lower case characters in the string to upper case
 */
void helpers_upper_case(signed char* string);


/**
 * Convert a character string into a uint8_t.
 * 0x or 0X indicates a hex integer
 * 0b or 0B indicates a binary integer
 * anything else is assumed to be a decimal integer
 */
uint8_t helpers_to_num(signed char* string);
