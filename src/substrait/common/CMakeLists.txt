# SPDX-License-Identifier: Apache-2.0

add_library(substrait_common Exceptions.cpp)
target_sources(
  substrait_common PUBLIC FILE_SET HEADERS BASE_DIRS ../../../include/ FILES
                          ../../../include/substrait/common/Exceptions.h)
target_link_libraries(substrait_common fmt::fmt-header-only)

add_library(substrait_io STATIC Io.cpp)
target_sources(substrait_io PUBLIC FILE_SET HEADERS BASE_DIRS ../../../include/
                                   FILES ../../../include/substrait/common/Io.h)

add_dependencies(
  substrait_io
  substrait_proto
  substrait_textplan_converter
  substrait_textplan_loader
  fmt::fmt-header-only
  absl::status
  absl::statusor)
target_include_directories(
  substrait_io
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../include/substrait/common>
    $<INSTALL_INTERFACE:include/substrait/common>)
target_link_libraries(substrait_io substrait_proto substrait_textplan_converter
                      substrait_textplan_loader absl::status absl::statusor)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

add_executable(plantransformer PlanTransformerTool.cpp)

target_link_libraries(plantransformer substrait_io)

install(
  TARGETS substrait_common substrait_io plantransformer
  EXPORT SubstraitTargets
  LIBRARY FILE_SET HEADERS)
