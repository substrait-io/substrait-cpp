# SPDX-License-Identifier: Apache-2.0

add_library(substrait_common Exceptions.cpp)
target_link_libraries(substrait_common fmt::fmt-header-only)

add_library(substrait_io STATIC Io.cpp)
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

if (${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif ()

install(TARGETS substrait_io LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ../../../include/substrait/common/Io.h
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/substrait/common")

add_executable(plantransformer Tool.cpp)

target_link_libraries(plantransformer substrait_io)
