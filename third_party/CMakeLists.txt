# SPDX-License-Identifier: Apache-2.0

if(NOT ${ABSL_INCLUDED_WITH_PROTOBUF})
  set(ABSL_PROPAGATE_CXX_STD ON)
  add_subdirectory(abseil-cpp)
endif()

include(datetime.cmake)

add_subdirectory(fmt)

find_package(GTest QUIET CONFIG)
if(NOT ${GTEST_FOUND})
  message(STATUS "Retrieving external GoogleTest library.")
  include(FetchContent)
  FetchContent_Declare(GTest
          GIT_REPOSITORY https://github.com/google/googletest.git
          GIT_TAG v1.14.0
          OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(GTest)
endif()

add_subdirectory(protobuf-matchers)

set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
