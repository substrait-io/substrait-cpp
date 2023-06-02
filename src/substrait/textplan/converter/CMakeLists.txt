# SPDX-License-Identifier: Apache-2.0

set(TEXTPLAN_SRCS
    InitialPlanProtoVisitor.cpp
    InitialPlanProtoVisitor.h
    BasePlanProtoVisitor.cpp
    BasePlanProtoVisitor.h
    PipelineVisitor.cpp
    PipelineVisitor.h
    PlanPrinterVisitor.cpp
    PlanPrinterVisitor.h
    LoadBinary.cpp
    LoadBinary.h
    ParseBinary.cpp
    ParseBinary.h)

add_library(substrait_textplan_converter ${TEXTPLAN_SRCS})

target_link_libraries(
  substrait_textplan_converter substrait_common substrait_expression
  substrait_proto symbol_table error_listener
  date::date)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

add_executable(planconverter Tool.cpp)

target_link_libraries(planconverter substrait_textplan_converter)
