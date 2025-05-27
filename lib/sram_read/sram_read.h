#pragma once

/**
 * @file sram_read.h
 * @brief Low-level driver to read from an HM62256LP-70 SRAM.
 *
 * Contains the implementation of @ref data_read() which reads a
 * single byte from an external SRAM using the pin configuration
 * specified in @ref config/pin_bus.c.
 *
 * This module is part of the @ref sram_driver group, together with
 * `sram_write.c`, `sram_gpio.c`
 */

/**
 * @defgroup sram_driver SRAM Driver
 * @brief Group containing all functions for SRAM access.
 * @{
 */

#include <stdint.h>

#define DEBUG_READ 0
#define DEBUG_READ_PRINT 0

/**
 * @brief Reads a byte from the external SRAM.
 *
 * Operation sequence:
 * 1. Sets the eight data lines as input.  
 * 2. Puts the 15-bit address on the address bus.  
 * 3. Sets CS\_ (Chip Select) to low level.  
 * 4. Sets OE\_ (Output Enable) to low level.  
 * 5. Sample the data lines.  
 * 6. Release the control lines.
 *
 * @param address 15-bit address (0 – 0x7FFF) to read from.
 * @return Byte present at the specified @p address.
 *
 * @warning This function assumes that the 6502 is currently in
 *          high z trough `6502_deactivate()` and that `_delay_us()`
 *          is accurate for the current clock frequency.
 *
 * @note    For UART debugging, define `DEBUG_READ` or `DEBUG_READ_PRINT`
 *          and uncomment the block at the end of the function.
 */
uint8_t data_read (uint16_t address);

/** @} */
