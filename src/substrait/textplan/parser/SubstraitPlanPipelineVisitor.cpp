/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/SubstraitPlanPipelineVisitor.h"

#include <memory>

#include "SubstraitPlanLexer/SubstraitPlanLexer.h"
#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/StructuredSymbolData.h"

namespace io::substrait::textplan {

void SubstraitPlanPipelineVisitor::updateRelationSymbol(
    SubstraitPlanParser::PipelineContext* ctx,
    const std::string& relationName) {
  const auto& symbol = symbolTable_->lookupSymbolByName(relationName);
  if (symbol == SymbolInfo::kUnknown) {
    // This is a reference to a missing relation so we create a stub for it.
    auto relationData = std::make_shared<RelationData>();
    symbolTable_->defineSymbol(
        relationName,
        Location(ctx),
        SymbolType::kRelation,
        RelationDetailType::kUnknown,
        relationData);
  } else {
    // Add our location to this symbol so we can find it either way.
    symbolTable_->updateLocation(symbol, Location(ctx));
  }
}

std::any SubstraitPlanPipelineVisitor::visitPipelines(
    SubstraitPlanParser::PipelinesContext* ctx) {
  return visitChildren(ctx);
}

/*
 * Creates symbols in order of discovery, since the first symbol encountered is
 * the bottom of the chain, create children nodes first, then create the symbol.
 * Visits children to accomplish that.
 * Children know their parents allowing them to document the relationship.
 */
std::any SubstraitPlanPipelineVisitor::visitPipeline(
    SubstraitPlanParser::PipelineContext* ctx) {
  // Create our symbol table entry if we don't already have one.
  std::string relationName = ctx->relation_ref()->id(0)->getText();
  updateRelationSymbol(ctx, relationName);

  // Process the rest of the pipeline first in order to evaluate left to right.
  std::any result;
  if (ctx->pipeline() != nullptr) {
    result = visitPipeline(ctx->pipeline());
  }

  // Refetch our symbol table entry to make sure we have the latest version.
  auto& symbol = symbolTable_->lookupSymbolByName(relationName);
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol.blob);

  if (relationData->continuingPipeline != nullptr) {
    errorListener_->addError(
        ctx->relation_ref()->getStart(),
        "Relation " + relationName +
            " is already a non-terminating participant in a pipeline.");
    return result;
  }

  // Keep track of the front of the pipeline.
  if (ctx->pipeline() == nullptr) {
    // This is the leftmost node in the pipeline so mark it as start.
    relationData->pipelineStart = &symbol;
  } else {
    // Look at the node to the left of us to see what the start is.
    const auto& leftSymbol =
        symbolTable_->lookupSymbolByLocation(PARSER_LOCATION(ctx->pipeline()));
    if (leftSymbol != SymbolInfo::kUnknown) {
      auto leftRelationData =
          ANY_CAST(std::shared_ptr<RelationData>, leftSymbol.blob);
      relationData->pipelineStart = leftRelationData->pipelineStart;
    }
  }

  // Connect us up to the next link in the chain to the right.
  if (dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)->getRuleIndex() ==
      SubstraitPlanParser::RulePipeline) {
    // We are not the last in the chain.
    const auto& parentSymbol =
        symbolTable_->lookupSymbolByLocation(PARSER_LOCATION(ctx->parent));

    if (ctx->pipeline() == nullptr) {
      relationData->newPipelines.push_back(&parentSymbol);
    } else {
      if (!relationData->newPipelines.empty()) {
        errorListener_->addError(
            ctx->relation_ref()->getStart(),
            "Relation " + relationName + " cannot be in the middle of a " +
                "pipeline when it is already starting pipelines.");
        return result;
      }

      relationData->continuingPipeline = &parentSymbol;
      auto parentRelationData =
          ANY_CAST(std::shared_ptr<RelationData>, parentSymbol.blob);
    }
  }

  // We are not the first node in the pipeline, make sure we don't duplicate.
  if (ctx->pipeline() != nullptr && relationData->pipelineStart != nullptr &&
      relationData->pipelineStart->name == relationName) {
    errorListener_->addError(
        ctx->relation_ref()->getStart(),
        "Relation " + relationName +
            " cannot start and end the same pipeline.");
    return result;
  }

  return result;
}

} // namespace io::substrait::textplan
