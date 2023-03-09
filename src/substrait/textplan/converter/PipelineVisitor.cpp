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
    case ::substrait::proto::Rel::RelTypeCase::kFilter:
      relationData->continuingPipeline = &relation.filter().input();
      break;
    case ::substrait::proto::Rel::RelTypeCase::kFetch:
      relationData->continuingPipeline = &relation.fetch().input();
      break;
    case ::substrait::proto::Rel::RelTypeCase::kAggregate:
      relationData->continuingPipeline = &relation.aggregate().input();
      break;
    case ::substrait::proto::Rel::RelTypeCase::kSort:
      relationData->continuingPipeline = &relation.sort().input();
      break;
    case ::substrait::proto::Rel::RelTypeCase::kJoin:
      relationData->newPipelines.push_back(&relation.join().left());
      relationData->newPipelines.push_back(&relation.join().right());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kProject:
      relationData->continuingPipeline = &relation.project().input();
      break;
    case ::substrait::proto::Rel::RelTypeCase::kSet:
      for (const auto& rel : relation.set().inputs()) {
        relationData->newPipelines.push_back(&rel);
      }
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionSingle:
      relationData->continuingPipeline = &relation.extension_single().input();
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionMulti:
      for (const auto& rel : relation.extension_multi().inputs()) {
        relationData->newPipelines.push_back(&rel);
      }
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionLeaf:
      // No children.
      break;
    case ::substrait::proto::Rel::RelTypeCase::kCross:
      relationData->newPipelines.push_back(&relation.cross().left());
      relationData->newPipelines.push_back(&relation.cross().right());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kHashJoin:
      relationData->newPipelines.push_back(&relation.hash_join().left());
      relationData->newPipelines.push_back(&relation.hash_join().right());
      break;
    case ::substrait::proto::Rel::RelTypeCase::kMergeJoin:
      relationData->newPipelines.push_back(&relation.merge_join().left());
      relationData->newPipelines.push_back(&relation.merge_join().right());
      break;
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
    case ::substrait::proto::PlanRel::kRel:
      relationData->newPipelines.push_back(&relation.rel());
      break;
    case ::substrait::proto::PlanRel::kRoot:
      relationData->newPipelines.push_back(&relation.root().input());
      break;
    case ::substrait::proto::PlanRel::REL_TYPE_NOT_SET:
      break;
  }
  return BasePlanProtoVisitor::visitPlanRelation(relation);
}

} // namespace io::substrait::textplan
