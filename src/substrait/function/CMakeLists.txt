# SPDX-License-Identifier: Apache-2.0

set(FUNCTION_SRCS
        Function.cpp
        Extension.cpp
        FunctionLookup.cpp)

add_library(substrait_function ${FUNCTION_SRCS})

target_link_libraries(
        substrait_function
        substrait_type
        yaml-cpp)

if (${SUBSTRAIT_CPP_BUILD_TESTING})
    add_subdirectory(tests)
endif ()