add_library(pin_config STATIC ../config/pin_bus.c)
target_include_directories(pin_config PUBLIC ../config)

add_library(uart STATIC uart/uart.c)
target_include_directories(uart PUBLIC uart)

add_library(sram_gpio STATIC sram_gpio/sram_gpio.c)
target_include_directories(sram_gpio PUBLIC sram_gpio)
target_link_libraries(sram_gpio PUBLIC pin_config uart)

add_library(sram_read STATIC sram_read/sram_read.c)
target_include_directories(sram_read PUBLIC sram_read)
target_link_libraries(sram_read PUBLIC sram_gpio uart)

add_library(sram_write STATIC sram_write/sram_write.c)
target_include_directories(sram_write PUBLIC sram_write)
target_link_libraries(sram_write PUBLIC sram_gpio uart)

add_library(6502_ctrl STATIC 6502_ctrl/6502_ctrl.c)
target_include_directories(6502_ctrl PUBLIC 6502_ctrl)
target_link_libraries(6502_ctrl PUBLIC sram_gpio uart clock_ctrl)  

add_library(sram_ctrl STATIC sram_ctrl/sram_ctrl.c)
target_include_directories(sram_ctrl PUBLIC sram_ctrl)        
target_link_libraries(sram_ctrl PUBLIC 6502_ctrl)    

add_library(sram_ops STATIC sram_ops/sram_ops.c)
target_include_directories(sram_ops PUBLIC sram_ops)        
target_link_libraries(sram_ops PUBLIC sram_read sram_write pin_config uart)

add_library(clock_ctrl STATIC clock_ctrl/clock_ctrl.c)
target_include_directories(clock_ctrl PUBLIC clock_ctrl)        
target_link_libraries(clock_ctrl PUBLIC sram_gpio pin_config 6502_ctrl sram_ctrl)