# SPDX-License-Identifier: Apache-2.0

add_library(parse_result_matchers ParseResultMatchers.cpp ParseResultMatchers.h)

add_dependencies(parse_result_matchers parse_result)

target_link_libraries(parse_result_matchers parse_result substrait_proto gmock)

add_test_case(
  symbol_table_test
  SOURCES
  SymbolTableTest.cpp
  EXTRA_LINK_LIBS
  symbol_table
  substrait_common
  substrait_proto
  fmt::fmt-header-only
  gmock
  gtest
  gtest_main)

option(SUBSTRAIT_CPP_ROUNDTRIP_TESTING
       "Enable substrait-cpp textplan roundtrip tests." OFF)

if(${SUBSTRAIT_CPP_ROUNDTRIP_TESTING})
  add_test_case(
    round_trip_test
    SOURCES
    RoundtripTest.cpp
    EXTRA_LINK_LIBS
    substrait_textplan_converter
    substrait_textplan_loader
    substrait_textplan_normalizer
    substrait_common
    substrait_proto
    parse_result_matchers
    protobuf-matchers
    fmt::fmt-header-only
    absl::strings
    gmock
    gtest
    gtest_main)

  cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH TEXTPLAN_SOURCE_DIR)

  add_custom_command(
    TARGET round_trip_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying unit test data.."
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data"
    COMMAND
      ${CMAKE_COMMAND} -E copy
      "${TEXTPLAN_SOURCE_DIR}/converter/data/q6_first_stage.json"
      "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data/q6_first_stage.json"
    COMMAND ${CMAKE_COMMAND} -E copy "${TEXTPLAN_SOURCE_DIR}/data/*.json"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data/")

  message(
    STATUS
      "test data will be here:  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data")
else()

  message(
    STATUS
      "Round trip testing is turned off.  Add SUBSTRAIT_CPP_ROUNDTRIP_TESTING=on to enable."
  )

endif()
