#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "SH1106.h"
#include "myi2c.h"

// int main() {
//     // Enable UART so we can print status output
//         stdio_init_all();
//         myi2c_init(20, 21);

//     while(true)
//     {
//         uint8_t on_bytes[] =
//         {
//             0x00,
//             0x8D, // enable charge pump
//             0x14,
//             0xAF // display on
//         };
        
//         myi2c_start();

//         myi2c_send((0x3C<<1));  // this variable is 8 bits but the address is 7 so it needs to be bit shifted to add a read/write bit to the LSB and then we can use myi2c.send to sent the adress byte which includes the r/w bit

//         for(uint8_t i = 0; i < sizeof(on_bytes); i++)
//         {
//             myi2c_send(on_bytes[i]);
//         }

//         myi2c_stop();
        


//         sleep_ms(10000);
//     }





// }

int main() {
    // Enable UART so we can print status output
    stdio_init_all();
        i2c_init(i2c_default, 100 * 1000);
        gpio_set_function(20, GPIO_FUNC_I2C);
        gpio_set_function(21, GPIO_FUNC_I2C);
        // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
        // Make the I2C pins available to picotool
        bi_decl(bi_2pins_with_func(20, 21, GPIO_FUNC_I2C));
        

    while(true)
    {
        uint8_t on_bytes[] =
        {
            0x00,
            0x8D, // enable charge pump
            0x14,
            0xAF // display on
        };
        
        //myi2c_start();
        

        //myi2c_send((0x3C<<1));  // this variable is 8 bits but the address is 7 so it needs to be bit shifted to add a read/write bit to the LSB and then we can use myi2c.send to sent the adress byte which includes the r/w bit
        
        i2c_write_byte_raw(i2c_default, (0x3C<<1));

        for(uint8_t i = 0; i < sizeof(on_bytes); i++)
        {
            //myi2c_send(on_bytes[i]);
            i2c_write_byte_raw(i2c_default, on_bytes[i]);
        }

        //myi2c_stop();
        


        sleep_ms(3000);
    }





}