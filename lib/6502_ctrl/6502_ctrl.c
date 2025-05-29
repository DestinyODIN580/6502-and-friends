#include <util/delay.h>

#include <6502_ctrl.h>
#include <pin_bus.h>
#include <sram_gpio.h>
#include <uart.h>
#include <clock_ctrl.h>

enum PROC_STATE g_6502_active = PROC_UNDEFINED;

enum PROC_STATE is_6502_active ()
{
    return g_6502_active;
}

inline void _6502_set_active ()
{
    g_6502_active = PROC_ACTIVE;
}

inline void _6502_set_deactive ()
{   
    g_6502_active = PROC_INACTIVE;
}

inline void _6502_set_undefined ()
{
    g_6502_active = PROC_UNDEFINED;
}

void init_6502 ()
{
    clock_stop () ;
    /*
    if (is_6502_active() != PROC_UNDEFINED)
    {
        // debug print
        return ;
    }
    */

    // 6502 started tri-stated with reset held.
    avr_pin_mode (BE_PIN, OUTPUT);
    avr_digital_write(BE_PIN, LOW);
    avr_pin_mode (RESET_PIN, OUTPUT);
    avr_digital_write (RESET_PIN, LOW);

    _delay_ms (10);
    _6502_set_deactive ();
}

void deactivate_6502 ()
{
    if (is_6502_active() != PROC_ACTIVE)
    {
        // debug print

        // probably best to initialize it to high z
        init_6502();
    }

    avr_digital_write (BE_PIN, LOW);
    avr_digital_write (RESET_PIN, LOW);
    clock_stop(); 

    _delay_ms (10);
    _6502_set_deactive ();
}

void activate_6502 ()
{
    if (is_6502_active() != PROC_INACTIVE)
    {
        // debug print
        return ;
    }

    clock_start(); 
    avr_digital_write(BE_PIN, HIGH);
    _delay_ms (2);
    avr_digital_write (RESET_PIN, HIGH);
    _delay_ms (10);
    _6502_set_active ();
}

void destroy_6502 ()
{
    clock_stop();
    avr_pin_mode (BE_PIN, INPUT);
    avr_pin_mode (RESET_PIN, INPUT);

    _6502_set_undefined ();
}