/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/SubstraitParserErrorListener.h"

#include <antlr4-runtime.h>
#include <string>

namespace io::substrait::textplan {

void SubstraitParserErrorListener::syntaxError(
    antlr4::Recognizer* recognizer,
    antlr4::Token* offendingSymbol,
    size_t line,
    size_t charPositionInLine,
    const std::string& msg,
    std::exception_ptr e) {
  SubstraitErrorListener::addError(line, charPositionInLine, msg);
}

void SubstraitParserErrorListener::addError(
    const antlr4::Token* offendingSymbol,
    const std::string& msg) {
  SubstraitErrorListener::addError(
      offendingSymbol->getLine(),
      offendingSymbol->getCharPositionInLine(),
      msg);
}

} // namespace io::substrait::textplan
