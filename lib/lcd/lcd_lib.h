#ifndef LCD_LIB_H
#define LCD_LIB_H

#include <stdint.h> 
/*
 * This is the interface with the 2004A LCD screen featuring an I2C
 * connection with the PCF8574T. Below is a list of defines with the
 * most important instructions.
 *
 * NOTE: could implement them by using masks with MSB representing the
 * command, and then using the other bits to get what you want. However many
 * of those masks wouldn't be used, so only time will tell if it's convenient
 * or not. Plus, some masks would need checks to see if they are used correctly
 *
 * NOTE: professor suggests using bitfields instead of masks or defines, but as of now
 * it's just more convenient to use 10 different defines and a global variable.
 * If for some reason I need to list more configurations then I shall consider them.
 *
 * The docs shows that there are 7 main commands families, each using
 * a different MSB (instructions.pdf).
 * 
 * 0000 0001      Clear display
 * 0000 001*      Return home
 * 0000 01*       Entry mode set
 * 0000 1*        Display on/off control
 * 0001 *         Cursor/Display shift
 * 001*           Function set
 * 01*            set CGRAM addr
*/

#define SCREEN_CLEAR                    0x01 // 0b00000001 - Clears the screen
#define CURS_RESET                      0x02 // 0b00000010 - Puts cursor at (0,0)

// Entry mode set -> 0000 0 1 I/D S
//    I/D = 0 --> Decrement
//    I/D = 1 --> Increment
//    S = 1   --> Accompanies display shift
#define CURS_LEFT_SHIFT                 0x04 // 0b00000100 - Cursor shifts left on wr (Entry Mode)
#define CURS_LEFT_SHIFT_SCR_SHIFT_ON    0x05 // 0b00000101 - Screen shifts, then cursor is shifted to left
#define CURS_RIGHT_SHIFT                0x06 // 0b00000110 - Cursor shifts right on wr (Entry Mode)
#define CURS_RIGHT_SHIFT_SCR_SHIFT_ON   0x07 // 0b00000111 - Screen shifts, then cursor is shifted to right

// Display on/off control --> 0000 1 D C B
//    D = 1 --> display on
//    D = 0 --> display off
//    C = 1 --> cursor on
//    C = 0 --> cursor off
//    B = 1 --> blinking on
//    B = 0 --> blinking off
#define SCREEN_OFF                      0x08 // 0b00001000 - Turns display OFF
#define SCREEN_ON_CURSOR_OFF_BLINK_OFF  0x0C // 0b00001100 - Display ON, cursor OFF, blink OFF
#define SCREEN_ON_CURSOR_OFF_BLINK_ON   0x0D // 0b00001101 - Display ON, cursor OFF, blink ON
#define SCREEN_ON_CURSOR_ON_BLINK_OFF   0x0E // 0b00001110 - Display ON, cursor ON, blink OFF
#define SCREEN_ON_CURSOR_ON_BLINK_ON    0x0F // 0b00001111 - Display ON, cursor ON, blink ON

// Cursor/Display shift --> 0001 S/C R/L - -
//    S/C = 1 --> display shift
//    S/C = 0 --> cursor move
//    R/L = 1 --> shift to the right
//    R/L = 0 --> shift to the left
#define CURS_LEFT_MOVE                  0x10 // 0b00010000 - Moves cursor left by one position
#define CURS_RIGHT_MOVE                 0x14 // 0b00010100 - Moves cursor right by one position
#define SCREEN_SHIFT_LEFT               0x18 // 0b00011000 - Shifts entire screen left
#define SCREEN_SHIFT_RIGHT              0x1C // 0b00011100 - Shifts entire screen right

// Function set --> 001 DL N F - -
//    DL = 1 --> 8 bits
//    DL = 0 --> 4 bits
//    N = 1  --> 2 lines
//    N = 0  --> 1 line
//    F = 1  --> 5x10 dots
//    F = 0  --> 5x8 dots
// XXX: atm using a 4 bit bus, so i guess i have to force it to 4 bit mode?
// NOTE: this display supports only 5x8 chars, so no point in defining something for 5x10
#define FUNCTION_SET_8BIT_1LINE_5x8     0x30 // 0b00110000 - 8-bit, 1-line, 5x8 dots
#define FUNCTION_SET_8BIT_2LINE_5x8     0x38 // 0b00111000 - 8-bit, 2-line, 5x8 dots
#define FUNCTION_SET_4BIT_1LINE_5x8     0x20 // 0b00100000 - 4-bit, 1-line, 5x8 dots
#define FUNCTION_SET_4BIT_2LINE_5x8     0x28 // 0b00101000 - 4-bit, 2-line, 5x8 dots

// CGRAM Address (custom chars)
#define SET_CGRAM_ADDRESS               0x40 // 0b01000000 - Base address for CGRAM

// DDRAM Address (to navigate the display directly)
//
// NOTE: the device has 4 lines that have not sequential lines in the videoRAM.
// An offset can be used to point to a specific cell of the 20x4 matrix.
// Lines are in order: 1 --> 3 --> 2 --> 4 --> 1...
#define SET_CURSOR_POSITION_BASE        0x80 // 0b10000000 - Base address for setting DDRAM cursor position
#define SET_CURSOR_LINE_1               0x80 // 0b10000000 - Cursor at start of line 1
#define SET_CURSOR_LINE_2               0xC0 // 0b11000000 - Cursor at start of line 2
#define SET_CURSOR_LINE_3               0x94 // 0b10010100 - Cursor at start of line 3
#define SET_CURSOR_LINE_4               0xD4 // 0b11010100 - Cursor at start of line 4

#define LCD_WIDTH 20                         
#define LCD_HEIGHT 4                         
//#define F_CPU 16000000UL
#define LCD_ADDR 0x27
#define F_SCL 100000UL
#define PRESCALER 1
#define TWBR_VALUE (((F_CPU / F_SCL) - 16) / (2 * PRESCALER))

extern uint8_t g_screen_state;

void I2C_init (void);
void I2C_start (void);
void I2C_stop (void);
void I2C_write (uint8_t);

void LCD_init (void);
void LCD_send_command (uint8_t);
void LCD_send_data (uint8_t);

void LCD_cursor_set_pos (uint8_t, uint8_t);
void LCD_cursor_set_line (uint8_t);
void LCD_cursor_set_on (void);
void LCD_cursor_set_off (void);
void LCD_blink_set_on (void);
void LCD_blink_set_off (void);
void LCD_cursor_reset (void);

void LCD_screen_clear (void);
void LCD_screen_shift_left (void);
void LCD_screen_shift_right (void);

void LCD_print (char const *);
void LCD_print_char (char);
void LCD_print_int (int);
void LCD_print_float (float, uint8_t);
void LCD_print_str (char const *);
void LCD_print_line (char const *);

#endif
