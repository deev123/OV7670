#pragma once
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"

#define VS 6    // correct
#define HS 5    // correct
#define PLK 7   // correct
#define XLK 21
#define SDA 0
#define SCL 1

#define D0 8
#define D1 9
#define D2 10
#define D3 11
#define D4 12
#define D5 13
#define D6 14
#define D7 15

#define OV7670_I2C_ADDR 0x21
//#define DEBUG  //debug flag

extern uint8_t OV7670_frame_buf[64000];

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
 * Set bit[7] (MSB) of register 0x12 to 1 to reset all registers
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


/**
 * put the output from the sensors into the OV7670_frame_buf array
*/
void OV7670_capture_frame(uint16_t width, uint16_t height);


void OV7670_print_frame();

uint8_t OV7670_read_data_bus();