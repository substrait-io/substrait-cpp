/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "SubstraitPlanParser/SubstraitPlanParserBaseVisitor.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/parser/SubstraitParserErrorListener.h"

namespace io::substrait::textplan {

class SubstraitPlanPipelineVisitor : public SubstraitPlanParserBaseVisitor {
 public:
  SubstraitPlanPipelineVisitor(
      const SymbolTable& symbolTable,
      std::shared_ptr<SubstraitParserErrorListener> errorListener) {
    symbol_table_ = std::make_shared<SymbolTable>(symbolTable);
    error_listener_ = std::move(errorListener);
  }

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbol_table_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitParserErrorListener> getErrorListener()
      const {
    return error_listener_;
  };

  std::any visitPipelines(SubstraitPlanParser::PipelinesContext* ctx) override;
  std::any visitPipeline(SubstraitPlanParser::PipelineContext* ctx) override;

 private:
  // Creates a symbol table entry if we don't already have one, then adds the
  // current location.
  void updateRelationSymbol(
      SubstraitPlanParser::PipelineContext* ctx,
      const std::string& relationName);

  std::shared_ptr<SymbolTable> symbol_table_;
  std::shared_ptr<SubstraitParserErrorListener> error_listener_;
};

} // namespace io::substrait::textplan
