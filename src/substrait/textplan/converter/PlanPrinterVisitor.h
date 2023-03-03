/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/SubstraitErrorListener.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

namespace io::substrait::textplan {

class PlanPrinterVisitor : public BasePlanProtoVisitor {
 public:
  // PlanPrinterVisitor takes ownership of the provided symbol table.
  explicit PlanPrinterVisitor(const SymbolTable& symbolTable) {
    symbolTable_ = std::make_shared<SymbolTable>(symbolTable);
    errorListener_ = std::make_shared<SubstraitErrorListener>();
  };

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbolTable_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitErrorListener> getErrorListener()
      const {
    return errorListener_;
  };

  std::string printRelation(
      const std::string& symbolName,
      const ::substrait::proto::Rel* relation);

 private:
  std::any visitAggregateFunction(
      const ::substrait::proto::AggregateFunction& function) override;
  std::any visitExpression(
      const ::substrait::proto::Expression& expression) override;
  std::any visitMaskExpression(
      const ::substrait::proto::Expression::MaskExpression& expression)
      override;

  std::any visitReadRelation(
      const ::substrait::proto::ReadRel& relation) override;
  std::any visitFilterRelation(
      const ::substrait::proto::FilterRel& relation) override;
  std::any visitFetchRelation(
      const ::substrait::proto::FetchRel& relation) override;
  std::any visitAggregateRelation(
      const ::substrait::proto::AggregateRel& relation) override;
  std::any visitSortRelation(
      const ::substrait::proto::SortRel& relation) override;
  std::any visitProjectRelation(
      const ::substrait::proto::ProjectRel& relation) override;

  std::shared_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<SubstraitErrorListener> errorListener_;
};

} // namespace io::substrait::textplan
