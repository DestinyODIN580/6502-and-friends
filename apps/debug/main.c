#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "../../lib/uart/uart.h"           

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_BIT   7         

int main(void) {
    LED_DDR  |= (1 << LED_BIT);

    UART_init();               
    UART_putString("UART ready\r\n");

    for (;;)
    {
        char c = UART_getChar(); 
        if (c != '\n')
        {
            UART_putString("Received: ");
            UART_putChar(c);         
        }
        else 
            UART_putChar('\n');   

        if (c == '\r')
            UART_putString("\n");
        LED_PORT ^= (1 << LED_BIT);      
    }
}
