# SPDX-License-Identifier: Apache-2.0

add_test_case(
        textplan_conversion_test
        SOURCES
        BinaryToTextPlanConversionTest.cpp
        EXTRA_LINK_LIBS
        substrait_textplan_converter
        substrait_common
        parse_result_matchers
        gmock
        gtest
        gtest_main)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH TEXTPLAN_SOURCE_DIR)

add_custom_command(
  TARGET textplan_conversion_test
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Copying unit test data.."
  COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${TEXTPLAN_SOURCE_DIR}/data
                           ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data
  )