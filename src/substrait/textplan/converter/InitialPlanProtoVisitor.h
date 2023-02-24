/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/SubstraitErrorListener.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

namespace io::substrait::textplan {

class InitialPlanProtoVisitor : public BasePlanProtoVisitor {
 public:
  explicit InitialPlanProtoVisitor(const ::substrait::proto::Plan& plan)
      : BasePlanProtoVisitor(plan) {
    symbol_table_ = std::make_shared<SymbolTable>();
    error_listener_ = std::make_shared<SubstraitErrorListener>();
  };

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbol_table_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitErrorListener> getErrorListener()
      const {
    return error_listener_;
  };

 private:
  std::any visitExtensionUri(
      const ::substrait::proto::extensions::SimpleExtensionURI& uri) override;
  std::any visitExtension(
      const ::substrait::proto::extensions::SimpleExtensionDeclaration&
          extension) override;

  std::shared_ptr<SymbolTable> symbol_table_;
  std::shared_ptr<SubstraitErrorListener> error_listener_;
};

} // namespace io::substrait::textplan