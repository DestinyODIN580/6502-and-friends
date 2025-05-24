#include <stdint.h>
#include <util/delay.h>
#include "sram_write.h"
#include "../../config/pin_bus.h"
#include "../uart/uart.h"
#include "../sram_gpio/sram_gpio.h"

void data_write (unsigned int address, uint8_t byte)
{
    // to assure high impedance
    for (int i = 0; i < 8; i++)
        avr_pin_mode (DATA_PINS[i], INPUT);
    _delay_us (1);

    for (int i = 0; i < 15; i++)
        avr_digital_write (ADDR_PINS[i], (address >> i) & 0x1);
    _delay_us (1);
  
    avr_digital_write (OE_PIN, HIGH);
    _delay_us(1);
  
    avr_digital_write (CS_PIN, LOW);
    _delay_us(1);
  
    _delay_us(1);

    avr_digital_write (WE_PIN, LOW);
    _delay_us(1);
  
    for (int i = 0; i < 8; i++)
    {
        avr_pin_mode (DATA_PINS[i], OUTPUT);
        avr_digital_write (DATA_PINS[i], (byte >> i) & 0x1);
    }
    _delay_us(1);

    avr_digital_write (WE_PIN, HIGH);
    _delay_us(1);

    avr_digital_write (CS_PIN, HIGH);
    _delay_us(1);
  
    // for safety
    for (int i = 0; i < 8; i++)
        avr_pin_mode(DATA_PINS[i], INPUT);
    _delay_us(1);

   // _debug_print (address, byte, WRITE);
    /*
    if (DEBUG_WRITE || DEBUG_WRITE_PRINT)
        _debug_print(address, data, WRITE);
*/
}
/*
void _debug_print (unsigned int address, uint8_t data, Mode mode)
{
    char buffer[20];
    if (mode == WRITE)
    //  Serial.print("w");
        UART_putChar ('w');
    else if (mode == READ)
        UART_putChar ('r');
    //  Serial.print("r");

    UART_setString (" | ADDR: ");
    sprintf (buffer, "%x", address);
    UART_setString (buffer);
    //Serial.print (address, HEX);

    UART_getString (" ( ");
    sprintf (buffer, "%d", address);
    UART_setString (buffer);
    // Serial.print(address, BIN);

    UART_getString (" ) DATA -> <");
    sprintf (buffer, "%x", data);
    UART_setString (buffer);
    // Serial.print(data, HEX);

    UART_getString ("> ( ");
    sprintf (buffer, "%d", data);
    UART_setString (buffer);
    // Serial.print(data, BIN);

    UART_getString (");");
  } */