# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/maciej/catkin_ws/src/k2_client-1.0.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maciej/catkin_ws/src/k2_client-1.0.0/build

# Include any dependencies generated for this target.
include CMakeFiles/startDepth.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/startDepth.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/startDepth.dir/flags.make

CMakeFiles/startDepth.dir/src/startDepth.cpp.o: CMakeFiles/startDepth.dir/flags.make
CMakeFiles/startDepth.dir/src/startDepth.cpp.o: ../src/startDepth.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maciej/catkin_ws/src/k2_client-1.0.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/startDepth.dir/src/startDepth.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/startDepth.dir/src/startDepth.cpp.o -c /home/maciej/catkin_ws/src/k2_client-1.0.0/src/startDepth.cpp

CMakeFiles/startDepth.dir/src/startDepth.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/startDepth.dir/src/startDepth.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maciej/catkin_ws/src/k2_client-1.0.0/src/startDepth.cpp > CMakeFiles/startDepth.dir/src/startDepth.cpp.i

CMakeFiles/startDepth.dir/src/startDepth.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/startDepth.dir/src/startDepth.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maciej/catkin_ws/src/k2_client-1.0.0/src/startDepth.cpp -o CMakeFiles/startDepth.dir/src/startDepth.cpp.s

CMakeFiles/startDepth.dir/src/startDepth.cpp.o.requires:

.PHONY : CMakeFiles/startDepth.dir/src/startDepth.cpp.o.requires

CMakeFiles/startDepth.dir/src/startDepth.cpp.o.provides: CMakeFiles/startDepth.dir/src/startDepth.cpp.o.requires
	$(MAKE) -f CMakeFiles/startDepth.dir/build.make CMakeFiles/startDepth.dir/src/startDepth.cpp.o.provides.build
.PHONY : CMakeFiles/startDepth.dir/src/startDepth.cpp.o.provides

CMakeFiles/startDepth.dir/src/startDepth.cpp.o.provides.build: CMakeFiles/startDepth.dir/src/startDepth.cpp.o


# Object files for target startDepth
startDepth_OBJECTS = \
"CMakeFiles/startDepth.dir/src/startDepth.cpp.o"

# External object files for target startDepth
startDepth_EXTERNAL_OBJECTS =

../bin/startDepth: CMakeFiles/startDepth.dir/src/startDepth.cpp.o
../bin/startDepth: CMakeFiles/startDepth.dir/build.make
../bin/startDepth: CMakeFiles/startDepth.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maciej/catkin_ws/src/k2_client-1.0.0/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/startDepth"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/startDepth.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/startDepth.dir/build: ../bin/startDepth

.PHONY : CMakeFiles/startDepth.dir/build

CMakeFiles/startDepth.dir/requires: CMakeFiles/startDepth.dir/src/startDepth.cpp.o.requires

.PHONY : CMakeFiles/startDepth.dir/requires

CMakeFiles/startDepth.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/startDepth.dir/cmake_clean.cmake
.PHONY : CMakeFiles/startDepth.dir/clean

CMakeFiles/startDepth.dir/depend:
	cd /home/maciej/catkin_ws/src/k2_client-1.0.0/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maciej/catkin_ws/src/k2_client-1.0.0 /home/maciej/catkin_ws/src/k2_client-1.0.0 /home/maciej/catkin_ws/src/k2_client-1.0.0/build /home/maciej/catkin_ws/src/k2_client-1.0.0/build /home/maciej/catkin_ws/src/k2_client-1.0.0/build/CMakeFiles/startDepth.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/startDepth.dir/depend

