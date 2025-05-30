# 6502 and friends
Welcome! This project contains some utilities to monitor a **W65C02S** and to
control a static RAM (**HM62256LP-70**) using two **Arduino**, one listening to what
the 6502 is saying and the other one programming and monitoring the RAM. 

![Screenshot](/images/glue_logic_attempt/first_working_attempt.jpeg)

---

The project is shipped with some programs, ready to be flashed to
the Arduino
- `sram_hexdump` makes a hexdump of the contents of the RAM
- `sram_injector` puts program stored in the Arduino flash into the RAM
- `sram_healthcheck` runs various tests on the RAM to ensure its integrity
- `6502_logic_analyzer` will monitor what the 6502 is doing

the programs can be flashed via the `flash.sh` script.

The AVR programs are to be found in the `avr_bin` folder, and their Arduino
equivalents are in the `arduino_bin` folder

---
## Instructions
- navigate to `build/` directory
- `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=../cmake/avr-toolchain.cmake ..`
- `make`
- go back to root directory and flash your programs with `./flash.sh <program_name>`!

---

## Documentation (Work in progress)
- Doxygen
- Available at `./html/index.html`

---

**Datasheets**

[W65C02S](https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf)

[HM62256LP-70](https://www.jameco.com/Jameco/Products/ProdDS/82472.pdf)


