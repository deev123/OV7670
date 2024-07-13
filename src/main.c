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
#include "ring_buf.h"
#include "command.h"



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

    command_buf_init();

    //char x;
    while(true)
    {
        command_process();
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
