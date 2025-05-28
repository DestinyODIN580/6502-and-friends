#pragma once

/**
 * @defgroup SRAM_Operations SRAM operations
 * @brief Functions for editing, injecting, and visualizing SRAM data.
 * @{
 */

/**
 * @brief Dumps the contents of the SRAM in a formatted hexadecimal view.
 *
 * This function reads the entire programmed memory (sizeof (program) as of now)
 * and outputs a hex dump via UART.
 */
 void sram_hexdump ();

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

/** @} */
