# 6502 and friends
Welcome! This project contains some utilities to monitor a **W65C02S** and to
control a static RAM (**HM62256LP-70**) using two **Arduino** boards, one listening to what
the 6502 is saying and the other one programming and monitoring the RAM. That's right, no ROMs!

![Screenshot](/images/functioning_as_ROM/my_child.jpeg)

---
## Setup summary
The **first Arduino** should be flashed with the the `controller` script, that is responsible for
driving the control signals that dictate how the RAM and the 6502 interact with eachother, alongside
the arduino itself, since he is also hooked up on the bus to read/write the RAM.
It contains logic to assure they do not drive the common bus together and functionalities
to control execution such as
 - pause/resume program execution
 - stopping/single-stepping/speeding/slowing the clock impulse for the processor,
 - making a hexdump of the RAM
 - editing the RAM
and many more.

All these operations can be done while the processor is powered on by _freezing_ it or _disabling_ it,
so the program the can be edited while the 6502 is executing it!

All the control signals are forced in a combinatory circuit that assures that the software logic
is enforced at hardware level. They could be implemented more easily with some multiplexers or tri-state
buffers (but I had to work with what I had :D).

The **second Arduino**, that is not mandatory (but highly recomended), is meant more like a debug/learning tool.
He can only listen to the 6502 ports, and give a feedback on what's going on over at the data and address
lines. On each clock pulse, it will read on all those ports (plus the rwb) and format the results in a nice,
readable way, acting pretty much like a logic analyzer.

---
## Content overview

The most relevant functions are provided as stand-alone programs, so that
they can be easly flashed if one may need them for other uses.

- `sram_hexdump` makes a hexdump of the contents of the RAM
- `sram_injector` puts program stored in the Arduino flash into the RAM
- `sram_healthcheck` runs various tests on the RAM to ensure its integrity
- `6502_logic_analyzer` will monitor what the 6502 is doing

All the programs can be flashed via the `flash.sh` script.

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
- Available in the doxygen-docs branch (html)

---

**Datasheets**

[W65C02S](https://www.westerndesigncenter.com/wdc/documentation/w65c02s.pdf)

[HM62256LP-70](https://www.jameco.com/Jameco/Products/ProdDS/82472.pdf)


