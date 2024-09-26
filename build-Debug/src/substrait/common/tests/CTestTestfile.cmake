# CMake generated Testfile for
# Source directory: /home/davids/projects/substrait-cpp/src/substrait/common/tests
# Build directory: /home/davids/projects/substrait-cpp/build-Debug/src/substrait/common/tests
#
# This file includes the relevant testing commands required for
# testing this directory and lists subdirectories to be tested as well.
add_test(
  [=[substrait_common_test]=]
  "/home/davids/projects/substrait-cpp/build-Debug/debug/tests/substrait_common_test"
)
set_tests_properties(
  [=[substrait_common_test]=]
  PROPERTIES
    WORKING_DIRECTORY
    "/home/davids/projects/substrait-cpp/build-Debug/debug/tests"
    _BACKTRACE_TRIPLES
    "/home/davids/projects/substrait-cpp/cmake_modules/BuildUtils.cmake;44;add_test;/home/davids/projects/substrait-cpp/src/substrait/common/tests/CMakeLists.txt;3;add_test_case;/home/davids/projects/substrait-cpp/src/substrait/common/tests/CMakeLists.txt;0;"
)
add_test(
  [=[substrait_io_test]=]
  "/home/davids/projects/substrait-cpp/build-Debug/debug/tests/substrait_io_test"
)
set_tests_properties(
  [=[substrait_io_test]=]
  PROPERTIES
    WORKING_DIRECTORY
    "/home/davids/projects/substrait-cpp/build-Debug/debug/tests"
    _BACKTRACE_TRIPLES
    "/home/davids/projects/substrait-cpp/cmake_modules/BuildUtils.cmake;44;add_test;/home/davids/projects/substrait-cpp/src/substrait/common/tests/CMakeLists.txt;13;add_test_case;/home/davids/projects/substrait-cpp/src/substrait/common/tests/CMakeLists.txt;0;"
)
