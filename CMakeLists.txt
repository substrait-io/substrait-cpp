# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.24)

# set the project name
project(
  substrait-cpp
  VERSION 0.1.0
  LANGUAGES C CXX)

include(GNUInstallDirs)

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
file(
  DOWNLOAD https://www.antlr.org/download/antlr-4.13.2-complete.jar
  "${ANTLR_EXECUTABLE_DIR}/antlr.jar"
  EXPECTED_HASH
    SHA256=eae2dfa119a64327444672aff63e9ec35a20180dc5b8090b7a6ab85125df4d76
  TLS_VERIFY ON)
set(ANTLR_EXECUTABLE "${ANTLR_EXECUTABLE_DIR}/antlr.jar")

# Local files come first.
include_directories(include)
include_directories(src)

add_subdirectory(third_party)

# Set up clang-tidy after third-party dependencies.
include(cmake/clang-tidy.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")
include(BuildUtils)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  enable_testing()
endif()

# Install the exported targets together with a package configuration file, so
# that consumers can use `find_package(Substrait CONFIG REQUIRED)` and link
# against `Substrait::<target>`.
include(CMakePackageConfigHelpers)
set(SUBSTRAIT_CPP_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/Substrait)

install(
  EXPORT SubstraitTargets
  NAMESPACE Substrait::
  DESTINATION ${SUBSTRAIT_CPP_CMAKE_INSTALL_DIR})

# The ANTLR4 runtime is built by ExternalProject and is not part of any export
# set; install it so that the exported static libraries can be linked.
get_filename_component(SUBSTRAIT_CPP_ANTLR4_LIBRARY_FILE
                       "${ANTLR4_STATIC_LIBRARIES}" NAME)
string(REPLACE "$(Configuration)" "$<CONFIG>" _SUBSTRAIT_ANTLR4_INSTALL_FILE "${ANTLR4_STATIC_LIBRARIES}")
string(REPLACE "$(CONFIGURATION)" "$<CONFIG>" _SUBSTRAIT_ANTLR4_INSTALL_FILE "${_SUBSTRAIT_ANTLR4_INSTALL_FILE}")
install(FILES ${_SUBSTRAIT_ANTLR4_INSTALL_FILE} DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(DEFINED Protobuf_VERSION AND Protobuf_VERSION)
  set(SUBSTRAIT_CPP_PROTOBUF_BUILD_VERSION "${Protobuf_VERSION}")
elseif(DEFINED protobuf_VERSION AND protobuf_VERSION)
  set(SUBSTRAIT_CPP_PROTOBUF_BUILD_VERSION "${protobuf_VERSION}")
else()
  set(SUBSTRAIT_CPP_PROTOBUF_BUILD_VERSION "5.28")
endif()

configure_package_config_file(
  cmake/SubstraitConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/SubstraitConfig.cmake"
  INSTALL_DESTINATION ${SUBSTRAIT_CPP_CMAKE_INSTALL_DIR}
  PATH_VARS CMAKE_INSTALL_LIBDIR)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/SubstraitConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SubstraitConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/SubstraitConfigVersion.cmake"
        DESTINATION ${SUBSTRAIT_CPP_CMAKE_INSTALL_DIR})

add_subdirectory(src/substrait)
add_subdirectory(export)
