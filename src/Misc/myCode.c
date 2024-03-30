/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

/// \tag::hello_uart[]

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

int main() {
    stdio_init_all();
    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    char x;
    while(true)
    {
        x = uart_getc(UART_ID);
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        printf("%c\r\n", x);
        if(x == '1')
        {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
        }
        else if(x == '0')
        {
            
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
            
        }
        sleep_ms(1000);
        
    }
}

/// \end::hello_uart[]
