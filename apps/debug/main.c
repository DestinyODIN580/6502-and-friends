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

    UART_putString ("\n\nController intialized\n");
    UART_putString ("----------------------------------------\n");
}

void closing_sequence ()
{
    deactivate_sram ();
    deactivate_6502 ();
    UART_putString ("\n\nController deactivated (ram and 6502 put to high z)\n");
    UART_putString ("----------------------------------------\n");
}

void processor_call ()
{
    deactivate_sram ();
    activate_6502 ();
    UART_putString ("\n\nProcessor activated\n");
    UART_putString ("----------------------------------------\n");
}

void processor_stop ()
{
    deactivate_6502();
    activate_sram ();
    UART_putString ("\n\nProcessor deactivated\n");
    UART_putString ("----------------------------------------\n");
}

void processor_freeze ()
{
    freeze_6502 ();
    activate_sram ();
    UART_putString ("\n\nProcessor frozen\n");
    UART_putString ("----------------------------------------\n");
}

void processor_unfreeze ()
{
    deactivate_sram ();
    unfreeze_6502 ();
    UART_putString ("\n\nProcessor unfrozen\n");
    UART_putString ("----------------------------------------\n");
}

inline void menu_print ()
{
    UART_putString ("Choose between:\n0. exit\n1. sram_inject\n2. sram_hexdump\n3. sram_edit_byte\n4. sram_edit_line\n");
    UART_putString ("5. 6502  activate (you better know what you are doing)\n6. 6502 deactivate (breathe now)\n");
    UART_putString ("7. 6502  freeze (you better know what you are doing)\n8. 6502 unfreeze (you better know what you are doing)");
    UART_putString ("\n----------------------------------------\n");
}