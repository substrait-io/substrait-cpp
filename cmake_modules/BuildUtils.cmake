# SPDX-License-Identifier: Apache-2.0

# This file contains various cmake helper functions that are used throughout the
# project.

# Add a new test case, with or without an executable that should be built.
#
# TEST_NAME is the name of the test.
#
# SOURCES is the list of C++ source files to compile into the test executable.
function(ADD_TEST_CASE TEST_NAME)
  set(multi_value_args SOURCES EXTRA_LINK_LIBS EXTRA_INCLUDES
                       EXTRA_DEPENDENCIES)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}"
                        "${multi_value_args}" ${ARGN})
  if(ARG_UNPARSED_ARGUMENTS)
    message(
      SEND_ERROR "Error: unrecognized arguments: ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  if(ARG_SOURCES)
    set(SOURCES ${ARG_SOURCES})
  else()
    message(SEND_ERROR "Error: SOURCES is a required argument to add_test_case")
  endif()

  add_executable(${TEST_NAME} ${SOURCES})
  set_target_properties(
    ${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests)

  if(ARG_EXTRA_LINK_LIBS)
    target_link_libraries(${TEST_NAME} PRIVATE ${ARG_EXTRA_LINK_LIBS})
  endif()

  if(ARG_EXTRA_INCLUDES)
    target_include_directories(${TEST_NAME} SYSTEM PUBLIC ${ARG_EXTRA_INCLUDES})
  endif()

  if(ARG_EXTRA_DEPENDENCIES)
    add_dependencies(${TEST_NAME} ${ARG_EXTRA_DEPENDENCIES})
  endif()

  add_test(
    NAME ${TEST_NAME}
    COMMAND $<TARGET_FILE:${TEST_NAME}>
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")
endfunction()
