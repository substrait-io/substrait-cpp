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
