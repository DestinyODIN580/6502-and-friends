cmake_minimum_required(VERSION 3.15)
project(sram_project C)

# ————— settings —————
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/avr-toolchain.cmake)

set(FLASH_DIR "${CMAKE_SOURCE_DIR}/avr_bin")
file(MAKE_DIRECTORY "${FLASH_DIR}")

# ————— libraries —————
add_library(sram_gpio  lib/sram_gpio/sram_gpio.c)
target_include_directories(sram_gpio  PUBLIC lib/sram_gpio config)
target_link_libraries(sram_gpio  PRIVATE uart)

add_library(sram_read  lib/sram_read/sram_read.c)
target_include_directories(sram_read  PUBLIC lib/sram_read)
target_link_libraries(sram_read  PRIVATE sram_gpio uart)

add_library(sram_write lib/sram_write/sram_write.c)
target_include_directories(sram_write PUBLIC lib/sram_write)
target_link_libraries(sram_write PRIVATE sram_gpio)

add_library(6502_ctrl lib/6502_ctrl/6502_ctrl.c)
target_include_directories(6502_ctrl PUBLIC lib/6502_ctrl)
target_link_libraries(6502_ctrl PUBLIC sram_gpio uart)  

add_library(sram_ctrl lib/sram_ctrl/sram_ctrl.c)
target_include_directories(sram_ctrl PUBLIC lib/sram_ctrl)        
target_link_libraries(sram_ctrl PUBLIC 6502_ctrl)    

add_library(pin_config config/pin_bus.c)
target_include_directories(pin_config PUBLIC config)

add_library(uart lib/uart/uart.c)
target_include_directories(uart PUBLIC lib/uart)

# ————— executables —————
add_executable(sram_hexdump      apps/sram_hexdump/main.c)
add_executable(sram_healthcheck  apps/sram_healthcheck/main.c)
add_executable(sram_injector     apps/sram_injector/main.c)
add_executable(debug             apps/debug/main.c)


foreach(tgt IN ITEMS sram_hexdump sram_healthcheck sram_injector debug)
  target_link_libraries(${tgt} PRIVATE sram_gpio sram_read sram_write pin_config uart 6502_ctrl sram_ctrl)
endforeach()

add_custom_target(hex ALL
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom
          $<TARGET_FILE:sram_hexdump>      ${FLASH_DIR}/sram_hexdump.hex
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom
          $<TARGET_FILE:sram_healthcheck>  ${FLASH_DIR}/sram_healthcheck.hex
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom
          $<TARGET_FILE:sram_injector>     ${FLASH_DIR}/sram_injector.hex
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom
          $<TARGET_FILE:debug> ${FLASH_DIR}/debug.hex
  DEPENDS sram_hexdump sram_healthcheck sram_injector debug
)
