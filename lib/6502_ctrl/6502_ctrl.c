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

inline void _6502_set_frozen ()
{
    g_6502_active = PROC_FROZEN;
}

void init_6502 ()
{
    clock_stop () ;

    // 6502 started tri-stated with reset held.
    avr_pin_mode (BE_PIN, OUTPUT);
    avr_digital_write (BE_PIN, LOW);
    avr_pin_mode (RESET_PIN, OUTPUT);
    avr_digital_write (RESET_PIN, LOW);

    _delay_ms (10);
    _6502_set_deactive ();
}

void deactivate_6502 ()
{
    if (is_6502_active () == PROC_FROZEN)
    {
        UART_putString ("CPU already frozen, skip deactivate\n");
        return;
    }


    if (is_6502_active () != PROC_ACTIVE)
    {
        UART_putString ("deactivate_6502(): CPU inactive\n");
        return;                          
    }

    avr_digital_write (BE_PIN, LOW);
    avr_digital_write (RESET_PIN, LOW);
    clock_stop(); 

    _delay_ms (10);
    _6502_set_deactive ();
}

void freeze_6502 (void)
{
    if (is_6502_active() != PROC_ACTIVE)
        return;

    avr_digital_write (BE_PIN, LOW);        

    while (avr_digital_read (CLK_PIN));     // awaits LOW

    clock_stop();

    _6502_set_frozen();                    
}


void unfreeze_6502 (void)
{
    if (is_6502_active() != PROC_FROZEN)
        return;

    clock_start();

    avr_digital_write (BE_PIN, HIGH);     

    _6502_set_active();
}


void activate_6502 ()
{
    if (is_6502_active () != PROC_INACTIVE)
    {
        UART_putString ("activate_6502 (): processor was not inactive (maybe frozen or unitialized), doing nothing\n");
        return ;
    }

    clock_start (); 
    avr_digital_write (BE_PIN, HIGH);
    _delay_ms (2);
    avr_digital_write (RESET_PIN, HIGH);
    _delay_ms (10);
    _6502_set_active ();
}

void destroy_6502 ()
{
    clock_stop ();
    avr_pin_mode (BE_PIN, INPUT);
    avr_pin_mode (RESET_PIN, INPUT);

    _6502_set_undefined ();
}