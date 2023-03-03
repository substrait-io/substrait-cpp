# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_CURRENT_BINARY_DIR}/../${BUILD_SUBDIR_NAME}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_CURRENT_BINARY_DIR}/../${BUILD_SUBDIR_NAME}/")
set(ADDITIONAL_CLEAN_FILES
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY};${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_subdirectory(common)
add_subdirectory(type)
add_subdirectory(function)
add_subdirectory(proto)
add_subdirectory(textplan)
