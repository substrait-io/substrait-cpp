# SPDX-License-Identifier: Apache-2.0

add_subdirectory(converter)
add_subdirectory(parser)

add_library(
  symbol_table
  Location.cpp
  Location.h
  PlanPrinterVisitor.cpp
  PlanPrinterVisitor.h
  StringManipulation.cpp
  StringManipulation.h
  SymbolTable.cpp
  SymbolTable.h
  SymbolTablePrinter.cpp
  SymbolTablePrinter.h
  StructuredSymbolData.h
  Any.h)

add_library(error_listener SubstraitErrorListener.cpp SubstraitErrorListener.h)

add_library(parse_result ParseResult.cpp ParseResult.h)

add_dependencies(symbol_table substrait_proto substrait_common absl::strings
                 fmt::fmt-header-only)

target_link_libraries(
  symbol_table
  substrait_base_proto_visitor
  substrait_proto
  substrait_common
  substrait_expression
  absl::strings
  fmt::fmt-header-only
  date::date)

# Provide access to the generated protobuffer headers hierarchy.
target_include_directories(
  symbol_table PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../..>)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

install(TARGETS error_listener parse_result symbol_table
        EXPORT SubstraitTargets)
