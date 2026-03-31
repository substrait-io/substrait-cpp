# SPDX-License-Identifier: Apache-2.0

include_guard(GLOBAL)

option(SUBSTRAIT_CPP_USE_SYSTEM_PROTOBUF "Use system Protobuf via find_package"
       ON)
option(SUBSTRAIT_CPP_FIND_PROTOBUF_CONFIG
       "Use CONFIG mode to find system Protobuf via find_package" ON)
option(SUBSTRAIT_CPP_FETCH_PROTOBUF
       "Download Protobuf via FetchContent if not found" ON)
set(SUBSTRAIT_CPP_PROTOBUF_FETCH_TAG
    ${SUBSTRAIT_CPP_PROTOBUF_DEFAULT_FETCH_TAG}
    CACHE STRING "Git tag or commit to use for Protobuf FetchContent")

# First use `find_package`. This allows downstream projects to inject their
# version with `FetchContent_Declare(... OVERRIDE_FIND_PACKAGE`.
if(SUBSTRAIT_CPP_USE_SYSTEM_PROTOBUF)
  if(SUBSTRAIT_CPP_FIND_PROTOBUF_CONFIG)
    find_package(Protobuf CONFIG)
  else()
    find_package(Protobuf)
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
    SYSTEM OVERRIDE_FIND_PACKAGE)
  set(protobuf_BUILD_TESTS
      OFF
      CACHE INTERNAL "")
  set(ABSL_PROPAGATE_CXX_STD ON)
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
