# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20)

# set the project name
project(substrait-cpp)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(
  SUBSTRAIT_CPP_BUILD_TESTING
  "Enable substrait-cpp tests. This will enable all other build options automatically."
  ON)

# Local files come first.
include_directories(include)
include_directories(src)

add_subdirectory(third_party)

# Due to packaging changes we use the combined protobuf/absl packaging if
# available otherwise we fallback to the older protobuf method. TODO: Simplify
# once we can require cmake 3.27 (where CONFIG is default).
find_package(Protobuf QUIET CONFIG)
if(${Protobuf_FOUND})
  set(PROTOBUF_LOADED_AS_CONFIG)
else()
  find_package(Protobuf REQUIRED)
  include_directories(${Protobuf_INCLUDE_DIRS})
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")
include(BuildUtils)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  enable_testing()
endif()

add_subdirectory(src/substrait)
