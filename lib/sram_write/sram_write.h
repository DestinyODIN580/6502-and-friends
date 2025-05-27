#pragma once

/**
 * @file sram_write.h
 * @brief Low-level driver to write a byte to HM62256LP-70 SRAM
 *
 * This file contains the implementation of @ref data_write(),
 * which writes a single byte to external SRAM using the pin configuration
 * specified in @ref config/pin_bus.c.
 *
 * The module is part of the @ref sram_driver group, along with
 * `sram_read.c`, `sram_gpio.c`.
 *
 */

/**
 * @addtogroup sram_driver
 * @{
 */

 #include <stdint.h>

/**
 * @brief Writes a byte to the external SRAM.
 *
 * Operation sequence:
 * 1. Sets data bus to high-impedance (input) to prevent conflicts.
 * 2. Sets the 15-bit address on the address bus.
 * 3. Disable OE\_ (set high) to prevent unwanted reads.
 * 4. Assert CS\_ (Chip Select, active low).
 * 5. Assert WE\_ (Write Enable, active low).
 * 6. Puts the 8-bit data onto the data bus.
 * 7. De-assert WE\_ and CS\_.
 * 8. Restore data bus to high-impedance for safety.
 *
 * @param address 15-bit address (0x0000 – 0x7FFF) to write to.
 * @param byte    Data byte to write to the specified address.
 *
 * @warning This function assumes that the 6502 is currently in
  *          high z trough `6502_deactivate()` and that `_delay_us()`
  *          is accurate for the current clock frequency.
 *
 * @note To enable UART debug printing, define `DEBUG_WRITE` or `DEBUG_WRITE_PRINT`
 *       and uncomment the relevant code block at the end of the function.
 */
void data_write (uint16_t address, uint8_t byte);

/** @} */
