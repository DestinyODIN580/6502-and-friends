#pragma once

#include <stdint.h>

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define HIGH 1
#define LOW 0


uint8_t avr_digital_read (uint8_t pin);
void avr_digital_write (uint8_t pin, uint8_t value);
void avr_pin_mode (uint8_t pin, uint8_t mode);