/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

// ParseResult contains the result of a parse (from text to binary) or the
// conversion (from binary to text).  The symbol table contains nearly
// all the information necessary to reconstruct either form.
class ParseResult {
 public:
  ParseResult(
      SymbolTable symbolTable,
      std::vector<std::string> syntaxErrors,
      std::vector<std::string> semanticErrors) {
    symbolTable_ = std::make_shared<SymbolTable>(std::move(symbolTable));
    syntaxErrors_ = std::move(syntaxErrors);
    semanticErrors_ = std::move(semanticErrors);
  }

  [[nodiscard]] bool successful() const {
    return syntaxErrors_.empty() && semanticErrors_.empty();
  }

  [[nodiscard]] const SymbolTable& getSymbolTable() const {
    return *symbolTable_;
  }

  [[nodiscard]] const std::vector<std::string>& getSyntaxErrors() const {
    return syntaxErrors_;
  }

  [[nodiscard]] const std::vector<std::string>& getSemanticErrors() const {
    return semanticErrors_;
  }

  [[nodiscard]] std::vector<std::string> getAllErrors() const {
    std::vector<std::string> errors;
    errors.insert(errors.end(), syntaxErrors_.begin(), syntaxErrors_.end());
    errors.insert(errors.end(), semanticErrors_.begin(), semanticErrors_.end());
    return errors;
  }

  // Add the capability for ::testing::PrintToString to print ParseResult.
  friend std::ostream& operator<<(std::ostream& os, const ParseResult& result);

 private:
  std::shared_ptr<const SymbolTable> symbolTable_;
  std::vector<std::string> syntaxErrors_;
  std::vector<std::string> semanticErrors_;
};

} // namespace io::substrait::textplan
