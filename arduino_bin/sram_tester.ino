#define SRAM_MAX_ADDR 0x7FFF  //32kb
#define TEST_RANGE 0xFF

#define DEBUG_CHECKS true
#define DEBUG_READ false
#define DEBUG_READ_PRINT false
#define DEBUG_WRITE false
#define DEBUG_WRITE_PRINT false

const int ADDR_PINS[15] = {
  23, 25, 27, 29, 31, 33, 35, 37,
  39, 41, 43, 45, 47, 49, 51
};

const int DATA_PINS[8] = {
  22, 24, 26, 28, 30, 32, 34, 36
};

const int WE_PIN = 2;
const int OE_PIN = 3;
const int CS_PIN = 4;

enum Mode { WRITE,
            READ };

typedef struct result {
  char check_name[20];
  unsigned int errors;
  unsigned int attempts;
  bool success;
} res_t;

uint8_t program[][16] = {
  0xa9, 0xff, 0x8d, 0x02, 0x60, 0xa9, 0xe0, 0x8d, 0x03, 0x60, 0xa9, 0x38, 0x8d, 0x00, 0x60, 0xa9,
  0x00, 0x8d, 0x01, 0x60, 0xa9, 0x80, 0x8d, 0x01, 0x60, 0xa9, 0x00, 0x8d, 0x01, 0x60, 0xa9, 0x0e,
  0x8d, 0x00, 0x60, 0xa9, 0x00, 0x8d, 0x01, 0x60, 0xa9, 0x80, 0x8d, 0x01, 0x60, 0xa9, 0x00, 0x8d,
  0x01, 0x60, 0xa9, 0x06, 0x8d, 0x00, 0x60, 0xa9, 0x00, 0x8d, 0x01, 0x60, 0xa9, 0x80, 0x8d, 0x01,
  0x60, 0xa9, 0x00, 0x8d, 0x01, 0x60, 0xa9, 0x01, 0x8d, 0x00, 0x60, 0xa9, 0x00, 0x8d, 0x01, 0x60,
  0xa9, 0x80, 0x8d, 0x01, 0x60, 0xa9, 0x00, 0x8d, 0x01, 0x60, 0xa9, 0x43, 0x20, 0x76, 0x80, 0xa9,
  0x69, 0x20, 0x76, 0x80, 0xa9, 0x61, 0x20, 0x76, 0x80, 0xa9, 0x6f, 0x20, 0x76, 0x80, 0xa9, 0x21,
  0x20, 0x76, 0x80, 0x4c, 0x92, 0x80, 0x8d, 0x00, 0x60, 0xa9, 0x20, 0x8d, 0x01, 0x60, 0xa9, 0xa0,
  0x8d, 0x01, 0x60, 0xa9, 0x20, 0x8d, 0x01, 0x60, 0x60, 0xa6, 0x00, 0x20, 0x8f, 0x80, 0x60, 0xa4,
  0x01, 0x60, 0xa9, 0x31, 0x20, 0x76, 0x80, 0x20, 0x89, 0x80, 0x4c, 0x92, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

void setup() {
  Serial.begin(57600);
  Serial.print("\n\nProgram initialization...");
  init_pins();
  Serial.println("complete.");
  res_t res;
  uint16_t base_address = 0x0;
  uint16_t range_address = TEST_RANGE;
    parse_result(check_blank(base_address, range_address));

  /*
  parse_result(check_detached(base_address, range_address));
  parse_result(check_sequential(base_address, range_address));
  parse_result(check_blank(base_address, range_address));
  parse_result(check_shift_bit());

  Serial.println ("starting complete check (detached)...");
  parse_result(check_complete_detached());
  Serial.println ("starting complete check (sequential)...");
  parse_result(check_complete_sequential());
  Serial.println ("done!");
  */
  inject_program ();
  sram_fetch ();

  end_pins ();
}

void sram_fetch ()
{
    Serial.println (sizeof (program));
    Serial.println ("hexdump of ram");
    char header[50];
    strcpy(header, "Address: ");

    for (int h = 0; h <= 0xF; h++) {
        char tmp[4];
        sprintf(tmp, "%02X ", h);
        strcat(header, tmp);
    }
    Serial.println(header);
    char buf[20];
    uint16_t addr = 0;
    int i, j;

    for (addr = 0; addr < sizeof (program);)
    {
        sprintf (buf, "%07x: ", addr);
        Serial.print (buf);
        for (i = 0; i < 16; i++)
        {
            uint8_t data = data_read (addr++);
            sprintf (buf, "%02x ", data);
            Serial.print (buf);
        }
        Serial.print ("\n");
    }
}

void end_pins ()
{
    digitalWrite (CS_PIN, HIGH);
    pinMode (OE_PIN, INPUT);
    pinMode (WE_PIN, INPUT);

    for (int i = 0; i < 15; i++)
        pinMode(ADDR_PINS[i], INPUT);
    for (int i = 0; i < 8; i++)
        pinMode(DATA_PINS[i], INPUT);
}

void loop() {}

void init_pins() {
  for (int i = 0; i < 15; i++)
    pinMode(ADDR_PINS[i], OUTPUT);
  for (int i = 0; i < 8; i++)
    pinMode(DATA_PINS[i], INPUT);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  pinMode(WE_PIN, OUTPUT);
  digitalWrite(WE_PIN, HIGH);

  pinMode(OE_PIN, OUTPUT);
  digitalWrite(OE_PIN, HIGH);
}

inline res_t check_complete_detached() {
  return check_detached(0x0, SRAM_MAX_ADDR);
}

inline res_t check_complete_sequential() {
  return check_sequential(0x0, SRAM_MAX_ADDR);
}

res_t check_detached(unsigned int address_start, unsigned int address_range) {
  unsigned int i;
  res_t res = { "check_detached", 0, address_range, true };

  for (i = 0; i < address_range; i++)
    data_write(address_start + i, i);
  for (i = 0; i < address_range; i++) {
    uint8_t data = data_read(address_start + i);
    //Serial.print(i << 8, HEX);

    uint16_t address = address_start + i;
    if (address_start + i > 0xFF)
        address &= 0xFF;

    if (data != address) {
      if (DEBUG_CHECKS) {
        Serial.print("check_detached ERROR: at address ");
        Serial.print(address_start + i);
        Serial.print("( ");
        Serial.print(address_start + i, HEX);
        Serial.print(") ");
        Serial.print(" read ");
        Serial.print(data);
        Serial.print("( ");
        Serial.print(data, HEX);
        Serial.print(") ");
        Serial.println ();
      }

      res.errors++;
      res.success = false;
    }
  }
  return res;
}

res_t check_sequential(unsigned int address_start, unsigned int address_range) {
  unsigned int i;
  res_t res = { "check_sequential", 0, address_range, true };

  for (i = 0; i < address_range; i++) {
    data_write(address_start + i, i);
    uint8_t data = data_read(address_start + i);

    uint16_t address = address_start + i;
    if (address_start + i > 0xFF)
        address &= 0xFF;

    if (data != address)
    {
      if (DEBUG_CHECKS) {
        Serial.print("check_sequential ERROR: at address ");
        Serial.print(address_start + i);
        Serial.print(" read ");
        Serial.println(data);
      }

      res.errors++;
      res.success = false;
    }
  }
  return res;
}

res_t check_blank(uint16_t address_start, uint16_t address_range) {
  res_t res = { "check_blank", 0, address_range, true };
  uint8_t data;
  for (uint16_t i = address_start; i < address_start + address_range; i++)
    data_write(i, 0x0);

  for (uint16_t i = address_start; i < address_start + address_range; i++) {
    if ((data = data_read(i)) != 0x00) {
      if (DEBUG_CHECKS) {
        Serial.print("check_blank ERROR: at address ");
        Serial.print(i);
        Serial.print(" read ");
        Serial.println(data);
      }
      res.errors++;
      res.success = false;
    }
  }
  return res;
}

res_t check_shift_bit() {
  res_t res = { "check_shift_bit", 0, 8, true };
  uint8_t data = 0x1;
  for (uint16_t i = 0; i < 8; i++)
    data_write(i, data << i);
  for (uint16_t i = 0; i < 8; i++) {
    if ((data = data_read(i)) != 0x1 << i) {
      if (DEBUG_CHECKS) {
        Serial.print("check_shift_bit ERROR: at address ");
        Serial.print(i);
        Serial.print(" read ");
        Serial.println(data);
      }
      res.errors++;
      res.success = false;
    }
  }
  return res;
}
/*
res_t check_data_interference ()
{
    res_t r = {"check_data_inteference", 0, SRAM_MAX_ADDR, true};
    for (uint16_t i = 0; i < SRAM_MAX_ADDR; i++)
        data_write (i, (i & 1) ? 0xAA : 0x55);
    for (uint16_t i = 0; i < SRAM_MAX_ADDR; i++)
        if ((data = data_read(i)) != ((a & 1) ? 0xAA : 0x55))
        {
            if (DEBUG_CHECKS)
            {
              Serial.print ("check_shift_bit ERROR: at address ");
              Serial.print (i);
              Serial.print (" read ");
              Serial.println (data);
            }
            res.errors++;
            res.success = false;
        }
    return r;
}

*/
uint8_t data_read(unsigned int address) {
  uint8_t data = 0x0;

  for (int i = 0; i < 8; i++)
    pinMode(DATA_PINS[i], INPUT);
  delayMicroseconds(1);

  for (int i = 0; i < 15; i++)
    digitalWrite(ADDR_PINS[i], (address >> i) & 0x1);

  digitalWrite(CS_PIN, LOW);
  delayMicroseconds(1);

  digitalWrite(OE_PIN, LOW);
  delayMicroseconds(1);

  for (int i = 0; i < 8; i++)
    data |= (digitalRead(DATA_PINS[i]) << i);

  digitalWrite(CS_PIN, HIGH);
  delayMicroseconds(1);

  digitalWrite(OE_PIN, HIGH);
  delayMicroseconds(1);

  if (DEBUG_READ || DEBUG_READ_PRINT)
    _debug_print(address, data, READ);

  return data;
}

void data_write(unsigned int address, uint8_t data) {
  // to assure high impedance
  for (int i = 0; i < 8; i++)
    pinMode(DATA_PINS[i], INPUT);
  delayMicroseconds(1);

  for (int i = 0; i < 15; i++)
    digitalWrite(ADDR_PINS[i], (address >> i) & 0x1);
  delayMicroseconds(1);

  digitalWrite(OE_PIN, HIGH);
  delayMicroseconds(1);

  digitalWrite(CS_PIN, LOW);
  delayMicroseconds(1);

  delayMicroseconds(1);

  digitalWrite(WE_PIN, LOW);
  delayMicroseconds(1);

  for (int i = 0; i < 8; i++) {
    pinMode(DATA_PINS[i], OUTPUT);
    digitalWrite(DATA_PINS[i], (data >> i) & 0x1);
  }
  delayMicroseconds(1);

  digitalWrite(WE_PIN, HIGH);
  delayMicroseconds(1);

  digitalWrite(CS_PIN, HIGH);
  delayMicroseconds(1);

  // for safety
  for (int i = 0; i < 8; i++)
    pinMode(DATA_PINS[i], INPUT);
  delayMicroseconds(1);

  if (DEBUG_WRITE | DEBUG_WRITE_PRINT)
    _debug_print(address, data, WRITE);
}

bool parse_result(res_t result) {
  char buffer[200];
  snprintf(buffer, sizeof(buffer), "%s | %s: errors/attempts --> %u/%u\n",
           result.success ? "SUCCESS" : "FAIL", result.check_name,
           result.errors, result.attempts);
  Serial.print(buffer);
  return result.success;
}

void _debug_print(unsigned int address, uint8_t data, Mode mode) {
  if (mode == WRITE)
    Serial.print("w");
  else if (mode == READ)
    Serial.print("r");
  Serial.print(" | ADDR: ");
  Serial.print(address, HEX);
  Serial.print(" ( ");
  Serial.print(address, BIN);
  Serial.print(" ) DATA -> <");
  Serial.print(data, HEX);
  Serial.print("> ( ");
  Serial.print(data, BIN);
  Serial.println(");");
}

void inject_program ()
{ 
    Serial.print ("Injecting program...");
    uint16_t addr = 0;
    int i, j;

    for (i = j = addr = 0; addr < sizeof (program); addr++, j++)
    {
        if (j == 0x10)
        {
            j = 0;
            i++;
        }
        data_write (addr, program[i][j]);
    }

/*
    for (i = 0; i < 20; i++)
      for (j = 0; j < 16; j++)
        data_write (addr++, program[i][j]);
*/
    //data_write (0x7ffc, 0x00);
    //data_write (0x7ffd, 0x80);
    Serial.print ("complete...integrity check...");

    for (addr = i = j = 0; addr < 0xA0; addr++, i++)
    {   
        if (i == 0x10)
        {
          i = 0; 
          j++;
        } 
        if (program[j][i] != data_read (addr))
        {
          Serial.println ("ERROR");
          Serial.println (program[j][i], HEX);
          return ;
        } 
    }

    Serial.println ("success");
} 