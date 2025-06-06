#pragma once

/**
 * @file pin_bus.h
 * @brief Hardware pin configuration of the Arduino to connect to the SRAM and 6502.
 *
 * This header declares constants and arrays used for
 * controlling address/data buses and special function pins
 * connected to external components.
 */

/**
 * @defgroup project_config Project pin configuration
 * @brief Group containing informations used by other functions
 * to connect the hardware together.
 * @{
 */

#include <stdint.h>
#include <string.h>

/** Number of data bus lines. */
#define  NUM_DATA_PINS   8
extern const uint8_t DATA_PINS[NUM_DATA_PINS];

/** Number of address bus lines. */
#define  NUM_ADDR_PINS   15
extern const uint8_t ADDR_PINS[NUM_ADDR_PINS];

/** SRAM size in bytes. */
#define RAM_SIZE        0x7FFF

/** Reset vector high byte address. */
#define RESETVECTOR_HB  0x80

/** Reset vector low byte address. */
#define RESETVECTOR_LB  0x00

/** Preloaded program to be written to SRAM. */
extern uint8_t program[][16];

/** SRAM control pins. */
extern const uint8_t WE_PIN;
extern const uint8_t OE_PIN;
extern const uint8_t CS_PIN;

/** 6502 control pins. */
extern const uint8_t RESET_PIN;
extern const uint8_t BE_PIN;
extern const uint8_t CLK_PIN;

/** Send control signal pin. */
extern const uint8_t SEND_PIN;

/**
 * @brief Get total size of the program in bytes.
 *
 * @return Size of the program (in bytes).
 */
size_t get_program_size ();

/**
 * @brief Get how many rows the program is composed of.
 *
 * @return #rows (count starts from 1).
 */
size_t get_program_rows ();

/** @} */
