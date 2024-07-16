#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "command.h"
#include "helpers.h"
#include <string.h>
#include "OV7670.h"


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

        // TODO: filter out characters we dont want like invisible or special characters other than \r etc?

        //if c is a backspace then decrement head
        if(c == '\b' || c == 127)  //127 is the delete character which putty uses by default
        {
            if(command_buf.tail == command_buf.head)
            {
                command_buf.tail = command_buf.head = ring_buf_index_decr(&command_buf, command_buf.tail);
            }
            else
            {
                command_buf.tail = ring_buf_index_decr(&command_buf, command_buf.tail);
            }
            return;
        }

        ring_buf_push(&command_buf, c);
        //printf("got[%c]\r\n", c);
        if(command_buf.buf[ring_buf_index_decr(&command_buf, command_buf.tail)] == '\r') //if the latest character is \n
        {
            //printf("\n");
            //printf("processing command...\r\n");
            signed char command[command_buf.size + 1];
            command_parse(command);
            helpers_upper_case(command);
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
    // uint8_t i = 0;

    // // get the first part of the command: the command name
    // char command_name[15];
    // while(cmd[i] != '=' && cmd[i] != '\r')
    // {
    //     command_name[i] = cmd[i];
    //     i++;
    // }
    // command_name[i] = '\0';
    char delims[] = " \r";

    char copy[strlen(cmd) + 1];
    char* token;

    // print all the interpreted tokens for debugging purposes:
    // strcpy(copy, cmd);
    // token = strtok(copy, delims);
    // printf("tokens: ");
    // while(token != NULL)
    // {
    //     printf("[%s]", token);
    //     token = strtok(NULL, delims);
    // }
    // printf("\r\n");

    strcpy(copy, cmd);
    token = strtok(copy, delims);

    // execute:

    if(strcmp(token, "AT") == 0)
    {
        command_respond_ok();
    }

    else if(strcmp(token, "GETREG") == 0)
    {
        uint8_t reg = 0;
        token = strtok(NULL, delims);
        if(token != NULL)
        {
            reg = helpers_to_num(token);
        }
        printf("%u", OV7670_read_register(reg));
    }

    else if(strcmp(token, "SETREG") == 0)
    {
        uint8_t reg = 0;
        token = strtok(NULL, delims);
        if(token != NULL)
        {
            reg = helpers_to_num(token);
        }
        
        uint8_t value = 0;
        token = strtok(NULL, delims);
        if(token != NULL)
        {
            value = helpers_to_num(token);
        }

        //printf("setting register %d to %d\r\n", reg, value);

        OV7670_write_register(reg, value);
        printf("%u", OV7670_read_register(reg));

        // respond something like: register x = x
        //command_respond_ok();
    }

    else if(strcmp(token, "PIC") == 0)
    {
        //printf("\r\n\r\n");
        OV7670_quick_frame();
        //OV7670_get_next_frame_buf();
        OV7670_print_frame_buf();
        // OV7670_get_next_frame_buf();
        // OV7670_print_frame_buf();
        // 0 args is default
        // otherwise the argument is a variation of the function
        
    }

    // find a match for the command in the command list

    // work out how many arguments to expect

    //parse the arguments

    //call the function for the command




}


void command_respond_ok()
{
    printf("OK\r");
}


// void command_set_register(token)
// {

// }


// void command_read_register(uint8_t reg_addr)
// {

// }


// void command_picture_default()
// {

// }