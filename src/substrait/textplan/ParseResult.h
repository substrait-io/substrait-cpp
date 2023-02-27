/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

class ParseResult {
 public:
  ParseResult(
      SymbolTable symbolTable,
      std::vector<std::string> syntaxErrors,
      std::vector<std::string> semanticErrors) {
    symbol_table_ = std::make_shared<SymbolTable>(std::move(symbolTable));
    syntax_errors_ = std::move(syntaxErrors);
    semantic_errors_ = std::move(semanticErrors);
  }

  [[nodiscard]] bool successful() const {
    return syntax_errors_.empty() && semantic_errors_.empty();
  }

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbol_table_;
  }

  [[nodiscard]] const std::vector<std::string>& getSyntaxErrors() const {
    return syntax_errors_;
  }

  [[nodiscard]] const std::vector<std::string>& getSemanticErrors() const {
    return semantic_errors_;
  }

  [[nodiscard]] std::vector<std::string> getAllErrors() const {
    std::vector<std::string> errors;
    errors.insert(errors.end(), syntax_errors_.begin(), syntax_errors_.end());
    errors.insert(
        errors.end(), semantic_errors_.begin(), semantic_errors_.end());
    return errors;
  }

  // Add the capability for ::testing::PrintToString to print ParseResult.
  friend std::ostream& operator<<(std::ostream& os, const ParseResult& result);

 private:
  std::shared_ptr<const SymbolTable> symbol_table_;
  std::vector<std::string> syntax_errors_;
  std::vector<std::string> semantic_errors_;
};

} // namespace io::substrait::textplan
