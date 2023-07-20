# SPDX-License-Identifier: Apache-2.0

add_library(substrait_common Exceptions.cpp)
target_link_libraries(substrait_common fmt::fmt-header-only)

add_library(substrait_io Io.cpp)
add_dependencies(
  substrait_io
  substrait_proto
  substrait_textplan_converter
  substrait_textplan_loader
  fmt::fmt-header-only
  absl::status
  absl::statusor)
target_link_libraries(substrait_io substrait_proto substrait_textplan_converter
                      substrait_textplan_loader absl::status absl::statusor)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()
