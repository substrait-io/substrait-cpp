/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/SubstraitPlanVisitor.h"

#include <memory>

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Finally.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/type/Type.h"

namespace io::substrait::textplan {

// Removes leading and trailing quotation marks.
std::string extractFromString(std::string s) {
  if (s.size() < 2) {
    return s;
  }
  if (s[s.size() - 1] == '"') {
    s.erase(s.size() - 1, 1);
  }
  if (s[0] == '"') {
    s.erase(0, 1);
  }
  return s;
}

// NOLINTBEGIN(readability-identifier-naming)
// NOLINTBEGIN(readability-convert-member-functions-to-static)

std::any SubstraitPlanVisitor::visitPlan(
    SubstraitPlanParser::PlanContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitPlan_detail(
    SubstraitPlanParser::Plan_detailContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitPipelines(
    SubstraitPlanParser::PipelinesContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitPipeline(
    SubstraitPlanParser::PipelineContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitExtensionspace(
    SubstraitPlanParser::ExtensionspaceContext* ctx) {
  if (ctx->URI() == nullptr) {
    // Nothing to keep track of at this level.
    return visitChildren(ctx);
  }

  const std::string& uri = ctx->URI()->getText();
  // TODO -- Transition to querying the symbol table for the space number.  #42
  uint32_t thisSpace = ++numSpacesSeen_;
  symbolTable_->defineSymbol(
      uri,
      Location(ctx),
      SymbolType::kExtensionSpace,
      defaultResult(),
      std::make_shared<ExtensionSpaceData>(thisSpace));
  auto result = visitChildren(ctx);

  // Update the contained functions to belong in this space.
  for (auto func : ctx->function()) {
    auto funcSymbol = symbolTable_->lookupSymbolByLocation(Location(func));
    auto functionData =
        ANY_CAST(std::shared_ptr<FunctionData>, funcSymbol.blob);
    functionData->extensionUriReference = thisSpace;
  }

  return result;
}

std::any SubstraitPlanVisitor::visitFunction(
    SubstraitPlanParser::FunctionContext* ctx) {
  // TODO -- Transition to using the symbol table for the function number.  #42
  // Let our enclosing extension space provide us with the detail.
  std::string referenceName;
  if (ctx->id() != nullptr) {
    referenceName = ctx->id()->getText();
  } else if (ctx->name() != nullptr) {
    referenceName = ctx->name()->getText();
    auto colonPos = referenceName.find_first_of(':');
    if (colonPos != std::string::npos) {
      referenceName = referenceName.substr(0, colonPos);
    }
  } else {
    referenceName = "";
  }

  // We do not yet examine the type of functions but we look for presence.
  if (ctx->name() != nullptr) {
    auto colonPos = ctx->name()->getText().find_first_of(':');
    if (colonPos == std::string::npos ||
        ctx->name()->getText().substr(colonPos+1).empty()) {
      errorListener_->addError(
          ctx->getStart(), "Functions should have an associated type.");
    }
  }

  symbolTable_->defineSymbol(
      referenceName,
      Location(ctx),
      SymbolType::kFunction,
      defaultResult(),
      std::make_shared<FunctionData>(
          ctx->name()->getText(), std::nullopt, numFunctionsSeen_++));
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitName(
    SubstraitPlanParser::NameContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitSignature(
    SubstraitPlanParser::SignatureContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitSchema_definition(
    SubstraitPlanParser::Schema_definitionContext* ctx) {
  symbolTable_->defineSymbol(
      ctx->id()->getText(),
      Location(ctx),
      SymbolType::kSchema,
      defaultResult(),
      defaultResult());

  // Mark all of the schema items so we can find the ones related to this
  // schema.
  for (const auto& item : ctx->schema_item()) {
    auto symbol = ANY_CAST(SymbolInfo*, visitSchema_item(item));
    if (symbol == nullptr) {
      continue;
    }
    symbol->location = Location(ctx);
  }

  return nullptr;
}

std::any SubstraitPlanVisitor::visitSchema_item(
    SubstraitPlanParser::Schema_itemContext* ctx) {
  return symbolTable_->defineSymbol(
      ctx->id()->getText(),
      Location(ctx),
      SymbolType::kSchemaColumn,
      defaultResult(),
      visitLiteral_complex_type(ctx->literal_complex_type()));
}

std::any SubstraitPlanVisitor::visitRelation(
    SubstraitPlanParser::RelationContext* ctx) {
  auto relType =
      ANY_CAST(RelationType, visitRelation_type(ctx->relation_type()));
  if (ctx->relation_ref() == nullptr) {
    return nullptr;
  }

  std::string relationName = ctx->relation_ref()->id(0)->getText();
  auto symbol = symbolTable_->lookupSymbolByName(relationName);
  if (symbol != nullptr) {
    errorListener_->addError(
        ctx->getStart(),
        "Relation named " + relationName + " already defined.");
  } else {
    auto relationData = std::make_shared<RelationData>();
    symbol = symbolTable_->defineSymbol(
        relationName,
        Location(ctx),
        SymbolType::kRelation,
        relType,
        relationData);
  }

  // Mark the current scope for any operations within this relation.
  auto previousScope = currentRelationScope_;
  auto resetCurrentScope =
      finally([&]() { currentRelationScope_ = previousScope; });
  currentRelationScope_ = symbol;

  visitRelation_ref(ctx->relation_ref());
  for (const auto detail : ctx->relation_detail()) {
    visit(detail);
  }
  return nullptr;
}

std::any SubstraitPlanVisitor::visitRelation_type(
    SubstraitPlanParser::Relation_typeContext* ctx) {
  std::string id = ctx->getText();
  std::transform(id.begin(), id.end(), id.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  // TODO -- Replace this with a handcrafted function or a trie.
  if (id == "read") {
    return RelationType::kRead;
  } else if (id == "project") {
    return RelationType::kProject;
  } else if (id == "join") {
    return RelationType::kJoin;
  } else if (id == "cross") {
    return RelationType::kCross;
  } else if (id == "fetch") {
    return RelationType::kFetch;
  } else if (id == "aggregate") {
    return RelationType::kAggregate;
  } else if (id == "sort") {
    return RelationType::kSort;
  } else if (id == "filter") {
    return RelationType::kFilter;
  } else if (id == "set") {
    return RelationType::kSet;
  }
  this->errorListener_->addError(
      ctx->getStart(), "Unrecognized relation type: " + ctx->getText());
  return RelationType::kUnknown;
}

std::any SubstraitPlanVisitor::visitSource_definition(
    SubstraitPlanParser::Source_definitionContext* ctx) {
  symbolTable_->defineSymbol(
      ctx->read_type()->children[1]->getText(),
      Location(ctx),
      SymbolType::kSource,
      defaultResult(),
      defaultResult());
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitLiteral_specifier(
    SubstraitPlanParser::Literal_specifierContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitMap_literal_value(
    SubstraitPlanParser::Map_literal_valueContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitMap_literal(
    SubstraitPlanParser::Map_literalContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitStruct_literal(
    SubstraitPlanParser::Struct_literalContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitConstant(
    SubstraitPlanParser::ConstantContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitColumn_name(
    SubstraitPlanParser::Column_nameContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitSource_reference(
    SubstraitPlanParser::Source_referenceContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitExpressionFunctionUse(
    SubstraitPlanParser::ExpressionFunctionUseContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitExpressionConstant(
    SubstraitPlanParser::ExpressionConstantContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitExpressionColumn(
    SubstraitPlanParser::ExpressionColumnContext* ctx) {
  auto relationData =
      ANY_CAST(std::shared_ptr<RelationData>, currentRelationScope_->blob);
  std::string column_name = ctx->column_name()->getText();
  auto symbol = symbolTable_->lookupSymbolByName(column_name);
  if (symbol == nullptr) {
    symbol = symbolTable_->defineSymbol(
        column_name,
        Location(ctx),
        SymbolType::kField,
        std::nullopt,
        std::nullopt);
    relationData->fieldReferences.push_back(symbol);
  }
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitExpressionCast(
    SubstraitPlanParser::ExpressionCastContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationCommon(
    SubstraitPlanParser::RelationCommonContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationUsesSchema(
    SubstraitPlanParser::RelationUsesSchemaContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelation_filter_behavior(
    SubstraitPlanParser::Relation_filter_behaviorContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitMeasure_detail(
    SubstraitPlanParser::Measure_detailContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationFilter(
    SubstraitPlanParser::RelationFilterContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationExpression(
    SubstraitPlanParser::RelationExpressionContext* ctx) {
  visitChildren(ctx);
  return nullptr;
}

std::any SubstraitPlanVisitor::visitRelationAdvancedExtension(
    SubstraitPlanParser::RelationAdvancedExtensionContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationSourceReference(
    SubstraitPlanParser::RelationSourceReferenceContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationGrouping(
    SubstraitPlanParser::RelationGroupingContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationMeasure(
    SubstraitPlanParser::RelationMeasureContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationSort(
    SubstraitPlanParser::RelationSortContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationCount(
    SubstraitPlanParser::RelationCountContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitRelationJoinType(
    SubstraitPlanParser::RelationJoinTypeContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitFile_location(
    SubstraitPlanParser::File_locationContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitFile_detail(
    SubstraitPlanParser::File_detailContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitFile(
    SubstraitPlanParser::FileContext* ctx) {
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitLocal_files_detail(
    SubstraitPlanParser::Local_files_detailContext* ctx) {
  for (const auto& f : ctx->file()) {
    symbolTable_->defineSymbol(
        f->getText(),
        PARSER_LOCATION(ctx->parent->parent), // The source we belong to.
        SymbolType::kSourceDetail,
        defaultResult(),
        defaultResult());
  }
  return nullptr;
}

std::any SubstraitPlanVisitor::visitLocalFiles(
    SubstraitPlanParser::LocalFilesContext* ctx) {
  // TODO -- Once we switch over to SourceData update our parent's subtype.
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitVirtualTable(
    SubstraitPlanParser::VirtualTableContext* ctx) {
  // TODO -- Once we switch over to SourceData update our parent's subtype.
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitNamedTable(
    SubstraitPlanParser::NamedTableContext* ctx) {
  // TODO -- Once we switch over to SourceData update our parent's subtype.
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitExtensionTable(
    SubstraitPlanParser::ExtensionTableContext* ctx) {
  // TODO -- Once we switch over to SourceData update our parent's subtype.
  return visitChildren(ctx);
}

std::any SubstraitPlanVisitor::visitNamed_table_detail(
    SubstraitPlanParser::Named_table_detailContext* ctx) {
  for (const auto& s : ctx->STRING()) {
    std::string str = s->getText();
    symbolTable_->defineSymbol(
        extractFromString(str),
        PARSER_LOCATION(ctx->parent->parent), // The source we belong to.
        SymbolType::kSourceDetail,
        defaultResult(),
        defaultResult());
  }
  return nullptr;
}

std::any SubstraitPlanVisitor::visitRelation_ref(
    SubstraitPlanParser::Relation_refContext* ctx) {
  auto rel = ANY_CAST(std::string, visitId(ctx->id(0)));
  if (ctx->id().size() > 1) {
    visitId(ctx->id(1)); // TODO -- Make use of the reference.
  }
  return rel;
}

std::any SubstraitPlanVisitor::visitSort_field(
    SubstraitPlanParser::Sort_fieldContext* ctx) {
  return defaultResult();
}

std::any SubstraitPlanVisitor::visitId(SubstraitPlanParser::IdContext* ctx) {
  return ctx->getText();
}

std::any SubstraitPlanVisitor::visitSimple_id(
    SubstraitPlanParser::Simple_idContext* ctx) {
  return defaultResult();
}

// NOLINTEND(readability-convert-member-functions-to-static)
// NOLINTEND(readability-identifier-naming)

} // namespace io::substrait::textplan
