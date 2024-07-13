#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "command.h"
#include "helpers.h"
#include <string.h>


signed char command_buffer[COMMAND_BUF_SIZE];
ring_buf_t command_buf;

void command_buf_init()
{
    ring_buf_init(&command_buf, command_buffer, COMMAND_BUF_SIZE);
    // assign the command function pointers here?
}


void command_parse(char* output_buf)
{
    uint8_t i = 0;
    while(command_buf.head != command_buf.tail)   // tail is the next char so this loop works
    {
        output_buf[i] = command_buf.buf[command_buf.head];
        command_buf.head = ring_buf_index_incr(&command_buf, command_buf.head);
        i++;
    }
    output_buf[i] = '\0'; // add the string terminator


}


void command_process()
{
    while (true) //get characters from the uart buffer until there are none left or a \r\n delimiter is reached
    {
        signed char c = getchar_timeout_us(0);
        if (c == -1) return;

        //if c is a backspace then decrement head
        if(c == '\b')
        {
            command_buf.tail = ring_buf_index_decr(&command_buf, command_buf.tail);
        }
        ring_buf_push(&command_buf, c);
        //printf("got[%c]\r\n", c);
        if(command_buf.buf[ring_buf_index_decr(&command_buf, command_buf.tail)] == '\r') //if the latest character is \n
        {
            //printf("\n");
            //printf("processing command...\r\n");
            signed char command[command_buf.size + 1];
            command_parse(command);
            command_execute(command);
            //command is now a normal c string with just the command
            //printf("entire buffer:\r\n[\r\n");
            // for(uint8_t i = 0; i < command_buf.size; i++)
            // {
            //     printf("%c\r\n", command_buf.buf[i]);
            // }
            // printf("]\r\n");
            helpers_sub_special_chars(command, command_buf.size + 1);
            //printf("received command \"%s\"\r\n", command);

            //TODO: edit out backspace characters
            //excecute command here
            
        }
    }	

    //see if any commmand is in commandbuf
    //x = getchar();
}


void command_execute(signed char* cmd)
{
    uint8_t i = 0;

    // get the first part of the command: the command name
    char command_name[15];
    while(cmd[i] != '=' && cmd[i] != '\r')
    {
        command_name[i] = cmd[i];
        i++;
    }
    command_name[i] = '\0';

    if(strcmp(command_name, "AT") == 0)
    {
        command_respond_ok();
    }

    // find a match for the command in the command list

    // work out how many arguments to expect

    //parse the arguments

    //call the function for the command




}


void command_respond_ok()
{
    printf("OK\r\n");
}


void command_set_register(uint8_t reg_addr, uint8_t value)
{

}


void command_read_register(uint8_t reg_addr)
{

}


void command_picture_default()
{

}