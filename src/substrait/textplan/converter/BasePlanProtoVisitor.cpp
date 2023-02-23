/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/BasePlanProtoVisitor.h"

#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

#include <google/protobuf/util/json_util.h>
#include "substrait/common/Exceptions.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait::textplan {

std::any BasePlanProtoVisitor::visitSubqueryScalar(
    const ::substrait::proto::Expression_Subquery_Scalar& query) {
  if (query.has_input()) {
    visitRelation(query.input());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSubqueryInPredicate(
    const ::substrait::proto::Expression_Subquery_InPredicate& query) {
  if (query.has_haystack()) {
    visitRelation(query.haystack());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSubquerySetPredicate(
    const ::substrait::proto::Expression_Subquery_SetPredicate& query) {
  if (query.has_tuples()) {
    visitRelation(query.tuples());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSubquerySetComparison(
    const ::substrait::proto::Expression_Subquery_SetComparison& query) {
  if (query.has_left()) {
    visitExpression(query.left());
  }
  if (query.has_right()) {
    visitRelation(query.right());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitNestedStruct(
    const ::substrait::proto::Expression_Nested_Struct& nested) {
  for (const auto& field : nested.fields()) {
    visitExpression(field);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitNestedList(
    const ::substrait::proto::Expression_Nested_List& nested) {
  for (const auto& value : nested.values()) {
    visitExpression(value);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitNestedMap(
    const ::substrait::proto::Expression_Nested_Map& nested) {
  for (const auto& kv : nested.key_values()) {
    visitNestedKeyValue(kv);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitNestedKeyValue(
    const ::substrait::proto::Expression_Nested_Map_KeyValue& kv) {
  if (kv.has_key()) {
    visitExpression(kv.key());
  }
  if (kv.has_value()) {
    visitExpression(kv.value());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitStructItem(
    const ::substrait::proto::Expression_MaskExpression_StructItem& item) {
  if (item.has_child()) {
    visitSelect(item.child());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitReferenceSegmentMapKey(
    const ::substrait::proto::Expression_ReferenceSegment_MapKey& mapkey) {
  if (mapkey.has_map_key()) {
    visitLiteral(mapkey.map_key());
  }
  if (mapkey.has_child()) {
    visitReferenceSegment(mapkey.child());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitReferenceSegmentStructField(
    const ::substrait::proto::Expression_ReferenceSegment_StructField&
        structure) {
  if (structure.has_child()) {
    visitReferenceSegment(structure.child());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitReferenceSegmentListElement(
    const ::substrait::proto::Expression_ReferenceSegment_ListElement&
        element) {
  if (element.has_child()) {
    visitReferenceSegment(element.child());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSelect(
    const ::substrait::proto::Expression_MaskExpression_Select& select) {
  switch (select.type_case()) {
    case ::substrait::proto::Expression_MaskExpression_Select::kStruct:
      return visitStructSelect(select.struct_());
    case ::substrait::proto::Expression_MaskExpression_Select::kList:
      return visitListSelect(select.list());
    case ::substrait::proto::Expression_MaskExpression_Select::kMap:
      return visitMapSelect(select.map());
    case ::substrait::proto::Expression_MaskExpression_Select::TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported maskexpression select type encountered: " +
      std::to_string(select.type_case()));
}

std::any BasePlanProtoVisitor::visitType(const ::substrait::proto::Type& type) {
  switch (type.kind_case()) {
    case ::substrait::proto::Type::kBool:
    case ::substrait::proto::Type::kI8:
    case ::substrait::proto::Type::kI16:
    case ::substrait::proto::Type::kI32:
    case ::substrait::proto::Type::kI64:
    case ::substrait::proto::Type::kFp32:
    case ::substrait::proto::Type::kFp64:
    case ::substrait::proto::Type::kString:
    case ::substrait::proto::Type::kBinary:
    case ::substrait::proto::Type::kTimestamp:
    case ::substrait::proto::Type::kDate:
    case ::substrait::proto::Type::kTime:
    case ::substrait::proto::Type::kIntervalYear:
    case ::substrait::proto::Type::kIntervalDay:
    case ::substrait::proto::Type::kTimestampTz:
    case ::substrait::proto::Type::kUuid:
    case ::substrait::proto::Type::kFixedChar:
    case ::substrait::proto::Type::kVarchar:
    case ::substrait::proto::Type::kFixedBinary:
    case ::substrait::proto::Type::kDecimal:
      return std::nullopt;
    case ::substrait::proto::Type::kStruct:
      return visitStruct(type.struct_());
    case ::substrait::proto::Type::kList:
      return visitTypeList(type.list());
    case ::substrait::proto::Type::kMap:
      return visitTypeMap(type.map());
    case ::substrait::proto::Type::kUserDefined:
      return visitTypeUserDefined(type.user_defined());
    case ::substrait::proto::Type::kUserDefinedTypeReference:
      SUBSTRAIT_UNSUPPORTED(
          "user_defined_type_reference was replaced by user_defined_type.  Please update your plan version.");
    case ::substrait::proto::Type::KIND_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported type kind encountered: " + std::to_string(type.kind_case()));
}

std::any BasePlanProtoVisitor::visitTypeUserDefined(
    const ::substrait::proto::Type_UserDefined& type) {
  for (const auto& parameter : type.type_parameters()) {
    visitTypeParameter(parameter);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitTypeParameter(
    const ::substrait::proto::Type_Parameter& type) {
  switch (type.parameter_case()) {
    case ::substrait::proto::Type_Parameter::kNull:
      return std::nullopt;
    case ::substrait::proto::Type_Parameter::kDataType:
      return visitType(type.data_type());
    case ::substrait::proto::Type_Parameter::kBoolean:
    case ::substrait::proto::Type_Parameter::kInteger:
    case ::substrait::proto::Type_Parameter::kEnum:
    case ::substrait::proto::Type_Parameter::kString:
      return std::nullopt;
    case ::substrait::proto::Type_Parameter::PARAMETER_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported type parameter encountered: " +
      std::to_string(type.parameter_case()));
}

std::any BasePlanProtoVisitor::visitMap(
    const ::substrait::proto::Expression_Literal_Map& map) {
  for (const auto& kv : map.key_values()) {
    visitMapKeyValue(kv);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitMapKeyValue(
    const ::substrait::proto::Expression_Literal_Map_KeyValue& kv) {
  if (kv.has_key()) {
    visitLiteral(kv.key());
  }
  if (kv.has_value()) {
    visitLiteral(kv.value());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitList(
    const ::substrait::proto::Expression_Literal_List& list) {
  for (const auto& value : list.values()) {
    visitLiteral(value);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitTypeList(
    const ::substrait::proto::Type_List& list) {
  if (list.has_type()) {
    visitType(list.type());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitTypeMap(
    const ::substrait::proto::Type_Map& list) {
  if (list.has_key()) {
    visitType(list.key());
  }
  if (list.has_value()) {
    visitType(list.value());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitUserDefined(
    const ::substrait::proto::Expression_Literal_UserDefined& type) {
  for (const auto& parameter : type.type_parameters()) {
    visitTypeParameter(parameter);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitFunctionArgument(
    const ::substrait::proto::FunctionArgument& argument) {
  switch (argument.arg_type_case()) {
    case ::substrait::proto::FunctionArgument::kEnum:
      return std::nullopt;
    case ::substrait::proto::FunctionArgument::kType:
      return visitType(argument.type());
    case ::substrait::proto::FunctionArgument::kValue:
      return visitExpression(argument.value());
    case ::substrait::proto::FunctionArgument::ARG_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported function argument type encountered: " +
      std::to_string(argument.arg_type_case()));
}

std::any BasePlanProtoVisitor::visitFunctionOption(
    const ::substrait::proto::FunctionOption& option) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitRecord(
    const ::substrait::proto::Expression_MultiOrList_Record& record) {
  for (const auto& field : record.fields()) {
    visitExpression(field);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitIfClause(
    const ::substrait::proto::Expression_IfThen_IfClause& ifclause) {
  if (ifclause.has_if_()) {
    visitExpression(ifclause.if_());
  }
  if (ifclause.has_then()) {
    visitExpression(ifclause.then());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitIfValue(
    const ::substrait::proto::Expression_SwitchExpression_IfValue& ifclause) {
  if (ifclause.has_if_()) {
    visitLiteral(ifclause.if_());
  }
  if (ifclause.has_then()) {
    visitExpression(ifclause.then());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitStruct(
    const ::substrait::proto::Type_Struct& structure) {
  for (const auto& t : structure.types()) {
    visitType(t);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitLiteral(
    const ::substrait::proto::Expression::Literal& literal) {
  switch (literal.literal_type_case()) {
    case ::substrait::proto::Expression_Literal::kBoolean:
    case ::substrait::proto::Expression_Literal::kI8:
    case ::substrait::proto::Expression_Literal::kI16:
    case ::substrait::proto::Expression_Literal::kI32:
    case ::substrait::proto::Expression_Literal::kI64:
    case ::substrait::proto::Expression_Literal::kFp32:
    case ::substrait::proto::Expression_Literal::kFp64:
    case ::substrait::proto::Expression_Literal::kString:
    case ::substrait::proto::Expression_Literal::kBinary:
    case ::substrait::proto::Expression_Literal::kTimestamp:
    case ::substrait::proto::Expression_Literal::kDate:
    case ::substrait::proto::Expression_Literal::kTime:
    case ::substrait::proto::Expression_Literal::kIntervalYearToMonth:
    case ::substrait::proto::Expression_Literal::kIntervalDayToSecond:
    case ::substrait::proto::Expression_Literal::kFixedChar:
    case ::substrait::proto::Expression_Literal::kVarChar:
    case ::substrait::proto::Expression_Literal::kFixedBinary:
    case ::substrait::proto::Expression_Literal::kDecimal:
      return std::nullopt;
    case ::substrait::proto::Expression_Literal::kStruct:
      return visitExpressionLiteralStruct(literal.struct_());
    case ::substrait::proto::Expression_Literal::kMap:
      return visitMap(literal.map());
    case ::substrait::proto::Expression_Literal::kTimestampTz:
    case ::substrait::proto::Expression_Literal::kUuid:
      return std::nullopt;
    case ::substrait::proto::Expression_Literal::kNull:
      return visitType(literal.null());
    case ::substrait::proto::Expression_Literal::kList:
      return visitList(literal.list());
    case ::substrait::proto::Expression_Literal::kEmptyList:
      return visitTypeList(literal.empty_list());
    case ::substrait::proto::Expression_Literal::kEmptyMap:
      return visitTypeMap(literal.empty_map());
    case ::substrait::proto::Expression_Literal::kUserDefined:
      return visitUserDefined(literal.user_defined());
    case ::substrait::proto::Expression_Literal::LITERAL_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported literal type encountered: " +
      std::to_string(literal.literal_type_case()));
}

std::any BasePlanProtoVisitor::visitScalarFunction(
    const ::substrait::proto::Expression::ScalarFunction& function) {
  for (const auto& arg : function.arguments()) {
    visitFunctionArgument(arg);
  }
  for (const auto& arg : function.options()) {
    visitFunctionOption(arg);
  }
  if (function.has_output_type()) {
    visitType(function.output_type());
  }
  for (const auto& arg : function.args()) {
    visitExpression(arg);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitWindowFunction(
    const ::substrait::proto::Expression::WindowFunction& function) {
  for (const auto& arg : function.arguments()) {
    visitFunctionArgument(arg);
  }
  for (const auto& arg : function.options()) {
    visitFunctionOption(arg);
  }
  if (function.has_output_type()) {
    visitType(function.output_type());
  }
  for (const auto& sort : function.sorts()) {
    visitSortField(sort);
  }
  for (const auto& partition : function.partitions()) {
    visitExpression(partition);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitIfThen(
    const ::substrait::proto::Expression::IfThen& ifthen) {
  for (const auto& if_ : ifthen.ifs()) {
    visitIfClause(if_);
  }
  if (ifthen.has_else_()) {
    visitExpression(ifthen.else_());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSwitchExpression(
    const ::substrait::proto::Expression::SwitchExpression& expression) {
  if (expression.has_match()) {
    visitExpression(expression.match());
  }
  for (const auto& if_ : expression.ifs()) {
    visitIfValue(if_);
  }
  if (expression.has_else_()) {
    visitExpression(expression.else_());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSingularOrList(
    const ::substrait::proto::Expression::SingularOrList& expression) {
  if (expression.has_value()) {
    visitExpression(expression.value());
  }
  for (const auto& option : expression.options()) {
    visitExpression(option);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitMultiOrList(
    const ::substrait::proto::Expression::MultiOrList& expression) {
  for (const auto& value : expression.value()) {
    visitExpression(value);
  }
  for (const auto& option : expression.options()) {
    visitRecord(option);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitCast(
    const ::substrait::proto::Expression::Cast& cast) {
  if (cast.has_input()) {
    visitExpression(cast.input());
  }
  if (cast.has_type()) {
    visitType(cast.type());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSubquery(
    const ::substrait::proto::Expression_Subquery& query) {
  switch (query.subquery_type_case()) {
    case ::substrait::proto::Expression_Subquery::kScalar:
      return visitSubqueryScalar(query.scalar());
    case ::substrait::proto::Expression_Subquery::kInPredicate:
      return visitSubqueryInPredicate(query.in_predicate());
    case ::substrait::proto::Expression_Subquery::kSetPredicate:
      return visitSubquerySetPredicate(query.set_predicate());
    case ::substrait::proto::Expression_Subquery::kSetComparison:
      return visitSubquerySetComparison(query.set_comparison());
    case ::substrait::proto::Expression_Subquery::SUBQUERY_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported subquery type encountered: " +
      std::to_string(query.subquery_type_case()));
}

std::any BasePlanProtoVisitor::visitNested(
    const ::substrait::proto::Expression_Nested& structure) {
  switch (structure.nested_type_case()) {
    case ::substrait::proto::Expression_Nested::kStruct:
      return visitNestedStruct(structure.struct_());
    case ::substrait::proto::Expression_Nested::kList:
      return visitNestedList(structure.list());
    case ::substrait::proto::Expression_Nested::kMap:
      return visitNestedMap(structure.map());
    case ::substrait::proto::Expression_Nested::NESTED_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported nested type encountered: " +
      std::to_string(structure.nested_type_case()));
}

std::any BasePlanProtoVisitor::visitEnum(
    const ::substrait::proto::Expression_Enum& value) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitStructSelect(
    const ::substrait::proto::Expression_MaskExpression_StructSelect&
        structure) {
  for (const auto& item : structure.struct_items()) {
    visitStructItem(item);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitListSelect(
    const ::substrait::proto::Expression_MaskExpression_ListSelect& select) {
  for (const auto& item : select.selection()) {
    visitListSelectItem(item);
  }
  if (select.has_child()) {
    visitSelect(select.child());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitListSelectItem(
    const ::substrait::proto::
        Expression_MaskExpression_ListSelect_ListSelectItem& item) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitMapSelect(
    const ::substrait::proto::Expression_MaskExpression_MapSelect& select) {
  if (select.has_child()) {
    visitSelect(select.child());
  }
  switch (select.select_case()) {
    case ::substrait::proto::Expression_MaskExpression_MapSelect::kKey:
      return std::nullopt;
    case ::substrait::proto::Expression_MaskExpression_MapSelect::kExpression:
      return std::nullopt;
    case ::substrait::proto::Expression_MaskExpression_MapSelect::
        SELECT_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported map select type encountered: " +
      std::to_string(select.select_case()));
}

std::any BasePlanProtoVisitor::visitExpressionLiteralStruct(
    const ::substrait::proto::Expression_Literal_Struct& structure) {
  for (const auto& literal : structure.fields()) {
    visitLiteral(literal);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitFileOrFiles(
    const ::substrait::proto::ReadRel_LocalFiles_FileOrFiles& structure) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitAggregateFunction(
    const ::substrait::proto::AggregateFunction& structure) {
  for (const auto& arg : structure.arguments()) {
    visitFunctionArgument(arg);
  }
  for (const auto& option : structure.options()) {
    visitFunctionOption(option);
  }
  if (structure.has_output_type()) {
    visitType(structure.output_type());
  }
  for (const auto& sort : structure.sorts()) {
    visitSortField(sort);
  }
  for (const auto& arg : structure.args()) {
    visitExpression(arg);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitReferenceSegment(
    const ::substrait::proto::Expression_ReferenceSegment& segment) {
  switch (segment.reference_type_case()) {
    case ::substrait::proto::Expression_ReferenceSegment::kMapKey:
      return visitReferenceSegmentMapKey(segment.map_key());
    case ::substrait::proto::Expression_ReferenceSegment::kStructField:
      return visitReferenceSegmentStructField(segment.struct_field());
    case ::substrait::proto::Expression_ReferenceSegment::kListElement:
      return visitReferenceSegmentListElement(segment.list_element());
    case ::substrait::proto::Expression_ReferenceSegment::
        REFERENCE_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported reference segment type encountered: " +
      std::to_string(segment.reference_type_case()));
}

std::any BasePlanProtoVisitor::visitRelationCommon(
    const ::substrait::proto::RelCommon& common) {
  if (common.has_advanced_extension()) {
    visitAdvancedExtension(common.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitNamedStruct(
    const ::substrait::proto::NamedStruct& named) {
  return visitStruct(named.struct_());
}

std::any BasePlanProtoVisitor::visitExpression(
    const ::substrait::proto::Expression& expression) {
  switch (expression.rex_type_case()) {
    case ::substrait::proto::Expression::RexTypeCase::kLiteral:
      return visitLiteral(expression.literal());
    case ::substrait::proto::Expression::RexTypeCase::kSelection:
      return visitFieldReference(expression.selection());
    case ::substrait::proto::Expression::RexTypeCase::kScalarFunction:
      return visitScalarFunction(expression.scalar_function());
    case ::substrait::proto::Expression::RexTypeCase::kWindowFunction:
      return visitWindowFunction(expression.window_function());
    case ::substrait::proto::Expression::RexTypeCase::kIfThen:
      return visitIfThen(expression.if_then());
    case ::substrait::proto::Expression::RexTypeCase::kSwitchExpression:
      return visitSwitchExpression(expression.switch_expression());
    case ::substrait::proto::Expression::RexTypeCase::kSingularOrList:
      return visitSingularOrList(expression.singular_or_list());
    case ::substrait::proto::Expression::RexTypeCase::kMultiOrList:
      return visitMultiOrList(expression.multi_or_list());
    case ::substrait::proto::Expression::RexTypeCase::kCast:
      return visitCast(expression.cast());
    case ::substrait::proto::Expression::RexTypeCase::kSubquery:
      return visitSubquery(expression.subquery());
    case ::substrait::proto::Expression::RexTypeCase::kNested:
      return visitNested(expression.nested());
    case ::substrait::proto::Expression::RexTypeCase::kEnum:
      return visitEnum(expression.enum_());
    case ::substrait::proto::Expression::RexTypeCase::REX_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported expression type encountered: " +
      std::to_string(expression.rex_type_case()));
}

std::any BasePlanProtoVisitor::visitMaskExpression(
    const ::substrait::proto::Expression::MaskExpression& expression) {
  if (expression.has_select()) {
    visitStructSelect(expression.select());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitVirtualTable(
    const ::substrait::proto::ReadRel_VirtualTable& table) {
  for (const auto& value : table.values()) {
    visitExpressionLiteralStruct(value);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitLocalFiles(
    const ::substrait::proto::ReadRel_LocalFiles& local) {
  for (const auto& item : local.items()) {
    visitFileOrFiles(item);
  }
  if (local.has_advanced_extension()) {
    visitAdvancedExtension(local.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitNamedTable(
    const ::substrait::proto::ReadRel_NamedTable& table) {
  if (table.has_advanced_extension()) {
    visitAdvancedExtension(table.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitExtensionTable(
    const ::substrait::proto::ReadRel_ExtensionTable& table) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitGrouping(
    const ::substrait::proto::AggregateRel_Grouping& grouping) {
  for (const auto& expr : grouping.grouping_expressions()) {
    visitExpression(expr);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitMeasure(
    const ::substrait::proto::AggregateRel_Measure& measure) {
  if (measure.has_measure()) {
    visitAggregateFunction(measure.measure());
  }
  if (measure.has_filter()) {
    visitExpression(measure.filter());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSortField(
    const ::substrait::proto::SortField& sort) {
  if (sort.has_expr()) {
    return visitExpression(sort.expr());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitFieldReference(
    const ::substrait::proto::Expression::FieldReference& ref) {
  if (ref.has_direct_reference()) {
    visitReferenceSegment(ref.direct_reference());
  }
  if (ref.has_masked_reference()) {
    visitMaskExpression(ref.masked_reference());
  }
  if (ref.has_expression()) {
    visitExpression(ref.expression());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitReadRelation(
    const ::substrait::proto::ReadRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_base_schema()) {
    visitNamedStruct(relation.base_schema());
  }
  if (relation.has_filter()) {
    visitExpression(relation.filter());
  }
  if (relation.has_best_effort_filter()) {
    visitExpression(relation.best_effort_filter());
  }
  if (relation.has_projection()) {
    visitMaskExpression(relation.projection());
  }
  visitAdvancedExtension(relation.advanced_extension());
  switch (relation.read_type_case()) {
    case ::substrait::proto::ReadRel::ReadTypeCase::kVirtualTable:
      return visitVirtualTable(relation.virtual_table());
    case ::substrait::proto::ReadRel::ReadTypeCase::kLocalFiles:
      return visitLocalFiles(relation.local_files());
    case ::substrait::proto::ReadRel::ReadTypeCase::kNamedTable:
      return visitNamedTable(relation.named_table());
    case ::substrait::proto::ReadRel::ReadTypeCase::kExtensionTable:
      return visitExtensionTable(relation.extension_table());
    case ::substrait::proto::ReadRel::ReadTypeCase::READ_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported read type encountered: " +
      std::to_string(relation.read_type_case()));
}

std::any BasePlanProtoVisitor::visitFilterRelation(
    const ::substrait::proto::FilterRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_condition()) {
    visitExpression(relation.condition());
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  if (relation.has_input()) {
    visitRelation(relation.input());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitFetchRelation(
    const ::substrait::proto::FetchRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  if (relation.has_input()) {
    visitRelation(relation.input());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitAggregateRelation(
    const ::substrait::proto::AggregateRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  for (const auto& grouping : relation.groupings()) {
    visitGrouping(grouping);
  }
  for (const auto& measure : relation.measures()) {
    visitMeasure(measure);
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  if (relation.has_input()) {
    visitRelation(relation.input());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSortRelation(
    const ::substrait::proto::SortRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  for (const auto& sort : relation.sorts()) {
    visitSortField(sort);
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  if (relation.has_input()) {
    visitRelation(relation.input());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitJoinRelation(
    const ::substrait::proto::JoinRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_left()) {
    visitRelation(relation.left());
  }
  if (relation.has_right()) {
    visitRelation(relation.right());
  }
  if (relation.has_expression()) {
    visitExpression(relation.expression());
  }
  if (relation.has_post_join_filter()) {
    visitExpression(relation.post_join_filter());
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitProjectRelation(
    const ::substrait::proto::ProjectRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_input()) {
    visitRelation(relation.input());
  }
  for (const auto& expr : relation.expressions()) {
    visitExpression(expr);
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitSetRelation(
    const ::substrait::proto::SetRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  for (const auto& input : relation.inputs()) {
    visitRelation(input);
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitExtensionSingleRelation(
    const ::substrait::proto::ExtensionSingleRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_input()) {
    visitRelation(relation.input());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitExtensionMultiRelation(
    const ::substrait::proto::ExtensionMultiRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  for (const auto& input : relation.inputs()) {
    visitRelation(input);
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitExtensionLeafRelation(
    const ::substrait::proto::ExtensionLeafRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitCrossRelation(
    const ::substrait::proto::CrossRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_left()) {
    visitRelation(relation.left());
  }
  if (relation.has_right()) {
    visitRelation(relation.right());
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitHashJoinRelation(
    const ::substrait::proto::HashJoinRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_left()) {
    visitRelation(relation.left());
  }
  if (relation.has_right()) {
    visitRelation(relation.right());
  }
  for (const auto& key : relation.left_keys()) {
    visitFieldReference(key);
  }
  for (const auto& key : relation.right_keys()) {
    visitFieldReference(key);
  }
  if (relation.has_post_join_filter()) {
    visitExpression(relation.post_join_filter());
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitMergeJoinRelation(
    const ::substrait::proto::MergeJoinRel& relation) {
  if (relation.has_common()) {
    visitRelationCommon(relation.common());
  }
  if (relation.has_left()) {
    visitRelation(relation.left());
  }
  if (relation.has_right()) {
    visitRelation(relation.right());
  }
  for (const auto& key : relation.left_keys()) {
    visitFieldReference(key);
  }
  for (const auto& key : relation.right_keys()) {
    visitFieldReference(key);
  }
  if (relation.has_post_join_filter()) {
    visitExpression(relation.post_join_filter());
  }
  if (relation.has_advanced_extension()) {
    visitAdvancedExtension(relation.advanced_extension());
  };
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  switch (relation.rel_type_case()) {
    case ::substrait::proto::Rel::RelTypeCase::kRead:
      return visitReadRelation(relation.read());
    case ::substrait::proto::Rel::RelTypeCase::kFilter:
      return visitFilterRelation(relation.filter());
    case ::substrait::proto::Rel::RelTypeCase::kFetch:
      return visitFetchRelation(relation.fetch());
    case ::substrait::proto::Rel::RelTypeCase::kAggregate:
      return visitAggregateRelation(relation.aggregate());
    case ::substrait::proto::Rel::RelTypeCase::kSort:
      return visitSortRelation(relation.sort());
    case ::substrait::proto::Rel::RelTypeCase::kJoin:
      return visitJoinRelation(relation.join());
    case ::substrait::proto::Rel::RelTypeCase::kProject:
      return visitProjectRelation(relation.project());
    case ::substrait::proto::Rel::RelTypeCase::kSet:
      return visitSetRelation(relation.set());
    case ::substrait::proto::Rel::RelTypeCase::kExtensionSingle:
      return visitExtensionSingleRelation(relation.extension_single());
    case ::substrait::proto::Rel::RelTypeCase::kExtensionMulti:
      return visitExtensionMultiRelation(relation.extension_multi());
    case ::substrait::proto::Rel::RelTypeCase::kExtensionLeaf:
      return visitExtensionLeafRelation(relation.extension_leaf());
    case ::substrait::proto::Rel::RelTypeCase::kCross:
      return visitCrossRelation(relation.cross());
    case ::substrait::proto::Rel::RelTypeCase::kHashJoin:
      return visitHashJoinRelation(relation.hash_join());
    case ::substrait::proto::Rel::RelTypeCase::kMergeJoin:
      return visitMergeJoinRelation(relation.merge_join());
    case ::substrait::proto::Rel::REL_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported relation type encountered: " +
      std::to_string(relation.rel_type_case()));
}

std::any BasePlanProtoVisitor::visitRelationRoot(
    const ::substrait::proto::RelRoot& relation) {
  return visitRelation(relation.input());
}

std::any BasePlanProtoVisitor::visitExtensionUri(
    const ::substrait::proto::extensions::SimpleExtensionURI& uri) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitExtension(
    const ::substrait::proto::extensions::SimpleExtensionDeclaration&
        extension) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitPlanRelation(
    const ::substrait::proto::PlanRel& relation) {
  switch (relation.rel_type_case()) {
    case ::substrait::proto::PlanRel::RelTypeCase::kRel:
      return visitRelation(relation.rel());
    case ::substrait::proto::PlanRel::RelTypeCase::kRoot:
      return visitRelationRoot(relation.root());
    case ::substrait::proto::PlanRel::RelTypeCase::REL_TYPE_NOT_SET:
      break;
  }
  SUBSTRAIT_UNSUPPORTED(
      "Unsupported plan relation type encountered: " +
      std::to_string(relation.rel_type_case()));
}

std::any BasePlanProtoVisitor::visitAdvancedExtension(
    const ::substrait::proto::extensions::AdvancedExtension& extension) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitExpectedTypeUrl(const std::string& url) {
  return std::nullopt;
}

std::any BasePlanProtoVisitor::visitPlan() {
  for (const auto& uri : plan_.extension_uris()) {
    visitExtensionUri(uri);
  }
  for (const auto& extension : plan_.extensions()) {
    visitExtension(extension);
  }
  for (const auto& relation : plan_.relations()) {
    visitPlanRelation(relation);
  }
  if (plan_.has_advanced_extensions()) {
    visitAdvancedExtension(plan_.advanced_extensions());
  }
  for (const auto& url : plan_.expected_type_urls()) {
    visitExpectedTypeUrl(url);
  }
  return std::nullopt;
}

} // namespace io::substrait::textplan