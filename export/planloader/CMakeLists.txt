# SPDX-License-Identifier: Apache-2.0

if(NOT BUILD_SUBDIR_NAME EQUAL "release")
  message(
    SEND_ERROR,
    "The planloader library does not work in Debug mode due to its dependencies."
  )
endif()

add_library(planloader SHARED planloader.cpp)
add_compile_options(-FPIC)
set_target_properties(planloader PROPERTIES SUBVERSION 1)

add_dependencies(planloader substrait_io)
target_link_libraries(planloader substrait_io)

install(
  TARGETS planloader
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCDIR})

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()
