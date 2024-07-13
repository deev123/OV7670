#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"


char* helpers_byte_to_binary(uint8_t num, char* string)
{
    for(uint8_t i = 0; i <= 7; i++)
    {
        string[7-i] = ((num & (1 << i))? '1' : '0');
    }
    string[8] = '\0';
    return string;
}

void helpers_sub_special_chars(signed char* string, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        if(string[i] == '\r') string[i] = '^';
        else if(string[i] == '\n') string[i] = '*';
        else if(string[i] == '\b') string[i] = '<';
    }
}