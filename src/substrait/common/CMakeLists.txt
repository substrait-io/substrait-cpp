# SPDX-License-Identifier: Apache-2.0

find_package(fmt)

add_library(substrait_common Exceptions.cpp)

target_link_libraries(substrait_common fmt::fmt-header-only)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()
