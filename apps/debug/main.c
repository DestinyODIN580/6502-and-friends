/**
 * NOTE: TESTING JSR IMPLEMENTATION: 
 * FIX:  tie A15 on 74ls04 (second not) to 5V rather than to the 6502 (A15 now floating around)
 * NOTE: The RAM will be seen twice, and addresses from 0100 to 01ff (8100 and 81ff) will be sacrificed for the
 * stack. This leaves for addresses from 0000-0099 and 02000 to 7fff to be used for the program. Maybe it's
 * not a good idea to use the first 100 bytes and then have a "hole" in the program, so I guess an educated
 * approach would be to leave them for reserved informations or for a possible VIA? This then gives me
 * this mmap
 *      
 *      0000 - 0099  | ???? (VIA?)
 *      0100 - 01FF  | 6502 stack
 *      0200 - 7FFF  | RAM/ROM
 *
 * NOTE: all the functions had hard coded addresses, so I had to manually change them so here is the TODO list
 * TODO: define macros to better define this new setup and integrate them in sram_ops functions
 *
 * DONE: update pictures for this setup, and make appropriate notes to explain why.
 *
 * TODO: putting multiple programs in the flash is not that hard, so maybe create a system that allows to switch
 * between multiple programs and to always know what is currently in the RAM.
 * TODO: could it be useful to have an upgrade of the sram editors to just change the line and not put the whole program
 * back in?
 * TODO: maybe could use a VERY little ROM to save the reset vectors. This way we can give the ROM the upper space addres and
 * the lower to the RAM, tying A15 to low to match with the stack addresses...but ROM = defeat. This also fixes the duplicate RAM 
 * issue.
 * TODO:: all these functions at the bottom ought to disappear. Have to reorganize them in a separate header or put them in the
 * already existing ones.
 *
 * DONE: (kinda) I'm scared to connect the LCD display, but it's ready I guess. It would be cool to have it ad the monitor of the logic
 * analyzer. Gotta research the power consumption and understand if it can break everything or be fine. And the same for the VIA.
 * Also, have to remember to put a link to the docs, but for now I will put the manual in the folder.
 */

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

enum PROG {PROG_NIL, PROG_LOOP, PROG_JSR}; // JUST FOR TESTING TODO: MOVE THIS TO PIN_BUS

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
void switch_program (enum PROG); // JUST FOR TESTING TODO: MOVE THIS TO PIN_BUS
void sram_edit_line_realtime ();

enum PROG g_prog_curr;

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
                sram_hexdump_stack ();
                sram_hexdump_program ();
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
            case 'a':
                deactivate_6502 ();
                activate_sram ();
                switch_program (PROG_LOOP);
                break;
            case 'b':
                deactivate_6502 ();
                activate_sram ();
                switch_program (PROG_JSR);
                break;
            case 'c':
                deactivate_6502 ();
                activate_sram ();
                switch_program (PROG_NIL);
                break;
            case 'd':
                deactivate_6502 ();
                activate_sram ();
                sram_edit_byte_partial (program, 0x0200);
                break;
            case 'e':
                deactivate_6502 ();
                activate_sram ();
                sram_edit_line_partial (program, 0x0200);
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
    g_prog_curr = PROG_NIL;
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
    UART_putString ("7. 6502 freeze (activate the processor first!!)\n8. 6502 unfreeze (after freezing!!)\n");
    UART_putString ("a. program load: loop\nb. program load: jsr\nc. program load: null program\n");
    UART_putString ("d. sram_edit_byte realtime\ne. sram_edit_line realtime\n");
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
    UART_putString (buffer);
    get_6502_state (buffer);
    UART_putString (buffer);
    UART_putString ("----------------------------------------\n");
}

void switch_program (enum PROG new_prog)
{
    switch (new_prog)
    {
        // horrible workaround, JUST FOR TESTING
        // TODO: enrich sram_inject ()
        case PROG_JSR:
            for (size_t i = 0; i < get_program_rows (); i++)
                for (int j = 0; j < 16; j++)
                    program[i][j] = program_jsr[i][j];
            // sram_inject (PROG_JSR);
            UART_putString ("Copied program_jsr: inject it now!\n");
            break;
        case PROG_LOOP:
            for (size_t i = 0; i < get_program_rows (); i++)
                for (int j = 0; j < 16; j++)
                    program[i][j] = program_loop[i][j];
            // sram_inject (PROG_JSR);
            UART_putString ("Copied program_loop: inject it now!\n");
            // sram_inject (PROG_LOOP);
            break;
        default:
            for (size_t i = 0; i < get_program_rows (); i++)
                for (int j = 0; j < 16; j++)
                    program[i][j] = 0xEA;
            // sram_inject (PROG_NIL);
            UART_putString("switch_program (): Progam in enum not implemented? Loading null program...\n");
            break;
    }
}
