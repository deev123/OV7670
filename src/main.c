#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "OV7670.h"
#include "helpers.h"


#define RING_BUF_SIZE 10

typedef struct ring_buf_t
{
    uint8_t size;
    uint8_t tail;
    uint8_t head;
    char buf[RING_BUF_SIZE];
}  ring_buf_t;

ring_buf_t command_buf = {RING_BUF_SIZE, 0, 0, {RING_BUF_SIZE}};
// command_buf.size = 20;
// command_buf.tail = 0;
// command_buf.head = 0;
// command_buf.buf = char[20];

const char* commands[] =
{
"AT\r\n",
"AT+GREET\r\n"
};

//void match_command

/**
 * Add char c to the ring buffer and increment tail index
 */
void append_buf(ring_buf_t* buf, char c)
{
    buf->buf[buf->tail] = c;
    buf->tail++;
    if(buf->tail >= buf->size) buf->tail = 0;
}

// /**
//  * return the character the given offset from the tail
//  */
// int buf_move(ring_buf_t* buf, int index)
// {

// }

/**
 * increment an index in the given ring buffer
 */
uint8_t index_incr(ring_buf_t* buf, uint8_t i)
{
    i++;
    if(i >= buf->size) i = 0;
    return i;
}

/**
 * decrement an index in the given ring buffer
 */
uint8_t index_decr(ring_buf_t* buf, uint8_t i)
{
    if(i <= 0) i = buf->size - 1;
    else i--;
    return i;
}

/**
 * UNSAFE! output_buf should be 1 bigger (51) than command_buf to ensure it never goes beyond (to accommodate string \0 terminator)
 */
void parse_command(char* output_buf)
{
    // char[20] command;
    // go backwards until the previous \n
    uint8_t i = 0;
    while(command_buf.head != command_buf.tail && i < RING_BUF_SIZE)   // tail is the next char so this loop works
    {
        output_buf[i] = command_buf.buf[command_buf.head];
        command_buf.head = index_incr(&command_buf, command_buf.head);
        i++;
    }
    output_buf[i] = '\0'; // add the string terminator


}

/**
 * Go through the uart buffer until a command delimiter <CR><LF> is reaches indicating a whole command is in the buffer
 */
void process_command()
{
    while (true/*uart_is_readable(uart0)*/)
    {
        signed char c = getchar_timeout_us(0);
        //printf("reading");
        if (c == -1) return;
        //return;

        //char c = getchar();
        append_buf(&command_buf, c);
        printf("got[%c]\r\n", c);
        if(command_buf.buf[index_decr(&command_buf, command_buf.tail)] == '\n') //if the latest character is \n
        {
            printf("processing command:\r\n");
            char command[RING_BUF_SIZE + 1];
            parse_command(command);
            //command is now a normal string with just the command
            printf("entire buffer:\r\n[\r\n");
            for(uint8_t i = 0; i < RING_BUF_SIZE; i++)
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



void count_plks_in_hrefs_in_vsync()
{
    uint32_t vsyncs = 0;
    uint32_t hrefs = 0;
    uint32_t pclcks = 0;

    while(vsyncs == 0)
    {
        while(!gpio_get(VS)) //wait for vsync high
        {
        }
        while(gpio_get(VS)) //wait for vsync low
        {
        }
        vsyncs++;

        while(true) //while in the vsync low phase
        {   

            while(!gpio_get(HS))  //wait for href to go high for row. Vsync may go high while hsync is low to designate the start of a new frame
            {
                if(gpio_get(VS)) goto vsync_escape;  //vsync may go high while href is low signifying the end of a frame in which case we exit the frame capturing
            }
            while(gpio_get(HS))  //while in href
            {
                while(!gpio_get(PLK))
                {
                }
                pclcks++;
                while(gpio_get(PLK)) //wait to go low again
                {
                }
            }
            hrefs++;
            //printf("End of row. hrefs: %d\n", hrefs);
            

        }
        vsync_escape:
        printf("End of frame. %d vsyncs, %d hrefs, %d plks\n", vsyncs, hrefs, pclcks);
    }
}





int main()
{
    stdio_init_all();
    //sleep_ms(5000);
    OV7670_init();
    OV7670_set_registers();
    //sleep_ms(1000);
    //OV7670_print_all_registers();
    //sleep_ms(1000);

    // Configure uart from usb for comms

    //char x;
    while(true)
    {
        process_command();
        // x = getchar();
        // printf("%c", x);
    }


    // printf("\n\n\nresetting registers\n\n");
    // sleep_ms(2000);
    // OV7670_reset_registers();
    // OV7670_print_all_registers();

    






    // int32_t vsyncs = 0;
    // int32_t hrefs = 0;
    // int32_t pclcks = 0;
    // int32_t nvsyncs = 0;
    // int32_t nhrefs = 0;
    // int32_t npclcks = 0;

    // while(true)
    // {
    //     count_plks_in_hrefs_in_vsync();
    //     sleep_ms(1000);

    // }
    sleep_ms(5000);
    // OV7670_capture_frame(0,0);



    //command processor:





    //below temporarily comented
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();



    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();
    // OV7670_get_next_frame_buf();
    // OV7670_print_frame_buf();

    // uint num = 0;
    // while(true)
    // {
    //     printf("Hello world %d", num);
    //     num++;
    //     sleep_ms(2000);
    // }
   

    // while(true)
    // {
    //     // OV7670_capture_frame(0,0);
    //     // OV7670_print_frame();
    //     //_read_half_pixel();
    //     sleep_ms(1000);
    //     //printf("\npixel: %d%d", OV7670_frame_buf[0], OV7670_frame_buf[1]);
    // }
    
    //printf("Done.\n");
    return 0;

}
