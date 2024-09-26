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
include src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/compiler_depend.make

# Include the progress variables for this target.
include src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/progress.make

# Include the compile flags for this target's objects.
include src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/flags.make

src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o: src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/flags.make
src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o: /home/davids/projects/substrait-cpp/src/substrait/textplan/tests/SymbolTableTest.cpp
src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o: src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o -MF CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o.d -o CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o -c /home/davids/projects/substrait-cpp/src/substrait/textplan/tests/SymbolTableTest.cpp

src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.i"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davids/projects/substrait-cpp/src/substrait/textplan/tests/SymbolTableTest.cpp > CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.i

src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.s"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davids/projects/substrait-cpp/src/substrait/textplan/tests/SymbolTableTest.cpp -o CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.s

# Object files for target symbol_table_test
symbol_table_test_OBJECTS = \
"CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o"

# External object files for target symbol_table_test
symbol_table_test_EXTERNAL_OBJECTS =

debug/tests/symbol_table_test: src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/SymbolTableTest.cpp.o
debug/tests/symbol_table_test: src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/build.make
debug/tests/symbol_table_test: debug/lib/libsymbol_table.a
debug/tests/symbol_table_test: debug/lib/libsubstrait_common.a
debug/tests/symbol_table_test: debug/lib/libsubstrait_proto.a
debug/tests/symbol_table_test: lib/libgmock.a
debug/tests/symbol_table_test: lib/libgtest.a
debug/tests/symbol_table_test: lib/libgtest_main.a
debug/tests/symbol_table_test: debug/lib/libsubstrait_base_proto_visitor.a
debug/tests/symbol_table_test: debug/lib/libsubstrait_common.a
debug/tests/symbol_table_test: debug/lib/liberror_listener.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/status/libabsl_statusor.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/status/libabsl_status.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_cord.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_cordz_info.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_cord_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_cordz_functions.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/profiling/libabsl_exponential_biased.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_cordz_handle.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/synchronization/libabsl_synchronization.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/synchronization/libabsl_graphcycles_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/time/libabsl_time.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/time/libabsl_civil_time.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/time/libabsl_time_zone.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/crc/libabsl_crc_cord_state.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/crc/libabsl_crc32c.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/crc/libabsl_crc_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/crc/libabsl_crc_cpu_detect.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/types/libabsl_bad_optional_access.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/debugging/libabsl_stacktrace.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_str_format_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/base/libabsl_strerror.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/debugging/libabsl_symbolize.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/debugging/libabsl_debugging_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/debugging/libabsl_demangle_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/base/libabsl_malloc_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/types/libabsl_bad_variant_access.a
debug/tests/symbol_table_test: debug/lib/libsubstrait_expression.a
debug/tests/symbol_table_test: debug/lib/libsubstrait_proto.a
debug/tests/symbol_table_test: /usr/lib/x86_64-linux-gnu/libprotobuf.so
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_strings.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/strings/libabsl_strings_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/base/libabsl_base.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/base/libabsl_spinlock_wait.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/numeric/libabsl_int128.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/base/libabsl_throw_delegate.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/base/libabsl_raw_logging_internal.a
debug/tests/symbol_table_test: third_party/abseil-cpp/absl/base/libabsl_log_severity.a
debug/tests/symbol_table_test: lib/libgtest.a
debug/tests/symbol_table_test: src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../debug/tests/symbol_table_test"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/symbol_table_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/build: debug/tests/symbol_table_test
.PHONY : src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/build

src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/clean:
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests && $(CMAKE_COMMAND) -P CMakeFiles/symbol_table_test.dir/cmake_clean.cmake
.PHONY : src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/clean

src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/depend:
	cd /home/davids/projects/substrait-cpp/build-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davids/projects/substrait-cpp /home/davids/projects/substrait-cpp/src/substrait/textplan/tests /home/davids/projects/substrait-cpp/build-Debug /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/substrait/textplan/tests/CMakeFiles/symbol_table_test.dir/depend

