#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "lcd_lib.h"

#define F_CPU 16000000UL

/* This library targets an LCD display with a I2C attached
 * 
 * The module for the I2C is the PCF8574T has the downsided of having
 * only 4 pins connected to the data lines of the LCD screen (D4-D7), and
 * other 4 connected to E (Enable), R/~W (Read/Write), RS (register select)
 *
 * NOTE: Still have to figure out how to read from the screen. Apparently I2C modules for
 * LCD are made to facilitate writing (and with just that >:( ). A possible workaround
 * would be to map the contents of the screen in a matrix whenever something is printed or
 * shifted. Maybe I should just do that regardless to optimize it? Writing everything at once instead of 
 * writing eveytime it's requested? EDIT: YOU CANT READ FROM IT :(
 *
 * TODO::
 * - add custom characters?
 */

uint8_t const row_offset[] = {0x00, 0x40, 0x14, 0x54};
uint8_t g_screen_state = 0x8;

// initializes I2C communication
void I2C_init()
{
    // TWBR: TwoWireBitrateRegister 
    // setting (communication speed)
    TWBR = (uint8_t) TWBR_VALUE;
}

// starts I2C communication
void I2C_start()
{
    // TWCR: TwoWireControlRegister
    // TWEN: TwoWriteENable
    // TWINT: TwoWireINTerrupt
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

// stops I2C communication
void I2C_stop()
{
    // TWSTO: TwoWireSTOp
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

// sends byte of data to I2C
void I2C_write (uint8_t data)
{
    // TWDR: TwoWireDataRegister
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

// sends command to LCD
void LCD_send_command (uint8_t cmd)
{
    uint8_t data_upper = (cmd & 0xF0);
    uint8_t data_lower = ((cmd << 4) & 0xF0);

    I2C_start ();
    I2C_write (LCD_ADDR << 1);
    I2C_write (data_upper | 0x0C);
    I2C_write (data_upper | 0x08);
    I2C_write (data_lower | 0x0C);
    I2C_write (data_lower | 0x08);
    I2C_stop ();
    _delay_ms (2);
}

// sends data to LCD
void LCD_send_data (uint8_t data)
{
    // NOTE: since I have to use a 4 bit communication
    // I need to have 2 exchanges where I first give the
    // high part and then the low part of the byte

    uint8_t data_high = (data & 0xF0);
    uint8_t data_low = ((data << 4) & 0xF0);

    I2C_start ();
    I2C_write (LCD_ADDR << 1); // Sends the address with the write bit

    // NOTE: to write something to DDRAM I have to put the RS pin high and the 
    //  RW pin low (W is active low), followed by the data i want to send.
    //  In my case tho, I also have to put high the Enable pin, and then low
    //  to let the display register the data.
    //  Each byte sent will be therefore split in 2 separate bytes where
    //  the first one contains in the upper part the upper part of the data
    //  and in the lower part the mask 0x0D (1101) where
    //  - bit 0 --> RS
    //  - bit 2 --> RW
    //  - bit 3 --> E
    //  and the second one will do the same thing, but with the lower part
    //  using the mask 0x09 (1100) to set the E pin low 
    I2C_write (data_high | 0x0D);
    I2C_write (data_high | 0x09);
    I2C_write (data_low | 0x0D);
    I2C_write (data_low | 0x09);
    I2C_stop ();
    _delay_ms (1);
}


// initializes LCD
void LCD_init ()
{
    _delay_ms (46);

    //HACK: this sequence apparently works for most LCD displays. Sometimes the display 
    // will start in 8 bit mode, so to make it go to 4 bit mode we need to
    // send to him an instruction to set to 8 bit mode, a second one to
    // stabilize it, and a third one to finally bring it to 4-bit mode
    // >> hopefull will not let me waste another evening :) 
    LCD_send_command (0x33);
    LCD_send_command (0x32);
    LCD_send_command (FUNCTION_SET_4BIT_2LINE_5x8);
    _delay_ms (10);

    LCD_send_command (SCREEN_ON_CURSOR_OFF_BLINK_OFF);
    LCD_send_command (CURS_RIGHT_SHIFT);
    LCD_send_command (SCREEN_CLEAR);
    _delay_ms (10);
    
    g_screen_state = SCREEN_ON_CURSOR_ON_BLINK_OFF;
}

// sets cursor on
void LCD_cursor_set_on ()
{
    if ((g_screen_state & 0x02) == 0x02)
        return ;     
    g_screen_state |= 0x02;
    LCD_send_command (g_screen_state);
}

// sets cursor off
void LCD_cursor_set_off () {
    if ((g_screen_state | 0x02) != g_screen_state)
        return ;     
    g_screen_state ^= (1 << 1);
    LCD_send_command (g_screen_state);
}

// sets blinking on
void LCD_blink_set_on () {
    if ((g_screen_state & 0x01) == 0x01)
        return ;     
    g_screen_state |= 0x01;
    LCD_send_command (g_screen_state);

}

// sets blinking off
void LCD_blink_set_off ()
{
    if ((g_screen_state | 0x01) != g_screen_state)
        return ;     
    g_screen_state ^= 0x01;
    LCD_send_command (g_screen_state);
}

// sets current line
void LCD_cursor_set_line (uint8_t line)
{
    // NOTE: rows are 0 to 3
    if (line > LCD_HEIGHT)
        return ;
    switch (line)
    {
        case 0:
            LCD_send_command (SET_CURSOR_LINE_1);
            break;
        case 1:
            LCD_send_command (SET_CURSOR_LINE_2);
            break;
        case 2:
            LCD_send_command (SET_CURSOR_LINE_3);
            break;
        case 3:
            LCD_send_command (SET_CURSOR_LINE_4);
            break;
        default:
            break;
    }
}

// sets cursor position 
void LCD_cursor_set_pos (uint8_t row, uint8_t col)
{
    if (row > LCD_HEIGHT || col > LCD_WIDTH)
        return; 
    LCD_send_command (SET_CURSOR_POSITION_BASE | (row_offset[row] + col));
}

// puts cursor to position (0,0)
inline void LCD_cursor_reset ()
{
    LCD_send_command (CURS_RESET);
}

// clears the screen
inline void LCD_screen_clear ()
{
    LCD_send_command (SCREEN_CLEAR);
}

inline void LCD_screen_shift_left ()
{
    LCD_send_command (SCREEN_SHIFT_LEFT);
}

inline void LCD_screen_shift_right ()
{
    LCD_send_command (SCREEN_SHIFT_RIGHT);
}

void LCD_print_line (const char *line)
{
    char buf[LCD_WIDTH + 1];
    if (strlen (line) > 20)
    {
        //TODO: implement warnings/logging
        strncpy (buf, line, LCD_WIDTH);
    }
    else
        strcpy (buf, line);
    LCD_print (buf);
}

void LCD_print_at (uint8_t row, uint8_t col, const char* str)
{
    LCD_cursor_set_pos (row, col);
    LCD_print (str);
}

void LCD_print (char const *str)
{
    for (; *str; )
        LCD_send_data (*str++);
}

void LCD_print_char (char c)
{
    LCD_send_data (c);
}

void LCD_print_str (char const *str)
{
    for (; *str; )
        LCD_send_data (*str++);
}

void LCD_print_int (int i)
{
    char buf[50];
    snprintf (buf, sizeof(buf), "%d", i);
    LCD_print (buf);
}

void LCD_print_float (float f, uint8_t precision)
{
    char buf[50];
    snprintf (buf, sizeof(buf), "%.*f", precision, f);
    LCD_print (buf);
}

static void _debug_start ()
{
    I2C_init();
    LCD_init();

    LCD_send_command (SET_CURSOR_LINE_2);
    LCD_print (" Starting debugging");
    LCD_send_command (SET_CURSOR_LINE_3);
    LCD_print ("      session!");
    _delay_ms (1000);
    LCD_send_command (SCREEN_CLEAR);
}

static void _debug_end ()
{
    LCD_screen_clear ();
    LCD_cursor_reset ();
    LCD_cursor_set_off ();
    LCD_send_command (SET_CURSOR_LINE_3);
    LCD_print ("  Debugging ended!");
    for (; ; );
}
static void _debug_session ()
{ }
/*
 * if it's ever needed as stand-alone program, here is the main
 * function
 *
int main (void)
{
    _debug_start ();
    _debug_session ();
    _debug_end ();
}
*/

