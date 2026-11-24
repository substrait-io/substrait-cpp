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
# Protobuf's C++ (language) version is 5.<minor>.<patch> for the v26-v29
# releases (e.g. v28.2 is 5.28.2), while the CMake package version of the same
# release is <minor>.<patch>.0 (28.2.0). Older system packages (e.g. 3.19
# shipped by some distributions) are rejected so that the FetchContent fallback
# is used.
set(SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION
    5.28
    CACHE STRING "Minimum acceptable version of a system Protobuf")

# Helper function to normalize Protobuf versions: Upstream Protobuf CONFIG
# packages report versions 26.x, 27.x, 28.x, 29.x. These correspond to language
# versions 5.26.x, 5.27.x, 5.28.x, 5.29.x. Other releases use a different C++
# major (4 for v22-v25, 6 for v30 and later), so they are left unchanged.
function(substrait_normalize_protobuf_version IN_VER OUT_VER)
  set(VER "${IN_VER}")
  if(VER MATCHES "^(2[6-9])(\\..*)?$")
    set(VER "5.${VER}")
  endif()
  set(${OUT_VER}
      "${VER}"
      PARENT_SCOPE)
endfunction()

# Records the C++ (language) Protobuf version used for this build, e.g. 5.29.3
# for the v29.3 release. Installed packages request this version from
# `find_dependency`, and Protobuf's generated version file compares the
# requested major version against the C++ major version (5), not the CONFIG
# package version (29.3.0).
function(substrait_record_protobuf_build_version IN_VER)
  substrait_normalize_protobuf_version("${IN_VER}" _normalized)
  if(_normalized MATCHES "^([0-9]+\\.[0-9]+\\.[0-9]+)")
    set(_normalized "${CMAKE_MATCH_1}")
  endif()
  set(SUBSTRAIT_CPP_PROTOBUF_BUILD_VERSION
      "${_normalized}"
      CACHE INTERNAL "Protobuf version used for Substrait build")
endfunction()

# Reads the C++ Protobuf version from a populated `protobuf` FetchContent
# dependency. Used when the version is not otherwise available, e.g. when a
# parent project provides Protobuf with `OVERRIDE_FIND_PACKAGE`, where the
# generated find_package redirect supplies targets but no version.
function(substrait_protobuf_source_version OUT_VER)
  set(${OUT_VER}
      ""
      PARENT_SCOPE)
  include(FetchContent)
  fetchcontent_getproperties(protobuf SOURCE_DIR _protobuf_src_dir)
  if(_protobuf_src_dir AND EXISTS "${_protobuf_src_dir}/CMakeLists.txt")
    file(STRINGS "${_protobuf_src_dir}/CMakeLists.txt" _version_line
         REGEX "^[ \t]*set\\([ \t]*protobuf_VERSION_STRING[ \t]+\"[^\"]+\"")
    if(_version_line MATCHES "\"([^\"]+)\"")
      set(${OUT_VER}
          "${CMAKE_MATCH_1}"
          PARENT_SCOPE)
    endif()
  endif()
endfunction()

# First use `find_package`. This allows downstream projects to inject their
# version with `FetchContent_Declare(... OVERRIDE_FIND_PACKAGE`.
if(SUBSTRAIT_CPP_USE_SYSTEM_PROTOBUF)
  if(SUBSTRAIT_CPP_FIND_PROTOBUF_CONFIG)
    # Protobuf's version file rejects a package whose C++ major version (5 for
    # v26-v29) differs from the requested one before it loads any imported
    # targets, so too-old (3.x, 4.x) or too-new (6.x) packages fall back to the
    # fetch cleanly. It cannot check the minor version: it compares the request
    # against the package version <minor>.<patch>.0 (29.3.0 for v29.3).
    substrait_normalize_protobuf_version("${SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION}"
                                         _min_ver_normalized)
    string(REGEX MATCH "^[0-9]+" _min_major "${_min_ver_normalized}")
    find_package(Protobuf ${_min_major} CONFIG QUIET)
    if(Protobuf_FOUND)
      set(_protobuf_ver "${Protobuf_VERSION}")
      if(NOT _protobuf_ver AND protobuf_VERSION)
        set(_protobuf_ver "${protobuf_VERSION}")
      endif()
      if(NOT _protobuf_ver)
        substrait_protobuf_source_version(_protobuf_ver)
      endif()
      if(_protobuf_ver)
        # A CONFIG package version (29.3.0) lacks the C++ major, which is the
        # one requested above.
        if(_protobuf_ver MATCHES "^[0-9][0-9]\\.")
          set(_protobuf_ver "${_min_major}.${_protobuf_ver}")
        endif()
        if(_protobuf_ver VERSION_LESS _min_ver_normalized)
          # The imported targets already exist in this scope, so fetching
          # Protobuf now would fail with a duplicate target.
          message(
            FATAL_ERROR
              "Found system Protobuf ${_protobuf_ver}, but at least "
              "${_min_ver_normalized} is required. Set "
              "SUBSTRAIT_CPP_USE_SYSTEM_PROTOBUF=OFF to fetch Protobuf instead."
          )
        endif()
        substrait_record_protobuf_build_version("${_protobuf_ver}")
      endif()
    endif()
  else()
    # In module mode, CMake's FindProtobuf creates imported targets before
    # checking version. To prevent leaked targets when system Protobuf is too
    # old, probe the header first.
    if(DEFINED Protobuf_INCLUDE_DIR AND Protobuf_INCLUDE_DIR)
      set(_SUBSTRAIT_PROTOBUF_PROBE_DIR "${Protobuf_INCLUDE_DIR}")
    elseif(DEFINED PROTOBUF_INCLUDE_DIR AND PROTOBUF_INCLUDE_DIR)
      set(_SUBSTRAIT_PROTOBUF_PROBE_DIR "${PROTOBUF_INCLUDE_DIR}")
    else()
      find_path(_SUBSTRAIT_PROTOBUF_PROBE_DIR
                NAMES google/protobuf/stubs/common.h
                      google/protobuf/port_def.inc)
    endif()
    set(_protobuf_module_acceptable FALSE)
    if(_SUBSTRAIT_PROTOBUF_PROBE_DIR)
      set(_proto_num "")
      if(EXISTS
         "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/stubs/common.h")
        file(
          STRINGS
          "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/stubs/common.h"
          _PROTO_DEF_LINE
          REGEX "#define[ \t]+(GOOGLE_)?PROTOBUF_VERSION[ \t]+[0-9]+")
        if(_PROTO_DEF_LINE)
          string(REGEX MATCH "[0-9]+" _proto_num "${_PROTO_DEF_LINE}")
        endif()
      endif()
      if(NOT _proto_num
         AND EXISTS
             "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/port_def.inc")
        file(
          STRINGS
          "${_SUBSTRAIT_PROTOBUF_PROBE_DIR}/google/protobuf/port_def.inc"
          _PROTO_DEF_LINE
          REGEX "#define[ \t]+(GOOGLE_)?PROTOBUF_VERSION[ \t]+[0-9]+")
        if(_PROTO_DEF_LINE)
          string(REGEX MATCH "[0-9]+" _proto_num "${_PROTO_DEF_LINE}")
        endif()
      endif()
      if(_proto_num)
        substrait_normalize_protobuf_version(
          "${SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION}" _min_norm)
        if(_min_norm MATCHES "^([0-9]+)\\.([0-9]+)")
          math(EXPR _min_proto_num
               "${CMAKE_MATCH_1} * 1000000 + ${CMAKE_MATCH_2} * 1000")
          math(EXPR _max_proto_num "(${CMAKE_MATCH_1} + 1) * 1000000")
        else()
          set(_min_proto_num 5028000)
          set(_max_proto_num 6000000)
        endif()
        # The upper bound keeps module mode consistent with CONFIG mode, which
        # does not accept a different C++ major version.
        if(_proto_num GREATER_EQUAL _min_proto_num AND _proto_num LESS
                                                       _max_proto_num)
          set(_protobuf_module_acceptable TRUE)
        endif()
      endif()
    endif()
    if(_protobuf_module_acceptable)
      find_package(Protobuf ${SUBSTRAIT_CPP_PROTOBUF_MIN_VERSION})
      if(Protobuf_FOUND)
        set(_protobuf_ver "${Protobuf_VERSION}")
        if(NOT _protobuf_ver AND protobuf_VERSION)
          set(_protobuf_ver "${protobuf_VERSION}")
        endif()
        if(NOT _protobuf_ver)
          substrait_protobuf_source_version(_protobuf_ver)
        endif()
        if(_protobuf_ver)
          substrait_record_protobuf_build_version("${_protobuf_ver}")
        endif()
      endif()
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
  substrait_protobuf_source_version(_fetched_ver)
  if(NOT _fetched_ver AND DEFINED Protobuf_VERSION)
    set(_fetched_ver "${Protobuf_VERSION}")
  endif()
  if(NOT _fetched_ver AND DEFINED protobuf_VERSION)
    set(_fetched_ver "${protobuf_VERSION}")
  endif()
  if(NOT _fetched_ver)
    string(REGEX REPLACE "^v" "" _fetched_ver
                         "${SUBSTRAIT_CPP_PROTOBUF_FETCH_TAG}")
    if(NOT _fetched_ver MATCHES "^[0-9]+\\.[0-9]+(\\.[0-9]+)?$")
      message(
        FATAL_ERROR
          "Cannot derive the Protobuf version from the fetched source or from "
          "SUBSTRAIT_CPP_PROTOBUF_FETCH_TAG='${SUBSTRAIT_CPP_PROTOBUF_FETCH_TAG}'."
      )
    endif()
  endif()
  substrait_record_protobuf_build_version("${_fetched_ver}")
endif()

if(NOT TARGET protobuf::libprotobuf)
  message(FATAL_ERROR "Protobuf is required but was not found or fetched.")
endif()
