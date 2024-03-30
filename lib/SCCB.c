
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "myi2c.h"

// You may change the communication speed here
//#define I2C_DELAY delayMicroseconds(3) //  68kHz@80Mhz or  83kHz@160Mhz
//#define I2C_DELAY delayMicroseconds(2) //  85kHz@80Mhz or 110kHz@160Mhz
  #define I2C_DELAY sleep_us(3) // 110kHz@80Mhz or 170kHz@160Mhz
//#define I2C_DELAY delayMicroseconds(0) // 150kHz@80Mhz or 300kHz@160Mhz
//#define I2C_DELAY                      // 250kHz@80Mhz or 500kHz@160Mhz
//doesnt work for some reason... timing or ack maybe? or pin configuration

uint8_t sda = 20;
uint8_t scl = 21;

void myi2c_init(uint8_t pSda, uint8_t pScl)
{
    sda = pSda;
    scl = pScl;
    //gpio_set_function(sda, GPIO_FUNC_PIO0);
    //gpio_set_function(scl, GPIO_FUNC_PIO0);
    //gpio_set_function(sda, GPIO_OUT);
    gpio_init(sda);
    gpio_init(scl);
    gpio_set_dir(sda, GPIO_IN);  
    gpio_set_dir(scl, GPIO_IN);
    //gpio_pull_up(sda);   // pullups may not be right value
    //gpio_pull_up(scl);
    //gpio_pull_down(sda);
    //gpio_pull_down(scl);
    gpio_put(sda, 0);
    gpio_put(scl, 0);
    I2C_DELAY;

    //sleep_ms(10000);
}

//i2c start bit
void myi2c_start()
{
    gpio_set_dir(sda, GPIO_IN);
    I2C_DELAY;
    gpio_set_dir(scl, GPIO_IN);
    I2C_DELAY;
    gpio_set_dir(sda, GPIO_OUT);
    I2C_DELAY;
    gpio_set_dir(scl, GPIO_OUT);
    I2C_DELAY;
}

//i2c stop bit
void myi2c_stop()
{
    gpio_set_dir(sda, GPIO_OUT);
    I2C_DELAY;
    gpio_set_dir(scl, GPIO_IN);
    I2C_DELAY;
    gpio_set_dir(sda, GPIO_IN);
    I2C_DELAY;
}

bool myi2c_send(uint8_t pByte)
{
    //printf("%x", pByte);
    for (uint8_t i = 8; i > 0; i--) {
        //gpio_put(sda, (pByte & (1 << (i - 1))) ? 1 : 0);
        if(pByte & (1 << (i - 1)))
        {
            gpio_set_dir(sda, GPIO_IN);
        }
        else
        {
            gpio_set_dir(sda, GPIO_OUT);
        }

        I2C_DELAY;
        gpio_set_dir(scl, GPIO_IN);
        I2C_DELAY;
        gpio_set_dir(scl, GPIO_OUT);
        I2C_DELAY;
    }
    gpio_set_dir(sda, GPIO_IN);
    I2C_DELAY;
    gpio_set_dir(scl, GPIO_IN);
    I2C_DELAY;
    gpio_set_dir(scl, GPIO_OUT);
}

void myi2c_write_bytes(uint8_t pAddr, uint8_t *pArray, uint8_t pArraySize)
 {
    myi2c_start();
    myi2c_send(pAddr);
    for(uint8_t i = 0; i < pArraySize; i++)
    {
        myi2c_send(pArray[i]);
    }
    myi2c_stop();
 }