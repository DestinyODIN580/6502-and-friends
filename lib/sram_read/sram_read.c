#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>

#include <sram_read.h>
#include <sram_gpio.h>
#include <pin_bus.h>
#include <uart.h>

enum Mode { WRITE, READ };

uint8_t data_read (uint16_t address)
{
    uint8_t data = 0x0;
  
    for (int i = 0; i < 8; i++)
        avr_pin_mode (DATA_PINS[i], INPUT);
    _delay_us (1);
  
    for (int i = 0; i < 15; i++)
        avr_digital_write (ADDR_PINS[i], (address >> i) & 0x1);
  
    avr_digital_write (CS_PIN, LOW);
    _delay_us (1);
  
    avr_digital_write (OE_PIN, LOW);
    _delay_us (1);
  
    for (int i = 0; i < 8; i++)
      data |= (avr_digital_read (DATA_PINS[i]) << i);
  
    avr_digital_write (CS_PIN, HIGH);
    _delay_us (1);
  
    avr_digital_write (OE_PIN, HIGH);
    _delay_us (1);

  
    /*
    if (DEBUG_READ || DEBUG_READ_PRINT)
        avr_digital_write (address, data, READ);
  */
  
    return data;
}


void _debug_print (unsigned int address, uint8_t data, enum Mode mode)
{
    char buffer[20];
    if (mode == WRITE)
    //  Serial.print("w");
        UART_putChar ('w');
    else if (mode == READ)
        UART_putChar ('r');
    //  Serial.print("r");

    UART_getString (" | ADDR: ");
    sprintf (buffer, "%x", address);
    UART_getString (buffer);
    //Serial.print (address, HEX);

    UART_getString (" ( ");
    sprintf (buffer, "%d", address);
    UART_getString (buffer);
    // Serial.print(address, BIN);

    UART_getString (" ) DATA -> <");
    sprintf (buffer, "%x", data);
    UART_getString (buffer);
    // Serial.print(data, HEX);

    UART_getString ("> ( ");
    sprintf (buffer, "%d", data);
    UART_getString (buffer);
    // Serial.print(data, BIN);

    UART_getString (");");
  }


/*
void _debug_print(unsigned int address, uint8_t data, Mode mode) {
  if (mode == WRITE)
    Serial.print("w");
  else if (mode == READ)
    Serial.print("r");
  Serial.print(" | ADDR: ");
  Serial.print(address, HEX);
  Serial.print(" ( ");
  Serial.print(address, BIN);
  Serial.print(" ) DATA -> <");
  Serial.print(data, HEX);
  Serial.print("> ( ");
  Serial.print(data, BIN);
  Serial.println(");");
}
  */