# SPDX-License-Identifier: Apache-2.0

set(ABSL_ENABLE_INSTALL ON)

option(SUBSTRAIT_CPP_USE_SYSTEM_ABSL "Use system absl via find_package" ON)
option(SUBSTRAIT_CPP_FIND_ABSL_CONFIG
       "Use CONFIG mode to find system absl via find_package" ON)
option(SUBSTRAIT_CPP_FETCH_ABSL "Download absl via FetchContent if not found"
       ON)
set(SUBSTRAIT_CPP_ABSL_FETCH_TAG
    ${SUBSTRAIT_CPP_ABSL_DEFAULT_FETCH_TAG}
    CACHE STRING "Git tag or commit to use for absl FetchContent")

# First use `find_package`. This allows downstream projects to inject their
# version with `FetchContent_Declare(... OVERRIDE_FIND_PACKAGE`.
if(SUBSTRAIT_CPP_USE_SYSTEM_ABSL)
  if(SUBSTRAIT_CPP_FIND_ABSL_CONFIG)
    find_package(absl CONFIG)
  else()
    find_package(absl)
  endif()
endif()

# Now fall back to using `FetchContent`.
if(NOT absl_FOUND AND SUBSTRAIT_CPP_FETCH_ABSL)
  message(STATUS "Fetching absl-cpp version ${SUBSTRAIT_CPP_ABSL_FETCH_TAG}")
  include(FetchContent)
  fetchcontent_declare(
    absl-cpp
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG ${SUBSTRAIT_CPP_ABSL_FETCH_TAG}
    SYSTEM OVERRIDE_FIND_PACKAGE CMAKE_ARGS
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})
  if(MSVC)
    add_compile_options("/W0")
  else()
    add_compile_options("-w")
  endif()
  set(ABSL_PROPAGATE_CXX_STD ON)
  fetchcontent_makeavailable(absl-cpp)
  fetchcontent_getproperties(absl-cpp SOURCE_DIR absl_SOURCE_DIR)
  if(TARGET status AND NOT TARGET absl::status)
    add_library(absl::status ALIAS status)
  endif()
  if(TARGET statusor AND NOT TARGET absl::statusor)
    add_library(absl::statusor ALIAS statusor)
  endif()
  if(TARGET strings AND NOT TARGET absl::strings)
    add_library(absl::strings ALIAS strings)
  endif()
endif()

if(NOT TARGET absl::status)
  message(FATAL_ERROR "absl-cpp is required but was not found or fetched.")
endif()
