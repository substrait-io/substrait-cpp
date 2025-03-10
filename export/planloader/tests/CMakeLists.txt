# SPDX-License-Identifier: Apache-2.0

cmake_path(GET CMAKE_CURRENT_BINARY_DIR PARENT_PATH
           CMAKE_CURRENT_BINARY_PARENT_DIR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_PARENT_DIR})

add_test_case(
  planloader_test
  SOURCES
  PlanLoaderTest.cpp
  EXTRA_LINK_LIBS
  planloader
  gmock
  gtest
  gtest_main)

set(TEXTPLAN_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/substrait/textplan")

add_custom_command(
  TARGET planloader_test
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Copying unit test data.."
  COMMAND ${CMAKE_COMMAND} -E make_directory
          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data"
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${TEXTPLAN_SOURCE_DIR}/converter/data/q6_first_stage.json"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data/q6_first_stage.json")

message(
  STATUS "test data will be here:  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data"
)

# For Windows: copy planloader dll to the test executable directory so that it
# can be found during test execution.
if(WIN32)
  add_custom_command(
    TARGET planloader_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:planloader>
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests")
endif()
