# SPDX-License-Identifier: Apache-2.0

add_library(planloader SHARED textplan.cpp)
add_compile_options(-FPIC)
set_target_properties(planloader PROPERTIES SUBVERSION 1)

add_dependencies(planloader substrait_io)
target_link_libraries(planloader substrait_io)
