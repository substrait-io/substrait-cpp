/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/PipelineVisitor.h"

#include "substrait/textplan/Any.h"
#include "substrait/textplan/Finally.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

std::any PipelineVisitor::visitExpression(
    const ::substrait::proto::Expression& expression) {
  if (expression.rex_type_case() ==
      ::substrait::proto::Expression::RexTypeCase::kSubquery) {
    auto result = visitSubquery(expression.subquery());

    const ::substrait::proto::Rel* subqueryRelation;
    switch (expression.subquery().subquery_type_case()) {
      case ::substrait::proto::Expression_Subquery::kScalar:
        subqueryRelation = &expression.subquery().scalar().input();
        break;
      case ::substrait::proto::Expression_Subquery::kInPredicate:
        subqueryRelation = &expression.subquery().in_predicate().haystack();
        break;
      case ::substrait::proto::Expression_Subquery::kSetPredicate:
        subqueryRelation = &expression.subquery().set_predicate().tuples();
        break;
      case ::substrait::proto::Expression_Subquery::kSetComparison:
        subqueryRelation = &expression.subquery().set_comparison().right();
        break;
      case ::substrait::proto::Expression_Subquery::SUBQUERY_TYPE_NOT_SET:
        // No need to raise as this would have been exposed earlier.
        return result;
    }
    if (subqueryRelation == nullptr) {
      // No need to raise as this would have been caught earlier.
      return result;
    }

    auto subquerySymbol = symbolTable_->lookupSymbolByLocationAndType(
        PROTO_LOCATION(*subqueryRelation), SymbolType::kRelation);
    auto currentRelationData =
        ANY_CAST(std::shared_ptr<RelationData>, currentRelationScope_->blob);
    currentRelationData->subQueryPipelines.push_back(subquerySymbol);

    // Populate the start of the pipeline for easy later access.
    const SymbolInfo* current;
    auto thisRelationData =
        ANY_CAST(std::shared_ptr<RelationData>, subquerySymbol->blob);
    thisRelationData->pipelineStart = subquerySymbol;
    while (thisRelationData->continuingPipeline != nullptr) {
      current = thisRelationData->continuingPipeline;
      thisRelationData = ANY_CAST(std::shared_ptr<RelationData>, current->blob);
      thisRelationData->pipelineStart = subquerySymbol;
    }
    return result;
  }
  return BasePlanProtoVisitor::visitExpression(expression);
}

std::any PipelineVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  auto symbol = symbolTable_->lookupSymbolByLocationAndType(
      PROTO_LOCATION(relation), SymbolType::kRelation);
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol->blob);

  auto previousRelationScope = currentRelationScope_;
  currentRelationScope_ = symbol;
  auto resetRelationScope = finally([this, &previousRelationScope]() {
    currentRelationScope_ = previousRelationScope;
  });

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
    case ::substrait::proto::Rel::kNestedLoopJoin: {
      const auto* leftSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.nested_loop_join().left()),
          SymbolType::kRelation);
      const auto* rightSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.nested_loop_join().right()),
          SymbolType::kRelation);
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
    case ::substrait::proto::Rel::kReference:
      // TODO -- Add support for references in text plans.
      break;
    case ::substrait::proto::Rel::kWrite: {
      const auto* inputSymbol = symbolTable_->lookupSymbolByLocationAndType(
          PROTO_LOCATION(relation.write().input()), SymbolType::kRelation);
      relationData->continuingPipeline = inputSymbol;
      break;
    }
    case ::substrait::proto::Rel::kDdl:
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
