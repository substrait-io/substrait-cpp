# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

option(SUBSTRAIT_CPP_USE_SYSTEM_GTEST "Use system GTest via find_package" ON)
option(SUBSTRAIT_CPP_FIND_GTEST_CONFIG
       "Use CONFIG mode to find system GTest via find_package" ON)
option(SUBSTRAIT_CPP_FETCH_GTEST "Download GTest via FetchContent if not found"
       ON)
set(SUBSTRAIT_CPP_GTEST_FETCH_TAG
    ${SUBSTRAIT_CPP_GTEST_DEFAULT_FETCH_TAG}
    CACHE STRING "Git tag or commit to use for GTest FetchContent")

# First use `find_package`. This allows downstream projects to inject their
# version with `FetchContent_Declare(... OVERRIDE_FIND_PACKAGE`.
if(SUBSTRAIT_CPP_USE_SYSTEM_GTEST)
  if(SUBSTRAIT_CPP_FIND_GTEST_CONFIG)
    find_package(GTest CONFIG)
  else()
    find_package(GTest)
  endif()
endif()

# Now fall back to using `FetchContent`.
if(NOT GTest_FOUND AND SUBSTRAIT_CPP_FETCH_GTEST)
  message(STATUS "Fetching googletest version ${SUBSTRAIT_CPP_GTEST_FETCH_TAG}")
  include(FetchContent)
  fetchcontent_declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG ${SUBSTRAIT_CPP_GTEST_FETCH_TAG}
    SYSTEM OVERRIDE_FIND_PACKAGE CMAKE_ARGS
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})
  if(MSVC)
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "" FORCE)
    add_compile_options("/W0")
  else()
    add_compile_options("-w")
  endif()
  fetchcontent_makeavailable(googletest)
  fetchcontent_getproperties(googletest SOURCE_DIR GTest_SOURCE_DIR)
  if(TARGET gmock AND NOT TARGET GTest::gmock)
    add_library(GTest::gmock ALIAS gmock)
  endif()
  if(TARGET gmock_main AND NOT TARGET GTest::gmock_main)
    add_library(GTest::gmock_main ALIAS gmock_main)
  endif()
endif()

if(NOT TARGET GTest::gmock)
  message(FATAL_ERROR "GTest is required but was not found or fetched.")
endif()
