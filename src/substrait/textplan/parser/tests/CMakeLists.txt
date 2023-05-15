# SPDX-License-Identifier: Apache-2.0

add_test_case(
  textplan_parser_test
  SOURCES
  TextPlanParserTest.cpp
  EXTRA_LINK_LIBS
  substrait_textplan_loader
  substrait_common
  parse_result_matchers
  protobuf-matchers
  gmock
  gtest
  gtest_main)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH TEXTPLAN_SOURCE_DIR)

add_custom_command(
  TARGET textplan_parser_test
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Copying unit test data..."
  COMMAND ${CMAKE_COMMAND} -E make_directory
          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data"
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${TEXTPLAN_SOURCE_DIR}/data/provided_sample1.splan"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data/provided_sample1.splan")
