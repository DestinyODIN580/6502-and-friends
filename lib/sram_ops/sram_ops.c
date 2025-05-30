#include <stdio.h>
#include <stdlib.h>
#include <uart.h>
#include <pin_bus.h>
#include <sram_read.h>
#include <sram_write.h>


void sram_hexdump ()
{
    UART_putString ("\n----------- HEXDUMP -----------");
    char buf[30];
    snprintf (buf, sizeof (buf), "\n>> program size: %d bytes\n", get_program_size ());
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
    UART_putString ("\n--------------------------------\n");
}

void sram_inject ()
{ 
    UART_putString ("\n\n----------- INJECTOR -----------\n");
    UART_putString ("Injecting program (in config file)...");
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
    data_write (0x7ffc, 0x00);
    data_write (0x7ffd, 0x80);

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
    UART_putString ("\n--------------------------------\n");
}

void sram_edit_byte()
{
    char buffer[16];

    while (UART_getChar() != '\n');
    UART_putString ("\n\n----------- BYTE CHANGE -----------\n");
    UART_putString ("Insert row index:\n");
    UART_getString (buffer);
    size_t row = (size_t) atoi (buffer);
    UART_putString (">> ");
    UART_putString (buffer);

    if (row < get_program_rows ())
    {
        UART_putString ("Invalid row (avalilable rows: ");
        snprintf(buffer, sizeof(buffer), "%d", get_program_rows());
        return ;
    }

    UART_putString ("Insert column index:\n");
    UART_getString (buffer);
    int col = atoi (buffer);
    UART_putString (">> ");
    UART_putString (buffer);

    if (col > 16)
    {
        UART_putString ("Invalid column (<= 16)");
        return ;
    }

    UART_putString("Old value: ");
    snprintf (buffer, sizeof(buffer), "%02X\n", program[row][col]);
    UART_putString (buffer);

    UART_putString ("New value: ");
    UART_getString (buffer);
    UART_putString (">> ");
    UART_putString (buffer);
    uint8_t newVal = (uint8_t) strtol (buffer, NULL, 16);

    program[row][col] = newVal;

    UART_putString ("Updated value: ");
    snprintf (buffer, sizeof(buffer), "%02X\n", program[row][col]);
    UART_putString (buffer);
    UART_putString ("\n--------------------------------\n");
}

void sram_edit_line ()
{
    char buffer[200];
    while (UART_getChar() != '\n');
    UART_putString ("Insert line to change: ");
    int line = UART_getChar() - '0';
    while (UART_getChar() != '\n');

    snprintf (buffer, sizeof (buffer), "\nChanging line [%d]; Insert bytes in csv format\n>> ", line);
    UART_putString (buffer);
    UART_getString (buffer);

    UART_putString ("Old line: ");
    for (int i = 0; i < 16; i++)
    {
        char byteStr[5];
        snprintf (byteStr, sizeof(byteStr), "%02X", program[line][i]);
        UART_putString (byteStr);
        if (i < 15)
            UART_putChar(',');
    }
    UART_putString ("\n");

    char *token = strtok (buffer, ",");
    int col = 0;
    while (token != NULL && col < 16)
    {
        uint8_t val = (uint8_t) strtol (token, NULL, 16);
        program[line][col++] = val;
        token = strtok (NULL, ",");
    }
    UART_putString ("New line: ");
    for (int i = 0; i < 16; i++)
    {
        char byteStr[5];
        snprintf (byteStr, sizeof(byteStr), "%02X", program[line][i]);
        UART_putString (byteStr);
        if (i < 15)
            UART_putChar(',');
    }
    UART_putString ("\nProgram edited (inject it again for the change to take effect)\n");
}
