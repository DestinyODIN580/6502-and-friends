add_library(pin_config STATIC ../config/pin_bus.c)
target_include_directories(pin_config PUBLIC ../config)

add_library(uart STATIC uart/uart.c)
target_include_directories(uart PUBLIC uart)

add_library(sram_gpio STATIC sram_gpio/sram_gpio.c)
target_include_directories(sram_gpio PUBLIC sram_gpio)
target_link_libraries(sram_gpio PUBLIC pin_config)

add_library(sram_read STATIC sram_read/sram_read.c)
target_include_directories(sram_read PUBLIC sram_read)
target_link_libraries(sram_read PUBLIC sram_gpio)

add_library(sram_write STATIC sram_write/sram_write.c)
target_include_directories(sram_write PUBLIC sram_write)
target_link_libraries(sram_write PUBLIC sram_gpio)
