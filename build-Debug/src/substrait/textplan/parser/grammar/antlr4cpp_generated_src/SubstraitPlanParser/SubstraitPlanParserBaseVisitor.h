
// Generated from SubstraitPlanParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "SubstraitPlanParserVisitor.h"


namespace io::substrait::textplan {

/**
 * This class provides an empty implementation of SubstraitPlanParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SubstraitPlanParserBaseVisitor : public SubstraitPlanParserVisitor {
public:

  virtual std::any visitPlan(SubstraitPlanParser::PlanContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPlan_detail(SubstraitPlanParser::Plan_detailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipelines(SubstraitPlanParser::PipelinesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPipeline(SubstraitPlanParser::PipelineContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelation(SubstraitPlanParser::RelationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRoot_relation(SubstraitPlanParser::Root_relationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelation_type(SubstraitPlanParser::Relation_typeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelation_ref(SubstraitPlanParser::Relation_refContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelation_filter_behavior(SubstraitPlanParser::Relation_filter_behaviorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMeasure_detail(SubstraitPlanParser::Measure_detailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationCommon(SubstraitPlanParser::RelationCommonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationUsesSchema(SubstraitPlanParser::RelationUsesSchemaContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationFilter(SubstraitPlanParser::RelationFilterContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationExpression(SubstraitPlanParser::RelationExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationAdvancedExtension(SubstraitPlanParser::RelationAdvancedExtensionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationSourceReference(SubstraitPlanParser::RelationSourceReferenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationGrouping(SubstraitPlanParser::RelationGroupingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationMeasure(SubstraitPlanParser::RelationMeasureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationSort(SubstraitPlanParser::RelationSortContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationCount(SubstraitPlanParser::RelationCountContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationJoinType(SubstraitPlanParser::RelationJoinTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationEmit(SubstraitPlanParser::RelationEmitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionScalarSubquery(SubstraitPlanParser::ExpressionScalarSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionConstant(SubstraitPlanParser::ExpressionConstantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionFunctionUse(SubstraitPlanParser::ExpressionFunctionUseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionColumn(SubstraitPlanParser::ExpressionColumnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionSetComparisonSubquery(SubstraitPlanParser::ExpressionSetComparisonSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionInPredicateSubquery(SubstraitPlanParser::ExpressionInPredicateSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionCast(SubstraitPlanParser::ExpressionCastContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionSetPredicateSubquery(SubstraitPlanParser::ExpressionSetPredicateSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression_list(SubstraitPlanParser::Expression_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstant(SubstraitPlanParser::ConstantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral_basic_type(SubstraitPlanParser::Literal_basic_typeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral_complex_type(SubstraitPlanParser::Literal_complex_typeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral_specifier(SubstraitPlanParser::Literal_specifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMap_literal(SubstraitPlanParser::Map_literalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMap_literal_value(SubstraitPlanParser::Map_literal_valueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStruct_literal(SubstraitPlanParser::Struct_literalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumn_name(SubstraitPlanParser::Column_nameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSource_reference(SubstraitPlanParser::Source_referenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFile_location(SubstraitPlanParser::File_locationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFile_detail(SubstraitPlanParser::File_detailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFile(SubstraitPlanParser::FileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocal_files_detail(SubstraitPlanParser::Local_files_detailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamed_table_detail(SubstraitPlanParser::Named_table_detailContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSchema_definition(SubstraitPlanParser::Schema_definitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSchema_item(SubstraitPlanParser::Schema_itemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSource_definition(SubstraitPlanParser::Source_definitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocalFiles(SubstraitPlanParser::LocalFilesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVirtualTable(SubstraitPlanParser::VirtualTableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNamedTable(SubstraitPlanParser::NamedTableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExtensionTable(SubstraitPlanParser::ExtensionTableContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExtensionspace(SubstraitPlanParser::ExtensionspaceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction(SubstraitPlanParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSort_field(SubstraitPlanParser::Sort_fieldContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitName(SubstraitPlanParser::NameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSignature(SubstraitPlanParser::SignatureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitId(SubstraitPlanParser::IdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimple_id(SubstraitPlanParser::Simple_idContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace io::substrait::textplan
