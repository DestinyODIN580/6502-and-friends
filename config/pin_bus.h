#pragma once

#include <stdint.h>

#define  NUM_DATA_PINS   8
extern const uint8_t DATA_PINS[NUM_DATA_PINS];

#define  NUM_ADDR_PINS   15
extern const uint8_t ADDR_PINS[NUM_ADDR_PINS];

extern const uint8_t WE_PIN;
extern const uint8_t OE_PIN;
extern const uint8_t CS_PIN;
extern const uint8_t RESET_PIN;
extern const uint8_t BE_PIN;
extern const uint8_t SEND_PIN;