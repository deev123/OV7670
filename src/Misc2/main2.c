#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/clocks.h"


#define PLK 7
#define VS 6
#define HS 5
#define XLK 21
#define XLK_FREQUENCY 24000000  // XLK is 24MHz
#define SDA 0
#define SCL 1

// D0-D7 are GPIO 15,14,13,12,11,10,9,8s

void init_pins(void)
{
    clocks_init();

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);

    gpio_init(PLK);              gpio_init(VS);              gpio_init(HS);              gpio_init(XLK);
    gpio_set_dir(PLK, GPIO_IN); gpio_set_dir(VS, GPIO_IN); gpio_set_dir(HS, GPIO_IN); gpio_set_dir(XLK, GPIO_OUT);


    uint clk_div = clock_get_hz(clk_sys) / (2 * XLK_FREQUENCY);
    clock_gpio_init(XLK, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, clk_div);


}

int main()
{
    stdio_init_all();
    init_pins();




    while(true)
    {
        uint32_t hsyncs = 0;
        uint32_t plk = 0;
        //count how many pixels in hsyc, and how many hsyncs within a vsync

        printf("Hello World");

        sleep_ms(5000);
    }
}