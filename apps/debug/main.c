#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#include <pin_bus.h>
#include <uart.h>
#include <6502_ctrl.h>
#include <sram_ctrl.h>
#include <sram_gpio.h>
#include <sram_read.h>
#include <sram_write.h>
#include <sram_ops.h>
#include <clock_ctrl.h>

#define DEBUG_START_FREQUENCY 100 // Hz

void init_sequence ();
void closing_sequence ();
void processor_call ();
void processor_stop ();
void processor_freeze ();
void processor_unfreeze ();
void menu_print ();

void single_step_mode ();
void automatic_step_mode ();
void components_state ();



int main ()
{
    init_sequence ();

    menu_print ();

    for (int c; (c = UART_getChar ()) != '0';)
    {
        switch (c)
        {
            case '0':
                break;
            case '1':
                deactivate_6502 ();
                activate_sram ();
                sram_inject ();
                break;
            case '2':
                deactivate_6502 ();
                activate_sram ();
                sram_hexdump ();
                break;
            case '3':
                deactivate_6502 ();
                activate_sram ();
                sram_edit_byte ();
                break;
            case '4':
                deactivate_6502 ();
                activate_sram ();
                sram_edit_line ();
                break;
            case 'x':
                single_step_mode ();
                break;
            case 'y':
                automatic_step_mode ();
                break;
            case '5':
                processor_call ();
                break;
            case '6':
                processor_stop ();
                break;
            case '7':
                processor_freeze ();
                break;
            case '8':
                processor_unfreeze ();
                break;
            case '\n':
                continue;
            default:
                UART_putString ("not recognized\n");
                break;
        }
        menu_print ();
        components_state ();
        UART_putString ("#######################################\n");
    }   
    
    closing_sequence ();
}

void init_sequence ()
{
    clock_init (DEBUG_START_FREQUENCY);     
    UART_init ();
    init_6502 ();
    deactivate_6502 ();
    _delay_ms (1);
    activate_sram ();

    UART_putString ("Controller intialized\n");
    UART_putString ("########################################\n");
}

void closing_sequence ()
{
    deactivate_sram ();
    deactivate_6502 ();
    UART_putString ("Controller deactivated (ram and 6502 put to high z)\n");
    UART_putString ("#######################################\n");
}

void processor_call ()
{
    deactivate_sram ();
    activate_6502 ();
    UART_putString ("Processor activated\n");
    UART_putString ("#######################################\n");
}

void processor_stop ()
{
    deactivate_6502();
    activate_sram ();
    UART_putString ("Processor deactivated\n");
    UART_putString ("#######################################\n");
}

void processor_freeze ()
{
    freeze_6502 ();
    activate_sram ();
    UART_putString ("Processor frozen\n");
    UART_putString ("#######################################\n");
}

void processor_unfreeze ()
{
    deactivate_sram ();
    unfreeze_6502 ();
    UART_putString ("\n\nProcessor unfrozen\n");
    UART_putString ("#######################################\n");
}

void automatic_step_mode ()
{
    if (is_6502_active() != PROC_ACTIVE)
    {
        UART_putString ("clock_tick (): 6502 is not active/frozen. Cannot tick.\n");
        return;
    }

    if (is_sram_active() == SRAM_ACTIVE)
    {
        UART_putString ("clock_tick (): Error: SRAM is active. Cannot tick.\n");
        return;
    }
    clock_start ();
}

void single_step_mode ()
{
    if (is_6502_active() != PROC_ACTIVE)
    {
        UART_putString ("clock_tick (): 6502 is not active/frozen. Cannot tick.\n");
        return;
    }

    if (is_sram_active() == SRAM_ACTIVE)
    {
        UART_putString ("clock_tick (): Error: SRAM is active. Cannot tick.\n");
        return;
    }
    clock_stop ();
    clock_tick ();
}


inline void menu_print ()
{
    UART_putString ("Choose between:\n0. exit\n1. sram_inject\n2. sram_hexdump\n3. sram_edit_byte\n4. sram_edit_line\n");
    UART_putString ("5. 6502  activate (you better know what you are doing)\n");
    UART_putString ("x. clock single step\ny. clock automatic step\n");
    UART_putString ("6. 6502 deactivate (breathe now)\n");
    UART_putString ("7. 6502 freeze (activate the processor first!!)\n8. 6502 unfreeze (after freezing!!)");
    UART_putString ("\n----------------------------------------\n");
}

inline void components_state ()
{
    char buffer[60];
    //snprintf (buffer, sizeof(buffer), "SRAM state = %d\n", is_sram_active());
    //UART_putString (buffer);
    //snprintf(buffer, sizeof(buffer), "6502 state = %d\n", is_6502_active());
    //UART_putString(buffer);
    get_sram_state (buffer);
    UART_putString(buffer);
    get_6502_state (buffer);
    UART_putString(buffer);
    UART_putString ("----------------------------------------\n");
}