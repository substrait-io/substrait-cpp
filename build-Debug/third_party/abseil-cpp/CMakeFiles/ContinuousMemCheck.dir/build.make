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

# Utility rule file for ContinuousMemCheck.

# Include any custom commands dependencies for this target.
include third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/progress.make

third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck:
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp && /home/davids/.local/lib/python3.10/site-packages/cmake/data/bin/ctest -D ContinuousMemCheck

ContinuousMemCheck: third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck
ContinuousMemCheck: third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/build.make
.PHONY : ContinuousMemCheck

# Rule to build all files generated by this target.
third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/build: ContinuousMemCheck
.PHONY : third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/build

third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/clean:
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousMemCheck.dir/cmake_clean.cmake
.PHONY : third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/clean

third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/depend:
	cd /home/davids/projects/substrait-cpp/build-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davids/projects/substrait-cpp /home/davids/projects/substrait-cpp/third_party/abseil-cpp /home/davids/projects/substrait-cpp/build-Debug /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : third_party/abseil-cpp/CMakeFiles/ContinuousMemCheck.dir/depend

