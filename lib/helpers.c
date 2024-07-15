#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
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
        else if(string[i] == '\b' || string[i] == 127) string[i] = '<';
    }
}


void helpers_upper_case(signed char* string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        string[i] = toupper(string[i]);
        i++;
    }
}


uint8_t helpers_to_num(signed char* string)
{
    uint8_t size = strlen(string);
    long int ret = 0;

    if(size >= 2 && string[0] == '0')
    {
        if(string[1] == 'X' || string[1] == 'x')
        {
            // hex
            ret = strtol(&string[2], NULL, 16);
            return (uint8_t)ret;
        }
        else if(string[1] == 'B' || string[1] == 'b')
        {
            // binary
            ret = strtol(&string[2], NULL, 2);
            return (uint8_t)ret;
        }
    }
    else    // interpret as decimal int
    {
        ret = strtol(string, NULL, 10);
        return (uint8_t)ret;
    }
}