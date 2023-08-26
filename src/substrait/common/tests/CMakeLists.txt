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

add_test_case(
  substrait_io_test
  SOURCES
  IoTest.cpp
  EXTRA_LINK_LIBS
  substrait_io
  protobuf-matchers
  gtest
  gtest_main)
