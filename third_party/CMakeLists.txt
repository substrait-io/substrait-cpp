# SPDX-License-Identifier: Apache-2.0

add_subdirectory(googletest)

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
