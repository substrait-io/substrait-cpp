# SPDX-License-Identifier: Apache-2.0

include(../../../../third_party/datetime.cmake)

add_subdirectory(grammar)

add_library(
  substrait_textplan_loader
  SubstraitPlanVisitor.cpp
  SubstraitPlanVisitor.h
  SubstraitPlanPipelineVisitor.cpp
  SubstraitPlanPipelineVisitor.h
  SubstraitPlanRelationVisitor.cpp
  SubstraitPlanRelationVisitor.h
  SubstraitPlanSubqueryRelationVisitor.cpp
  SubstraitPlanSubqueryRelationVisitor.h
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

if(UNIX)
  add_executable(planparser Tool.cpp)
  target_link_libraries(planparser substrait_textplan_loader error_listener)
  install(TARGETS planparser EXPORT SubstraitTargets)
endif()
install(TARGETS substrait_textplan_loader EXPORT SubstraitTargets)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

