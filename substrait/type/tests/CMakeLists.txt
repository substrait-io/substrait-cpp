# SPDX-License-Identifier: Apache-2.0

add_executable(
        substrait_type_test
        TypeTest.cpp)

add_test(
        substrait_type_test
        substrait_type_test)

target_link_libraries(
        substrait_type_test
        substrait_type
        gtest
        gtest_main)
