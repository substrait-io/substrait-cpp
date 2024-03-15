# SPDX-License-Identifier: Apache-2.0

add_library(planloader SHARED planloader.cpp)

add_dependencies(planloader substrait_io)
target_link_libraries(planloader substrait_io)

install(
  TARGETS planloader
          substrait_io
          substrait_common
          substrait_proto
          substrait_textplan_converter
          substrait_textplan_loader
          substrait_base_proto_visitor
          symbol_table
          error_listener
          substrait_type
          substrait_expression
          textplan_grammar
  EXPORT SubstraitTargets)
install(FILES ../../include/substrait/common/Io.h
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/substrait/common")
install(EXPORT SubstraitTargets DESTINATION lib/cmake/Substrait)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()
