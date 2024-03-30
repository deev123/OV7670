/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Sweep through all 7-bit I2C addresses, to see if any slaves are present on
// the I2C bus. Print out a table that looks like this:
//
// I2C Bus Scan
//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
// 0
// 1       @
// 2
// 3             @
// 4
// 5
// 6
// 7
//
// E.g. if slave addresses 0x12 and 0x34 were acknowledged.

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include "OV7670.h"


#define VS 6    // correct
#define HS 5    // correct
#define PLK 7   // correct
//#define XLK 21
//#define XLK_FREQUENCY 24000000  // XLK is 24MHz
#define SDA 0
#define SCL 1

// D0-D7 are GPIO 15,14,13,12,11,10,9,8s

void init_pins(void)
{
    //clocks_init();

    //i2c_init(i2c_default, 100 * 1000);
    //gpio_set_function(SDA, GPIO_FUNC_I2C);
    //gpio_set_function(SCL, GPIO_FUNC_I2C);

    gpio_init(PLK);              gpio_init(VS);              gpio_init(HS);              //gpio_init(XLK);   //XLK is an input and is set with clock
    gpio_set_dir(PLK, GPIO_IN); gpio_set_dir(VS, GPIO_IN); gpio_set_dir(HS, GPIO_IN); //gpio_set_dir(XLK, GPIO_OUT);


    //uint clk_div = clock_get_hz(clk_sys) / (2 * XLK_FREQUENCY);
    //clock_gpio_init(XLK, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, clk_div);


}




// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

int main() {
    // Enable UART so we can print status output
    stdio_init_all();


    init_pins();

    sleep_ms(3000);

    clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS/*CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0*/, 4);   //5 minimum




    //uint32_t div = sys_freq/1;
    // configure clock. Pin 21 is a clock pin

    printf("CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS = %dkHz\n", frequency_count_mhz(CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS));

    // uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    // uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    // uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    // uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    // uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    // uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    // uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    // uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

    // printf("pll_sys = %dkHz\n", f_pll_sys);
    // printf("pll_usb = %dkHz\n", f_pll_usb);
    // printf("rosc = %dkHz\n", f_rosc);
    // printf("clk_sys = %dkHz\n", f_clk_sys);
    // printf("clk_peri = %dkHz\n", f_clk_peri);
    // printf("clk_usb = %dkHz\n", f_clk_usb);
    // printf("clk_adc = %dkHz\n", f_clk_adc);
    // printf("clk_rtc = %dkHz\n", f_clk_rtc);

    // CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLKSRC_GPIN0 = 125000kHz
    // pll_sys = 125000kHz
    // pll_usb = 48000kHz
    // rosc = 5464kHz
    // clk_sys = 125000kHz
    // clk_peri = 125000kHz
    // clk_usb = 48000kHz
    // clk_adc = 48000kHz
    // clk_rtc = 47kHz





    // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);
    // Make the I2C pins available to picotool
    //bi_decl(bi_2pins_with_func(0, 1, GPIO_FUNC_I2C));

    uint8_t ADDRESS = 0x21;


    // make sure of address 0x21
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    for (int addr = 0; addr < (1 << 7); ++addr)
    {
        if (addr % 16 == 0)
        {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }

    sleep_ms(3000);



    
    int32_t vsyncs = 0;
    int32_t hrefs = 0;
    int32_t pclcks = 0;

    while(true)
    {
        vsyncs = 0;
        hrefs = 0;
        pclcks = 0;

        while(vsyncs == 0)
        {
            while(!gpio_get(VS)) //wait for a vsync
            {
            }
            vsyncs++;
            while(gpio_get(VS)) //wait to go low again
            {
            }

            while(!gpio_get(VS)) //while in the vsync low phase
            {   

                // while(!gpio_get(HS))  //wait for hsync to go high for row
                // {
                // }
                // hrefs++;
                while(gpio_get(HS))  //while in hsync
                {
                    while(!gpio_get(PLK))
                    {
                    }
                    pclcks++;
                    while(gpio_get(PLK)) //wait to go low again
                    {
                    }
                }

                

            }



            
        }

    printf("number of vsyncs: %d\n", vsyncs);
    printf("number of hsyncs: %d\n", hrefs);
    printf("number of pclcks: %d\n", pclcks);
    //printf("hi\n");

    sleep_ms(2000);

    }
    
    printf("Done.\n");
    return 0;

}
