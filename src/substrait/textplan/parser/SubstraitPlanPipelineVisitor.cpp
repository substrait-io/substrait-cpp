/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/SubstraitPlanPipelineVisitor.h"

#include <memory>

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

namespace {

bool continuingPipelineContains(
    SubstraitPlanParser::PipelineContext* ctx,
    const std::string& relationName) {
  if (ctx->relation_ref()->getText() == relationName) {
    return true;
  }
  if (dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)->getRuleIndex() !=
      SubstraitPlanParser::RulePipeline) {
    return false;
  }
  return continuingPipelineContains(
      dynamic_cast<SubstraitPlanParser::PipelineContext*>(ctx->parent),
      relationName);
}

} // namespace

void SubstraitPlanPipelineVisitor::updateRelationSymbol(
    SubstraitPlanParser::PipelineContext* ctx,
    const std::string& relationName) {
  const auto* symbol = symbolTable_->lookupSymbolByName(relationName);
  if (symbol == nullptr) {
    // This is a reference to a missing relation so we create a stub for it.
    auto relationData = std::make_shared<RelationData>();
    symbolTable_->defineSymbol(
        relationName,
        Location(ctx),
        SymbolType::kRelation,
        RelationType::kUnknown,
        relationData);
  } else {
    // Update the location on this symbol, so we can find it in its canonical
    // location.
    symbolTable_->updateLocation(*symbol, Location(ctx));
  }
}

std::any SubstraitPlanPipelineVisitor::visitPipelines(
    SubstraitPlanParser::PipelinesContext* ctx) {
  return visitChildren(ctx);
}

/*
 * Creates symbols in order of discovery, since the first symbol encountered
 * is the bottom of the chain, create children nodes first, then create the
 * symbol. Visits children to accomplish that. Children know their parents
 * allowing them to document the relationship.
 */
std::any SubstraitPlanPipelineVisitor::visitPipeline(
    SubstraitPlanParser::PipelineContext* ctx) {
  // Create our symbol table entry if we don't already have one.
  std::string relationName = ctx->relation_ref()->id(0)->getText();
  updateRelationSymbol(ctx, relationName);

  // Process the rest of the pipeline first in order to evaluate left to
  // right.
  std::any result;
  if (ctx->pipeline() != nullptr) {
    result = visitPipeline(ctx->pipeline());
  }

  // Refetch our symbol table entry to make sure we have the latest version.
  auto* symbol = symbolTable_->lookupSymbolByName(relationName);
  if (symbol->blob.type() != typeid(std::shared_ptr<RelationData>)) {
    return defaultResult();
  }
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol->blob);

  // Check for accidental cross-pipeline use.
  if (relationData->continuingPipeline != nullptr) {
    errorListener_->addError(
        ctx->relation_ref()->getStart(),
        "Relation " + relationName +
            " is already a non-terminating participant in a pipeline.");
    return result;
  }

  // Directions are calculated when viewing the pipeline as text:
  //    a -> b -> c -> d
  const SymbolInfo* leftSymbol = &SymbolInfo::kUnknown;
  if (ctx->pipeline() != nullptr) {
    leftSymbol =
        symbolTable_->lookupSymbolByLocation(PARSER_LOCATION(ctx->pipeline()));
  }
  const SymbolInfo* rightSymbol = &SymbolInfo::kUnknown;
  if (dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)->getRuleIndex() ==
      SubstraitPlanParser::RulePipeline) {
    rightSymbol =
        symbolTable_->lookupSymbolByLocation(PARSER_LOCATION(ctx->parent));
  }
  const SymbolInfo* rightmostSymbol = rightSymbol;
  if (*rightSymbol != SymbolInfo::kUnknown) {
    if (rightSymbol->blob.type() != typeid(std::shared_ptr<RelationData>)) {
      errorListener_->addError(
          ctx->getStart(), "No relation definition present for this symbol.");
    }
    auto rightRelationData =
        ANY_CAST(std::shared_ptr<RelationData>, rightSymbol->blob);
    if (rightRelationData->pipelineStart != nullptr) {
      rightmostSymbol = rightRelationData->pipelineStart;
    }
  }

  // Prevent loops within the pipeline.
  if (dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)->getRuleIndex() ==
      SubstraitPlanParser::RulePipeline) {
    if (continuingPipelineContains(
            dynamic_cast<SubstraitPlanParser::PipelineContext*>(ctx->parent),
            relationName)) {
      errorListener_->addError(
          ctx->relation_ref()->getStart(),
          "Relation " + relationName + " is already in this pipeline.");
      return result;
    }
  }

  // Keep track of the front of the pipeline.
  if (rightmostSymbol != nullptr && *rightmostSymbol != SymbolInfo::kUnknown) {
    relationData->pipelineStart = rightmostSymbol;
  }

  // Connect us up to the next link in the chain to the left.
  if (*leftSymbol != SymbolInfo::kUnknown) {
    if (*rightSymbol == SymbolInfo::kUnknown) {
      relationData->newPipelines.push_back(leftSymbol);
    } else {
      relationData->continuingPipeline = leftSymbol;
    }
  }

  return result;
}

} // namespace io::substrait::textplan
