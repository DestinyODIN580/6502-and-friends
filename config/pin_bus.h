#pragma once

#include <stdint.h>
#include <string.h>

#define  NUM_DATA_PINS   8
extern const uint8_t DATA_PINS[NUM_DATA_PINS];

#define  NUM_ADDR_PINS   15
extern const uint8_t ADDR_PINS[NUM_ADDR_PINS];

#define RAM_SIZE        0x7FFF
#define RESETVECTOR_HB  0x80
#define RESETVECTOR_LB  0x00
extern const uint8_t program[][16];

extern const uint8_t WE_PIN;
extern const uint8_t OE_PIN;
extern const uint8_t CS_PIN;
extern const uint8_t RESET_PIN;
extern const uint8_t BE_PIN;
extern const uint8_t SEND_PIN;

size_t get_program_size ();