# SPDX-License-Identifier: Apache-2.0

add_test_case(
  substrait_common_test
  SOURCES
  NumberUtilsTest.cpp
  StringUtilsTest.cpp
  EXTRA_LINK_LIBS
  substrait_common
  gtest
  gtest_main)
