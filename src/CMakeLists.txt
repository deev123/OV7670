add_executable(OV7670_Camera main.c)

target_include_directories(OV7670_Camera PUBLIC ../lib)

target_link_libraries(OV7670_Camera pico_stdlib hardware_i2c OV7670 hardware_uart)

pico_enable_stdio_usb(OV7670_Camera 1)
pico_enable_stdio_uart(OV7670_Camera 0)

pico_add_extra_outputs(OV7670_Camera)
