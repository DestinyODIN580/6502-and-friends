#pragma once

#include <stdint.h>

void UART_putString(const char* buf);
uint8_t UART_getString(char * buf);
char UART_getChar(void);
void UART_putChar(char  c);
void UART_init(void);