# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

option(SUBSTRAIT_CPP_USE_SYSTEM_PROTOBUF "Use system Protobuf via find_package"
       ON)
option(SUBSTRAIT_CPP_FIND_PROTOBUF_CONFIG
       "Use CONFIG mode to find system Protobuf via find_package" ON)
option(SUBSTRAIT_CPP_FETCH_PROTOBUF
       "Download Protobuf via FetchContent if not found" ON)
set(SUBSTRAIT_CPP_ABSL_FETCH_TAG
    ${SUBSTRAIT_CPP_ABSL_DEFAULT_FETCH_TAG}
    CACHE STRING "Git tag or commit to use for Abseil FetchContent")
set(SUBSTRAIT_CPP_PROTOBUF_FETCH_TAG
    ${SUBSTRAIT_CPP_PROTOBUF_DEFAULT_FETCH_TAG}
    CACHE STRING "Git tag or commit to use for Protobuf FetchContent")
# Protobuf's CMake package version is 5.<minor>.<patch> for the v<minor>.<patch>
# releases (e.g. v28.2 is 5.28.2). Older system packages (e.g. 3.19 shipped by
# some distributions) are rejected so that the FetchContent fallback is used.
set(SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION
    5.28
    CACHE STRING "Minimum acceptable version of a system Protobuf")

# First use `find_package`. This allows downstream projects to inject their
# version with `FetchContent_Declare(... OVERRIDE_FIND_PACKAGE`.
if(SUBSTRAIT_CPP_USE_SYSTEM_PROTOBUF)
  if(SUBSTRAIT_CPP_FIND_PROTOBUF_CONFIG)
    find_package(Protobuf ${SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION} CONFIG)
  else()
    find_package(Protobuf ${SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION})
  endif()
endif()

# Now fall back to using `FetchContent`.
if(NOT Protobuf_FOUND AND SUBSTRAIT_CPP_FETCH_PROTOBUF)
  message(
    STATUS "Fetching Protobuf version ${SUBSTRAIT_CPP_PROTOBUF_FETCH_TAG}")
  include(FetchContent)
  set(protobuf_BUILD_TESTS
      OFF
      CACHE BOOL "Disable Protobuf tests" FORCE)
  fetchcontent_declare(
    protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG ${SUBSTRAIT_CPP_PROTOBUF_FETCH_TAG}
    SYSTEM OVERRIDE_FIND_PACKAGE CMAKE_ARGS
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})
  set(protobuf_BUILD_TESTS
      OFF
      CACHE INTERNAL "")
  set(ABSL_PROPAGATE_CXX_STD ON)
  # The Abseil bundled with older Protobuf releases does not compile with recent
  # compilers (e.g. GCC 14+ on Fedora), so fetch a newer Abseil separately and
  # tell Protobuf to use it via find_package.
  message(STATUS "Fetching Abseil version ${SUBSTRAIT_CPP_ABSL_FETCH_TAG}")
  fetchcontent_declare(
    absl
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG ${SUBSTRAIT_CPP_ABSL_FETCH_TAG}
    SYSTEM OVERRIDE_FIND_PACKAGE)
  set(protobuf_ABSL_PROVIDER
      "package"
      CACHE STRING "")
  # Needed so the Abseil targets are in an export set for install(EXPORT).
  set(ABSL_ENABLE_INSTALL
      ON
      CACHE BOOL "")
  if(MSVC)
    set(protobuf_MSVC_STATIC_RUNTIME OFF)
    add_compile_options("/W0")
  else()
    add_compile_options("-w")
  endif()
  fetchcontent_makeavailable(protobuf)
  fetchcontent_getproperties(Protobuf SOURCE_DIR Protobuf_SOURCE_DIR)
  # This exists from Protobuf version 22.0 and up.
  include(${Protobuf_SOURCE_DIR}/cmake/protobuf-generate.cmake)
  if(TARGET libprotobuf AND NOT TARGET protobuf::libprotobuf)
    add_library(protobuf::libprotobuf ALIAS libprotobuf)
  endif()
  if(TARGET protoc AND NOT TARGET protobuf::protoc)
    add_executable(protobuf::protoc ALIAS protoc)
  endif()
endif()

if(NOT TARGET protobuf::libprotobuf)
  message(FATAL_ERROR "Protobuf is required but was not found or fetched.")
endif()
