# SPDX-License-Identifier: Apache-2.0

add_executable(
        substrait_function_test
        FunctionLookupTest.cpp)

add_test(
        substrait_function_test
        substrait_function_test)

target_link_libraries(
        substrait_function_test
        substrait_function
        gtest
        gtest_main)
