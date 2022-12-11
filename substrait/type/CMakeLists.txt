# SPDX-License-Identifier: Apache-2.0

set(TYPE_SRCS
        Type.cpp)

add_library(substrait_type ${TYPE_SRCS})

target_link_libraries(
        substrait_type
        substrait_common)

if (${BUILD_TESTING})
    add_subdirectory(tests)
endif ()