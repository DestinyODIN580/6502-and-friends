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
include CMakeFiles/sram_read.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sram_read.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sram_read.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sram_read.dir/flags.make

CMakeFiles/sram_read.dir/codegen:
.PHONY : CMakeFiles/sram_read.dir/codegen

CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj: CMakeFiles/sram_read.dir/flags.make
CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj: /home/arcueid/Desktop/6502_beta/lib/sram_read/sram_read.c
CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj: CMakeFiles/sram_read.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/arcueid/Desktop/6502_beta/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj -MF CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj.d -o CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj -c /home/arcueid/Desktop/6502_beta/lib/sram_read/sram_read.c

CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.i"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arcueid/Desktop/6502_beta/lib/sram_read/sram_read.c > CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.i

CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.s"
	/usr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arcueid/Desktop/6502_beta/lib/sram_read/sram_read.c -o CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.s

# Object files for target sram_read
sram_read_OBJECTS = \
"CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj"

# External object files for target sram_read
sram_read_EXTERNAL_OBJECTS =

libsram_read.a: CMakeFiles/sram_read.dir/lib/sram_read/sram_read.c.obj
libsram_read.a: CMakeFiles/sram_read.dir/build.make
libsram_read.a: CMakeFiles/sram_read.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/arcueid/Desktop/6502_beta/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libsram_read.a"
	$(CMAKE_COMMAND) -P CMakeFiles/sram_read.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sram_read.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sram_read.dir/build: libsram_read.a
.PHONY : CMakeFiles/sram_read.dir/build

CMakeFiles/sram_read.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sram_read.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sram_read.dir/clean

CMakeFiles/sram_read.dir/depend:
	cd /home/arcueid/Desktop/6502_beta/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arcueid/Desktop/6502_beta /home/arcueid/Desktop/6502_beta /home/arcueid/Desktop/6502_beta/build /home/arcueid/Desktop/6502_beta/build /home/arcueid/Desktop/6502_beta/build/CMakeFiles/sram_read.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/sram_read.dir/depend

