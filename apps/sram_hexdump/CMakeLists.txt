add_executable(sram_hexdump main.c)
target_link_libraries(sram_hexdump
    PRIVATE
    sram_read
    sram_write
    sram_gpio
    pin_config
)

add_custom_command(TARGET sram_hexdump POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:sram_hexdump> ${PROJECT_BINARY_DIR}/sram_hexdump.hex
  COMMENT "Generating .hex file for sram_hexdump"
)