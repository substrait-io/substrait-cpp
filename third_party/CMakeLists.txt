# SPDX-License-Identifier: Apache-2.0

if(NOT ${ABSL_INCLUDED_WITH_PROTOBUF})
  set(ABSL_PROPAGATE_CXX_STD ON)
  add_subdirectory(abseil-cpp)
endif()

include(datetime.cmake)

add_subdirectory(fmt)

if(NOT ${GOOGLETEST_INCLUDED_WITH_PROTOBUF})
  add_subdirectory(googletest)
endif()

add_subdirectory(protobuf-matchers)

set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
