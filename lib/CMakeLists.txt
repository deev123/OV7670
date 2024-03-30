add_library(OV7670 OV7670.c OV7670.h)

target_link_libraries(OV7670 pico_stdlib hardware_i2c)