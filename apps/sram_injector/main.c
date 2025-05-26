#include <stdint.h>
#include <util/delay.h>

#include <pin_bus.h>
#include <uart.h>
#include <sram_gpio.h>
#include <sram_read.h>
#include <sram_write.h>
#include <6502_ctrl.h>
#include <sram_ctrl.h>

void inject_program ();

int main ()
{
    UART_init ();
    UART_putString ("\n\n----------- INJECTOR -----------\n");

    init_6502 ();
    deactivate_6502();

    _delay_ms (1000);
    activate_sram ();

    _delay_ms (1000);
    inject_program ();
    _delay_ms (1000);

    deactivate_sram ();    
    _delay_ms (1000);
    // correct, but now commented to avoid damages
    // activate_6502 ();

    UART_putString ("\n--------------------------------\n");
    _delay_ms (1000);
}


void inject_program ()
{ 
    UART_putString ("Injecting program...");
    uint16_t addr = 0;
    int i, j;

    for (i = j = addr = 0; addr < get_program_size(); addr++, j++)
    {
        if (j == 0x10)
        {
            j = 0;
            i++;
        }
        data_write (addr, program[i][j]);
    }

//  reset vector at location fffc (lb) fffd (hb)
//  data_write (0x7ffc, 0x00);
//  data_write (0x7ffd, 0x80);

    UART_putString ("complete...integity check...");

    for (addr = i = j = 0; addr < get_program_size(); addr++, i++)
    {   
        if (i == 0x10)
        {
            i = 0; 
            j++;
        } 
        if (program[j][i] != data_read (addr))
        {
            UART_putString ("ERROR");
            return ;
        } 
    }
    UART_putString ("success.");
} 