# SPDX-License-Identifier: Apache-2.0

add_subdirectory(grammar)

add_library(
  substrait_textplan_loader
  SubstraitPlanVisitor.cpp
  SubstraitPlanVisitor.h
  SubstraitPlanPipelineVisitor.cpp
  SubstraitPlanPipelineVisitor.h
  SubstraitPlanRelationVisitor.cpp
  SubstraitPlanRelationVisitor.h
  SubstraitPlanTypeVisitor.cpp
  SubstraitPlanTypeVisitor.h
  LoadText.cpp
  LoadText.h
  ParseText.cpp
  ParseText.h
  SubstraitParserErrorListener.cpp)

target_link_libraries(
  substrait_textplan_loader
  symbol_table
  error_listener
  substrait_proto
  substrait_type
  substrait_expression
  textplan_grammar
  fmt::fmt-header-only
  date::date
  date::date-tz
  absl::status
  absl::statusor)

add_executable(planparser Tool.cpp)

target_link_libraries(planparser substrait_textplan_loader error_listener)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()
