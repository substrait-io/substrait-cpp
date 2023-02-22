# SPDX-License-Identifier: Apache-2.0

add_test_case(
  substrait_type_test
  EXTRA_LINK_LIBS
  substrait_type
  gtest
  gtest_main
  SOURCES
  TypeTest.cpp)
