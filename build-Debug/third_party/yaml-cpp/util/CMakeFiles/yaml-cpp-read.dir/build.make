# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /home/davids/.local/lib/python3.10/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/davids/.local/lib/python3.10/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/davids/projects/substrait-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/davids/projects/substrait-cpp/build-Debug

# Include any dependencies generated for this target.
include third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/flags.make

third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.o: third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/flags.make
third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.o: /home/davids/projects/substrait-cpp/third_party/yaml-cpp/util/read.cpp
third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.o: third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.o"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/yaml-cpp/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.o -MF CMakeFiles/yaml-cpp-read.dir/read.cpp.o.d -o CMakeFiles/yaml-cpp-read.dir/read.cpp.o -c /home/davids/projects/substrait-cpp/third_party/yaml-cpp/util/read.cpp

third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/yaml-cpp-read.dir/read.cpp.i"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/yaml-cpp/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davids/projects/substrait-cpp/third_party/yaml-cpp/util/read.cpp > CMakeFiles/yaml-cpp-read.dir/read.cpp.i

third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/yaml-cpp-read.dir/read.cpp.s"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/yaml-cpp/util && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davids/projects/substrait-cpp/third_party/yaml-cpp/util/read.cpp -o CMakeFiles/yaml-cpp-read.dir/read.cpp.s

# Object files for target yaml-cpp-read
yaml__cpp__read_OBJECTS = \
"CMakeFiles/yaml-cpp-read.dir/read.cpp.o"

# External object files for target yaml-cpp-read
yaml__cpp__read_EXTERNAL_OBJECTS =

third_party/yaml-cpp/util/read: third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/read.cpp.o
third_party/yaml-cpp/util/read: third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/build.make
third_party/yaml-cpp/util/read: third_party/yaml-cpp/libyaml-cppd.a
third_party/yaml-cpp/util/read: third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable read"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/yaml-cpp/util && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/yaml-cpp-read.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/build: third_party/yaml-cpp/util/read
.PHONY : third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/build

third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/clean:
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/yaml-cpp/util && $(CMAKE_COMMAND) -P CMakeFiles/yaml-cpp-read.dir/cmake_clean.cmake
.PHONY : third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/clean

third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/depend:
	cd /home/davids/projects/substrait-cpp/build-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davids/projects/substrait-cpp /home/davids/projects/substrait-cpp/third_party/yaml-cpp/util /home/davids/projects/substrait-cpp/build-Debug /home/davids/projects/substrait-cpp/build-Debug/third_party/yaml-cpp/util /home/davids/projects/substrait-cpp/build-Debug/third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : third_party/yaml-cpp/util/CMakeFiles/yaml-cpp-read.dir/depend

