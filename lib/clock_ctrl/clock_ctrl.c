#include <avr/io.h>

#include "clock_ctrl.h"
#include "pin_bus.h"       
#include "sram_gpio.h"
#include <6502_ctrl.h>
#include <sram_ctrl.h>
#include <util/delay.h>
#include <uart.h>

#define TIMER_CS_MASK  ((1<<CS12) | (1<<CS11) | (1<<CS10))
#define OC1A_TOGGLE   (1<<COM1A0)

static uint8_t _prescaler_bits (uint16_t presc)
{
    switch (presc)
    {
        case 1:
            return (1 << CS10);
        case 8:
            return (1 << CS11);
        case 64:
            return (1 << CS11) | (1 << CS10);
        case 256:
            return (1 << CS12);
        case 1024:
            return (1 << CS12) | (1 << CS10);
        default:
            return 0;        
    }
}

static inline uint8_t _timer_active(void)
{
    return (TCCR1B & TIMER_CS_MASK);      
}

void clock_init (uint32_t freq_hz)
{
    avr_pin_mode (CLK_PIN, OUTPUT);
    avr_digital_write (CLK_PIN, LOW);

    TCCR1A = (1 << COM1A0);    
    TCCR1B = (1 << WGM12);    

    const uint16_t presc_tbl[] = {1,8,64,256,1024};
    for (uint8_t i = 0; i < 5; i++)
    {
        uint32_t ocr = (F_CPU / (2UL * presc_tbl[i] * freq_hz)) - 1;
        if (ocr <= 0xFFFF)
        {
            OCR1A = (uint16_t) ocr;
            TCCR1B = (TCCR1B & ~TIMER_CS_MASK) | _prescaler_bits (presc_tbl[i]);
            break;
        }
    }
    clock_stop ();
}

void clock_start (void)
{
    TCCR1B |= (TCCR1B & TIMER_CS_MASK) ? (TCCR1B & TIMER_CS_MASK)
                                       : _prescaler_bits(1024); // fallback
}

void clock_stop (void)
{
    TCCR1B &= ~TIMER_CS_MASK;
}


void clock_set_frequency (uint32_t freq_hz)
{
    // Stop the timer before modifying
    TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);

    // Calculate and set new frequency
    uint32_t prescaler_values[] = {1, 8, 64, 256, 1024};
    uint8_t cs_bits[] = 
    {
        (1 << CS10),
        (1 << CS11),
        (1 << CS11) | (1 << CS10),
        (1 << CS12),
        (1 << CS12) | (1 << CS10)
    };

    for (uint8_t i = 0; i < sizeof (prescaler_values) / sizeof(prescaler_values[0]); i++)
    {
        uint32_t top = (F_CPU / (2UL * prescaler_values[i] * freq_hz)) - 1;

        if (top <= 0xFFFF)
        {
            OCR1A = (uint16_t) top;

            // Reset counter and apply new prescaler
            TCNT1 = 0;
            TCCR1B = (1 << WGM12) | cs_bits[i];
            return;
        }
    }
}

void clock_tick (void)
{
    if (is_6502_active() != PROC_ACTIVE)
    { 
        UART_putString ("[Clock] 6502 not active - tick aborted.\n");
        return;
    }
    if (is_sram_active() == SRAM_ACTIVE)
    { 
        UART_putString ("[Clock] SRAM active - tick aborted.\n");
        return;
    }

    // prev. setup
    uint8_t timer_was_on   = _timer_active ();
    uint8_t com1a_was_set  = TCCR1A & OC1A_TOGGLE;

    TCCR1B &= ~TIMER_CS_MASK;   // stop prescaler            
    TCCR1A &= ~OC1A_TOGGLE;     // OC1A back to normal I/O   

    avr_pin_mode (CLK_PIN, OUTPUT);
    avr_digital_write (CLK_PIN, HIGH);
    _delay_us (2);              
    avr_digital_write (CLK_PIN, LOW);
    _delay_us (2);

    // restore previous timer state if it was active 
    if (com1a_was_set)
        TCCR1A |= OC1A_TOGGLE;
    if (timer_was_on)
        TCCR1B |= timer_was_on; 
}