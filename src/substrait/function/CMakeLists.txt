# SPDX-License-Identifier: Apache-2.0

set(FUNCTION_SRCS Function.cpp Extension.cpp FunctionLookup.cpp)

add_library(substrait_function ${FUNCTION_SRCS})
target_sources(
  substrait_function
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         ../../../include/
         FILES
         ../../../include/substrait/function/Extension.h
         ../../../include/substrait/function/Function.h
         ../../../include/substrait/function/FunctionLookup.h
         ../../../include/substrait/function/FunctionSignature.h)

target_link_libraries(substrait_function substrait_type yaml-cpp)

if(${SUBSTRAIT_CPP_BUILD_TESTING})
  add_subdirectory(tests)
endif()

install(
  TARGETS substrait_function
  EXPORT SubstraitTargets
  FILE_SET HEADERS)
