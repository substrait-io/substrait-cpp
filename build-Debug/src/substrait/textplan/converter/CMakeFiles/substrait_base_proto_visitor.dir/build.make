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
include src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/compiler_depend.make

# Include the progress variables for this target.
include src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/progress.make

# Include the compile flags for this target's objects.
include src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/flags.make

src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o: src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/flags.make
src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o: /home/davids/projects/substrait-cpp/src/substrait/textplan/converter/BasePlanProtoVisitor.cpp
src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o: src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o -MF CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o.d -o CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o -c /home/davids/projects/substrait-cpp/src/substrait/textplan/converter/BasePlanProtoVisitor.cpp

src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.i"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/davids/projects/substrait-cpp/src/substrait/textplan/converter/BasePlanProtoVisitor.cpp > CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.i

src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.s"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/davids/projects/substrait-cpp/src/substrait/textplan/converter/BasePlanProtoVisitor.cpp -o CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.s

# Object files for target substrait_base_proto_visitor
substrait_base_proto_visitor_OBJECTS = \
"CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o"

# External object files for target substrait_base_proto_visitor
substrait_base_proto_visitor_EXTERNAL_OBJECTS =

debug/lib/libsubstrait_base_proto_visitor.a: src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/BasePlanProtoVisitor.cpp.o
debug/lib/libsubstrait_base_proto_visitor.a: src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/build.make
debug/lib/libsubstrait_base_proto_visitor.a: src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/davids/projects/substrait-cpp/build-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../../debug/lib/libsubstrait_base_proto_visitor.a"
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter && $(CMAKE_COMMAND) -P CMakeFiles/substrait_base_proto_visitor.dir/cmake_clean_target.cmake
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/substrait_base_proto_visitor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/build: debug/lib/libsubstrait_base_proto_visitor.a
.PHONY : src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/build

src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/clean:
	cd /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter && $(CMAKE_COMMAND) -P CMakeFiles/substrait_base_proto_visitor.dir/cmake_clean.cmake
.PHONY : src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/clean

src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/depend:
	cd /home/davids/projects/substrait-cpp/build-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/davids/projects/substrait-cpp /home/davids/projects/substrait-cpp/src/substrait/textplan/converter /home/davids/projects/substrait-cpp/build-Debug /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/substrait/textplan/converter/CMakeFiles/substrait_base_proto_visitor.dir/depend

