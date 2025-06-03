#pragma once

/**
 * @defgroup 6502_ctrl 6502 Control Module
 * @brief Functions and definitions for managing the W65C02 processor.
 *
 * This module provides initialization, state tracking, and control mechanisms
 * for the WDC 65C02 microprocessor, such as activating, freezing, or deactivating the CPU.
 * It includes low-level pin manipulation and synchronization with clock control.
 * @{
 */

/**
* @brief Represents the current state of the W65C02 processor.
*
* This enum is used throughout the 6502 control module to track and manage
* the operational state of the 6502 microprocessor.
*/
enum PROC_STATE
{
    /**
    * @brief The processor state is undefined or uninitialized.
    *
    * This state is (as of now) used at startup or after detaching control from the Arduino with @ref destroy_6502 ().
    */
    PROC_UNDEFINED,
    /**
     * @brief The processor is active and running.
     *
     * In this state, the bus is enabled (BE high) and reset is released (RESET high).
     * Does not mean the clock is running.
     */
    PROC_ACTIVE,
    /**
     * @brief The processor is inactive.
     *
     * Both the bus enable and reset lines are held low. The clock is stopped.
     * This state is safe for Arduino to access shared buses.
     */
    PROC_INACTIVE,
    /**
     * @brief The processor is frozen.
     *
     * The bus is disabled (BE low) but reset remains high, so that execution can be resumed from the last instruction
     * preserving the 6502 internal state. The clock is stopped after a safe LOW phase. Used for safe temporary bus control.
     */
    PROC_FROZEN
};

/**
 * @brief Global variable holding the current state of the 6502 processor.
 */
extern enum PROC_STATE g_6502_active;

/**
 * @brief Returns the current state of the 6502 processor.
 * @return The current processor state as a value of PROC_STATE.
 */
enum PROC_STATE is_6502_active ();

/**
 * @brief Sets the 6502 processor state to ACTIVE.
 * @note Internal use only (private function).
 */
void _6502_set_active ();

/**
 * @brief Sets the 6502 processor state to INACTIVE.
 * @note Internal use only (private function).
 */
void _6502_set_deactive ();

/**
 * @brief Sets the 6502 processor state to UNDEFINED.
 * @note Internal use only.
 */
void _6502_set_undefined ();

/**
 * @brief Sets the 6502 processor state to FROZEN.
 * @note Internal use only.
 */
void _6502_set_frozen ();

/**
 * @brief Writes the current 6502 processor state into the provided buffer.
 * @param buffer Pointer to a buffer where the formatted state string will be stored.
 * @note Buffer contents will be overwritten.
 */
void get_6502_state (char *);

/**
 * @brief Initializes the 6502 processor by setting control pins and holding it in reset with BE
 * held low (high z).
 *
 * The CPU is initialized in a tri-stated, held-reset state. The clock is stopped.
 */
void init_6502 ();

/**
 * @brief Activates the 6502 processor by releasing reset and enabling the bus.
 *
 * The CPU must be in an INACTIVE state prior to calling this.
 * @note Does not start the clock.
 */
void activate_6502 ();

/**
 * @brief Freezes the 6502 by stopping the clock and pulling BE low.
 *
 * Waits for a falling edge on CLK before stopping the clock to ensure safe halting.
 * @note similiar to @ref deactivate_6502 (), but does not reset the 6502
 */
void freeze_6502 ();

/**
 * @brief Unfreezes the 6502 processor by releasing the bus enable signal.
 *
 * Only valid if the processor is currently in the FROZEN state.
 */
void unfreeze_6502 ();

/**
 * @brief Deactivates the 6502 processor by pulling reset and bus enable low.
 *
 * This stops the clock and places the CPU in an INACTIVE state.
 */
void deactivate_6502 ();

/**
 * @brief Fully disables the 6502 processor by placing control pins in high-Z mode.
 *
 * Also sets the processor state to UNDEFINED.
 * @note: detaches arduino from the processor. Never used as of now, so be careful.
 */
void destroy_6502 ();

/** @} */
