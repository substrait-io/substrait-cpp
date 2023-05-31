/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <antlr4-runtime.h>

#include "substrait/textplan/SubstraitErrorListener.h"

namespace io::substrait::textplan {

class SubstraitParserErrorListener : public antlr4::BaseErrorListener,
                                     public SubstraitErrorListener {
 public:
  SubstraitParserErrorListener() = default;

  void syntaxError(
      antlr4::Recognizer* recognizer,
      antlr4::Token* offendingSymbol,
      size_t line,
      size_t charPositionInLine,
      const std::string& msg,
      std::exception_ptr e) override;

  void addError(const antlr4::Token* offendingSymbol, const std::string& msg);
};

} // namespace io::substrait::textplan
