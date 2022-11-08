# SPDX-License-Identifier: Apache-2.0

find_package(fmt)

add_library(
        substrait_common
        Exceptions.cpp)

target_link_libraries(
        substrait_common
        fmt::fmt-header-only)

