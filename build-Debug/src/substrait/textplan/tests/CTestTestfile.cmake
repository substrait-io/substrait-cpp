# CMake generated Testfile for
# Source directory: /home/davids/projects/substrait-cpp/src/substrait/textplan/tests
# Build directory: /home/davids/projects/substrait-cpp/build-Debug/src/substrait/textplan/tests
#
# This file includes the relevant testing commands required for
# testing this directory and lists subdirectories to be tested as well.
add_test(
  [=[symbol_table_test]=]
  "/home/davids/projects/substrait-cpp/build-Debug/debug/tests/symbol_table_test"
)
set_tests_properties(
  [=[symbol_table_test]=]
  PROPERTIES
    WORKING_DIRECTORY
    "/home/davids/projects/substrait-cpp/build-Debug/debug/tests"
    _BACKTRACE_TRIPLES
    "/home/davids/projects/substrait-cpp/cmake_modules/BuildUtils.cmake;44;add_test;/home/davids/projects/substrait-cpp/src/substrait/textplan/tests/CMakeLists.txt;10;add_test_case;/home/davids/projects/substrait-cpp/src/substrait/textplan/tests/CMakeLists.txt;0;"
)
add_test(
  [=[substrait_textplan_round_trip_test]=]
  "/home/davids/projects/substrait-cpp/build-Debug/debug/tests/substrait_textplan_round_trip_test"
)
set_tests_properties(
  [=[substrait_textplan_round_trip_test]=]
  PROPERTIES
    WORKING_DIRECTORY
    "/home/davids/projects/substrait-cpp/build-Debug/debug/tests"
    _BACKTRACE_TRIPLES
    "/home/davids/projects/substrait-cpp/cmake_modules/BuildUtils.cmake;44;add_test;/home/davids/projects/substrait-cpp/src/substrait/textplan/tests/CMakeLists.txt;23;add_test_case;/home/davids/projects/substrait-cpp/src/substrait/textplan/tests/CMakeLists.txt;0;"
)
