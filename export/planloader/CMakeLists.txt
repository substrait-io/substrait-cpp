# SPDX-License-Identifier: Apache-2.0

add_library(planloader SHARED planloader.cpp)

add_dependencies(planloader substrait_io)
target_link_libraries(planloader substrait_io)

install(
  TARGETS planloader
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCDIR})

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()
