#include "OV7670.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "helpers.h"


// rx is for reg data from OV7670
// tx is for transmitted register addresses and data
uint8_t OV7670_SCCB_rx_buf[10];
uint8_t OV7670_SCCB_tx_buf[10];

//uint32_t total_pixels = 307200; // each pixel is two bytes
uint8_t OV7670_frame_buf[70800];      // 640x480 image is too large to store so we can only store part at a time
//uint8_t OV7670_frame_buf[64000]; //first 100 rows
uint32_t OV7670_current_pbyte;
uint32_t OV7670_frame_buf_size;  // for if not the whole buffer is used by the data


void OV7670_end_frame_buf()
{
    OV7670_current_pbyte += OV7670_frame_buf_size;
}

void OV7670_print_full_frame()
{

}



void OV7670_init()
{
    gpio_init(PLK);              gpio_init(VS);              gpio_init(HS);              //gpio_init(XLK);   //XLK is an input and is set with clock
    gpio_set_dir(PLK, GPIO_IN);  gpio_set_dir(VS, GPIO_IN);  gpio_set_dir(HS, GPIO_IN);  //gpio_set_dir(XLK, GPIO_OUT);

    // Set up D[0-7] for pixel data
    gpio_init(D0);  gpio_set_dir(D0, GPIO_IN);
    gpio_init(D1);  gpio_set_dir(D1, GPIO_IN);
    gpio_init(D2);  gpio_set_dir(D2, GPIO_IN);
    gpio_init(D3);  gpio_set_dir(D3, GPIO_IN);
    gpio_init(D4);  gpio_set_dir(D4, GPIO_IN);
    gpio_init(D5);  gpio_set_dir(D5, GPIO_IN);
    gpio_init(D6);  gpio_set_dir(D6, GPIO_IN);
    gpio_init(D7);  gpio_set_dir(D7, GPIO_IN);

    clock_gpio_init(XLK, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 5); // XLK clock set to sys / 5 (125MHz / 4 = 24MHz)

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    sleep_ms(1);

    #ifdef DEBUG
        if(OV7670_responsive()) printf("OV7670_init(): OV7670 acked to address 0x%02X on SCCB bus\n", OV7670_I2C_ADDR);
        else printf("OV7670_init(): OV7670 was unresponsive to address 0x%02X on SCCB bus. Check XLK input clock is correctly configured\n", OV7670_I2C_ADDR);
    #endif

}


void OV7670_reset_registers()
{
    OV7670_write_register(0x12, OV7670_read_register(0x12) | (0b10000000));  // set reset bit [7] (MSB) to reset all resgisters
}


void OV7670_set_registers()
{
    OV7670_write_register(0x11, (OV7670_read_register(0x11) & 0b10000000) | 0b00011111);  // scale clock /63 (slowest possible)   // TODO: Make it easier to set bits to 1s and 0s in same operation (maybe string like 01###100 to say which bits are on or off)
    // Add all configuration here...
    
    OV7670_write_register(0x12, (OV7670_read_register(0x12) & 0b11111010) | 0b00000100);  // rgb but not bayer
    

    OV7670_write_register(0x40, (OV7670_read_register(0x40) & 0b11001111) | 0b00010000);  // rgb 565

    //OV7670_output_8_bar_color_test_pattern();


    // OV7670_write_register(0x00, 0xFF); // gain
    // OV7670_write_register(0x02, 0xFF); // red channel gain max
}

void OV7670_output_8_bar_color_test_pattern()
{
    OV7670_write_register(0x12, (OV7670_read_register(0x12) & 0b11111101) | 0b00000010); // colour bar
    OV7670_write_register(0x70, (OV7670_read_register(0x70) & 0b11001111) | 0b00010000); // test pattern
    OV7670_write_register(0x42, (OV7670_read_register(0x42) & 0b11110111) | 0b00001000); // enable color bar
    OV7670_write_register(0x70, (OV7670_read_register(0x70) & 0b01111111) | 0b00000000); // test_pattern[0] colour bar
    OV7670_write_register(0x71, (OV7670_read_register(0x71) & 0b01111111) | 0b10000000); // test_pattern[1] colour bar
}


bool OV7670_responsive()
{
    int ret = i2c_read_blocking(i2c_default, OV7670_I2C_ADDR, OV7670_SCCB_rx_buf, 1, false);
    if (ret < 0) return false;
    else return true;
}


uint8_t OV7670_read_register(uint8_t reg_addr)
{
    // set the address to next read from to be reg_addr by writing just the address
    OV7670_SCCB_tx_buf[0] = reg_addr;
    i2c_write_blocking(i2c_default, OV7670_I2C_ADDR, OV7670_SCCB_tx_buf, 1, false);

    // read the value of the register into rx_buf
    i2c_read_blocking(i2c_default, OV7670_I2C_ADDR, OV7670_SCCB_rx_buf, 1, false);

    return OV7670_SCCB_rx_buf[0];
}


void OV7670_write_register(uint8_t reg_addr, uint8_t reg_val)
{
    OV7670_SCCB_tx_buf[0] = reg_addr;
    OV7670_SCCB_tx_buf[1] = reg_val;
    i2c_write_blocking(i2c_default, OV7670_I2C_ADDR, OV7670_SCCB_tx_buf, 2, false);
}


void OV7670_print_register(uint8_t reg_addr)
{  
    char binary_string[9];
    uint8_t val = OV7670_read_register(reg_addr);
    printf("Register 0x%02X = 0x%02X (0b%s)\n", reg_addr, val, helpers_byte_to_binary(val, binary_string));
}


void OV7670_print_all_registers()
{
    char binary_string[9];
    
    for(int i = 0; i < 0xC9; i++)
    {
        OV7670_print_register(i);
    }
}


uint8_t OV7670_read_data_bus()
{
    return (gpio_get_all() >> 8);  // TODO: This may be the wrong way round. If so change the wiring not this because the other way requires more instructions
}


void OV7670_quick_frame()  // scale argument for taking apicture using every second or every 4th pixel etc
{
    OV7670_frame_buf_size = 0;
    uint32_t pbyte = 0;

    while(!gpio_get(VS)); //wait for vsync high        // this is not very good because there is no way out of a failure here
    while(gpio_get(VS));  //wait for vsync low

    while(true) //while in the vsync low phase
    {   
        while(!gpio_get(HS))  //wait for href to go high for row. Vsync may go high while href is low to designate the start of a new frame
        {
            if(gpio_get(VS)) //vsync may go high while href is low signifying the end of a frame in which case we end the frame capturing function
            {
                OV7670_current_pbyte = 0;
                return;
            }
        }
        while(gpio_get(HS))  //while in href
        {
            while(!gpio_get(PLK));  // wait for pixel plk pulse

            //capture frame here
            if(pbyte >= OV7670_current_pbyte) //framebuf can start
            {
                //pixel is within the new framebuf so add onto framebuf if not full
                if(OV7670_frame_buf_size < sizeof(OV7670_frame_buf))
                {
                    if(((pbyte / 1280) % 4 == 0) && ((pbyte / 2) % 4 == 0))  // only add every other pixel of every other row to reduce size 1280 is 640 * 2 since every pixel is 2 bytes so a rowis 1280 bytes
                    {
                        OV7670_frame_buf[OV7670_frame_buf_size] = OV7670_read_data_bus();
                        OV7670_current_pbyte++;
                        OV7670_frame_buf_size++;
                    }
                }
                else // else framebuf is full so return and keep current OV7670_current_pbyte
                {
                    return;
                }
                
            }
            pbyte++;

            while(gpio_get(PLK)) //necessary to prevent reading the same pixel
            {
            }
        }
        

    }
}

// TODO: In progress...
void OV7670_capture_frame(uint16_t width, uint16_t height)     // TODO: Make work with for loops to allow it to exit from failed capture
{
    uint16_t column = 0;
    uint16_t row = 0;

    uint32_t index = 0;

    while(!gpio_get(VS)); //wait for vsync high        // this is not very good because there is no way out of a failure here
    while(gpio_get(VS));  //wait for vsync low

    while(true) //while in the vsync low phase
    {   

        while(!gpio_get(HS))  //wait for href to go high for row. Vsync may go high while href is low to designate the start of a new frame
        {
            if(gpio_get(VS)) return;  //vsync may go high while href is low signifying the end of a frame in which case we end the frame capturing function
        }
        while(gpio_get(HS))  //while in href
        {
            while(!gpio_get(PLK));  // wait for pixel plk pulse

            if(row % 4 > 1 && column % 2 > 0) //read pixel every second pixel (2 half pixels) and every second row 
            {
                //capture frame here
                if(index < sizeof(OV7670_frame_buf)) OV7670_frame_buf[index] = OV7670_read_data_bus();
                index++;
                
            }
            else
            {
                // TODO: wasted processing time here
            }

            while(gpio_get(PLK)) //wait to go low again
            {
            }
            column++;
        }
        row++;
        column = 0;
        

    }
    
}

void OV7670_get_next_frame_buf()
{
    OV7670_frame_buf_size = 0;
    uint32_t pbyte = 0;

    while(!gpio_get(VS)); //wait for vsync high        // this is not very good because there is no way out of a failure here
    while(gpio_get(VS));  //wait for vsync low

    while(true) //while in the vsync low phase
    {   
        while(!gpio_get(HS))  //wait for href to go high for row. Vsync may go high while href is low to designate the start of a new frame
        {
            if(gpio_get(VS)) //vsync may go high while href is low signifying the end of a frame in which case we end the frame capturing function
            {
                OV7670_current_pbyte = 0;
                return;
            }
        }
        while(gpio_get(HS))  //while in href
        {
            while(!gpio_get(PLK));  // wait for pixel plk pulse

            //capture frame here
            if(pbyte >= OV7670_current_pbyte) //framebuf can start
            {
                //pixel is within the new framebuf so ad onto framebuf if not full
                if(OV7670_frame_buf_size < sizeof(OV7670_frame_buf))
                {
                    OV7670_frame_buf[OV7670_frame_buf_size] = OV7670_read_data_bus();
                    OV7670_current_pbyte++;
                    OV7670_frame_buf_size++;
                }
                else // else framebuf is full so return and keep current OV7670_current_pbyte
                {
                    return;
                }
                
            }
            pbyte++;

            while(gpio_get(PLK)) //necessary to prevent reading the same pixel
            {
            }
        }
        

    }

}

void OV7670_print_frame_buf()
{
    #ifdef DEBUG
        printf("OV7670_print_frame(): Begin frame\n");
    #endif
    for(uint32_t i = 0; i < OV7670_frame_buf_size; i++)
    {
        printf("%02X", OV7670_frame_buf[i]);
    }
    #ifdef DEBUG
        printf("OV7670_print_frame(): End frame\n");
    #endif
}