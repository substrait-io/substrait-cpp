/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "SubstraitPlanParser/SubstraitPlanParserVisitor.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/parser/SubstraitParserErrorListener.h"

namespace io::substrait::textplan {

class SubstraitPlanVisitor : public SubstraitPlanParserVisitor {
 public:
  SubstraitPlanVisitor() {
    symbolTable_ = std::make_shared<SymbolTable>();
    errorListener_ = std::make_shared<SubstraitParserErrorListener>();
  }

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbolTable_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitParserErrorListener> getErrorListener()
      const {
    return errorListener_;
  };

  std::any visitPlan(SubstraitPlanParser::PlanContext* context) override;
  std::any visitPlan_detail(
      SubstraitPlanParser::Plan_detailContext* ctx) override;
  std::any visitPipelines(SubstraitPlanParser::PipelinesContext* ctx) override;
  std::any visitPipeline(SubstraitPlanParser::PipelineContext* ctx) override;
  std::any visitExtensionspace(
      SubstraitPlanParser::ExtensionspaceContext* ctx) override;
  std::any visitFunction(SubstraitPlanParser::FunctionContext* ctx) override;
  std::any visitName(SubstraitPlanParser::NameContext* ctx) override;
  std::any visitSignature(SubstraitPlanParser::SignatureContext* ctx) override;
  std::any visitSchema_definition(
      SubstraitPlanParser::Schema_definitionContext* ctx) override;
  std::any visitColumn_attribute(
      SubstraitPlanParser::Column_attributeContext* ctx) override;
  std::any visitColumn_type(
      SubstraitPlanParser::Column_typeContext* ctx) override;
  std::any visitSchema_item(
      SubstraitPlanParser::Schema_itemContext* ctx) override;
  std::any visitRelation(SubstraitPlanParser::RelationContext* ctx) override;
  std::any visitRelation_type(
      SubstraitPlanParser::Relation_typeContext* ctx) override;
  std::any visitSource_definition(
      SubstraitPlanParser::Source_definitionContext* ctx) override;
  std::any visitLiteral_specifier(
      SubstraitPlanParser::Literal_specifierContext* ctx) override;
  std::any visitLiteral_basic_type(
      SubstraitPlanParser::Literal_basic_typeContext* ctx) override;
  std::any visitLiteral_complex_type(
      SubstraitPlanParser::Literal_complex_typeContext* ctx) override;
  std::any visitMap_literal_value(
      SubstraitPlanParser::Map_literal_valueContext* ctx) override;
  std::any visitMap_literal(
      SubstraitPlanParser::Map_literalContext* ctx) override;
  std::any visitStruct_literal(
      SubstraitPlanParser::Struct_literalContext* ctx) override;
  std::any visitConstant(SubstraitPlanParser::ConstantContext* ctx) override;
  std::any visitColumn_name(
      SubstraitPlanParser::Column_nameContext* ctx) override;
  std::any visitSource_reference(
      SubstraitPlanParser::Source_referenceContext* ctx) override;
  std::any visitExpressionFunctionUse(
      SubstraitPlanParser::ExpressionFunctionUseContext* ctx) override;
  std::any visitExpressionConstant(
      SubstraitPlanParser::ExpressionConstantContext* ctx) override;
  std::any visitExpressionCast(
      SubstraitPlanParser::ExpressionCastContext* ctx) override;
  std::any visitExpressionColumn(
      SubstraitPlanParser::ExpressionColumnContext* ctx) override;
  std::any visitRelationCommon(
      SubstraitPlanParser::RelationCommonContext* ctx) override;
  std::any visitRelationUsesSchema(
      SubstraitPlanParser::RelationUsesSchemaContext* ctx) override;
  std::any visitRelation_filter_behavior(
      SubstraitPlanParser::Relation_filter_behaviorContext* ctx) override;
  std::any visitRelationFilter(
      SubstraitPlanParser::RelationFilterContext* ctx) override;
  std::any visitRelationProjection(
      SubstraitPlanParser::RelationProjectionContext* ctx) override;
  std::any visitRelationExpression(
      SubstraitPlanParser::RelationExpressionContext* ctx) override;
  std::any visitRelationAdvancedExtension(
      SubstraitPlanParser::RelationAdvancedExtensionContext* ctx) override;
  std::any visitRelationSourceReference(
      SubstraitPlanParser::RelationSourceReferenceContext* ctx) override;
  std::any visitFile_location(
      SubstraitPlanParser::File_locationContext* ctx) override;
  std::any visitFile_detail(
      SubstraitPlanParser::File_detailContext* ctx) override;
  std::any visitFile(SubstraitPlanParser::FileContext* ctx) override;
  std::any visitLocal_files_detail(
      SubstraitPlanParser::Local_files_detailContext* ctx) override;
  std::any visitLocalFiles(
      SubstraitPlanParser::LocalFilesContext* ctx) override;
  std::any visitVirtualTable(
      SubstraitPlanParser::VirtualTableContext* ctx) override;
  std::any visitNamedTable(
      SubstraitPlanParser::NamedTableContext* ctx) override;
  std::any visitExtensionTable(
      SubstraitPlanParser::ExtensionTableContext* ctx) override;
  std::any visitNamed_table_detail(
      SubstraitPlanParser::Named_table_detailContext* ctx) override;
  std::any visitRelation_ref(
      SubstraitPlanParser::Relation_refContext* ctx) override;
  std::any visitId(SubstraitPlanParser::IdContext* ctx) override;

 private:
  std::shared_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<SubstraitParserErrorListener> errorListener_;

  int numFunctionsSeen_{0};
};

} // namespace io::substrait::textplan
