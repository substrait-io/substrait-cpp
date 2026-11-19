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
    find_package(Protobuf CONFIG QUIET)
    if(Protobuf_FOUND)
      # Check version suitability:
      # Protobuf upstream CONFIG reports either v2X.Y.Z or 5.X.Y.Z.
      # We require at least 5.28 (i.e. 28.0 in new CONFIG numbering).
      set(_protobuf_ver "${Protobuf_VERSION}")
      if(NOT _protobuf_ver AND protobuf_VERSION)
        set(_protobuf_ver "${protobuf_VERSION}")
      endif()
      if(_protobuf_ver)
        if((_protobuf_ver VERSION_GREATER_EQUAL "21.0.0" AND _protobuf_ver VERSION_LESS "28.0.0")
           OR (_protobuf_ver VERSION_LESS "5.28.0" AND _protobuf_ver VERSION_LESS "21.0.0"))
          message(STATUS "Found system Protobuf CONFIG version ${_protobuf_ver}, but at least 5.28 (28.0) is required.")
          set(Protobuf_FOUND FALSE)
        endif()
      endif()
    endif()
  else()
    # In module mode, CMake's FindProtobuf creates imported targets before checking version.
    # To prevent leaked targets when system Protobuf is too old, probe the header first.
    find_path(_SUBSTRAIT_PROTOBUF_PROBE_DIR
              NAMES google/protobuf/port_def.inc google/protobuf/stubs/common.h)
    set(_protobuf_module_acceptable FALSE)
    if(_SUBSTRAIT_PROTOBUF_PROBE_DIR)
      if(EXISTS "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/port_def.inc")
        file(STRINGS "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/port_def.inc"
             _PROTO_DEF_LINE REGEX "#define[ \t]+GOOGLE_PROTOBUF_VERSION[ \t]+[0-9]+")
      elseif(EXISTS "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/stubs/common.h")
        file(STRINGS "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/stubs/common.h"
             _PROTO_DEF_LINE REGEX "#define[ \t]+GOOGLE_PROTOBUF_VERSION[ \t]+[0-9]+")
      endif()
      if(_PROTO_DEF_LINE)
        string(REGEX MATCH "[0-9]+" _PROTO_NUM "${_PROTO_DEF_LINE}")
        # 5.28.0 corresponds to GOOGLE_PROTOBUF_VERSION 5028000
        if(_PROTO_NUM GREATER_EQUAL 5028000)
          set(_protobuf_module_acceptable TRUE)
        endif()
      endif()
    endif()
    if(_protobuf_module_acceptable)
      find_package(Protobuf ${SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION})
    endif()
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
      CACHE STRING "" FORCE)
  # Needed so the Abseil targets are in an export set for install(EXPORT).
  set(ABSL_ENABLE_INSTALL
      ON
      CACHE BOOL "" FORCE)
  if(MSVC)
    set(protobuf_MSVC_STATIC_RUNTIME OFF)
    add_compile_options("/W0")
  else()
    add_compile_options("-w")
  endif()
  fetchcontent_makeavailable(absl protobuf)
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
