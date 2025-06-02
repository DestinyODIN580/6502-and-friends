#pragma once

#include <stdint.h>

/**
 * @file clock_ctrl.h
 * @brief Control interface for the W65C02 Φ2 clock signal.
 *
 * This module configures and manages the system clock signal
 * for the W65C02 CPU using Timer1 (OC1A on Arduino Mega: pin D11).
 * It allows for dynamic start/stop and runtime frequency configuration.
 */

/**
 * @defgroup 6502_clock 6502 Clock Control
 * @brief Functions to manage the W65C02 Φ2 system clock.
 * @{
 */

/**
 * @brief Initialize the 6502 clock system.
 *
 * Configures Timer1 in CTC toggle mode using OC1A (pin D11) to generate
 * a square wave clock signal. The frequency is computed automatically
 * using the most appropriate prescaler to match the given value.
 *
 * @note This function must be called only once during system startup,
 *       before calling `clock_start()`.
 *
 * @param freq_hz Desired clock frequency in Hz (e.g. 1, 1000, 2000000).
 */
void clock_init (uint32_t freq_hz);

/**
 * @brief Start the clock signal output. 
 *
 * Enables the Timer1 prescaler, allowing the OC1A output to toggle.
 * Activates the Φ2 signal to the 6502.
 */
void clock_start (void);

/**
 * @brief Stop the clock signal output.
 *
 * Disables the Timer1 prescaler, freezing the timer
 * and stopping the Φ2 signal. The output will remain at its last level.
 */
void clock_stop (void);

/**
 * @brief Change the W65C02 clock frequency at runtime.
 *
 * Stops the timer, updates the compare register and prescaler
 * to match the new frequency, and restarts the clock.
 *
 * @param freq_hz New desired frequency in Hz.
 */
void clock_set_frequency (uint32_t freq_hz);

/**
 * @brief Advance the clock for 1 cycle
 *
 * Stops the clock and "artificially" creates a clock impulse.
 * Multiple calls can be chained together to create a single-step
 * clock.
 */
void clock_tick ();