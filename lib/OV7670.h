#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"

#define VS 6    // correct
#define HS 5    // correct
#define PLK 7   // correct
//#define XLK 21
//#define XLK_FREQUENCY 24000000  // XLK is 24MHz
#define SDA 0
#define SCL 1
// D0-D7 will be GPIO 15,14,13,12,11,10,9,8s ....

#define OV7670_I2C_ADDR 0x21

#define DEBUG  //debug flag

/**
 * Call this before any other function from OV7670 library
 * 
 * Initiates with:
 * 
 * SDA: GPIO_0
 * SCL: GPIO_1
 * VS: GPIO_6
 * HS: GPIO_5
 * PLK: GPIO_7
 * XLK: GPIO_21 (and outputting 31MHz signal)
 * 
*/
void OV7670_init();


/**
 * Set bit[7] (LSB) of register 0x12 to 1 to reset all registers
*/
void OV7670_reset_registers();


/**
 * Set various registers from default to configurations we want
*/
void OV7670_set_registers();


/**
 * Attempt to read from the OV7670 SCCB bus.
 * 
 * Returns `true` if acked, `false` if not
*/
bool OV7670_responsive();


/**
 * Read the value of register `reg_addr` over SCCB bus
 * #### *Requires XLK clock input to the OV7670 which is set to output from GPIO 21 by OV7670_init()
*/
uint8_t OV7670_read_register(uint8_t reg_addr);


/**
 * Write to register `reg_addr` the value `reg_val` over SCCB bus
 * #### *Requires XLK clock input to the OV7670 which is set to output from GPIO 21 by OV7670_init()
*/
void OV7670_write_register(uint8_t reg_addr, uint8_t reg_val);


/**
 * print the value of register `reg_addr`
*/
void OV7670_print_register(uint8_t reg_addr);


/**
 * print the value of every register from 0 to 0xC9
*/
void OV7670_print_all_registers();