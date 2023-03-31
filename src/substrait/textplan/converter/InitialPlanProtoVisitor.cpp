/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/InitialPlanProtoVisitor.h"

#include <iterator>
#include <optional>
#include <string>

#include "substrait/common/Exceptions.h"
#include "substrait/proto/ProtoUtils.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/RelationData.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/HierarchyStack.h"

namespace io::substrait::textplan {

namespace {

std::string shortName(std::string str) {
  auto loc = str.find(':');
  if (loc != std::string::npos) {
    return str.substr(0, loc);
  }
  return str;
}

} // namespace

std::any InitialPlanProtoVisitor::visitExtension(
    const ::substrait::proto::extensions::SimpleExtensionDeclaration&
        extension) {
  if (extension.mapping_type_case() !=
      ::substrait::proto::extensions::SimpleExtensionDeclaration::
          kExtensionFunction) {
    SUBSTRAIT_FAIL(
        "Unknown mapping type case " +
        std::to_string(extension.mapping_type_case()) + " encountered.");
  }
  const auto& uniqueName = symbolTable_->getUniqueName(
      shortName(extension.extension_function().name()));
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(extension.extension_function()),
      SymbolType::kFunction,
      std::nullopt,
      &extension.extension_function());
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitExtensionUri(
    const ::substrait::proto::extensions::SimpleExtensionURI& uri) {
  symbolTable_->defineSymbol(
      uri.uri(),
      PROTO_LOCATION(uri),
      SymbolType::kExtensionSpace,
      std::nullopt,
      uri.extension_uri_anchor());
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitPlanRelation(
    const ::substrait::proto::PlanRel& relation) {
  BasePlanProtoVisitor::visitPlanRelation(relation);
  std::string name =
      ::substrait::proto::planRelTypeCaseName(relation.rel_type_case());
  auto uniqueName = symbolTable_->getUniqueName(name);
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(relation),
      SymbolType::kPlanRelation,
      std::nullopt,
      // TODO -- Deal with the fact that we aren't passing the right type here.
      std::make_shared<RelationData>(
          (const ::substrait::proto::Rel*)(&relation), nullptr));
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  std::string name =
      ::substrait::proto::relTypeCaseName(relation.rel_type_case());
  HierarchyScope mark(relationStack_.get(), &relation);
  BasePlanProtoVisitor::visitRelation(relation);
  auto uniqueName = symbolTable_->getUniqueName(name);
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(relation),
      SymbolType::kRelation,
      relation.rel_type_case(),
      std::make_shared<RelationData>(
          &relation, relationStack_->getEnclosingScope()));
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitRelationRoot(
    const ::substrait::proto::RelRoot& relation) {
  BasePlanProtoVisitor::visitRelationRoot(relation);
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitReadRelation(
    const ::substrait::proto::ReadRel& relation) {
  if (relation.has_base_schema()) {
    const std::string& name = symbolTable_->getUniqueName("schema");
    symbolTable_->defineSymbol(
        name,
        PROTO_LOCATION(relation.base_schema()),
        SymbolType::kSchema,
        std::nullopt,
        &relation.base_schema());
    visitNamedStruct(relation.base_schema());
  }

  return BasePlanProtoVisitor::visitReadRelation(relation);
}

std::any InitialPlanProtoVisitor::visitVirtualTable(
    const ::substrait::proto::ReadRel_VirtualTable& table) {
  const auto& uniqueName = symbolTable_->getUniqueName("virtual");
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(table),
      SymbolType::kSource,
      SourceType::kVirtualTable,
      &table);
  return BasePlanProtoVisitor::visitVirtualTable(table);
}

std::any InitialPlanProtoVisitor::visitLocalFiles(
    const ::substrait::proto::ReadRel_LocalFiles& local) {
  const auto& uniqueName = symbolTable_->getUniqueName("local");
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(local),
      SymbolType::kSource,
      SourceType::kLocalFiles,
      &local);
  return BasePlanProtoVisitor::visitLocalFiles(local);
}

std::any InitialPlanProtoVisitor::visitNamedTable(
    const ::substrait::proto::ReadRel_NamedTable& table) {
  const auto& uniqueName = symbolTable_->getUniqueName("named");
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(table),
      SymbolType::kSource,
      SourceType::kNamedTable,
      &table);
  for (const auto& name : table.names()) {
    symbolTable_->defineSymbol(
        name,
        Location::kUnknownLocation,
        SymbolType::kField,
        SourceType::kUnknown,
        &table); // Field names are in this scope.
  }
  return BasePlanProtoVisitor::visitNamedTable(table);
}

std::any InitialPlanProtoVisitor::visitExtensionTable(
    const ::substrait::proto::ReadRel_ExtensionTable& table) {
  const auto& uniqueName = symbolTable_->getUniqueName("extensiontable");
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(table),
      SymbolType::kSource,
      SourceType::kExtensionTable,
      &table);
  return BasePlanProtoVisitor::visitExtensionTable(table);
}

std::any InitialPlanProtoVisitor::visitNamedStruct(
    const ::substrait::proto::NamedStruct& named) {
  for (const auto& name : named.names()) {
    if (symbolTable_->lookupSymbolByName(name) != SymbolInfo::kUnknown) {
      continue;
    }
    symbolTable_->defineSymbol(
        name,
        PROTO_LOCATION(named),
        SymbolType::kField,
        SourceType::kUnknown,
        &named); // Field names are in this scope.
  }
  return BasePlanProtoVisitor::visitNamedStruct(named);
}

} // namespace io::substrait::textplan
