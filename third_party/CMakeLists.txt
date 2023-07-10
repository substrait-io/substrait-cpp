# SPDX-License-Identifier: Apache-2.0

if(${ABSL_INCLUDED_WITH_PROTOBUF})
  # Remove abseil-cpp from the repo when we can require cmake 3.27.
else()
  set(ABSL_PROPAGATE_CXX_STD ON)
  add_subdirectory(abseil-cpp)
endif()

set(BUILD_TZ_LIB ON)
add_subdirectory(datetime)

add_subdirectory(fmt)
add_subdirectory(googletest)

add_subdirectory(protobuf-matchers)

set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
