
// Generated from SubstraitPlanParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "SubstraitPlanParser.h"


namespace io::substrait::textplan {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by SubstraitPlanParser.
 */
class  SubstraitPlanParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SubstraitPlanParser.
   */
    virtual std::any visitPlan(SubstraitPlanParser::PlanContext *context) = 0;

    virtual std::any visitPlan_detail(SubstraitPlanParser::Plan_detailContext *context) = 0;

    virtual std::any visitPipelines(SubstraitPlanParser::PipelinesContext *context) = 0;

    virtual std::any visitPipeline(SubstraitPlanParser::PipelineContext *context) = 0;

    virtual std::any visitRelation(SubstraitPlanParser::RelationContext *context) = 0;

    virtual std::any visitRoot_relation(SubstraitPlanParser::Root_relationContext *context) = 0;

    virtual std::any visitRelation_type(SubstraitPlanParser::Relation_typeContext *context) = 0;

    virtual std::any visitRelation_ref(SubstraitPlanParser::Relation_refContext *context) = 0;

    virtual std::any visitRelation_filter_behavior(SubstraitPlanParser::Relation_filter_behaviorContext *context) = 0;

    virtual std::any visitMeasure_detail(SubstraitPlanParser::Measure_detailContext *context) = 0;

    virtual std::any visitRelationCommon(SubstraitPlanParser::RelationCommonContext *context) = 0;

    virtual std::any visitRelationUsesSchema(SubstraitPlanParser::RelationUsesSchemaContext *context) = 0;

    virtual std::any visitRelationFilter(SubstraitPlanParser::RelationFilterContext *context) = 0;

    virtual std::any visitRelationExpression(SubstraitPlanParser::RelationExpressionContext *context) = 0;

    virtual std::any visitRelationAdvancedExtension(SubstraitPlanParser::RelationAdvancedExtensionContext *context) = 0;

    virtual std::any visitRelationSourceReference(SubstraitPlanParser::RelationSourceReferenceContext *context) = 0;

    virtual std::any visitRelationGrouping(SubstraitPlanParser::RelationGroupingContext *context) = 0;

    virtual std::any visitRelationMeasure(SubstraitPlanParser::RelationMeasureContext *context) = 0;

    virtual std::any visitRelationSort(SubstraitPlanParser::RelationSortContext *context) = 0;

    virtual std::any visitRelationCount(SubstraitPlanParser::RelationCountContext *context) = 0;

    virtual std::any visitRelationJoinType(SubstraitPlanParser::RelationJoinTypeContext *context) = 0;

    virtual std::any visitRelationEmit(SubstraitPlanParser::RelationEmitContext *context) = 0;

    virtual std::any visitExpressionScalarSubquery(SubstraitPlanParser::ExpressionScalarSubqueryContext *context) = 0;

    virtual std::any visitExpressionConstant(SubstraitPlanParser::ExpressionConstantContext *context) = 0;

    virtual std::any visitExpressionFunctionUse(SubstraitPlanParser::ExpressionFunctionUseContext *context) = 0;

    virtual std::any visitExpressionColumn(SubstraitPlanParser::ExpressionColumnContext *context) = 0;

    virtual std::any visitExpressionSetComparisonSubquery(SubstraitPlanParser::ExpressionSetComparisonSubqueryContext *context) = 0;

    virtual std::any visitExpressionInPredicateSubquery(SubstraitPlanParser::ExpressionInPredicateSubqueryContext *context) = 0;

    virtual std::any visitExpressionCast(SubstraitPlanParser::ExpressionCastContext *context) = 0;

    virtual std::any visitExpressionSetPredicateSubquery(SubstraitPlanParser::ExpressionSetPredicateSubqueryContext *context) = 0;

    virtual std::any visitExpression_list(SubstraitPlanParser::Expression_listContext *context) = 0;

    virtual std::any visitConstant(SubstraitPlanParser::ConstantContext *context) = 0;

    virtual std::any visitLiteral_basic_type(SubstraitPlanParser::Literal_basic_typeContext *context) = 0;

    virtual std::any visitLiteral_complex_type(SubstraitPlanParser::Literal_complex_typeContext *context) = 0;

    virtual std::any visitLiteral_specifier(SubstraitPlanParser::Literal_specifierContext *context) = 0;

    virtual std::any visitMap_literal(SubstraitPlanParser::Map_literalContext *context) = 0;

    virtual std::any visitMap_literal_value(SubstraitPlanParser::Map_literal_valueContext *context) = 0;

    virtual std::any visitStruct_literal(SubstraitPlanParser::Struct_literalContext *context) = 0;

    virtual std::any visitColumn_name(SubstraitPlanParser::Column_nameContext *context) = 0;

    virtual std::any visitSource_reference(SubstraitPlanParser::Source_referenceContext *context) = 0;

    virtual std::any visitFile_location(SubstraitPlanParser::File_locationContext *context) = 0;

    virtual std::any visitFile_detail(SubstraitPlanParser::File_detailContext *context) = 0;

    virtual std::any visitFile(SubstraitPlanParser::FileContext *context) = 0;

    virtual std::any visitLocal_files_detail(SubstraitPlanParser::Local_files_detailContext *context) = 0;

    virtual std::any visitNamed_table_detail(SubstraitPlanParser::Named_table_detailContext *context) = 0;

    virtual std::any visitSchema_definition(SubstraitPlanParser::Schema_definitionContext *context) = 0;

    virtual std::any visitSchema_item(SubstraitPlanParser::Schema_itemContext *context) = 0;

    virtual std::any visitSource_definition(SubstraitPlanParser::Source_definitionContext *context) = 0;

    virtual std::any visitLocalFiles(SubstraitPlanParser::LocalFilesContext *context) = 0;

    virtual std::any visitVirtualTable(SubstraitPlanParser::VirtualTableContext *context) = 0;

    virtual std::any visitNamedTable(SubstraitPlanParser::NamedTableContext *context) = 0;

    virtual std::any visitExtensionTable(SubstraitPlanParser::ExtensionTableContext *context) = 0;

    virtual std::any visitExtensionspace(SubstraitPlanParser::ExtensionspaceContext *context) = 0;

    virtual std::any visitFunction(SubstraitPlanParser::FunctionContext *context) = 0;

    virtual std::any visitSort_field(SubstraitPlanParser::Sort_fieldContext *context) = 0;

    virtual std::any visitName(SubstraitPlanParser::NameContext *context) = 0;

    virtual std::any visitSignature(SubstraitPlanParser::SignatureContext *context) = 0;

    virtual std::any visitId(SubstraitPlanParser::IdContext *context) = 0;

    virtual std::any visitSimple_id(SubstraitPlanParser::Simple_idContext *context) = 0;


};

}  // namespace io::substrait::textplan
