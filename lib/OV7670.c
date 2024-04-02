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


void OV7670_init()
{
    gpio_init(PLK);              gpio_init(VS);              gpio_init(HS);              //gpio_init(XLK);   //XLK is an input and is set with clock
    gpio_set_dir(PLK, GPIO_IN);  gpio_set_dir(VS, GPIO_IN);  gpio_set_dir(HS, GPIO_IN);  //gpio_set_dir(XLK, GPIO_OUT);

    clock_gpio_init(21, CLOCKS_CLK_GPOUT0_CTRL_AUXSRC_VALUE_CLK_SYS, 5); // XLK clock set to sys / 4 (125MHz / 4 = 31)  (/5 would be 25MHz (just over the minimum speed))

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    sleep_ms(1);

    #ifdef DEBUG
        if(OV7670_responsive()) printf("OV7670 acked to address 0x%02X on SCCB bus\n", OV7670_I2C_ADDR);
        else printf("OV7670 was unresponsive to address 0x%02X on SCCB bus. Check XLK input clock is correctly configured\n", OV7670_I2C_ADDR);
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