# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/yblin/Documents/GitHub/instant-ngp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yblin/Documents/GitHub/instant-ngp/build

# Utility rule file for generate_openxr_header.

# Include any custom commands dependencies for this target.
include dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/compiler_depend.make

# Include the progress variables for this target.
include dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/progress.make

generate_openxr_header: dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/build.make
.PHONY : generate_openxr_header

# Rule to build all files generated by this target.
dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/build: generate_openxr_header
.PHONY : dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/build

dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/clean:
	cd /home/yblin/Documents/GitHub/instant-ngp/build/dependencies/OpenXR-SDK/include/openxr && $(CMAKE_COMMAND) -P CMakeFiles/generate_openxr_header.dir/cmake_clean.cmake
.PHONY : dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/clean

dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/depend:
	cd /home/yblin/Documents/GitHub/instant-ngp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yblin/Documents/GitHub/instant-ngp /home/yblin/Documents/GitHub/instant-ngp/dependencies/OpenXR-SDK/include/openxr /home/yblin/Documents/GitHub/instant-ngp/build /home/yblin/Documents/GitHub/instant-ngp/build/dependencies/OpenXR-SDK/include/openxr /home/yblin/Documents/GitHub/instant-ngp/build/dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : dependencies/OpenXR-SDK/include/openxr/CMakeFiles/generate_openxr_header.dir/depend

