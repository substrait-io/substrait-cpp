# SPDX-License-Identifier: Apache-2.0

option(SUBSTRAIT_CPP_ENABLE_IWYU
       "Enable include-what-you-use static analysis on Substrait MLIR targets"
       OFF)
option(SUBSTRAIT_CPP_IWYU_PATH "path to 'include-what-you-use' executable")

if(SUBSTRAIT_CPP_ENABLE_IWYU)
  # Find `include-what-you-use` executable.
  get_filename_component(IWYU_NAME "${SUBSTRAIT_CPP_IWYU_PATH}" NAME)
  get_filename_component(IWYU_DIR "${SUBSTRAIT_CPP_IWYU_PATH}" DIRECTORY)
  find_program(
    IWYU_EXE
    HINTS ${IWYU_DIR}
    NAMES "include-what-you-use" "${IWYU_NAME}" REQUIRED)
  if("${IWYU_EXE}" STREQUAL "")
    message(FATAL_ERROR "failed to find include-what-you-use executable")
  endif()

  # Assemble command line.
  set(SUBSTRAIT_CPP_IWYU_MAPPING_FILE
      "${CMAKE_CURRENT_SOURCE_DIR}/.iwyu-mapping.imp")
  set(SUBSTRAIT_CPP_IWYU_COMMAND
      "${IWYU_EXE};-Xiwyu;--regex=ecmascript;-Xiwyu;--no_fwd_decls;"
      "-Xiwyu;--mapping_file=${SUBSTRAIT_CPP_IWYU_MAPPING_FILE}")
  message(
    STATUS
      "include-what-you-use enabled, command: ${SUBSTRAIT_CPP_IWYU_COMMAND}")
endif(SUBSTRAIT_CPP_ENABLE_IWYU)

# Enable include-what-you-use for all C++ targets.
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${SUBSTRAIT_CPP_IWYU_COMMAND})
