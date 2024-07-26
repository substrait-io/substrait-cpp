# SPDX-License-Identifier: Apache-2.0

# Ensure `option()` in subdirectories honors normal variables set here.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(ABSL_ENABLE_INSTALL ON)

include(datetime.cmake)

add_subdirectory(fmt)

find_package(GTest QUIET)
if(NOT ${GTEST_FOUND})
  message(STATUS "Retrieving external GoogleTest library.")
  include(FetchContent)
  fetchcontent_declare(
    GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    OVERRIDE_FIND_PACKAGE)
  fetchcontent_makeavailable(GTest)
endif()

set(PREVIOUS_BUILD_TESTING ${BUILD_TESTING})
set(BUILD_TESTING OFF)
add_subdirectory(protobuf-matchers)
set(BUILD_TESTING ${PREVIOUS_BUILD_TESTING})

set(YAML_CPP_INSTALL ON)
set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
