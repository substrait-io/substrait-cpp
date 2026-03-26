# SPDX-License-Identifier: Apache-2.0

# Ensure `option()` in subdirectories honors normal variables set here.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(SUBSTRAIT_CPP_PROTOBUF_DEFAULT_FETCH_TAG "v29.3")
set(SUBSTRAIT_CPP_GTEST_DEFAULT_FETCH_TAG "v1.14.0")

include(gtest.cmake)
include(datetime.cmake)
include(protobuf.cmake)

set(ABSL_ENABLE_INSTALL ON)
if(NOT ${ABSL_INCLUDED_WITH_PROTOBUF})
  set(ABSL_PROPAGATE_CXX_STD ON)
  add_subdirectory(abseil-cpp)
endif()

add_subdirectory(fmt)

set(PROTOBUF_MATCHERS_BUILD_TESTING OFF)
add_subdirectory(protobuf-matchers)

set(YAML_CPP_INSTALL ON)
set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
