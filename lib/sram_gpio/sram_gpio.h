#pragma once

/**
 * @file sram_gpio.h
 * @brief GPIO abstraction layer for SRAM communication.
 *
 * This module defines functions to configure and manipulate digital pins
 * on the Arduino for use with external SRAM.
 *
 * It includes:
 * - A mapping between digital pin numbers and their corresponding registers.
 * - Functions to set pin modes (input, output, pull-up).
 * - Functions to read and write digital values to/from the pins.
 *
 */

/**
 * @addtogroup sram_driver
 * @{
 */

#include <stdint.h>

#define INPUT          0    /**< Configure pin as high-impedance input. */
#define OUTPUT         1    /**< Configure pin as push-pull output.     */
#define INPUT_PULLUP   2    /**< Input with internal pull-up enabled.   */
#define HIGH 1              /**< Logic high level (5 V).                */
#define LOW  0              /**< Logic low level (0 V).                 */

/**
 * @brief Sets a digital pin’s mode (direction).
 *
 * @param pin  Arduino-style digital pin number (0 – 53).
 * @param mode One of @c INPUT, @c OUTPUT, or @c INPUT_PULLUP.
 */
void avr_pin_mode (uint8_t pin, uint8_t mode);

/**
 * @brief Drives a pin HIGH or LOW (or toggles pull-up if pin is input).
 *
 * @param pin    Digital pin number.
 * @param value  @c HIGH or @c LOW.
 */
void avr_digital_write (uint8_t pin, uint8_t value);

/**
 * @brief Reads the current logic level from a digital pin.
 *
 * @param pin Digital pin number.
 * @return    @c HIGH or @c LOW.
 */
uint8_t avr_digital_read (uint8_t pin);

/** @} */
