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
    symbolTable_ = std::make_shared<SymbolTable>(symbolTable);
    errorListener_ = std::move(errorListener);
  }

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbolTable_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitParserErrorListener> getErrorListener()
      const {
    return errorListener_;
  };

  std::any visitPipelines(SubstraitPlanParser::PipelinesContext* ctx) override;
  std::any visitPipeline(SubstraitPlanParser::PipelineContext* ctx) override;

 private:
  // Creates a symbol table entry if we don't already have one, then adds the
  // current location.
  void updateRelationSymbol(
      SubstraitPlanParser::PipelineContext* ctx,
      const std::string& relationName);

  std::shared_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<SubstraitParserErrorListener> errorListener_;
};

} // namespace io::substrait::textplan
