# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\joshu\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\joshu\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\211.6693.114\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/OS_Wet_HW1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OS_Wet_HW1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OS_Wet_HW1.dir/flags.make

CMakeFiles/OS_Wet_HW1.dir/smash.c.obj: CMakeFiles/OS_Wet_HW1.dir/flags.make
CMakeFiles/OS_Wet_HW1.dir/smash.c.obj: ../smash.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/OS_Wet_HW1.dir/smash.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\OS_Wet_HW1.dir\smash.c.obj -c "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\smash.c"

CMakeFiles/OS_Wet_HW1.dir/smash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/OS_Wet_HW1.dir/smash.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\smash.c" > CMakeFiles\OS_Wet_HW1.dir\smash.c.i

CMakeFiles/OS_Wet_HW1.dir/smash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/OS_Wet_HW1.dir/smash.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\smash.c" -o CMakeFiles\OS_Wet_HW1.dir\smash.c.s

# Object files for target OS_Wet_HW1
OS_Wet_HW1_OBJECTS = \
"CMakeFiles/OS_Wet_HW1.dir/smash.c.obj"

# External object files for target OS_Wet_HW1
OS_Wet_HW1_EXTERNAL_OBJECTS =

OS_Wet_HW1.exe: CMakeFiles/OS_Wet_HW1.dir/smash.c.obj
OS_Wet_HW1.exe: CMakeFiles/OS_Wet_HW1.dir/build.make
OS_Wet_HW1.exe: CMakeFiles/OS_Wet_HW1.dir/linklibs.rsp
OS_Wet_HW1.exe: CMakeFiles/OS_Wet_HW1.dir/objects1.rsp
OS_Wet_HW1.exe: CMakeFiles/OS_Wet_HW1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable OS_Wet_HW1.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\OS_Wet_HW1.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OS_Wet_HW1.dir/build: OS_Wet_HW1.exe

.PHONY : CMakeFiles/OS_Wet_HW1.dir/build

CMakeFiles/OS_Wet_HW1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\OS_Wet_HW1.dir\cmake_clean.cmake
.PHONY : CMakeFiles/OS_Wet_HW1.dir/clean

CMakeFiles/OS_Wet_HW1.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1" "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1" "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\cmake-build-debug" "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\cmake-build-debug" "C:\Users\joshu\Desktop\Technion\Operating Systems Wet Homework\OS-Wet-HW1\cmake-build-debug\CMakeFiles\OS_Wet_HW1.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/OS_Wet_HW1.dir/depend
