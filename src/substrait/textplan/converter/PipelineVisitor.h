/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/textplan/RelationData.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

namespace io::substrait::textplan {

class PipelineVisitor : public BasePlanProtoVisitor {
 public:
  explicit PipelineVisitor(const SymbolTable& symbol_table)
      : BasePlanProtoVisitor() {
    symbol_table_ = std::make_shared<SymbolTable>(symbol_table);
  }

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbol_table_;
  };

 private:
  std::shared_ptr<RelationData> getRelationData(
      const google::protobuf::Message& relation);

  std::any visitRelation(const ::substrait::proto::Rel& relation) override;

  std::any visitPlanRelation(
      const ::substrait::proto::PlanRel& relation) override;

  std::shared_ptr<SymbolTable> symbol_table_;
};

} // namespace io::substrait::textplan
