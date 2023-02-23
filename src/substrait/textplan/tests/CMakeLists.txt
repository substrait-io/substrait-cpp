# SPDX-License-Identifier: Apache-2.0

add_library(parse_result_matchers
        ParseResultMatchers.cpp ParseResultMatchers.h)

add_dependencies(parse_result_matchers parse_result)

target_link_libraries(
        parse_result_matchers
        parse_result
        gmock)

add_test_case(
        symbol_table_test
        SOURCES
        SymbolTableTest.cpp
        EXTRA_LINK_LIBS
        symbol_table
        substrait_proto
        fmt::fmt-header-only
        gmock
        gtest
        gtest_main)