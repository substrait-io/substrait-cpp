/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <any>

#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait::textplan {

// BasePlanProtoVisitor provides a visitor that will visit all the major
// messages within a Plan protobuffer object.  Subclass the visitor to add your
// own functionality.
class BasePlanProtoVisitor {
 public:
  BasePlanProtoVisitor() = default;

  // visit() begins the traversal of the entire plan.
  virtual void visit(const ::substrait::proto::Plan& plan) {
    visitPlan(plan);
  }

 protected:
  virtual std::any visitSubqueryScalar(
      const ::substrait::proto::Expression_Subquery_Scalar& query);
  virtual std::any visitSubqueryInPredicate(
      const ::substrait::proto::Expression_Subquery_InPredicate& query);
  virtual std::any visitSubquerySetPredicate(
      const ::substrait::proto::Expression_Subquery_SetPredicate& query);
  virtual std::any visitSubquerySetComparison(
      const ::substrait::proto::Expression_Subquery_SetComparison& query);
  virtual std::any visitNestedStruct(
      const ::substrait::proto::Expression_Nested_Struct& nested);
  virtual std::any visitNestedList(
      const ::substrait::proto::Expression_Nested_List& nested);
  virtual std::any visitNestedMap(
      const ::substrait::proto::Expression_Nested_Map& nested);
  virtual std::any visitNestedKeyValue(
      const ::substrait::proto::Expression_Nested_Map_KeyValue& kv);
  virtual std::any visitStructItem(
      const ::substrait::proto::Expression_MaskExpression_StructItem& item);
  virtual std::any visitReferenceSegmentMapKey(
      const ::substrait::proto::Expression_ReferenceSegment_MapKey& mapkey);
  virtual std::any visitReferenceSegmentStructField(
      const ::substrait::proto::Expression_ReferenceSegment_StructField&
          structure);
  virtual std::any visitReferenceSegmentListElement(
      const ::substrait::proto::Expression_ReferenceSegment_ListElement&
          element);
  virtual std::any visitSelect(
      const ::substrait::proto::Expression_MaskExpression_Select& select);

  virtual std::any visitType(const ::substrait::proto::Type& type);
  virtual std::any visitMap(
      const ::substrait::proto::Expression_Literal_Map& map);
  virtual std::any visitMapKeyValue(
      const ::substrait::proto::Expression_Literal_Map_KeyValue& kv);
  virtual std::any visitList(
      const ::substrait::proto::Expression_Literal_List& list);
  virtual std::any visitTypeList(const ::substrait::proto::Type_List& list);
  virtual std::any visitTypeMap(const ::substrait::proto::Type_Map& list);
  virtual std::any visitTypeUserDefined(
      const ::substrait::proto::Type_UserDefined& type);
  virtual std::any visitTypeParameter(
      const ::substrait::proto::Type_Parameter& parameter);
  virtual std::any visitUserDefined(
      const ::substrait::proto::Expression_Literal_UserDefined& list);
  virtual std::any visitFunctionArgument(
      const ::substrait::proto::FunctionArgument& argument);
  virtual std::any visitFunctionOption(
      const ::substrait::proto::FunctionOption& option);
  virtual std::any visitIfClause(
      const ::substrait::proto::Expression_IfThen_IfClause& ifclause);
  virtual std::any visitIfValue(
      const ::substrait::proto::Expression_SwitchExpression_IfValue& ifclause);
  virtual std::any visitRecord(
      const ::substrait::proto::Expression_MultiOrList_Record& record);

  virtual std::any visitStruct(
      const ::substrait::proto::Type_Struct& structure);
  virtual std::any visitLiteral(
      const ::substrait::proto::Expression::Literal& literal);
  virtual std::any visitScalarFunction(
      const ::substrait::proto::Expression::ScalarFunction& function);
  virtual std::any visitWindowFunction(
      const ::substrait::proto::Expression::WindowFunction& function);
  virtual std::any visitWindowRelFunction(
      const ::substrait::proto::ConsistentPartitionWindowRel::WindowRelFunction&
          function);
  virtual std::any visitIfThen(
      const ::substrait::proto::Expression::IfThen& ifthen);
  virtual std::any visitSwitchExpression(
      const ::substrait::proto::Expression::SwitchExpression& expression);
  virtual std::any visitSingularOrList(
      const ::substrait::proto::Expression::SingularOrList& expression);
  virtual std::any visitMultiOrList(
      const ::substrait::proto::Expression::MultiOrList& expression);
  virtual std::any visitCast(const ::substrait::proto::Expression::Cast& cast);
  virtual std::any visitSubquery(
      const ::substrait::proto::Expression_Subquery& query);
  virtual std::any visitNested(
      const ::substrait::proto::Expression_Nested& structure);
  virtual std::any visitEnum(const ::substrait::proto::Expression_Enum& value);
  virtual std::any visitStructSelect(
      const ::substrait::proto::Expression_MaskExpression_StructSelect&
          structure);
  virtual std::any visitListSelect(
      const ::substrait::proto::Expression_MaskExpression_ListSelect& select);
  virtual std::any visitListSelectItem(
      const ::substrait::proto::
          Expression_MaskExpression_ListSelect_ListSelectItem& item);
  virtual std::any visitMapSelect(
      const ::substrait::proto::Expression_MaskExpression_MapSelect& select);
  virtual std::any visitExpressionLiteralStruct(
      const ::substrait::proto::Expression_Literal_Struct& structure);
  virtual std::any visitFileOrFiles(
      const ::substrait::proto::ReadRel_LocalFiles_FileOrFiles& structure);
  virtual std::any visitAggregateFunction(
      const ::substrait::proto::AggregateFunction& function);
  virtual std::any visitReferenceSegment(
      const ::substrait::proto::Expression_ReferenceSegment& segment);

  virtual std::any visitRelationCommon(
      const ::substrait::proto::RelCommon& common);
  virtual std::any visitNamedStruct(
      const ::substrait::proto::NamedStruct& named);
  virtual std::any visitExpression(
      const ::substrait::proto::Expression& expression);
  virtual std::any visitMaskExpression(
      const ::substrait::proto::Expression::MaskExpression& expression);
  virtual std::any visitVirtualTable(
      const ::substrait::proto::ReadRel_VirtualTable& table);
  virtual std::any visitLocalFiles(
      const ::substrait::proto::ReadRel_LocalFiles& local);
  virtual std::any visitNamedTable(
      const ::substrait::proto::ReadRel_NamedTable& table);
  virtual std::any visitExtensionTable(
      const ::substrait::proto::ReadRel_ExtensionTable& table);
  virtual std::any visitGrouping(
      const ::substrait::proto::AggregateRel_Grouping& grouping);
  virtual std::any visitMeasure(
      const ::substrait::proto::AggregateRel_Measure& measure);
  virtual std::any visitSortField(const ::substrait::proto::SortField& sort);
  virtual std::any visitFieldReference(
      const ::substrait::proto::Expression::FieldReference& ref);
  virtual std::any visitExpandField(
      const ::substrait::proto::ExpandRel::ExpandField& field);

  virtual std::any visitReadRelation(
      const ::substrait::proto::ReadRel& relation);
  virtual std::any visitFilterRelation(
      const ::substrait::proto::FilterRel& relation);
  virtual std::any visitFetchRelation(
      const ::substrait::proto::FetchRel& relation);
  virtual std::any visitAggregateRelation(
      const ::substrait::proto::AggregateRel& relation);
  virtual std::any visitSortRelation(
      const ::substrait::proto::SortRel& relation);
  virtual std::any visitJoinRelation(
      const ::substrait::proto::JoinRel& relation);
  virtual std::any visitProjectRelation(
      const ::substrait::proto::ProjectRel& relation);
  virtual std::any visitSetRelation(const ::substrait::proto::SetRel& relation);
  virtual std::any visitExtensionSingleRelation(
      const ::substrait::proto::ExtensionSingleRel& relation);
  virtual std::any visitExtensionMultiRelation(
      const ::substrait::proto::ExtensionMultiRel& relation);
  virtual std::any visitExtensionLeafRelation(
      const ::substrait::proto::ExtensionLeafRel& relation);
  virtual std::any visitCrossRelation(
      const ::substrait::proto::CrossRel& relation);

  virtual std::any visitReferenceRelation(
      const ::substrait::proto::ReferenceRel& relation);
  virtual std::any visitWriteRelation(
      const ::substrait::proto::WriteRel& relation);
  virtual std::any visitDdlRelation(const ::substrait::proto::DdlRel& relation);

  virtual std::any visitHashJoinRelation(
      const ::substrait::proto::HashJoinRel& relation);
  virtual std::any visitMergeJoinRelation(
      const ::substrait::proto::MergeJoinRel& relation);
  virtual std::any visitNestedLoopJoinRelation(
      const ::substrait::proto::NestedLoopJoinRel& relation);
  virtual std::any visitWindowRelation(
      const ::substrait::proto::ConsistentPartitionWindowRel& relation);
  virtual std::any visitExchangeRelation(
      const ::substrait::proto::ExchangeRel& relation);
  virtual std::any visitExpandRelation(
      const ::substrait::proto::ExpandRel& relation);

  virtual std::any visitRelation(const ::substrait::proto::Rel& relation);
  virtual std::any visitRelationRoot(
      const ::substrait::proto::RelRoot& relation);

  virtual std::any visitExtensionUri(
      const ::substrait::proto::extensions::SimpleExtensionURI& uri);
  virtual std::any visitExtension(
      const ::substrait::proto::extensions::SimpleExtensionDeclaration&
          extension);
  virtual std::any visitPlanRelation(
      const ::substrait::proto::PlanRel& relation);
  virtual std::any visitAdvancedExtension(
      const ::substrait::proto::extensions::AdvancedExtension& extension);
  virtual std::any visitExpectedTypeUrl(const std::string& url);

  virtual std::any visitPlan(const ::substrait::proto::Plan& plan);
};

} // namespace io::substrait::textplan
