#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define BAUD 19200
#define MYUBRR (F_CPU/16/BAUD-1)

void UART_init(void){
  UBRR0H = (uint8_t)(MYUBRR>>8);
  UBRR0L = (uint8_t)MYUBRR;
  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00); 
  UCSR0B = (1<<RXEN0) | (1<<TXEN0);   
}

void UART_putChar(char c){
  while (!(UCSR0A & (1<<UDRE0)));
  UDR0 = c;
}

char UART_getChar(void){
  while (!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

void UART_putString(const char* buf){
  while (*buf) {
    UART_putChar(*buf++);
  }
}

uint8_t UART_getString(char* buf){
  char* b0 = buf;
  while (1){
    char c = UART_getChar();
    *buf++ = c;
    if (c == 0) return buf - b0;
    if (c == '\n' || c == '\r') {
      *buf = 0;
      return buf - b0;
    }
  }
}
