/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/SubstraitErrorListener.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

namespace io::substrait::textplan {

class PlanPrinterVisitor : public BasePlanProtoVisitor {
 public:
  // PlanPrinterVisitor takes ownership of the provided symbol table.
  explicit PlanPrinterVisitor(const SymbolTable& symbolTable) {
    symbolTable_ = std::make_shared<SymbolTable>(symbolTable);
    errorListener_ = std::make_shared<SubstraitErrorListener>();
    currentScope_ = &SymbolInfo::kUnknown;
    functionDepth_ = 0;
  };

  [[nodiscard]] std::shared_ptr<const SymbolTable> getSymbolTable() const {
    return symbolTable_;
  };

  [[nodiscard]] std::shared_ptr<SubstraitErrorListener> getErrorListener()
      const {
    return errorListener_;
  };

  std::string printRelation(const SymbolInfo& symbol);
  std::string typeToText(const ::substrait::proto::Type& type);

 private:
  std::string lookupFieldReference(uint32_t field_reference);
  std::string lookupFunctionReference(uint32_t function_reference);

  std::any visitSelect(
      const ::substrait::proto::Expression_MaskExpression_Select& select)
      override;
  std::any visitType(const ::substrait::proto::Type& type) override;
  std::any visitStruct(
      const ::substrait::proto::Type_Struct& structure) override;
  std::any visitLiteral(
      const ::substrait::proto::Expression::Literal& literal) override;
  std::any visitFieldReference(
      const ::substrait::proto::Expression::FieldReference& ref) override;
  std::any visitScalarFunction(
      const ::substrait::proto::Expression::ScalarFunction& function) override;
  std::any visitWindowFunction(
      const ::substrait::proto::Expression::WindowFunction& function) override;
  std::any visitIfThen(
      const ::substrait::proto::Expression::IfThen& ifthen) override;
  std::any visitSwitchExpression(
      const ::substrait::proto::Expression::SwitchExpression& expression)
      override;
  std::any visitSingularOrList(
      const ::substrait::proto::Expression::SingularOrList& expression)
      override;
  std::any visitMultiOrList(
      const ::substrait::proto::Expression::MultiOrList& expression) override;
  std::any visitCast(const ::substrait::proto::Expression::Cast& cast) override;
  std::any visitSubquery(
      const ::substrait::proto::Expression_Subquery& query) override;
  std::any visitNested(
      const ::substrait::proto::Expression_Nested& structure) override;
  std::any visitEnum(const ::substrait::proto::Expression_Enum& value) override;
  std::any visitStructSelect(
      const ::substrait::proto::Expression_MaskExpression_StructSelect&
          structure) override;
  std::any visitListSelect(
      const ::substrait::proto::Expression_MaskExpression_ListSelect& select)
      override;
  std::any visitListSelectItem(
      const ::substrait::proto::
          Expression_MaskExpression_ListSelect_ListSelectItem& item) override;
  std::any visitMapSelect(
      const ::substrait::proto::Expression_MaskExpression_MapSelect& select)
      override;
  std::any visitExpressionLiteralStruct(
      const ::substrait::proto::Expression_Literal_Struct& structure) override;
  std::any visitFileOrFiles(
      const ::substrait::proto::ReadRel_LocalFiles_FileOrFiles& structure)
      override;
  std::any visitReferenceSegment(
      const ::substrait::proto::Expression_ReferenceSegment& segment) override;

  std::any visitAggregateFunction(
      const ::substrait::proto::AggregateFunction& function) override;
  std::any visitExpression(
      const ::substrait::proto::Expression& expression) override;
  std::any visitMaskExpression(
      const ::substrait::proto::Expression::MaskExpression& expression)
      override;

  std::any visitRelation(const ::substrait::proto::Rel& relation) override;

  std::any visitReadRelation(
      const ::substrait::proto::ReadRel& relation) override;
  std::any visitFilterRelation(
      const ::substrait::proto::FilterRel& relation) override;
  std::any visitFetchRelation(
      const ::substrait::proto::FetchRel& relation) override;
  std::any visitAggregateRelation(
      const ::substrait::proto::AggregateRel& relation) override;
  std::any visitSortRelation(
      const ::substrait::proto::SortRel& relation) override;
  std::any visitProjectRelation(
      const ::substrait::proto::ProjectRel& relation) override;
  std::any visitJoinRelation(
      const ::substrait::proto::JoinRel& relation) override;

  std::shared_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<SubstraitErrorListener> errorListener_;
  const SymbolInfo* currentScope_; /* not owned */
  int functionDepth_;
};

} // namespace io::substrait::textplan
