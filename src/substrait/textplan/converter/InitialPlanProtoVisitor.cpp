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
#include "substrait/textplan/Finally.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/StructuredSymbolData.h"
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

void eraseInputs(::substrait::proto::Rel* relation) {
  switch (relation->rel_type_case()) {
    case ::substrait::proto::Rel::kRead:
      break;
    case ::substrait::proto::Rel::kFilter:
      relation->mutable_filter()->clear_input();
      break;
    case ::substrait::proto::Rel::kFetch:
      relation->mutable_fetch()->clear_input();
      break;
    case ::substrait::proto::Rel::kAggregate:
      relation->mutable_aggregate()->clear_input();
      break;
    case ::substrait::proto::Rel::kSort:
      relation->mutable_sort()->clear_input();
      break;
    case ::substrait::proto::Rel::kJoin:
      relation->mutable_join()->clear_left();
      relation->mutable_join()->clear_right();
      break;
    case ::substrait::proto::Rel::kProject:
      relation->mutable_project()->clear_input();
      break;
    case ::substrait::proto::Rel::kSet:
      relation->mutable_set()->clear_inputs();
      break;
    case ::substrait::proto::Rel::kExtensionSingle:
      relation->mutable_extension_single()->clear_input();
      break;
    case ::substrait::proto::Rel::kExtensionMulti:
      relation->mutable_extension_multi()->clear_inputs();
      break;
    case ::substrait::proto::Rel::kExtensionLeaf:
      break;
    case ::substrait::proto::Rel::kCross:
      relation->mutable_cross()->clear_left();
      relation->mutable_cross()->clear_right();
      break;
    case ::substrait::proto::Rel::kHashJoin:
      relation->mutable_hash_join()->clear_left();
      relation->mutable_hash_join()->clear_right();
      break;
    case ::substrait::proto::Rel::kMergeJoin:
      relation->mutable_merge_join()->clear_left();
      relation->mutable_merge_join()->clear_right();
      break;
    case ::substrait::proto::Rel::REL_TYPE_NOT_SET:
      break;
  }
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
      std::make_shared<FunctionData>(
          extension.extension_function().name(),
          extension.extension_function().extension_uri_reference(),
          extension.extension_function().function_anchor()));
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitExtensionUri(
    const ::substrait::proto::extensions::SimpleExtensionURI& uri) {
  symbolTable_->defineSymbol(
      uri.uri(),
      PROTO_LOCATION(uri),
      SymbolType::kExtensionSpace,
      std::nullopt,
      std::make_shared<ExtensionSpaceData>(uri.extension_uri_anchor()));
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitPlanRelation(
    const ::substrait::proto::PlanRel& relation) {
  BasePlanProtoVisitor::visitPlanRelation(relation);
  std::string name =
      ::substrait::proto::planRelTypeCaseName(relation.rel_type_case());
  auto uniqueName = symbolTable_->getUniqueName(name);
  auto relationData = std::make_shared<RelationData>();
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(relation),
      SymbolType::kPlanRelation,
      std::nullopt,
      relationData);
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  std::string name =
      ::substrait::proto::relTypeCaseName(relation.rel_type_case());

  auto previousRelationScope = currentRelationScope_;
  currentRelationScope_ = &relation;
  auto resetRelationScope = finally([this, &previousRelationScope]() {
    currentRelationScope_ = previousRelationScope;
  });

  BasePlanProtoVisitor::visitRelation(relation);

  auto uniqueName = symbolTable_->getUniqueName(name);
  auto relationData = std::make_shared<RelationData>();
  relationData->relation = relation;
  eraseInputs(&relationData->relation);
  updateLocalSchema(relationData, relation);
  if (readRelationSources_.find(&relation) != readRelationSources_.end()) {
    relationData->source = readRelationSources_[&relation];
  }
  if (readRelationSchemas_.find(&relation) != readRelationSchemas_.end()) {
    relationData->schema = readRelationSchemas_[&relation];
  }
  auto symbol = symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(relation),
      SymbolType::kRelation,
      relation.rel_type_case(),
      relationData);
  symbolTable_->updateLocation(*symbol, PROTO_LOCATION(relationData->relation));
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
    auto symbol = symbolTable_->defineSymbol(
        name,
        PROTO_LOCATION(relation.base_schema()),
        SymbolType::kSchema,
        std::nullopt,
        &relation.base_schema());
    readRelationSchemas_[currentRelationScope_] = symbol;
    visitNamedStruct(relation.base_schema());
  }

  return BasePlanProtoVisitor::visitReadRelation(relation);
}

std::any InitialPlanProtoVisitor::visitVirtualTable(
    const ::substrait::proto::ReadRel_VirtualTable& table) {
  const auto& uniqueName = symbolTable_->getUniqueName("virtual");
  auto symbol = symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(table),
      SymbolType::kSource,
      SourceType::kVirtualTable,
      &table);
  readRelationSources_[currentRelationScope_] = symbol;
  return BasePlanProtoVisitor::visitVirtualTable(table);
}

std::any InitialPlanProtoVisitor::visitLocalFiles(
    const ::substrait::proto::ReadRel_LocalFiles& local) {
  const auto& uniqueName = symbolTable_->getUniqueName("local");
  auto symbol = symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(local),
      SymbolType::kSource,
      SourceType::kLocalFiles,
      &local);
  readRelationSources_[currentRelationScope_] = symbol;
  return BasePlanProtoVisitor::visitLocalFiles(local);
}

std::any InitialPlanProtoVisitor::visitNamedTable(
    const ::substrait::proto::ReadRel_NamedTable& table) {
  const auto& uniqueName = symbolTable_->getUniqueName("named");
  auto symbol = symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(table),
      SymbolType::kSource,
      SourceType::kNamedTable,
      &table);
  readRelationSources_[currentRelationScope_] = symbol;
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
  auto symbol = symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(table),
      SymbolType::kSource,
      SourceType::kExtensionTable,
      &table);
  readRelationSources_[currentRelationScope_] = symbol;
  return BasePlanProtoVisitor::visitExtensionTable(table);
}

std::any InitialPlanProtoVisitor::visitNamedStruct(
    const ::substrait::proto::NamedStruct& named) {
  for (const auto& name : named.names()) {
    if (symbolTable_->lookupSymbolByName(name) != nullptr) {
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

void InitialPlanProtoVisitor::addFieldsToRelation(
    const std::shared_ptr<RelationData>& relationData,
    const ::substrait::proto::Rel& relation) {
  auto symbol = symbolTable_->lookupSymbolByLocation(PROTO_LOCATION(relation));
  if (symbol == SymbolInfo::kUnknown || symbol.type != SymbolType::kRelation) {
    return;
  }
  auto symbolRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, symbol.blob);
  for (const auto& field : symbolRelationData->fieldReferences) {
    relationData->fieldReferences.push_back(field);
  }
}

void InitialPlanProtoVisitor::addFieldsToRelation(
    const std::shared_ptr<RelationData>& relationData,
    const ::substrait::proto::Rel& left,
    const ::substrait::proto::Rel& right) {
  addFieldsToRelation(relationData, left);
  addFieldsToRelation(relationData, right);
}

void InitialPlanProtoVisitor::updateLocalSchema(
    const std::shared_ptr<RelationData>& relationData,
    const ::substrait::proto::Rel& relation) {
  switch (relation.rel_type_case()) {
    case ::substrait::proto::Rel::RelTypeCase::kRead:
      if (relation.read().has_base_schema()) {
        for (const auto& name : relation.read().base_schema().names()) {
          auto* symbol = symbolTable_->lookupSymbolByName(name);
          if (symbol == nullptr) {
            symbol = symbolTable_->defineSymbol(
                name,
                PROTO_LOCATION(relation.read().base_schema()),
                SymbolType::kField,
                std::nullopt,
                std::nullopt);
          }
          relationData->fieldReferences.push_back(symbol);
        }
      }
      break;
    case ::substrait::proto::Rel::RelTypeCase::kFilter:
      addFieldsToRelation(relationData, relation.filter().input());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kFetch:
      addFieldsToRelation(relationData, relation.fetch().input());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kAggregate:
      addFieldsToRelation(relationData, relation.aggregate().input());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kSort:
      addFieldsToRelation(relationData, relation.sort().input());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kJoin:
      addFieldsToRelation(
          relationData, relation.join().left(), relation.join().right());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kProject:
      addFieldsToRelation(relationData, relation.project().input());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kSet:
      addFieldsToRelation(relationData, relation.set().inputs());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionSingle:
      addFieldsToRelation(relationData, relation.extension_single().input());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionMulti:
      addFieldsToRelation(relationData, relation.extension_multi().inputs());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionLeaf:
      // There is no defined way to get the schema for a leaf.
      break;
    case ::substrait::proto::Rel::RelTypeCase::kCross:
      addFieldsToRelation(
          relationData, relation.cross().left(), relation.cross().right());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kHashJoin:
      addFieldsToRelation(
          relationData,
          relation.hash_join().left(),
          relation.hash_join().right());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kMergeJoin:
      addFieldsToRelation(
          relationData,
          relation.merge_join().left(),
          relation.merge_join().right());
      break;
    case ::substrait::proto::Rel::REL_TYPE_NOT_SET:
      break;
  }
  // TODO -- Utilize the data in relation.common().emit() to alter the order
  // of the fields that leave this relation.
}

} // namespace io::substrait::textplan
