# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/marcin/Pobrane/Telegram Desktop/slope"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/marcin/Pobrane/Telegram Desktop/slope/build"

# Include any dependencies generated for this target.
include demos/CMakeFiles/animation.dir/depend.make

# Include the progress variables for this target.
include demos/CMakeFiles/animation.dir/progress.make

# Include the compile flags for this target's objects.
include demos/CMakeFiles/animation.dir/flags.make

demos/CMakeFiles/animation.dir/animation.c.o: demos/CMakeFiles/animation.dir/flags.make
demos/CMakeFiles/animation.dir/animation.c.o: ../demos/animation.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/marcin/Pobrane/Telegram Desktop/slope/build/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object demos/CMakeFiles/animation.dir/animation.c.o"
	cd "/home/marcin/Pobrane/Telegram Desktop/slope/build/demos" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/animation.dir/animation.c.o   -c "/home/marcin/Pobrane/Telegram Desktop/slope/demos/animation.c"

demos/CMakeFiles/animation.dir/animation.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/animation.dir/animation.c.i"
	cd "/home/marcin/Pobrane/Telegram Desktop/slope/build/demos" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E "/home/marcin/Pobrane/Telegram Desktop/slope/demos/animation.c" > CMakeFiles/animation.dir/animation.c.i

demos/CMakeFiles/animation.dir/animation.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/animation.dir/animation.c.s"
	cd "/home/marcin/Pobrane/Telegram Desktop/slope/build/demos" && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S "/home/marcin/Pobrane/Telegram Desktop/slope/demos/animation.c" -o CMakeFiles/animation.dir/animation.c.s

demos/CMakeFiles/animation.dir/animation.c.o.requires:
.PHONY : demos/CMakeFiles/animation.dir/animation.c.o.requires

demos/CMakeFiles/animation.dir/animation.c.o.provides: demos/CMakeFiles/animation.dir/animation.c.o.requires
	$(MAKE) -f demos/CMakeFiles/animation.dir/build.make demos/CMakeFiles/animation.dir/animation.c.o.provides.build
.PHONY : demos/CMakeFiles/animation.dir/animation.c.o.provides

demos/CMakeFiles/animation.dir/animation.c.o.provides.build: demos/CMakeFiles/animation.dir/animation.c.o

# Object files for target animation
animation_OBJECTS = \
"CMakeFiles/animation.dir/animation.c.o"

# External object files for target animation
animation_EXTERNAL_OBJECTS =

demos/animation: demos/CMakeFiles/animation.dir/animation.c.o
demos/animation: demos/CMakeFiles/animation.dir/build.make
demos/animation: slope/libslope.so
demos/animation: demos/CMakeFiles/animation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable animation"
	cd "/home/marcin/Pobrane/Telegram Desktop/slope/build/demos" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/animation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
demos/CMakeFiles/animation.dir/build: demos/animation
.PHONY : demos/CMakeFiles/animation.dir/build

demos/CMakeFiles/animation.dir/requires: demos/CMakeFiles/animation.dir/animation.c.o.requires
.PHONY : demos/CMakeFiles/animation.dir/requires

demos/CMakeFiles/animation.dir/clean:
	cd "/home/marcin/Pobrane/Telegram Desktop/slope/build/demos" && $(CMAKE_COMMAND) -P CMakeFiles/animation.dir/cmake_clean.cmake
.PHONY : demos/CMakeFiles/animation.dir/clean

demos/CMakeFiles/animation.dir/depend:
	cd "/home/marcin/Pobrane/Telegram Desktop/slope/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/marcin/Pobrane/Telegram Desktop/slope" "/home/marcin/Pobrane/Telegram Desktop/slope/demos" "/home/marcin/Pobrane/Telegram Desktop/slope/build" "/home/marcin/Pobrane/Telegram Desktop/slope/build/demos" "/home/marcin/Pobrane/Telegram Desktop/slope/build/demos/CMakeFiles/animation.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : demos/CMakeFiles/animation.dir/depend

