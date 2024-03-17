# SPDX-License-Identifier: Apache-2.0

add_library(substrait_expression DecimalLiteral.cpp)
target_sources(
  substrait_expression
  PUBLIC FILE_SET HEADERS BASE_DIRS ../../../include/ FILES
         ../../../include/substrait/expression/DecimalLiteral.h)

target_link_libraries(substrait_expression substrait_proto absl::numeric
                      absl::strings)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

install(
  TARGETS substrait_expression
  EXPORT SubstraitTargets
  FILE_SET HEADERS)
