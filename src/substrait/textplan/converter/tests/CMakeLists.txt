# SPDX-License-Identifier: Apache-2.0

INCLUDE_DIRECTORIES(${PROTO_INCLUDE_DIR} ${substrait_textplan_loader_INCLUDE_DIRS})
add_executable(
        textplan_conversion_test
        BinaryToTextPlanConversionTest.cpp)

add_dependencies(textplan_conversion_test substrait_textplan_converter)

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH TEXTPLAN_SOURCE_DIR)

add_test(
        NAME textplan_conversion_test
        COMMAND textplan_conversion_test
        WORKING_DIRECTORY ${TEXTPLAN_SOURCE_DIR})

target_link_libraries(
        textplan_conversion_test
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
                           ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
  )