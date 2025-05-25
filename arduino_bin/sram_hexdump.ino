#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define DEBUG_HEXUMP 1
#define DEBUG_GLOBAL 0
#define SRAM_SIZE 0x7FFF

char header[] = "Address: 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F";

void setup ()
{
    ram_hexdump ();
}

void loop () {}

void ram_hexdump ()
{
    #ifndef DEBUG_GLOBAL || DEBUG_HEXUMP
        printf ("hexdump requested")
    #endif
        
    strcpy (header, "Address: ");
    printf (header);
    
    char buf[20];
    uint16_t addr;
    int i, j;

    for (addr = 0; addr < SRAM_SIZE;)
    {
        sprintf (buf, "%07x: ", addr);
        printf (buf);
        for (i = 0; i < 16; i++)
        {
            uint8_t data = data_read (addr++);
            sprintf (buf, "%02x ", data);
            printf (buf);
        }
        printf ("\n");
    }
}

