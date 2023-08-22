/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/PipelineVisitor.h"

#include "substrait/textplan/Any.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

std::any PipelineVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  auto symbol = symbolTable_->lookupSymbolByLocationAndType(
      PROTO_LOCATION(relation), SymbolType::kRelation);
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol->blob);
  switch (relation.rel_type_case()) {
    case ::substrait::proto::Rel::RelTypeCase::kRead:
      // No relations beyond this one.
      break;
    case ::substrait::proto::Rel::RelTypeCase::kFilter: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.filter().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kFetch: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.fetch().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kAggregate: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.aggregate().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kSort: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.sort().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kJoin: {
      const auto* leftSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.join().left()), SymbolType::kRelation);
      const auto* rightSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.join().right()), SymbolType::kRelation);
      relationData->newPipelines.push_back(leftSymbol);
      relationData->newPipelines.push_back(rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kProject: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.project().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kSet:
      for (const auto& rel : relation.set().inputs()) {
        const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
            PROTO_LOCATION(rel), SymbolType::kRelation);
        relationData->newPipelines.push_back(inputSymbol);
      }
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionSingle: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.extension_single().input()),
          SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kExtensionMulti:
      for (const auto& rel : relation.extension_multi().inputs()) {
        const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
            PROTO_LOCATION(rel), SymbolType::kRelation);
        relationData->newPipelines.push_back(inputSymbol);
      }
      break;
    case ::substrait::proto::Rel::RelTypeCase::kExtensionLeaf:
      // No children.
      break;
    case ::substrait::proto::Rel::RelTypeCase::kCross: {
      const auto* leftSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.cross().left()), SymbolType::kRelation);
      const auto* rightSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.cross().right()), SymbolType::kRelation);
      relationData->newPipelines.push_back(leftSymbol);
      relationData->newPipelines.push_back(rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kHashJoin: {
      const auto* leftSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.hash_join().left()), SymbolType::kRelation);
      const auto* rightSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.hash_join().right()), SymbolType::kRelation);
      relationData->newPipelines.push_back(leftSymbol);
      relationData->newPipelines.push_back(rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::RelTypeCase::kMergeJoin: {
      const auto* leftSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.merge_join().left()), SymbolType::kRelation);
      const auto* rightSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.merge_join().right()), SymbolType::kRelation);
      relationData->newPipelines.push_back(leftSymbol);
      relationData->newPipelines.push_back(rightSymbol);
      break;
    }
    case ::substrait::proto::Rel::kWindow: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.window().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::kExchange: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.exchange().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::kExpand: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.expand().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::REL_TYPE_NOT_SET:
      break;
  }

  return BasePlanProtoVisitor::visitRelation(relation);
}

std::any PipelineVisitor::visitPlanRelation(
    const ::substrait::proto::PlanRel& relation) {
  auto symbols =
      symbolTable_->lookupSymbolsByLocation(PROTO_LOCATION(relation));
  // A symbol is guaranteed as we previously visited the parse tree.
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbols[0]->blob);
  switch (relation.rel_type_case()) {
    case ::substrait::proto::PlanRel::kRel: {
      const auto& relSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.rel()), SymbolType::kRelation);
      relationData->newPipelines.push_back(relSymbol);
      break;
    }
    case ::substrait::proto::PlanRel::kRoot: {
      const auto& inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.root().input()), SymbolType::kRelation);
      relationData->newPipelines.push_back(inputSymbol);
      break;
    }
    case ::substrait::proto::PlanRel::REL_TYPE_NOT_SET:
      break;
  }
  return BasePlanProtoVisitor::visitPlanRelation(relation);
}

} // namespace io::substrait::textplan
