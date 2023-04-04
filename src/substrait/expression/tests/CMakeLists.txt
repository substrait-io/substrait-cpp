# SPDX-License-Identifier: Apache-2.0

add_test_case(
  substrait_expression_test
  SOURCES
  DecimalTest.cpp
  EXTRA_LINK_LIBS
  substrait_expression
  gmock
  gtest
  gtest_main)
