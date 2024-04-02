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


int main()
{
    stdio_init_all();
    sleep_ms(5000);
    OV7670_init();
    OV7670_set_registers();
    sleep_ms(1000);
    OV7670_print_all_registers();
    sleep_ms(1000);



    // printf("\n\n\nresetting registers\n\n");
    // sleep_ms(2000);
    // OV7670_reset_registers();
    // OV7670_print_all_registers();

    






    int32_t vsyncs = 0;
    int32_t hrefs = 0;
    int32_t pclcks = 0;
    int32_t nvsyncs = 0;
    int32_t nhrefs = 0;
    int32_t npclcks = 0;

    while(true)
    {
        vsyncs = 0;
        hrefs = 0;
        pclcks = 0;
        nvsyncs = 0;
        nhrefs = 0;
        npclcks = 0;

        // while(vsyncs == 0)
        // {
        //     while(!gpio_get(VS)) //wait for a vsync
        //     {
        //     }
        //     vsyncs++;
        //     while(gpio_get(VS)) //wait to go low again
        //     {
        //     }

        //     while(!gpio_get(VS)) //while in the vsync low phase
        //     {   

        //         // while(!gpio_get(HS))  //wait for hsync to go high for row
        //         // {
        //         // }
        //         // hrefs++;
        //         while(gpio_get(HS))  //while in hsync
        //         {
        //             while(!gpio_get(PLK))
        //             {
        //             }
        //             pclcks++;
        //             while(gpio_get(PLK)) //wait to go low again
        //             {
        //             }
        //         }

                

        //     }
        // while(!gpio_get(VS))
        // {
        //     if(gpio_get(HS)) hrefs++; else nhrefs++;
        //     if(gpio_get(PLK)) pclcks++; else npclcks++;
        // }


            
        

        // printf("number of vsyncs: %d\n", vsyncs);
        // printf("number of hsyncs: %d\n", hrefs);
        // printf("number of pclcks: %d\n", pclcks);
        // printf("number of not vsyncs: %d\n", nvsyncs);
        // printf("number of not hsyncs: %d\n", nhrefs);
        // printf("number of not pclcks: %d\n", npclcks);
        // //printf("hi\n");

        // sleep_ms(2000);

    }
    
    printf("Done.\n");
    return 0;

}
