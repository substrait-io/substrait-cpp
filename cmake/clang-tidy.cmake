# SPDX-License-Identifier: Apache-2.0

option(SUBSTRAIT_CPP_ENABLE_CLANG_TIDY
       "Enable clang-tidy static analysis on Substrait MLIR targets" OFF)
option(SUBSTRAIT_CPP_CLANG_TIDY_PATH "path to 'clang-tidy' executable")

if(SUBSTRAIT_CPP_ENABLE_CLANG_TIDY)
  # Find `clang-tidy` executable.
  get_filename_component(CLANG_TIDY_NAME "${SUBSTRAIT_CPP_CLANG_TIDY_PATH}"
                         NAME)
  get_filename_component(CLANG_TIDY_DIR "${SUBSTRAIT_CPP_CLANG_TIDY_PATH}"
                         DIRECTORY)
  find_program(
    CLANG_TIDY_EXE
    HINTS ${CLANG_TIDY_DIR}
    NAMES "clang-tidy" "${CLANG_TIDY_NAME}" REQUIRED)
  if("${CLANG_TIDY_EXE}" STREQUAL "")
    message(FATAL_ERROR "failed to find clang-tidy executable")
  endif()

  # Assemble command line.
  set(CLANG_TIDY_CONFIG_FILE "${CMAKE_SOURCE_DIR}/.clang-tidy")
  set(SUBSTRAIT_CPP_CLANG_TIDY_COMMAND
      "${CLANG_TIDY_EXE};--config-file=${CLANG_TIDY_CONFIG_FILE}")
  set(CMAKE_CXX_CLANG_TIDY_EXPORT_FIXES_DIR
      "${CMAKE_BINARY_DIR}/clang-tidy-fixes")
  message(
    STATUS "clang-tidy enabled, command: ${SUBSTRAIT_CPP_CLANG_TIDY_COMMAND}")
endif(SUBSTRAIT_CPP_ENABLE_CLANG_TIDY)

# Enable clang-tidy for all C++ targets.
set(CMAKE_CXX_CLANG_TIDY ${SUBSTRAIT_CPP_CLANG_TIDY_COMMAND})
