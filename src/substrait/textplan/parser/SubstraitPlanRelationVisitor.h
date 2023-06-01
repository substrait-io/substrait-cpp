/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "SubstraitPlanParser/SubstraitPlanParserBaseVisitor.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/parser/SubstraitParserErrorListener.h"
#include "substrait/type/Type.h"

namespace substrait::proto {
class Expression_Literal;
class Expression_Literal_Map_KeyValue;
class Type;
class Type_Struct;
} // namespace substrait::proto

namespace io::substrait::textplan {

class SubstraitPlanRelationVisitor : public SubstraitPlanParserBaseVisitor {
 public:
  SubstraitPlanRelationVisitor(
      const SymbolTable& symbolTable,
      std::shared_ptr<SubstraitParserErrorListener> errorListener) {
    symbolTable_ = std::make_shared<SymbolTable>(symbolTable);
    errorListener_ = std::move(errorListener);
  }

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbolTable_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitParserErrorListener> getErrorListener()
      const {
    return errorListener_;
  };

  std::any aggregateResult(std::any aggregate, std::any nextResult) override;

  std::any visitRelation(SubstraitPlanParser::RelationContext* ctx) override;

  std::any visitRelation_filter_behavior(
      SubstraitPlanParser::Relation_filter_behaviorContext* ctx) override;

  std::any visitRelationFilter(
      SubstraitPlanParser::RelationFilterContext* ctx) override;

  std::any visitRelationExpression(
      SubstraitPlanParser::RelationExpressionContext* ctx) override;

  // visitExpression is a new method delegating to the methods below.
  std::any visitExpression(SubstraitPlanParser::ExpressionContext* ctx);

  std::any visitExpressionFunctionUse(
      SubstraitPlanParser::ExpressionFunctionUseContext* ctx) override;

  std::any visitExpressionConstant(
      SubstraitPlanParser::ExpressionConstantContext* ctx) override;

  std::any visitExpressionCast(
      SubstraitPlanParser::ExpressionCastContext* ctx) override;

  std::any visitExpressionColumn(
      SubstraitPlanParser::ExpressionColumnContext* ctx) override;

  std::any visitConstant(SubstraitPlanParser::ConstantContext* ctx) override;

  std::any visitLiteral_basic_type(
      SubstraitPlanParser::Literal_basic_typeContext* ctx) override;

  std::any visitLiteral_complex_type(
      SubstraitPlanParser::Literal_complex_typeContext* ctx) override;

  std::any visitLiteral_specifier(
      SubstraitPlanParser::Literal_specifierContext* ctx) override;

  std::any visitMap_literal(
      SubstraitPlanParser::Map_literalContext* ctx) override;

  std::any visitMap_literal_value(
      SubstraitPlanParser::Map_literal_valueContext* ctx) override;

  std::any visitStruct_literal(
      SubstraitPlanParser::Struct_literalContext* ctx) override;

  std::any visitColumn_name(
      SubstraitPlanParser::Column_nameContext* ctx) override;

  ::substrait::proto::Expression_Literal visitConstantWithType(
      SubstraitPlanParser::ConstantContext* ctx,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal_Map_KeyValue
  visitMapLiteralValueWithType(
      SubstraitPlanParser::Map_literal_valueContext* ctx,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal visitMapLiteralWithType(
      SubstraitPlanParser::Map_literalContext* ctx,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal visitStructLiteralWithType(
      SubstraitPlanParser::Struct_literalContext* ctx,
      const ::substrait::proto::Type_Struct& literalType);

  ::substrait::proto::Expression_Literal visitString(
      antlr4::tree::TerminalNode* node,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal visitNumber(
      antlr4::tree::TerminalNode* node,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal visitList(
      SubstraitPlanParser::ConstantContext* ctx,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal visitMap(
      SubstraitPlanParser::ConstantContext* ctx,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal visitStruct(
      SubstraitPlanParser::ConstantContext* ctx,
      const ::substrait::proto::Type& literalType);

  ::substrait::proto::Expression_Literal visitIntervalYear(
      SubstraitPlanParser::ConstantContext* ctx);

  ::substrait::proto::Expression_Literal visitIntervalDay(
      SubstraitPlanParser::ConstantContext* ctx);

  ::substrait::proto::Expression_Literal visitTimestamp(
      SubstraitPlanParser::ConstantContext* ctx);

  ::substrait::proto::Expression_Literal visitTimestampTz(
      SubstraitPlanParser::ConstantContext* ctx);

  ::substrait::proto::Expression_Literal visitDate(
      SubstraitPlanParser::ConstantContext* ctx);

  ::substrait::proto::Expression_Literal visitTime(
      SubstraitPlanParser::ConstantContext* ctx);

 private:
  std::string escapeText(
      const antlr4::tree::TerminalNode* node,
      const std::string& str);

  ::substrait::proto::Type typeToProto(
      const antlr4::Token* token,
      const ParameterizedType& decodedType);

  std::shared_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<SubstraitParserErrorListener> errorListener_;
};

} // namespace io::substrait::textplan
