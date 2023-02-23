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
  PlanPrinterVisitor(
      const ::substrait::proto::Plan& plan,
      const SymbolTable& symbol_table)
      : BasePlanProtoVisitor(plan) {
    symbol_table_ = std::make_shared<SymbolTable>(symbol_table);
    error_listener_ = std::make_shared<SubstraitErrorListener>();
  };

  [[nodiscard]] std::shared_ptr<const SymbolTable> GetSymbolTable() const {
    return symbol_table_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitErrorListener> GetErrorListener()
      const {
    return error_listener_;
  };

  void visit() override;

 private:
  std::any visitPlan() override;

  std::any visitRelationRoot(
      const ::substrait::proto::RelRoot& relation) override;

  std::shared_ptr<SymbolTable> symbol_table_;
  std::shared_ptr<SubstraitErrorListener> error_listener_;
};

} // namespace io::substrait::textplan
