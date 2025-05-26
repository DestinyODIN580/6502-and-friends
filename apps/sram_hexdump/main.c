#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <pin_bus.h>
#include <uart.h>
#include <sram_gpio.h>
#include <sram_read.h>
#include <sram_write.h>
#include <6502_ctrl.h>
#include <sram_ctrl.h>

void sram_hexdump ();

int main ()
{
    UART_init ();
    UART_putString ("\n\n----------- HEXDUMP -----------\n");

    init_6502 ();
    deactivate_6502();

    _delay_ms (1000);
    activate_sram ();

    _delay_ms (1000);
    sram_hexdump ();
    _delay_ms (1000);

    deactivate_sram ();
    _delay_ms (1000);
    // correct, but now commented to avoid damages
    // activate_6502 ();

    UART_putString ("\n--------------------------------\n");
    _delay_ms (1000);
}

void sram_hexdump ()
{
    char buf[30];
    snprintf (buf, sizeof (buf), "program size: %d bytes\n", get_program_size ());
    UART_putString (buf);

    char header[] = "\nAddress: 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n";
    UART_putString (header);

    uint16_t addr = 0;
    int i;

    for (addr = 0; addr < get_program_size ();)
    {
        sprintf (buf, "%07x: ", addr);
        UART_putString (buf);
        for (i = 0; i < 16; i++)
        {
            uint8_t data = data_read (addr++);
            sprintf (buf, "%02x ", data);
            UART_putString (buf);
        }
        UART_putString ("\n");
    }
}
