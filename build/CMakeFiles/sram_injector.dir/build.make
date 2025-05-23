# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/arcueid/Desktop/6502_beta

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arcueid/Desktop/6502_beta/build

# Include any dependencies generated for this target.
include CMakeFiles/sram_injector.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sram_injector.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sram_injector.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sram_injector.dir/flags.make

CMakeFiles/sram_injector.dir/codegen:
.PHONY : CMakeFiles/sram_injector.dir/codegen

CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj: CMakeFiles/sram_injector.dir/flags.make
CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj: /home/arcueid/Desktop/6502_beta/apps/sram_injector/main.c
CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj: CMakeFiles/sram_injector.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/arcueid/Desktop/6502_beta/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj -MF CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj.d -o CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj -c /home/arcueid/Desktop/6502_beta/apps/sram_injector/main.c

CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.i"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arcueid/Desktop/6502_beta/apps/sram_injector/main.c > CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.i

CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.s"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arcueid/Desktop/6502_beta/apps/sram_injector/main.c -o CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.s

# Object files for target sram_injector
sram_injector_OBJECTS = \
"CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj"

# External object files for target sram_injector
sram_injector_EXTERNAL_OBJECTS =

sram_injector: CMakeFiles/sram_injector.dir/apps/sram_injector/main.c.obj
sram_injector: CMakeFiles/sram_injector.dir/build.make
sram_injector: libsram_gpio.a
sram_injector: libsram_read.a
sram_injector: libsram_write.a
sram_injector: libpin_config.a
sram_injector: libuart.a
sram_injector: libsram_gpio.a
sram_injector: CMakeFiles/sram_injector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/arcueid/Desktop/6502_beta/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable sram_injector"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sram_injector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sram_injector.dir/build: sram_injector
.PHONY : CMakeFiles/sram_injector.dir/build

CMakeFiles/sram_injector.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sram_injector.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sram_injector.dir/clean

CMakeFiles/sram_injector.dir/depend:
	cd /home/arcueid/Desktop/6502_beta/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arcueid/Desktop/6502_beta /home/arcueid/Desktop/6502_beta /home/arcueid/Desktop/6502_beta/build /home/arcueid/Desktop/6502_beta/build /home/arcueid/Desktop/6502_beta/build/CMakeFiles/sram_injector.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/sram_injector.dir/depend

