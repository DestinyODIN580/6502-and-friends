add_executable(debug main.c)
target_link_libraries(debug
    PRIVATE
    sram_read
    sram_write
    sram_gpio
    pin_config
    uart
)

add_custom_command(TARGET debug POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:debug> ${PROJECT_BINARY_DIR}/debug.hex
  COMMENT "Generating .hex file for debug"
)