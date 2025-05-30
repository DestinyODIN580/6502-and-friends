#include <sram_ctrl.h>
#include <6502_ctrl.h>
#include <sram_gpio.h>
#include <pin_bus.h>
#include <uart.h>

enum SRAM_STATE g_sram_active = SRAM_UNDEFINED;

enum SRAM_STATE is_sram_active ()
{
    return g_sram_active;
}

void _sram_set_active ()
{
    g_sram_active = SRAM_ACTIVE;
}

void _sram_set_deactive ()
{   
    g_sram_active = SRAM_INACTIVE;
}

void _sram_set_undefined ()
{
    g_sram_active = SRAM_UNDEFINED;
}

void init_sram ()
{
    if (is_6502_active () != PROC_INACTIVE)
    {
        UART_putString ("init_sram: processor is not inactive, going high z");
        for (int i = 0; i < 15; i++)
            avr_pin_mode (ADDR_PINS[i], INPUT);
        for (int i = 0; i < 8; i++)
            avr_pin_mode (DATA_PINS[i], INPUT);

        avr_pin_mode (CS_PIN, INPUT);
        avr_pin_mode (OE_PIN, INPUT);
        avr_pin_mode (WE_PIN, INPUT);
        return ;
    }

    for (int i = 0; i < 15; i++)
        avr_pin_mode (ADDR_PINS[i], OUTPUT);
    for (int i = 0; i < 8; i++)
        avr_pin_mode (DATA_PINS[i], INPUT);

    // ram not selected (cs active low)
    avr_pin_mode (CS_PIN, OUTPUT);
    avr_digital_write (CS_PIN, HIGH);
    avr_pin_mode (WE_PIN, OUTPUT);
    avr_digital_write (WE_PIN, HIGH);
    avr_pin_mode (OE_PIN, OUTPUT);
    avr_digital_write (OE_PIN, HIGH);

    _sram_set_active (); 
}

void activate_sram ()
{   
    if (is_sram_active () == SRAM_ACTIVE)
    {
        UART_putString ("activate_sram (): SRAM already active, doing nothing");
        return ;
    } 

    if (is_6502_active () == PROC_ACTIVE)
    {
        UART_putString ("activate_sram (): CPU still running, cannot enable SRAM\n");
        return;
    }

    // absolutely cannot start messing up with the ram
    // if the processor is enabled
    /*
    if (is_6502_active() != PROC_INACTIVE)
    {
        UART_putString ("DEBUG 1");
        if (is_6502_active() == PROC_ACTIVE)
            deactivate_6502 ();
        else
        UART_putString ("DEBUG 2");
            return ;
    }*/

    for (int i = 0; i < 15; i++)
        avr_pin_mode (ADDR_PINS[i], OUTPUT);
    for (int i = 0; i < 8; i++)
        avr_pin_mode (DATA_PINS[i], INPUT);

    // ram not selected (cs active low)
    avr_pin_mode (CS_PIN, OUTPUT);
    avr_digital_write (CS_PIN, HIGH);
    avr_pin_mode (WE_PIN, OUTPUT);
    avr_digital_write (WE_PIN, HIGH);
    avr_pin_mode (OE_PIN, OUTPUT);
    avr_digital_write (OE_PIN, HIGH);

    _sram_set_active ();
}


void deactivate_sram ()
{
    if (is_sram_active () != SRAM_ACTIVE)
    {
        UART_putString ("deactivate_sram (): SRAM is not active, doing nothing");
        return ;
    }
/*
    avr_digital_write (WE_PIN, HIGH);
    avr_digital_write (OE_PIN, HIGH);
    avr_digital_write (CS_PIN, HIGH);
*/
    for (int i = 0; i < 15; i++)
        avr_pin_mode (ADDR_PINS[i], INPUT);
    for (int i = 0; i < 8; i++)
        avr_pin_mode (DATA_PINS[i], INPUT);

    avr_pin_mode (CS_PIN, INPUT);
    avr_pin_mode (OE_PIN, INPUT);
    avr_pin_mode (WE_PIN, INPUT);
    //avr_digital_write (OE_PIN, LOW);
    //avr_digital_write (WE_PIN,; HIGH);

    _sram_set_deactive ();
}
