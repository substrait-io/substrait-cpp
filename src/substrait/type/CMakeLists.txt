# SPDX-License-Identifier: Apache-2.0

set(TYPE_SRCS Type.cpp)

add_library(substrait_type ${TYPE_SRCS})
target_sources(
  substrait_type PUBLIC FILE_SET HEADERS BASE_DIRS ../../../include/ FILES
                        ../../../include/substrait/type/Type.h)

target_link_libraries(substrait_type substrait_common)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

install(
  TARGETS substrait_type
  EXPORT SubstraitTargets
  FILE_SET HEADERS)
