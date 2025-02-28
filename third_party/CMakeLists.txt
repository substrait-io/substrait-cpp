# SPDX-License-Identifier: Apache-2.0

# Ensure `option()` in subdirectories honors normal variables set here.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(ABSL_ENABLE_INSTALL ON)
if(NOT ${ABSL_INCLUDED_WITH_PROTOBUF})
  set(ABSL_PROPAGATE_CXX_STD ON)
  add_subdirectory(abseil-cpp)
endif()

include(datetime.cmake)

add_subdirectory(fmt)

if(WIN32)
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt
      ON
      CACHE BOOL "" FORCE)
endif()

find_package(GTest QUIET)
if(NOT ${GTEST_FOUND})
  message(STATUS "Retrieving external GoogleTest library.")
  include(FetchContent)
  fetchcontent_declare(
    GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    OVERRIDE_FIND_PACKAGE)
  fetchcontent_makeavailable(GTest)
endif()
if(MSVC)
  # ------------------------------------------------------------------------------
  # gtest MSVC fix
  # ------------------------------------------------------------------------------
  # For some reason, googletest has include path issues when built with MSVC.
  # Specifically, this seems like some incorrect assumptions about include paths
  # inside the gmock project.
  # We can fix this by injecting the include paths here.
  function(fix_gtest_include TARGET)
    target_include_directories(
      ${TARGET}
      PUBLIC $<BUILD_INTERFACE:${gtest_SOURCE_DIR}>
              $<BUILD_INTERFACE:${gtest_SOURCE_DIR}/include>
              $<BUILD_INTERFACE:${gtest_SOURCE_DIR}/googletest>
              $<BUILD_INTERFACE:${gtest_SOURCE_DIR}/googletest/include>
              $<INSTALL_INTERFACE:include/${TARGET}>)
  endfunction()
  set(gtest_erroneous_targets gmock gmock_main)
  foreach(target ${gtest_erroneous_targets})
    fix_gtest_include(${target})
  endforeach()
endif()

set(PREVIOUS_BUILD_TESTING ${BUILD_TESTING})
set(BUILD_TESTING OFF)
add_subdirectory(protobuf-matchers)
set(BUILD_TESTING ${PREVIOUS_BUILD_TESTING})

set(YAML_CPP_INSTALL ON)
set(YAML_CPP_BUILD_TESTS
    OFF
    CACHE BOOL "Enable testing")
include_directories(yaml-cpp/include)
add_subdirectory(yaml-cpp)
