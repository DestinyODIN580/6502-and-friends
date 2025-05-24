#include <stdint.h>

enum Mode { WRITE,
    READ };

void data_write (uint16_t address, uint8_t byte);