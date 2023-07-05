/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/InitialPlanProtoVisitor.h"

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

const std::string kIntermediateNodeName = "intermediate";
const std::string kRootNames{"root.names"};

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

::google::protobuf::RepeatedField<int32_t> getOutputMapping(
    const ::substrait::proto::Rel& relation) {
  switch (relation.rel_type_case()) {
    case ::substrait::proto::Rel::kRead:
      return relation.read().common().emit().output_mapping();
    case ::substrait::proto::Rel::kFilter:
      return relation.filter().common().emit().output_mapping();
    case ::substrait::proto::Rel::kFetch:
      return relation.fetch().common().emit().output_mapping();
      ;
    case ::substrait::proto::Rel::kAggregate:
      return relation.aggregate().common().emit().output_mapping();
    case ::substrait::proto::Rel::kSort:
      return relation.sort().common().emit().output_mapping();
    case ::substrait::proto::Rel::kJoin:
      return relation.join().common().emit().output_mapping();
    case ::substrait::proto::Rel::kProject:
      return relation.project().common().emit().output_mapping();
    case ::substrait::proto::Rel::kSet:
      return relation.sort().common().emit().output_mapping();
    case ::substrait::proto::Rel::kExtensionSingle:
      return relation.extension_single().common().emit().output_mapping();
    case ::substrait::proto::Rel::kExtensionMulti:
      return relation.extension_multi().common().emit().output_mapping();
    case ::substrait::proto::Rel::kExtensionLeaf:
      return relation.extension_leaf().common().emit().output_mapping();
    case ::substrait::proto::Rel::kCross:
      return relation.cross().common().emit().output_mapping();
    case ::substrait::proto::Rel::kHashJoin:
      return relation.hash_join().common().emit().output_mapping();
    case ::substrait::proto::Rel::kMergeJoin:
      return relation.merge_join().common().emit().output_mapping();
    case ::substrait::proto::Rel::REL_TYPE_NOT_SET:
      break;
  }

  // The compiler will prevent us from reaching here.
  return {};
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
  const std::string name =
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
  const std::string name =
      ::substrait::proto::relTypeCaseName(relation.rel_type_case());

  auto previousRelationScope = currentRelationScope_;
  currentRelationScope_ = &relation;
  auto resetRelationScope = finally([this, &previousRelationScope]() {
    currentRelationScope_ = previousRelationScope;
  });

  BasePlanProtoVisitor::visitRelation(relation);

  // Create a reduced copy of the relation for use in the symbol table.
  auto relationData = std::make_shared<RelationData>();
  relationData->relation = relation;
  eraseInputs(&relationData->relation);

  // Update the relation data for long term use.
  updateLocalSchema(relationData, relation, relationData->relation);
  if (readRelationSources_.find(currentRelationScope_) !=
      readRelationSources_.end()) {
    relationData->source = readRelationSources_[currentRelationScope_];
  }
  if (readRelationSchemas_.find(currentRelationScope_) !=
      readRelationSchemas_.end()) {
    relationData->schema = readRelationSchemas_[currentRelationScope_];
  }

  // Finally create our entry in the symbol table.
  auto uniqueName = symbolTable_->getUniqueName(name);
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
  std::vector<std::string> names;
  names.insert(names.end(), relation.names().begin(), relation.names().end());
  auto uniqueName = symbolTable_->getUniqueName(kRootNames);
  symbolTable_->defineSymbol(
      uniqueName,
      PROTO_LOCATION(relation),
      SymbolType::kRoot,
      SourceType::kUnknown,
      names);

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
        SymbolType::kTable,
        SourceType::kUnknown,
        std::nullopt);
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
  int nameNum = 0;
  for (const auto& name : named.names()) {
    if (symbolTable_->lookupSymbolByName(name) != nullptr) {
      continue;
    }
    ::substrait::proto::Type type;
    if (nameNum < named.struct_().types().size()) {
      type = named.struct_().types(nameNum);
    } else {
      errorListener_->addError(
          "Type number " + std::to_string(nameNum) +
          " requested but there are not that many types in the proto.");
      return std::nullopt;
    }
    symbolTable_->defineSymbol(
        name,
        PROTO_LOCATION(named),
        SymbolType::kField,
        SourceType::kUnknown,
        type);
  }
  return BasePlanProtoVisitor::visitNamedStruct(named);
}

void InitialPlanProtoVisitor::addFieldToRelation(
    const std::shared_ptr<RelationData>& relationData,
    const SymbolInfo* field) {
  relationData->fieldReferences.push_back(field);
}

void InitialPlanProtoVisitor::addFieldsToRelation(
    const std::shared_ptr<RelationData>& relationData,
    const ::substrait::proto::Rel& relation) {
  auto* symbol = symbolTable_->lookupSymbolByLocationAndType(
      PROTO_LOCATION(relation), SymbolType::kRelation);
  if (symbol == nullptr || symbol->type != SymbolType::kRelation) {
    return;
  }
  auto symbolRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, symbol->blob);
  if (!symbolRelationData->outputFieldReferences.empty()) {
    for (const auto& field : symbolRelationData->outputFieldReferences) {
      addFieldToRelation(relationData, field);
    }
  } else {
    for (const auto& field : symbolRelationData->fieldReferences) {
      addFieldToRelation(relationData, field);
    }
    for (const auto& field : symbolRelationData->generatedFieldReferences) {
      addFieldToRelation(relationData, field);
    }
  }
}

void InitialPlanProtoVisitor::addFieldsToRelation(
    const std::shared_ptr<RelationData>& relationData,
    const ::substrait::proto::Rel& left,
    const ::substrait::proto::Rel& right) {
  addFieldsToRelation(relationData, left);
  addFieldsToRelation(relationData, right);
}

std::string getSchemaName(const SymbolInfo* field) {
  if (field->schema != nullptr) {
    return field->schema->name;
  }
  return "";
}

void InitialPlanProtoVisitor::updateLocalSchema(
    const std::shared_ptr<RelationData>& relationData,
    const ::substrait::proto::Rel& relation,
    const ::substrait::proto::Rel& internalRelation) {
  switch (relation.rel_type_case()) {
    case ::substrait::proto::Rel::RelTypeCase::kRead:
      if (relation.read().has_base_schema()) {
        for (const auto& name : relation.read().base_schema().names()) {
          auto symbol = symbolTable_->defineSymbol(
              name,
              PROTO_LOCATION(relation.read().base_schema()),
              SymbolType::kField,
              SourceType::kUnknown,
              std::nullopt);

          symbol->schema = readRelationSchemas_[currentRelationScope_];
          relationData->fieldReferences.emplace_back(symbol);
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
      for (const auto& grouping : relation.aggregate().groupings()) {
        addGroupingToRelation(relationData, grouping);
      }
      for (const auto& measure : internalRelation.aggregate().measures()) {
        auto uniqueName = symbolTable_->getUniqueName("measurename");
        auto symbol = symbolTable_->defineSymbol(
            uniqueName,
            PROTO_LOCATION(measure),
            SymbolType::kMeasure,
            SourceType::kUnknown,
            std::nullopt);
        symbol->location = PROTO_LOCATION(measure);
        relationData->generatedFieldReferences.emplace_back(symbol);
      }
      // Aggregate relations are different in that they alter the emitted fields
      // by default.
      relationData->outputFieldReferences.insert(
          relationData->outputFieldReferences.end(),
          relationData->generatedFieldReferences.begin(),
          relationData->generatedFieldReferences.end());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kSort:
      addFieldsToRelation(relationData, relation.sort().input());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kJoin:
      addFieldsToRelation(
          relationData, relation.join().left(), relation.join().right());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kProject: {
      addFieldsToRelation(relationData, relation.project().input());
      for (const auto& expr : relation.project().expressions()) {
        // TODO -- Add support for other kinds of direct references.
        if (expr.selection().has_direct_reference() &&
            expr.selection().direct_reference().has_struct_field()) {
          auto mapping =
              expr.selection().direct_reference().struct_field().field();
          if (mapping < relationData->fieldReferences.size()) {
            auto field = relationData->fieldReferences[mapping];
            relationData->generatedFieldReferences.push_back(field);
            auto prevInstance =
                relationData->seenFieldReferenceNames.find(field->name);
            if (field->alias.empty() &&
                prevInstance != relationData->seenFieldReferenceNames.end()) {
              // Add a version with the schema supplied.
              auto schemaName = getSchemaName(field);
              if (!schemaName.empty()) {
                relationData->generatedFieldReferenceAlternativeExpression
                    [relationData->generatedFieldReferences.size() - 1] =
                    schemaName + "." + field->name;
              }
              // Now update the first occurrence if it hasn't already.
              auto schemaNamePrev = getSchemaName(
                  relationData->generatedFieldReferences[prevInstance->second]);
              if (!schemaNamePrev.empty()) {
                relationData->generatedFieldReferenceAlternativeExpression
                    [prevInstance->second] = schemaNamePrev + "." + field->name;
              }
            }
            if (field->alias.empty()) {
              size_t fieldRefNum = relationData->generatedFieldReferences.size() - 1;
              relationData->seenFieldReferenceNames.insert(
                  std::make_tuple(field->name, fieldRefNum));
            }
          } else {
            ::substrait::proto::Rel sanitizedRelation = *currentRelationScope_;
            sanitizedRelation.mutable_project()->clear_input();
            errorListener_->addError(
                "Asked to project a field that isn't available - " +
                std::to_string(mapping) + " by relation " +
                sanitizedRelation.ShortDebugString());
          }
        } else {
          const auto& uniqueName =
              symbolTable_->getUniqueName(kIntermediateNodeName);
          auto newSymbol = symbolTable_->defineSymbol(
              uniqueName,
              PROTO_LOCATION(relation.project()),
              SymbolType::kUnknown,
              std::nullopt,
              std::nullopt);
          relationData->generatedFieldReferences.push_back(newSymbol);
          symbolTable_->addAlias(uniqueName, newSymbol);
        }
      }
      break;
    }
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

  // Revamp the output based on the output mapping if present.
  auto mapping = getOutputMapping(relation);
  if (!mapping.empty()) {
    if (!relationData->outputFieldReferences.empty()) {
      errorListener_->addError(
          "Aggregate relations do not yet support output mapping changes.");
      return;
    }
    for (auto item : mapping) {
      auto fieldReferenceSize = relationData->fieldReferences.size();
      if (item < fieldReferenceSize) {
        relationData->outputFieldReferences.push_back(
            relationData->fieldReferences[item]);
      } else if (
          item <
          fieldReferenceSize + relationData->generatedFieldReferences.size()) {
        relationData->outputFieldReferences.push_back(
            relationData->generatedFieldReferences[item - fieldReferenceSize]);
      } else {
        errorListener_->addError(
            "Asked to emit field " + std::to_string(item) +
            " which isn't available.");
      }
    }
  }
}

void InitialPlanProtoVisitor::addGroupingToRelation(
    const std::shared_ptr<RelationData>& relationData,
    const ::substrait::proto::AggregateRel_Grouping& grouping) {
  for (const auto& expr : grouping.grouping_expressions()) {
    // TODO -- Add support for groupings made up of complicated expressions.
    if (expr.has_selection()) {
      auto mapping = expr.selection().direct_reference().struct_field().field();
      // TODO -- Figure out if we need to not add fields we've already seen.
      if (mapping >= relationData->fieldReferences.size()) {
        errorListener_->addError(
            "Grouping attempted to use a field reference not in the input field mapping.");
        continue;
      }
      relationData->generatedFieldReferences.push_back(
          relationData->fieldReferences[mapping]);
    }
  }
}

} // namespace io::substrait::textplan
