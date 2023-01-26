# SPDX-License-Identifier: Apache-2.0

set(TEXTPLAN_SRCS
        PlanConverter.cpp SymbolTable.cpp PlanConverter.h SymbolTable.h)

add_library(substrait_textplan ${TEXTPLAN_SRCS})

target_link_libraries(
        substrait_textplan
        substrait_common
        proto)

if (${BUILD_TESTING})
    add_subdirectory(tests)
endif ()
