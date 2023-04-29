/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/PipelineVisitor.h"

#include "substrait/textplan/Any.h"
#include "substrait/textplan/StructuredSymbolData.h"

namespace io::substrait::textplan {

std::shared_ptr<RelationData> PipelineVisitor::getRelationData(
    const google::protobuf::Message& relation) {
  auto symbol = symbolTable_->lookupSymbolByLocation(Location(&relation));
  if (symbol == SymbolInfo::kUnknown) {
    return nullptr;
  }
  return ANY_CAST(std::shared_ptr<RelationData>, symbol.blob);
}

std::any PipelineVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  auto relationData = getRelationData(relation);

  switch (relation.rel_type_case()) {
    case ::substrait::proto::Rel::RelTypeCase::kRead:
      // No relations beyond this one.
      break;
    case ::substrait::proto::Rel::RelTypeCase::kFilter: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.filter().input()));
      relationData->continuingPipeline = &inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kFetch: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.fetch().input()));
      relationData->continuingPipeline = &inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kAggregate: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.aggregate().input()));
      relationData->continuingPipeline = &inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kSort: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.sort().input()));
      relationData->continuingPipeline = &inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kJoin: {
      const auto& leftSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.join().left()));
      const auto& rightSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.join().right()));
      relationData->newPipelines.push_back(&leftSymbol);
      relationData->newPipelines.push_back(&rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kProject: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.project().input()));
      relationData->continuingPipeline = &inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kSet:
      for (const auto& rel : relation.set().inputs()) {
        const auto& inputSymbol =
            symbolTable_->lookupSymbolByLocation(Location(&rel));
        relationData->newPipelines.push_back(&inputSymbol);
      }
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionSingle: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.extension_single().input()));
      relationData->continuingPipeline = &inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kExtensionMulti:
      for (const auto& rel : relation.extension_multi().inputs()) {
        const auto& inputSymbol =
            symbolTable_->lookupSymbolByLocation(Location(&rel));
        relationData->newPipelines.push_back(&inputSymbol);
      }
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionLeaf:
      // No children.
      break;
    case ::substrait::proto::Rel::RelTypeCase::kCross: {
      const auto& leftSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.cross().left()));
      const auto& rightSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.cross().right()));
      relationData->newPipelines.push_back(&leftSymbol);
      relationData->newPipelines.push_back(&rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kHashJoin: {
      const auto& leftSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.hash_join().left()));
      const auto& rightSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.hash_join().right()));
      relationData->newPipelines.push_back(&leftSymbol);
      relationData->newPipelines.push_back(&rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kMergeJoin: {
      const auto& leftSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.merge_join().left()));
      const auto& rightSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.merge_join().right()));
      relationData->newPipelines.push_back(&leftSymbol);
      relationData->newPipelines.push_back(&rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::REL_TYPE_NOT_SET:
      break;
  }

  return BasePlanProtoVisitor::visitRelation(relation);
}

std::any PipelineVisitor::visitPlanRelation(
    const ::substrait::proto::PlanRel& relation) {
  auto symbol = symbolTable_->lookupSymbolByLocation(
      Location((const google::protobuf::Message*)&relation));
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol.blob);
  switch (relation.rel_type_case()) {
    case ::substrait::proto::PlanRel::kRel: {
      const auto& relSymbol =
          symbolTable_->lookupSymbolByLocation(Location(&relation.rel()));
      relationData->newPipelines.push_back(&relSymbol);
      break;
    }
    case ::substrait::proto::PlanRel::kRoot: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocation(
          Location(&relation.root().input()));
      relationData->newPipelines.push_back(&inputSymbol);
      break;
    }
    case ::substrait::proto::PlanRel::REL_TYPE_NOT_SET:
      break;
  }
  return BasePlanProtoVisitor::visitPlanRelation(relation);
}

} // namespace io::substrait::textplan
