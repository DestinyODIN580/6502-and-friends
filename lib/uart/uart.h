#pragma once

#include <stdint.h>

void UART_putString(uint8_t* buf);
uint8_t UART_getString(uint8_t* buf);
uint8_t UART_getChar(void);
void UART_putChar(uint8_t c);
void UART_init(void);