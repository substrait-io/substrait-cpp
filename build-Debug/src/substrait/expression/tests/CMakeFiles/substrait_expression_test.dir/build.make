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
include src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/compiler_depend.make

# Include the progress variables for this target.
include src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/progress.make

# Include the compile flags for this target's objects.
include src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/flags.make

src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o: src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/flags.make
src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o: /home/davids/projects/substrait-cpp/src/substrait/expression/tests/DecimalTest.cpp
src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o: src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/expression/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o -MF CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o.d -o CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o -c /home/davids/projects/substrait-cpp/src/substrait/expression/tests/DecimalTest.cpp

src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.i"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/expression/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davids/projects/substrait-cpp/src/substrait/expression/tests/DecimalTest.cpp > CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.i

src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.s"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/expression/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davids/projects/substrait-cpp/src/substrait/expression/tests/DecimalTest.cpp -o CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.s

# Object files for target substrait_expression_test
substrait_expression_test_OBJECTS = \
"CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o"

# External object files for target substrait_expression_test
substrait_expression_test_EXTERNAL_OBJECTS =

debug/tests/substrait_expression_test: src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DecimalTest.cpp.o
debug/tests/substrait_expression_test: src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/build.make
debug/tests/substrait_expression_test: debug/lib/libsubstrait_expression.a
debug/tests/substrait_expression_test: lib/libgmock.a
debug/tests/substrait_expression_test: lib/libgtest.a
debug/tests/substrait_expression_test: lib/libgtest_main.a
debug/tests/substrait_expression_test: debug/lib/libsubstrait_proto.a
debug/tests/substrait_expression_test: /usr/lib/x86_64-linux-gnu/libprotobuf.so
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/strings/libabsl_strings.a
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/numeric/libabsl_int128.a
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/strings/libabsl_strings_internal.a
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/base/libabsl_base.a
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/base/libabsl_spinlock_wait.a
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/base/libabsl_throw_delegate.a
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/base/libabsl_raw_logging_internal.a
debug/tests/substrait_expression_test: third_party/abseil-cpp/absl/base/libabsl_log_severity.a
debug/tests/substrait_expression_test: lib/libgtest.a
debug/tests/substrait_expression_test: src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../debug/tests/substrait_expression_test"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/expression/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/substrait_expression_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/build: debug/tests/substrait_expression_test
.PHONY : src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/build

src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/clean:
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/expression/tests && $(CMAKE_COMMAND) -P CMakeFiles/substrait_expression_test.dir/cmake_clean.cmake
.PHONY : src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/clean

src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/depend:
	cd /home/davids/projects/substrait-cpp/build-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davids/projects/substrait-cpp /home/davids/projects/substrait-cpp/src/substrait/expression/tests /home/davids/projects/substrait-cpp/build-Debug /home/davids/projects/substrait-cpp/build-Debug/src/substrait/expression/tests /home/davids/projects/substrait-cpp/build-Debug/src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/substrait/expression/tests/CMakeFiles/substrait_expression_test.dir/depend

