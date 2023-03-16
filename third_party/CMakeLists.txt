# SPDX-License-Identifier: Apache-2.0

set(ABSL_PROPAGATE_CXX_STD ON)
add_subdirectory(abseil-cpp)

add_subdirectory(fmt)
add_subdirectory(googletest)

set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
