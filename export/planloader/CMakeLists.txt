# SPDX-License-Identifier: Apache-2.0

# MEGAHACK -- Given that the compiler options include lots of undesirable stuff in Debug mode, either cleanse it or refuse to build unless we're in release mode.

add_library(planloader SHARED textplan.cpp)
add_compile_options(-FPIC)
set_target_properties(planloader PROPERTIES SUBVERSION 1)

add_dependencies(planloader substrait_io)
target_link_libraries(planloader substrait_io)

install(TARGETS planloader LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
