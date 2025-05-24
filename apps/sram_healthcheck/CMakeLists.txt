add_executable(sram_healthcheck main.c)
target_link_libraries(sram_healthcheck
    PRIVATE
    sram_read
    sram_write
    sram_gpio
    pin_config
)

add_custom_command(TARGET sram_healthcheck POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:sram_healthcheck> ${PROJECT_BINARY_DIR}/sram_healthcheck.hex
  COMMENT "Generating .hex file for sram_healthcheck"
)