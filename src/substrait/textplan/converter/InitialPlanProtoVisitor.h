/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/RelationData.h"
#include "substrait/textplan/SubstraitErrorListener.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

namespace io::substrait::textplan {

// InitialPlanProtoVisitor is the first part of the binary to text conversion
// process which identifies the prominent symbols and gives them names.
class InitialPlanProtoVisitor : public BasePlanProtoVisitor {
 public:
  explicit InitialPlanProtoVisitor() {
    symbolTable_ = std::make_shared<SymbolTable>();
    errorListener_ = std::make_shared<SubstraitErrorListener>();
  };

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbolTable_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitErrorListener> getErrorListener()
      const {
    return errorListener_;
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

  std::any visitNamedStruct(
      const ::substrait::proto::NamedStruct& named) override;

  void updateLocalSchema(
      const std::shared_ptr<RelationData>& relationData,
      const ::substrait::proto::Rel& relation);

  void addFieldsToRelation(
      const std::shared_ptr<RelationData>& relationData,
      const ::substrait::proto::Rel& relation);

  void addFieldsToRelation(
      const std::shared_ptr<RelationData>& relationData,
      const ::substrait::proto::Rel& left,
      const ::substrait::proto::Rel& right);

  template <typename T>
  void addFieldsToRelation(
      const std::shared_ptr<RelationData>& relationData,
      const T& relations) {
    for (const auto& rel : relations) {
      addFieldsToRelation(relationData, rel);
    }
  };

  std::shared_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<SubstraitErrorListener> errorListener_;
};

} // namespace io::substrait::textplan
