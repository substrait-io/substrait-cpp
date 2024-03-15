# SPDX-License-Identifier: Apache-2.0

add_library(planloader SHARED planloader.cpp)

add_dependencies(planloader substrait_io)
target_link_libraries(planloader substrait_io)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

install(TARGETS planloader EXPORT SubstraitTargets)

install(EXPORT SubstraitTargets DESTINATION lib/cmake/Substrait)
