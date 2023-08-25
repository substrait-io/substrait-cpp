/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

namespace io::substrait::textplan {

class PipelineVisitor : public BasePlanProtoVisitor {
 public:
  explicit PipelineVisitor(const SymbolTable& symbolTable) {
    symbolTable_ = std::make_shared<SymbolTable>(symbolTable);
  }

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbolTable_;
  };

 private:
  std::any visitExpression(
    const ::substrait::proto::Expression& expression) override;

  std::any visitRelation(const ::substrait::proto::Rel& relation) override;

  std::any visitPlanRelation(
      const ::substrait::proto::PlanRel& relation) override;

  std::shared_ptr<SymbolTable> symbolTable_;

  const SymbolInfo* currentRelationScope_{nullptr};
};

} // namespace io::substrait::textplan
