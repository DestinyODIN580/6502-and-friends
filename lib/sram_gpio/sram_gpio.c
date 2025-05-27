#include <avr/io.h>
#include <stdint.h>

#include "sram_gpio.h"

 /**
 * @brief Structure to represent a GPIO pin and its control registers.
 *
 * Each pin is associated with its DDR (data direction register), PORT
 * (output register), PIN (input register), and bit index.
 */
typedef struct {
    volatile uint8_t* ddr;   /**< Pointer to DDR register. */
    volatile uint8_t* port;  /**< Pointer to PORT register. */
    volatile uint8_t* pin;   /**< Pointer to PIN register. */
    uint8_t bit;             /**< Bit position in the register. */
} pinmap_t;


const pinmap_t pin_map[] =
{
    { &DDRE, &PORTE, &PINE, 0 },  // D0
    { &DDRE, &PORTE, &PINE, 1 },  // D1
    { &DDRE, &PORTE, &PINE, 4 },  // D2
    { &DDRE, &PORTE, &PINE, 5 },  // D3
    { &DDRG, &PORTG, &PING, 5 },  // D4
    { &DDRE, &PORTE, &PINE, 3 },  // D5
    { &DDRH, &PORTH, &PINH, 3 },  // D6
    { &DDRH, &PORTH, &PINH, 4 },  // D7
    { &DDRH, &PORTH, &PINH, 5 },  // D8
    { &DDRH, &PORTH, &PINH, 6 },  // D9
    { &DDRB, &PORTB, &PINB, 4 },  // D10
    { &DDRB, &PORTB, &PINB, 5 },  // D11
    { &DDRB, &PORTB, &PINB, 6 },  // D12
    { &DDRB, &PORTB, &PINB, 7 },  // D13
    { &DDRJ, &PORTJ, &PINJ, 1 },  // D14
    { &DDRJ, &PORTJ, &PINJ, 0 },  // D15
    { &DDRH, &PORTH, &PINH, 1 },  // D16
    { &DDRH, &PORTH, &PINH, 0 },  // D17
    { &DDRD, &PORTD, &PIND, 3 },  // D18
    { &DDRD, &PORTD, &PIND, 2 },  // D19
    { &DDRD, &PORTD, &PIND, 1 },  // D20
    { &DDRD, &PORTD, &PIND, 0 },  // D21
    { &DDRA, &PORTA, &PINA, 0 },  // D22
    { &DDRA, &PORTA, &PINA, 1 },  // D23
    { &DDRA, &PORTA, &PINA, 2 },  // D24
    { &DDRA, &PORTA, &PINA, 3 },  // D25
    { &DDRA, &PORTA, &PINA, 4 },  // D26
    { &DDRA, &PORTA, &PINA, 5 },  // D27
    { &DDRA, &PORTA, &PINA, 6 },  // D28
    { &DDRA, &PORTA, &PINA, 7 },  // D29
    { &DDRC, &PORTC, &PINC, 7 },  // D30
    { &DDRC, &PORTC, &PINC, 6 },  // D31
    { &DDRC, &PORTC, &PINC, 5 },  // D32
    { &DDRC, &PORTC, &PINC, 4 },  // D33
    { &DDRC, &PORTC, &PINC, 3 },  // D34
    { &DDRC, &PORTC, &PINC, 2 },  // D35
    { &DDRC, &PORTC, &PINC, 1 },  // D36
    { &DDRC, &PORTC, &PINC, 0 },  // D37
    { &DDRD, &PORTD, &PIND, 7 },  // D38
    { &DDRG, &PORTG, &PING, 2 },  // D39
    { &DDRG, &PORTG, &PING, 1 },  // D40
    { &DDRG, &PORTG, &PING, 0 },  // D41
    { &DDRL, &PORTL, &PINL, 7 },  // D42
    { &DDRL, &PORTL, &PINL, 6 },  // D43
    { &DDRL, &PORTL, &PINL, 5 },  // D44
    { &DDRL, &PORTL, &PINL, 4 },  // D45
    { &DDRL, &PORTL, &PINL, 3 },  // D46
    { &DDRL, &PORTL, &PINL, 2 },  // D47
    { &DDRL, &PORTL, &PINL, 1 },  // D48
    { &DDRL, &PORTL, &PINL, 0 },  // D49
    { &DDRB, &PORTB, &PINB, 3 },  // D50
    { &DDRB, &PORTB, &PINB, 2 },  // D51
    { &DDRB, &PORTB, &PINB, 1 },  // D52
    { &DDRB, &PORTB, &PINB, 0 }   // D53
};

/**
 * @brief Returns the total number of defined digital pins.
 *
 * @return Number of entries in the pin map array.
 */
static inline uint8_t _pin_count (void)
{
    return sizeof(pin_map) / sizeof(pinmap_t);
}

void avr_pin_mode (uint8_t pin, uint8_t mode)
{
    if (pin >= _pin_count())
        return;

    volatile uint8_t *ddr  = pin_map[pin].ddr;
    volatile uint8_t *port = pin_map[pin].port;
    uint8_t mask = 1u << pin_map[pin].bit;

    switch (mode)
    {
        case OUTPUT:
            *ddr  |=  mask;        
            break;
        case INPUT_PULLUP:
            *ddr  &= ~mask;        
            *port |=  mask;        
            break;
        default:
            *ddr  &= ~mask;        
            *port &= ~mask;        
            break;
    }
}

void avr_digital_write (uint8_t pin, uint8_t value)
{
    if (pin >= _pin_count())
        return;

    volatile uint8_t *ddr  = pin_map[pin].ddr;
    volatile uint8_t *port = pin_map[pin].port;
    uint8_t mask = 1u << pin_map[pin].bit;

    
    if (*ddr & mask)
    {
        if (value)
            *port |= mask; 
        else
            *port &= ~mask;
    }
    else
    {                      
        if (value)
            *port |=  mask;
        else
            *port &= ~mask;
    }
}

uint8_t avr_digital_read (uint8_t pin)
{
    if (pin >= _pin_count ())
        return LOW;

    const pinmap_t *p = &pin_map[pin];
    uint8_t mask = (1u << p->bit);

    return ( (*(p->pin) & mask) ? HIGH : LOW );
}
