set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(MCU             "atmega2560"    CACHE STRING "AVR MCU type")
set(F_CPU           "16000000UL"    CACHE STRING "CPU frequency")

set(CMAKE_C_COMPILER    avr-gcc)
set(CMAKE_CXX_COMPILER  avr-g++)

add_compile_options(
  -mmcu=${MCU}
  -DF_CPU=${F_CPU}
  -Os -funsigned-char -funsigned-bitfields -fshort-enums
  -Wall -Wextra
)

set(CMAKE_EXE_LINKER_FLAGS
  "-mmcu=${MCU} -Wl,--gc-sections"
  CACHE STRING "")

set(CMAKE_OBJCOPY avr-objcopy)
