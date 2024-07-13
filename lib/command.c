#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "command.h"

const char* commands[] =
{
"AT\r\n",
"AT+GREET\r\n"
};

signed char command_buffer[COMMAND_BUF_SIZE];
ring_buf_t command_buf;

void command_buf_init()
{
    ring_buf_init(&command_buf, command_buffer, COMMAND_BUF_SIZE);
}


void command_parse(char* output_buf)
{
    // char[20] command;
    // go backwards until the previous \n
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

        ring_buf_push(&command_buf, c);
        printf("got[%c]\r\n", c);
        if(command_buf.buf[ring_buf_index_decr(&command_buf, command_buf.tail)] == '\n') //if the latest character is \n
        {
            printf("processing command:\r\n");
            signed char command[command_buf.size + 1];
            command_parse(command);
            //command is now a normal c string with just the command
            printf("entire buffer:\r\n[\r\n");
            for(uint8_t i = 0; i < command_buf.size; i++)
            {
                printf("%c\r\n", command_buf.buf[i]);
            }
            printf("]\r\n");
            
            printf("got command[%s]\r\n", command);
            
        }
    }	

    //see if any commmand is in commandbuf
    //x = getchar();
}
