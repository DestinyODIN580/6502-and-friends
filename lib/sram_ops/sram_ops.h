#pragma once

#include <stdint.h>

/**
 * @defgroup SRAM_Operations SRAM operations
 * @brief Functions for editing, injecting, and visualizing SRAM data.
 * @{
 */

/**
 * @brief Dumps the contents of the SRAM in a formatted hexadecimal view.
 *
 * This function reads the RAM within specified range and outputs it via UART
 * alongside an optional label.
 * @param start_addr start address where to read
 * @param end_addr end address where to read
 * @param label label to display, if null it's ignored
 */
void _sram_hexdump_range (uint16_t, uint16_t, const char *);

/**
 * @brief Dumps the contents of the SRAM where the program is supposed to be stored
 * (0x0200 as of now).
 *
 * Wrapper to @ref _sram_hexdump_range.
 */
void sram_hexdump_program ();

/**
 * @brief Dumps the contents of the SRAM where the stack is supposed to be stored 
 * (on the 6502 is from location 0100 to 01ff).
 *
 * Wrapper to @ref _sram_hexdump_range.
 */
void sram_hexdump_stack ();

/**
 * @brief Injects the contents of the `program` matrix into SRAM.
 *
 * This function writes the contents of the `program` matrix into
 * the SRAM via `data_write`, then verifies the integrity by reading
 * the memory back and comparing with the original `program` data.
*/
void sram_inject ();
 
/**
 * @brief Edits a single byte in the program matrix.
 *
 * Asks the user via UART for row and column coordinates and the new value (in hex),
 * then updates that byte in the `program` matrix. It displays the previous
 * and updated value via UART.
 */
void sram_edit_byte ();

/**
 * @brief Edits an entire line (row) in the program matrix.
 *
 * Prompts the user to input a specific row index and a new sequence
 * of bytes in CSV hex format (e.g., "AA,BB,CC,..."). It replaces the
 * corresponding row in the `program` matrix and prints both the old
 * and new values for confirmation. The row index and column start on
 * 0, since they literally translate to the matrix indexes.
 */
void sram_edit_line ();

/**
 * TODO: ADD DOCS
 */
void sram_inject_partial ();
void sram_edit_byte_partial (uint8_t (*)[16], uint16_t);
void sram_edit_line_partial (uint8_t (*)[16], uint16_t);


/** @} */
