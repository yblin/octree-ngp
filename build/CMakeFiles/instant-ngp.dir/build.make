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

# Include any dependencies generated for this target.
include CMakeFiles/instant-ngp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/instant-ngp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/instant-ngp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/instant-ngp.dir/flags.make

CMakeFiles/instant-ngp.dir/src/main.cu.o: CMakeFiles/instant-ngp.dir/flags.make
CMakeFiles/instant-ngp.dir/src/main.cu.o: ../src/main.cu
CMakeFiles/instant-ngp.dir/src/main.cu.o: CMakeFiles/instant-ngp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yblin/Documents/GitHub/instant-ngp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CUDA object CMakeFiles/instant-ngp.dir/src/main.cu.o"
	/usr/bin/nvcc -forward-unknown-to-host-compiler $(CUDA_DEFINES) $(CUDA_INCLUDES) $(CUDA_FLAGS) -MD -MT CMakeFiles/instant-ngp.dir/src/main.cu.o -MF CMakeFiles/instant-ngp.dir/src/main.cu.o.d -x cu -c /home/yblin/Documents/GitHub/instant-ngp/src/main.cu -o CMakeFiles/instant-ngp.dir/src/main.cu.o

CMakeFiles/instant-ngp.dir/src/main.cu.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CUDA source to CMakeFiles/instant-ngp.dir/src/main.cu.i"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CUDA_CREATE_PREPROCESSED_SOURCE

CMakeFiles/instant-ngp.dir/src/main.cu.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CUDA source to assembly CMakeFiles/instant-ngp.dir/src/main.cu.s"
	$(CMAKE_COMMAND) -E cmake_unimplemented_variable CMAKE_CUDA_CREATE_ASSEMBLY_SOURCE

# Object files for target instant-ngp
instant__ngp_OBJECTS = \
"CMakeFiles/instant-ngp.dir/src/main.cu.o"

# External object files for target instant-ngp
instant__ngp_EXTERNAL_OBJECTS =

instant-ngp: CMakeFiles/instant-ngp.dir/src/main.cu.o
instant-ngp: CMakeFiles/instant-ngp.dir/build.make
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/context.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/init.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/input.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/monitor.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/vulkan.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/window.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/x11_init.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/x11_monitor.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/x11_window.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/xkb_unicode.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/posix_time.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/posix_thread.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/glx_context.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/egl_context.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/osmesa_context.c.o
instant-ngp: dependencies/glfw/src/CMakeFiles/glfw_objects.dir/linux_joystick.c.o
instant-ngp: libngp.a
instant-ngp: /usr/lib/x86_64-linux-gnu/libvulkan.so
instant-ngp: /usr/lib/x86_64-linux-gnu/libGLEW.so
instant-ngp: /usr/lib/x86_64-linux-gnu/libz.so
instant-ngp: dependencies/tiny-cuda-nn/libtiny-cuda-nn.a
instant-ngp: dependencies/tiny-cuda-nn/dependencies/fmt/libfmt.a
instant-ngp: CMakeFiles/instant-ngp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yblin/Documents/GitHub/instant-ngp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable instant-ngp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/instant-ngp.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E create_symlink /home/yblin/Documents/GitHub/instant-ngp/build/instant-ngp "/home/yblin/Documents/GitHub/instant-ngp/instant-ngp"

# Rule to build all files generated by this target.
CMakeFiles/instant-ngp.dir/build: instant-ngp
.PHONY : CMakeFiles/instant-ngp.dir/build

CMakeFiles/instant-ngp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/instant-ngp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/instant-ngp.dir/clean

CMakeFiles/instant-ngp.dir/depend:
	cd /home/yblin/Documents/GitHub/instant-ngp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yblin/Documents/GitHub/instant-ngp /home/yblin/Documents/GitHub/instant-ngp /home/yblin/Documents/GitHub/instant-ngp/build /home/yblin/Documents/GitHub/instant-ngp/build /home/yblin/Documents/GitHub/instant-ngp/build/CMakeFiles/instant-ngp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/instant-ngp.dir/depend

