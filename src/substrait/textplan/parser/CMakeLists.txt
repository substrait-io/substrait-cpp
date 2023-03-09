# SPDX-License-Identifier: Apache-2.0

add_subdirectory(grammar)

add_library(
  substrait_textplan_loader
  SubstraitPlanVisitor.cpp SubstraitPlanVisitor.h ParseText.cpp ParseText.h
  SubstraitParserErrorListener.cpp)

target_link_libraries(substrait_textplan_loader symbol_table error_listener
                      substrait_proto textplan_grammar fmt::fmt-header-only)

add_executable(planparser Tool.cpp)

include_directories(${textplan_grammar_INCLUDE_DIRS})
target_link_libraries(planparser substrait_textplan_loader error_listener)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()
