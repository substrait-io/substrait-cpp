/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/InitialPlanProtoVisitor.h"

#include <iterator>
#include <optional>
#include <string>

#include "substrait/common/Exceptions.h"
#include "substrait/proto/ProtoUtils.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/SymbolTable.h"

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
  const auto& unique_name = symbol_table_->getUniqueName(
      shortName(extension.extension_function().name()));
  symbol_table_->defineSymbol(
      unique_name,
      Location((::google::protobuf::Message*)&extension.extension_function()),
      SymbolType::kFunction,
      std::nullopt,
      &extension.extension_function());
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitExtensionUri(
    const ::substrait::proto::extensions::SimpleExtensionURI& uri) {
  symbol_table_->defineSymbol(
      uri.uri(),
      Location((::google::protobuf::Message*)&uri),
      SymbolType::kExtensionSpace,
      std::nullopt,
      uri.extension_uri_anchor());
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitPlanRelation(
    const ::substrait::proto::PlanRel& relation) {
  BasePlanProtoVisitor::visitPlanRelation(relation);
  std::string name = PlanRelTypeCaseName(relation.rel_type_case());
  auto unique_name = symbol_table_->getUniqueName(name);
  symbol_table_->defineSymbol(
      unique_name,
      Location((google::protobuf::Message*)&relation),
      SymbolType::kPlanRelation,
      std::nullopt,
      &relation);
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  std::string name = RelTypeCaseName(relation.rel_type_case());
  BasePlanProtoVisitor::visitRelation(relation);
  auto unique_name = symbol_table_->getUniqueName(name);
  symbol_table_->defineSymbol(
      unique_name,
      Location((google::protobuf::Message*)&relation),
      SymbolType::kRelation,
      relation.rel_type_case(),
      &relation);
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
    const std::string& name = symbol_table_->getUniqueName("schema");
    symbol_table_->defineSymbol(
        name,
        Location((google::protobuf::Message*)&relation.base_schema()),
        SymbolType::kSchema,
        std::nullopt,
        &relation.base_schema());
  }

  return BasePlanProtoVisitor::visitReadRelation(relation);
}

std::any InitialPlanProtoVisitor::visitVirtualTable(
    const ::substrait::proto::ReadRel_VirtualTable& table) {
  const auto& unique_name = symbol_table_->getUniqueName("virtual");
  symbol_table_->defineSymbol(
      unique_name,
      Location((google::protobuf::Message*)&table),
      SymbolType::kSource,
      SourceType::kVirtualTable,
      &table);
  return BasePlanProtoVisitor::visitVirtualTable(table);
}

std::any InitialPlanProtoVisitor::visitLocalFiles(
    const ::substrait::proto::ReadRel_LocalFiles& local) {
  const auto& unique_name = symbol_table_->getUniqueName("local");
  symbol_table_->defineSymbol(
      unique_name,
      Location((google::protobuf::Message*)&local),
      SymbolType::kSource,
      SourceType::kLocalFiles,
      &local);
  return BasePlanProtoVisitor::visitLocalFiles(local);
}

std::any InitialPlanProtoVisitor::visitNamedTable(
    const ::substrait::proto::ReadRel_NamedTable& table) {
  const auto& unique_name = symbol_table_->getUniqueName("named");
  symbol_table_->defineSymbol(
      unique_name,
      Location((google::protobuf::Message*)&table),
      SymbolType::kSource,
      SourceType::kNamedTable,
      &table);
  return BasePlanProtoVisitor::visitNamedTable(table);
}

std::any InitialPlanProtoVisitor::visitExtensionTable(
    const ::substrait::proto::ReadRel_ExtensionTable& table) {
  const auto& unique_name = symbol_table_->getUniqueName("extensiontable");
  symbol_table_->defineSymbol(
      unique_name,
      Location((google::protobuf::Message*)&table),
      SymbolType::kSource,
      SourceType::kExtensionTable,
      &table);
  return BasePlanProtoVisitor::visitExtensionTable(table);
}

} // namespace io::substrait::textplan
