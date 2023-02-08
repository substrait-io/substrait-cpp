# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.10)

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

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

add_subdirectory(third_party)
include_directories(include)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")
include(BuildUtils)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  enable_testing()
endif()

add_subdirectory(substrait)
