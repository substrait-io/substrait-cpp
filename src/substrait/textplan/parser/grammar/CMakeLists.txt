# SPDX-License-Identifier: Apache-2.0

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

set(GRAMMAR_DIR ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "generated dir:  ${GRAMMAR_DIR}/antlr4cpp_generated_src")

target_include_directories(
  textplan_grammar
  PUBLIC $<BUILD_INTERFACE:${GRAMMAR_DIR}/antlr4cpp_generated_src>)

target_link_libraries(textplan_grammar antlr4_static)

# Things which link against textplan_grammar (and pull in antlr4 headers) should
# always set 'ANTLR4CPP_STATIC', to avoid declaring things in the antlr4
# headers as dllexport'ed.
target_compile_definitions(textplan_grammar PUBLIC ANTLR4CPP_STATIC)

install(TARGETS textplan_grammar EXPORT SubstraitTargets)
