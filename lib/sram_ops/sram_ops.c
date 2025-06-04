#include <stdio.h>
#include <stdlib.h>
#include <uart.h>
#include <pin_bus.h>
#include <sram_read.h>
#include <sram_write.h>
#include "sram_ops.h"

void _sram_hexdump_range (uint16_t start_addr, uint16_t end_addr, const char* label)
{
    UART_putString ("\n----------- HEXDUMP -----------\n");

    if (label)
    {
        UART_putString (">> ");
        UART_putString (label);
        UART_putString ("\n");
    }

    char buf[32];
    char header[] = "Address: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n";
    UART_putString (header);

    for (uint16_t addr = start_addr; addr < end_addr;)
    {
        snprintf (buf, sizeof (buf), "%07x: ", addr);
        UART_putString (buf);

        for (int i = 0; i < 16 && addr < end_addr; i++, addr++)
        {
            uint8_t data = data_read (addr);
            snprintf (buf, sizeof (buf), "%02x ", data);
            UART_putString (buf);
        }
        UART_putString("\n");
    }

    UART_putString ("--------------------------------\n");
}

void sram_hexdump_stack ()
{
    _sram_hexdump_range (0x0100, 0x0200, "Stack memory");
}

void sram_hexdump_program ()
{
    _sram_hexdump_range (0x0200, 0x0200 + get_program_size (), "Program memory");
}

/* TODO: add wrappers and give more options like in hexdump */
void sram_inject ()
{ 
    UART_putString ("\n\n----------- INJECTOR -----------\n");
    UART_putString ("Injecting program (in config file)...");
    uint16_t addr = 0;
    int i, j;

    for (i = j = 0, addr = 0x200; addr < 0x200 + get_program_size(); addr++, j++)
    {
        if (j == 0x10)
        {
            j = 0;
            i++;
        }
        data_write (addr, program[i][j]);
    }

//  reset vector at location fffc (lb) fffd (hb)
//  NOTE: if mapped with a15, then rsv is 8000
    data_write (0x7ffc, 0x00);
    data_write (0x7ffd, 0x02);
    data_write (0x0101, 0xea);
    data_write (0x0102, 0x00);

    UART_putString ("complete...integity check...");

    for (addr = 0x200, i = j = 0; addr < 0x200 + get_program_size(); addr++, i++)
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


void sram_inject_partial (uint16_t start_addr, uint16_t end_addr, uint8_t *src[16])
{
    UART_putString("\n\n----- PARTIAL INJECTION -----\n");

    uint16_t addr = start_addr;
    size_t offset = 0;
    while (addr < end_addr)
    {
        size_t row = offset / 16;
        size_t col = offset % 16;

        uint8_t val = src[row][col];
        data_write (addr, val);

        uint8_t actual = data_read (addr);
        if (actual != val)
        {
            UART_putString ("ERROR");
            return;
        }

        addr++;
        offset++;
    }
    UART_putString ("success.");
    UART_putString ("\n--------------------------------\n");
}

void sram_edit_byte ()
{
    char buffer[16];

    while (UART_getChar() != '\n');
    UART_putString ("\n\n----------- BYTE CHANGE -----------\n");
    UART_putString ("Insert row index:\n");
    UART_getString (buffer);
    size_t row = (size_t) atoi (buffer);
    UART_putString (">> ");
    UART_putString (buffer);

    if (row > get_program_rows ())
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

void sram_edit_byte_partial (uint8_t (*src)[16], uint16_t base_addr)
{
    char buffer[16];
    while (UART_getChar() != '\n');

    UART_putString ("\n\n--- EDIT SINGLE BYTE ---\nRow index: ");
    UART_getString (buffer);
    size_t row = atoi (buffer);
    
    UART_putString ("Column index: ");
    UART_getString (buffer);
    size_t col = atoi (buffer);

    UART_putString ("Old value: ");
    snprintf (buffer, sizeof(buffer), "%02X\n", src[row][col]);
    UART_putString (buffer);

    UART_putString ("New value (hex): ");
    UART_getString (buffer);
    uint8_t new_val = (uint8_t) strtol (buffer, NULL, 16);

    src[row][col] = new_val;

    uint16_t addr = base_addr + row * 16 + col;
    data_write (addr, new_val);

    UART_putString ("byte updated.\n----------------------------\n");
}

void sram_edit_line_partial (uint8_t (*src)[16], uint16_t base_addr)
{
    char buffer[200];
    while (UART_getChar() != '\n');

    UART_putString ("Line number: ");
    int line = UART_getChar() - '0';
    while (UART_getChar() != '\n');

    snprintf (buffer, sizeof(buffer), "Editing line [%d], input 16 bytes (csv):\n>> ", line);
    UART_putString (buffer);
    UART_getString (buffer);

    char* token = strtok (buffer, ",");
    int i = 0;
    while (token != NULL && i < 16)
    {
        uint8_t val = (uint8_t) strtol (token, NULL, 16);
        src[line][i] = val;
        data_write (base_addr + line * 16 + i, val);
        token = strtok (NULL, ",");
        i++;
    }
    UART_putString("line updated.\n----------------------------\n");
}
