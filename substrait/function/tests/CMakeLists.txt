# SPDX-License-Identifier: Apache-2.0

add_test_case(
  substrait_function_test
  SOURCES
  FunctionLookupTest.cpp
  EXTRA_LINK_LIBS
  substrait_function
  gtest
  gtest_main)
