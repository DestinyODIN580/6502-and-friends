#include "pin_bus.h"

const uint8_t DATA_PINS[NUM_DATA_PINS] =
{
       22, 24, 26, 28, 30, 32, 34, 36
};


const uint8_t ADDR_PINS[NUM_ADDR_PINS] =
{
       23, 25, 27, 29, 31, 33, 35, 37,
       39, 41, 43, 45, 47, 49, 51
};

const uint8_t WE_PIN    = 2;
const uint8_t OE_PIN    = 3;
const uint8_t CS_PIN    = 4;
const uint8_t RESET_PIN = 5;
const uint8_t BE_PIN    = 6;
const uint8_t SEND_PIN  = 7;
