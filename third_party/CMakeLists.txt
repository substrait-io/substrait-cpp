# SPDX-License-Identifier: Apache-2.0

if(NOT ${ABSL_INCLUDED_WITH_PROTOBUF})
  set(ABSL_PROPAGATE_CXX_STD ON)
  add_subdirectory(abseil-cpp)
endif()

include(datetime.cmake)

add_subdirectory(fmt)

find_package(GTest QUIET CONFIG)
if(NOT ${GTEST_FOUND})
  message(STATUS "Using bundled GoogleTest library.")
  add_subdirectory(googletest)
  FetchContent_SetPopulated(
          GTest
          SOURCE_DIR googletest
          BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest
  )
endif()

add_subdirectory(protobuf-matchers)

set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
