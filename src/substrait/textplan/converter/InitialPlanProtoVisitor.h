/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/SubstraitErrorListener.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

namespace io::substrait::textplan {

// InitialPlanProtoVisitor is the first part of the binary to text conversion
// process which identifies the prominent symbols and gives them names.
class InitialPlanProtoVisitor : public BasePlanProtoVisitor {
 public:
  explicit InitialPlanProtoVisitor() : BasePlanProtoVisitor() {
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

  std::any visitPlanRelation(
      const ::substrait::proto::PlanRel& relation) override;
  std::any visitRelation(const ::substrait::proto::Rel& relation) override;
  std::any visitRelationRoot(
      const ::substrait::proto::RelRoot& relation) override;
  std::any visitReadRelation(
      const ::substrait::proto::ReadRel& relation) override;

  std::any visitVirtualTable(
      const ::substrait::proto::ReadRel_VirtualTable& table) override;
  std::any visitLocalFiles(
      const ::substrait::proto::ReadRel_LocalFiles& local) override;
  std::any visitNamedTable(
      const ::substrait::proto::ReadRel_NamedTable& table) override;
  std::any visitExtensionTable(
      const ::substrait::proto::ReadRel_ExtensionTable& table) override;

  std::shared_ptr<SymbolTable> symbol_table_;
  std::shared_ptr<SubstraitErrorListener> error_listener_;
};

} // namespace io::substrait::textplan
