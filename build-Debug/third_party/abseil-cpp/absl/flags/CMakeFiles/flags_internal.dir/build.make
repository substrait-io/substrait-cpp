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
include third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/flags.make

third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.o: third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/flags.make
third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.o: /home/davids/projects/substrait-cpp/third_party/abseil-cpp/absl/flags/internal/flag.cc
third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.o: third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.o"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.o -MF CMakeFiles/flags_internal.dir/internal/flag.cc.o.d -o CMakeFiles/flags_internal.dir/internal/flag.cc.o -c /home/davids/projects/substrait-cpp/third_party/abseil-cpp/absl/flags/internal/flag.cc

third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/flags_internal.dir/internal/flag.cc.i"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davids/projects/substrait-cpp/third_party/abseil-cpp/absl/flags/internal/flag.cc > CMakeFiles/flags_internal.dir/internal/flag.cc.i

third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/flags_internal.dir/internal/flag.cc.s"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davids/projects/substrait-cpp/third_party/abseil-cpp/absl/flags/internal/flag.cc -o CMakeFiles/flags_internal.dir/internal/flag.cc.s

# Object files for target flags_internal
flags_internal_OBJECTS = \
"CMakeFiles/flags_internal.dir/internal/flag.cc.o"

# External object files for target flags_internal
flags_internal_EXTERNAL_OBJECTS =

third_party/abseil-cpp/absl/flags/libabsl_flags_internal.a: third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/internal/flag.cc.o
third_party/abseil-cpp/absl/flags/libabsl_flags_internal.a: third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/build.make
third_party/abseil-cpp/absl/flags/libabsl_flags_internal.a: third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libabsl_flags_internal.a"
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags && $(CMAKE_COMMAND) -P CMakeFiles/flags_internal.dir/cmake_clean_target.cmake
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/flags_internal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/build: third_party/abseil-cpp/absl/flags/libabsl_flags_internal.a
.PHONY : third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/build

third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/clean:
	cd /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags && $(CMAKE_COMMAND) -P CMakeFiles/flags_internal.dir/cmake_clean.cmake
.PHONY : third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/clean

third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/depend:
	cd /home/davids/projects/substrait-cpp/build-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davids/projects/substrait-cpp /home/davids/projects/substrait-cpp/third_party/abseil-cpp/absl/flags /home/davids/projects/substrait-cpp/build-Debug /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags /home/davids/projects/substrait-cpp/build-Debug/third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : third_party/abseil-cpp/absl/flags/CMakeFiles/flags_internal.dir/depend

