add_executable(sram_injector main.c)
target_link_libraries(sram_injector
    PRIVATE
    sram_read
    sram_write
    sram_gpio
    pin_config
    uart
    6502_ctrl
    sram_ctrl
)

add_custom_command(TARGET sram_injector POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:sram_injector> ${PROJECT_BINARY_DIR}/sram_injector.hex
  COMMENT "Generating .hex file for sram_injector"
)