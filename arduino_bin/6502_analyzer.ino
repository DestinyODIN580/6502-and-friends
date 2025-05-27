#include <string.h>

#define FLIPPED_PINS 0
#define MODE_FULL 1
#define MODE_STEP 0

volatile bool     g_awaiting_operand = false;
volatile uint8_t  g_mode = MODE_FULL;
volatile uint8_t  g_instr_buffer[3];
volatile uint8_t  g_instr_index = 0;
volatile uint16_t g_instr_address= 0;

const char ADDR_PINS[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA_PINS[] = {39, 41, 43, 45, 47, 49, 51, 53};

const int CLK_PIN = 2;
const int RDW_PIN = 3;

const char* opcodeTable[256] =
{
  /* 0x00 */ "BRK", "ORA ($nn,X)", NULL, NULL, NULL, "ORA $nn", "ASL $nn", NULL,
  /* 0x08 */ "PHP", "ORA #$nn", "ASL A", NULL, "ORA $nnnn", "ASL $nnnn", NULL, NULL,
  /* 0x10 */ "BPL $nn", "ORA ($nn),Y", NULL, NULL, NULL, "ORA $nn,X", "ASL $nn,X", NULL,
  /* 0x18 */ "CLC", "ORA $nnnn,Y", NULL, NULL, NULL, "ORA $nnnn,X", "ASL $nnnn,X", NULL,
  /* 0x20 */ "JSR $nnnn", "AND ($nn,X)", NULL, NULL, "BIT $nn", "AND $nn", "ROL $nn", NULL,
  /* 0x28 */ "PLP", "AND #$nn", "ROL A", NULL, "BIT $nnnn", "AND $nnnn", "ROL $nnnn", NULL,
  /* 0x30 */ "BMI $nn", "AND ($nn),Y", NULL, NULL, NULL, "AND $nn,X", "ROL $nn,X", NULL,
  /* 0x38 */ "SEC", "AND $nnnn,Y", NULL, NULL, NULL, "AND $nnnn,X", "ROL $nnnn,X", NULL,
  /* 0x40 */ "RTI", "EOR ($nn,X)", NULL, NULL, NULL, "EOR $nn", "LSR $nn", NULL,
  /* 0x48 */ "PHA", "EOR #$nn", "LSR A", NULL, "JMP $nnnn", "EOR $nnnn", "LSR $nnnn", NULL,
  /* 0x50 */ "BVC $nn", "EOR ($nn),Y", NULL, NULL, NULL, "EOR $nn,X", "LSR $nn,X", NULL,
  /* 0x58 */ "CLI", "EOR $nnnn,Y", NULL, NULL, NULL, "EOR $nnnn,X", "LSR $nnnn,X", NULL,
  /* 0x60 */ "RTS", "ADC ($nn,X)", NULL, NULL, NULL, "ADC $nn", "ROR $nn", NULL,
  /* 0x68 */ "PLA", "ADC #$nn", "ROR A", NULL, "JMP ($nnnn)", "ADC $nnnn", "ROR $nnnn", NULL,
  /* 0x70 */ "BVS $nn", "ADC ($nn),Y", NULL, NULL, NULL, "ADC $nn,X", "ROR $nn,X", NULL,
  /* 0x78 */ "SEI", "ADC $nnnn,Y", NULL, NULL, NULL, "ADC $nnnn,X", "ROR $nnnn,X", NULL,
  /* 0x80 */ NULL, "STA ($nn,X)", NULL, NULL, "STY $nn", "STA $nn", "STX $nn", NULL,
  /* 0x88 */ "DEY", NULL, "TXA", NULL, "STY $nnnn", "STA $nnnn", "STX $nnnn", NULL,
  /* 0x90 */ "BCC $nn", "STA ($nn),Y", NULL, NULL, "STY $nn,X", "STA $nn,X", "STX $nn,Y", NULL,
  /* 0x98 */ "TYA", "STA $nnnn,Y", "TXS", NULL, NULL, "STA $nnnn,X", NULL, NULL,
  /* 0xA0 */ "LDY #$nn", "LDA ($nn,X)", "LDX #$nn", "LDY $nn", "LDA $nn", "LDX $nn", NULL, NULL,
  /* 0xA8 */ "TAY", "LDA #$nn", "TAX", NULL, "LDY $nnnn", "LDA $nnnn", "LDX $nnnn", NULL,
  /* 0xB0 */ "BCS $nn", "LDA ($nn),Y", NULL, NULL, "LDY $nn,X", "LDA $nn,X", "LDX $nn,Y", NULL,
  /* 0xB8 */ "CLV", "LDA $nnnn,Y", "TSX", NULL, "LDY $nnnn,X", "LDA $nnnn,X", "LDX $nnnn,Y", NULL,
  /* 0xC0 */ "CPY #$nn", "CMP ($nn,X)", NULL, NULL, "CPY $nn", "CMP $nn", "DEC $nn", NULL,
  /* 0xC8 */ "INY", "CMP #$nn", "DEX", NULL, "CPY $nnnn", "CMP $nnnn", "DEC $nnnn", NULL,
  /* 0xD0 */ "BNE $nn", "CMP ($nn),Y", NULL, NULL, NULL, "CMP $nn,X", "DEC $nn,X", NULL,
  /* 0xD8 */ "CLD", "CMP $nnnn,Y", NULL, NULL, NULL, "CMP $nnnn,X", "DEC $nnnn,X", NULL,
  /* 0xE0 */ "CPX #$nn", "SBC ($nn,X)", NULL, NULL, "CPX $nn", "SBC $nn", "INC $nn", NULL,
  /* 0xE8 */ "INX", "SBC #$nn", "NOP", NULL, "CPX $nnnn", "SBC $nnnn", "INC $nnnn", NULL,
  /* 0xF0 */ "BEQ $nn", "SBC ($nn),Y", NULL, NULL, NULL, "SBC $nn,X", "INC $nn,X", NULL,
  /* 0xF8 */ "SED", "SBC $nnnn,Y", NULL, NULL, NULL, "SBC $nnnn,X", "INC $nnnn,X", NULL
};

enum AddressingMode { NONE, IMM, ZP, ABS };
typedef struct instr_t
{
    const char* mnemonic;
    uint8_t size;
    AddressingMode mode;
} instr_t;
instr_t instr_info[256];

void setup ()
{
    Serial.begin (57600);
    init_pins ();
    init_instructions ();
}

void init_pins ()
{    
    Serial.print ("\n\nPins initialization...");
    int i;
    for (i = 0; i < 16; i++)
        pinMode (ADDR_PINS[i], INPUT);
    for (i = 0; i < 8; i++)
        pinMode (DATA_PINS[i], INPUT);  

    pinMode (CLK_PIN, INPUT);
    pinMode (RDW_PIN, INPUT);

    attachInterrupt (digitalPinToInterrupt (CLK_PIN), get_state, RISING);
    Serial.println ("complete");
}

void init_instructions ()
{
    Serial.print ("Instructions initialization...");
    for (uint16_t i = 0; i < 256; i++)
      instr_info[i] = { "???", 1, NONE };

    instr_info[0xA9] = { "LDA ", 2, IMM };
    instr_info[0xA5] = { "LDA ", 2, ZP  };
    instr_info[0xAD] = { "LDA ", 3, ABS };
    instr_info[0x85] = { "STA ", 2, ZP  };
    instr_info[0x8D] = { "STA ", 3, ABS };
    instr_info[0xA2] = { "LDX ", 2, IMM };
    instr_info[0xA0] = { "LDY ", 2, IMM };
    instr_info[0x4C] = { "JMP ", 3, ABS };
    instr_info[0x6C] = { "JMP ", 3, ABS };
    Serial.println ("complete");
}

inline uint16_t get_address_lines ()
{
    uint16_t address = 0;
    if (FLIPPED_PINS)
      for (int i = 15; i >= 0; i--)
      {
          int b = digitalRead (ADDR_PINS[i]) ? 1 : 0;
          Serial.print (b);
          address = (address << 1) | b;
      }
    else
      for (int i = 0; i < 16; i++)
      {
            int b = digitalRead (ADDR_PINS[i]) ? 1 : 0;
            Serial.print (b);
            address = (address << 1) | b;
      }
    return address;
}

inline uint8_t get_data_lines ()
{
    uint8_t data = 0;
    if (FLIPPED_PINS)
      for (int i = 7; i >= 0; i--)
      {
          int b = digitalRead (DATA_PINS[i]) ? 1 : 0;
          Serial.print (b);
          data = (data << 1) | b;
      }
    else
      for (int i = 0; i < 8; i++)
        {
            int b = digitalRead (DATA_PINS[i]) ? 1 : 0;
            Serial.print (b);
            data = (data << 1) | b;
        }
    return data;
}

void print_instruction()
{
    instr_t info = instr_info[g_instr_buffer[0]];
    const char* fallback_mnemonic = opcodeTable[g_instr_buffer[0]];

    Serial.print (g_instr_address, HEX);
    Serial.print (" | r | ");
    for (uint8_t i = 0; i < g_instr_index; ++i)
    {
      if (g_instr_buffer[i] < 0x10)
          Serial.print ('0');
      Serial.print (g_instr_buffer[i], HEX);
      Serial.print (' ');
    }

    Serial.print(" ; ");

    if (strcmp (info.mnemonic, "???"))
        Serial.print (info.mnemonic);
    else if (fallback_mnemonic)
      Serial.print (fallback_mnemonic);
    else
      Serial.print ("???");

    if (info.size == 2)
        _print_formatted_byte(g_instr_buffer[1]);
    else if (info.size == 3)
    {
        uint16_t addr = g_instr_buffer[1] | (g_instr_buffer[2] << 8);
        Serial.print(" --> $");
        if (addr < 0x1000)
            Serial.print('0');
        if (addr < 0x0100)
            Serial.print('0');
        Serial.print(addr, HEX);
    }
    Serial.println();
}

void _print_formatted_byte (uint8_t val)
{
  char bin[9];
  for (int i = 0; i < 8; i++)
      bin[7 - i] = (val & (1 << i)) ? '1' : '0';
  bin[8] = '\0';

  Serial.print (" --> 0x");
  if (val < 0x10)
      Serial.print ("0");
  Serial.print (val, HEX);
  Serial.print (" | ");
  Serial.print (val, DEC);
  Serial.print (" | %");
  Serial.print (bin);
}


void get_state ()
{
    char output[32];
    unsigned int address, data;
    bool is_read;

    is_read = digitalRead (RDW_PIN);
    address = get_address_lines ();
    Serial.print("\t");
    data = get_data_lines ();

    // reset vector (fffc and fffd)
    if (is_read && (address == 0xFFFC || address == 0xFFFD))
    {
      Serial.print (address, HEX);
      Serial.print (" | r | ");
      if (data < 0x10) 
          Serial.print('0');
      Serial.print(data, HEX);
      Serial.print("  ; RESET VECTOR READ: $");
      if (data < 0x10)
          Serial.print('0');
      Serial.println(data, HEX);
      return;
    }

    if (is_read) {
      if (!g_awaiting_operand) {                   // new opcode
        instr_t info = instr_info[data];
        g_instr_buffer[0] = data;
        g_instr_index     = 1;
        g_instr_address   = address;

        if (info.size <= 1) {
          print_instruction();
        } else {
          g_awaiting_operand = true;
        }
      } else {                                     // operand fetch
        g_instr_buffer[g_instr_index++] = data;
        if (g_instr_index >= instr_info[g_instr_buffer[0]].size) {
          print_instruction();
          g_awaiting_operand = false;
        }
      }
    }
    //snprintf (output, sizeof (output), "\t%04x | %c | %02x ", address, digitalRead (RW) ? 'r' : 'w', data)
  }

void loop () {}