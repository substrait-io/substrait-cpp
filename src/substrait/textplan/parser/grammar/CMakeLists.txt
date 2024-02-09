# SPDX-License-Identifier: Apache-2.0

list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../third_party/antlr4/cmake")

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DANTLR4CPP_STATIC)

set(GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR})

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)

set(ANTLR4_TAG 4.13.0)
set(ANTLR4_ZIP_REPOSITORY
    https://github.com/antlr/antlr4/archive/refs/tags/${ANTLR4_TAG}.zip)

include(ExternalAntlr4Cpp)

include_directories(${ANTLR4_INCLUDE_DIRS})

file(DOWNLOAD https://www.antlr.org/download/antlr-4.13.0-complete.jar
     "${GRAMMAR_DIR}/antlr.jar")
set(ANTLR_EXECUTABLE "${GRAMMAR_DIR}/antlr.jar")
find_package(ANTLR REQUIRED)

antlr_target(SubstraitPlanLexer SubstraitPlanLexer.g4 LEXER PACKAGE
             io::substrait::textplan)
antlr_target(
  SubstraitPlanParser
  SubstraitPlanParser.g4
  PARSER
  PACKAGE
  io::substrait::textplan
  DEPENDS_ANTLR
  SubstraitPlanLexer
  COMPILE_FLAGS
  -lib
  ${ANTLR_SubstraitPlanLexer_OUTPUT_DIR}
  VISITOR)

include_directories(${ANTLR_SubstraitPlanLexer_OUTPUT_DIR})
include_directories(${ANTLR_SubstraitPlanParser_OUTPUT_DIR})

add_custom_target(textplan_grammar_headers
                  DEPENDS ${ANTLR_SubstraitPlanParser_OUTPUTS})

message(
  STATUS
    "ANTLR4 generated files: ${ANTLR_SubstraitPlanLexer_CXX_OUTPUTS} ${ANTLR_SubstraitPlanParser_CXX_OUTPUTS}"
)
add_library(textplan_grammar ${ANTLR_SubstraitPlanLexer_CXX_OUTPUTS}
                             ${ANTLR_SubstraitPlanParser_CXX_OUTPUTS})

message(STATUS "generated dir:  ${GRAMMAR_DIR}/antlr4cpp_generated_src")

target_include_directories(textplan_grammar
                           PUBLIC "${GRAMMAR_DIR}/antlr4cpp_generated_src")

target_link_libraries(textplan_grammar antlr4_static)
