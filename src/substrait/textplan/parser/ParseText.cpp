/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/ParseText.h"

#include <ANTLRErrorStrategy.h>
#include <antlr4-runtime.h>
#include <fstream>
#include <memory>
#include <sstream>

#include "SubstraitPlanLexer/SubstraitPlanLexer.h"
#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/parser/SubstraitParserErrorListener.h"
#include "substrait/textplan/parser/SubstraitPlanPipelineVisitor.h"
#include "substrait/textplan/parser/SubstraitPlanRelationVisitor.h"
#include "substrait/textplan/parser/SubstraitPlanVisitor.h"

namespace io::substrait::textplan {

std::optional<antlr4::ANTLRInputStream> loadTextFile(
    std::string_view filename) {
  std::ifstream stream(std::string{filename});
  if (stream.bad() || stream.fail()) {
    std::cout << "Bad stream." << std::endl;
    return std::nullopt;
  }
  if (!stream.is_open()) {
    std::cout << "Stream is not open." << std::endl;
    return std::nullopt;
  }
  return {stream};
}

antlr4::ANTLRInputStream loadTextString(std::string_view text) {
  return {text};
}

ParseResult parseStream(antlr4::ANTLRInputStream stream) {
  io::substrait::textplan::SubstraitParserErrorListener errorListener;

  SubstraitPlanLexer lexer(&stream);
  lexer.removeErrorListeners();
  lexer.addErrorListener(&errorListener);
  antlr4::CommonTokenStream tokens(&lexer);

  tokens.fill();

  SubstraitPlanParser parser(&tokens);
  parser.removeErrorListeners();
  parser.addErrorListener(&errorListener);
  auto* tree = parser.plan();

  SymbolTable visitorSymbolTable;
  auto visitorErrorListener = std::make_shared<SubstraitParserErrorListener>();
  auto visitor = std::make_shared<SubstraitPlanVisitor>(
      visitorSymbolTable, visitorErrorListener);
  try {
    visitor->visitPlan(tree);
  } catch (...) {
    errorListener.syntaxError(
        &parser,
        nullptr,
        /*line=*/1,
        /*charPositionInLine=*/1,
        "uncaught parser exception encountered",
        std::current_exception());
  }

  auto pipelineVisitor = std::make_shared<SubstraitPlanPipelineVisitor>(
      *visitor->getSymbolTable(), visitor->getErrorListener());
  try {
    pipelineVisitor->visitPlan(tree);
  } catch (std::invalid_argument ex) {
    // Catches the any_cast exception and logs a useful error message.
    errorListener.syntaxError(
        &parser,
        nullptr,
        /*line=*/1,
        /*charPositionInLine=*/1,
        ex.what(),
        std::current_exception());
  } catch (...) {
    errorListener.syntaxError(
        &parser,
        nullptr,
        /*line=*/1,
        /*charPositionInLine=*/1,
        "uncaught parser exception encountered",
        std::current_exception());
  }

  auto relationVisitor = std::make_shared<SubstraitPlanRelationVisitor>(
      *pipelineVisitor->getSymbolTable(), pipelineVisitor->getErrorListener());
  try {
    relationVisitor->visitPlan(tree);
  } catch (std::invalid_argument ex) {
    // Catches the any_cast exception and logs a useful error message.
    errorListener.syntaxError(
        &parser,
        nullptr,
        /*line=*/1,
        /*charPositionInLine=*/1,
        ex.what(),
        std::current_exception());
  } catch (...) {
    errorListener.syntaxError(
        &parser,
        nullptr,
        /*line=*/1,
        /*charPositionInLine=*/1,
        "uncaught parser relation exception encountered",
        std::current_exception());
  }

  auto finalSymbolTable = relationVisitor->getSymbolTable();
  return {
      *finalSymbolTable,
      errorListener.getErrorMessages(),
      relationVisitor->getErrorListener()->getErrorMessages()};
}

} // namespace io::substrait::textplan
