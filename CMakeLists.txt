# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.24)

# set the project name
project(substrait-cpp)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

option(SUBSTRAIT_CPP_SANITIZE_DEBUG_BUILD
       "Turns on address and undefined memory sanitization runtime checking."
       OFF)

if(${SUBSTRAIT_CPP_SANITIZE_DEBUG_BUILD})
  add_compile_options($<$<CONFIG:Debug>:-fsanitize=undefined>)
  add_link_options($<$<CONFIG:Debug>:-fsanitize=undefined>)

  add_compile_options($<$<CONFIG:Debug>:-fsanitize=address>)
  add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)
endif()

option(
  SUBSTRAIT_CPP_BUILD_TESTING
  "Enable substrait-cpp tests. This will enable all other build options automatically."
  ON)

set(SUBSTRAIT_CPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Setup ANTLR4 dependency. It's important that this is done at the
# top-level CMakeLists.txt, given that the ANTLR4 build may generate deeply nested
# paths - which may be a problem for MSVC, with its 260 character path limit.
list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/third_party/antlr4/cmake")

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DANTLR4CPP_STATIC)
# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR4_BUILD_CPP_TESTS OFF)
# Note: df4d68c adds a fix for MSVC compilers. No release has been made since;
# latest release was 4.13.2. Revert back to a tag once 4.13.3 is released.
set(ANTLR4_TAG df4d68c09cdef73e023b8838a8bc7ca4dff1d1de)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})
set(ANTLR_EXECUTABLE_DIR ${CMAKE_CURRENT_BINARY_DIR})
file(DOWNLOAD https://www.antlr.org/download/antlr-4.13.2-complete.jar
     "${ANTLR_EXECUTABLE_DIR}/antlr.jar")
set(ANTLR_EXECUTABLE "${ANTLR_EXECUTABLE_DIR}/antlr.jar")

# Local files come first.
include_directories(include)
include_directories(src)

# TODO: Simplify once we can require cmake 3.27 (where CONFIG is default).

# Due to packaging changes we use the combined protobuf/absl packaging if
# available otherwise we fallback to the older protobuf method.
find_package(Protobuf QUIET CONFIG)
if(${Protobuf_FOUND})
  message(STATUS "Modern protobuf library located.")
  set(ABSL_INCLUDED_WITH_PROTOBUF ON)
else()
  find_package(Protobuf QUIET)
  if(${Protobuf_FOUND})
    message(STATUS "Legacy protobuf library located.")
    include_directories(${Protobuf_INCLUDE_DIRS})
    set(ABSL_INCLUDED_WITH_PROTOBUF OFF)
  else()
    message(STATUS "Fetching external protobuf library.")
    include(third_party/protobuf.cmake)
    set(ABSL_INCLUDED_WITH_PROTOBUF ON)
  endif()
endif()

add_subdirectory(third_party)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")
include(BuildUtils)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  enable_testing()
endif()

install(EXPORT SubstraitTargets DESTINATION lib/cmake/Substrait)

add_subdirectory(src/substrait)
add_subdirectory(export)
