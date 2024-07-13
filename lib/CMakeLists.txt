add_library(helpers helpers.c helpers.h)
add_library(OV7670 OV7670.c OV7670.h)
add_library(ring_buf ring_buf.c ring_buf.h)
add_library(command command.c command.h)

target_link_libraries(helpers pico_stdlib)
target_link_libraries(OV7670 pico_stdlib hardware_i2c helpers)
target_link_libraries(ring_buf pico_stdlib)
target_link_libraries(command pico_stdlib ring_buf hardware_uart)
