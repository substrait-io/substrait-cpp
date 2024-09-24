/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/PlanPrinterVisitor.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sstream>
#include <string>

#include "date/date.h"
#include "substrait/expression/DecimalLiteral.h"
#include "substrait/proto/ProtoUtils.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Finally.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"

// The visitor should try and be tolerant of older plans.  This
// requires calling deprecated APIs.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma gcc diagnostic push
#pragma gcc diagnostic ignored "-Wdeprecated-declarations"

namespace io::substrait::textplan {

namespace {

std::string stringEscape(std::string_view str) {
  // TODO - Implement all of the escapes defined in the document.
  std::stringstream result;
  for (auto c : str) {
    if (c == '"') {
      result << '\\';
    }
    result << c;
  }
  return result.str();
}

std::string invocationToString(
    ::substrait::proto::AggregateFunction_AggregationInvocation invocation) {
  switch (invocation) {
    case ::substrait::proto::
        AggregateFunction_AggregationInvocation_AGGREGATION_INVOCATION_ALL:
      return "all";
    case ::substrait::proto::
        AggregateFunction_AggregationInvocation_AGGREGATION_INVOCATION_DISTINCT:
      return "distinct";
    case ::substrait::proto::
        AggregateFunction_AggregationInvocation_AGGREGATION_INVOCATION_UNSPECIFIED:
      return "unspecified";
    case ::substrait::proto::
        AggregateFunction_AggregationInvocation_AggregateFunction_AggregationInvocation_INT_MIN_SENTINEL_DO_NOT_USE_:
    case ::substrait::proto::
        AggregateFunction_AggregationInvocation_AggregateFunction_AggregationInvocation_INT_MAX_SENTINEL_DO_NOT_USE_:
      break;
  }
  // We shouldn't reach here but return something to make the compiler happy.
  return "unspecified";
}

std::string visitEnumArgument(const std::string& str) {
  std::stringstream text;
  text << str << "_enum";
  return text.str();
}

bool isAggregate(const SymbolInfo* symbol) {
  if (const auto typeCase =
          ANY_CAST_IF(::substrait::proto::Rel::RelTypeCase, symbol->subtype)) {
    if (typeCase == ::substrait::proto::Rel::kAggregate) {
      return true;
    }
  }
  if (const auto typeCase = ANY_CAST_IF(RelationType, symbol->subtype)) {
    return (typeCase == RelationType::kAggregate);
  }
  return false;
}

} // namespace

std::string PlanPrinterVisitor::printRelation(const SymbolInfo& symbol) {
  std::stringstream text;
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol.blob);
  text << ::substrait::proto::relTypeCaseName(
              relationData->relation.rel_type_case())
       << " relation " << symbol.name << " {\n";
  if (relationData->source != nullptr) {
    text << "  source " << relationData->source->name << ";\n";
  }
  if (relationData->schema != nullptr) {
    text << "  base_schema " << relationData->schema->name << ";\n";
  }

  auto result = this->visitRelation(relationData->relation);
  if (result.type() != typeid(std::string)) {
    errorListener_->addError(
        "ERROR:  Relation subtype " +
        ::substrait::proto::relTypeCaseName(
            (relationData->relation.rel_type_case())) +
        " not supported!");
    return "";
  }
  text << ANY_CAST(std::string, result);
  text << "}\n";

  return text.str();
}

std::string PlanPrinterVisitor::typeToText(
    const ::substrait::proto::Type& type) {
  return ANY_CAST(std::string, visitType(type));
}

Location getParentQueryLocation(const SymbolInfo* scope) {
  if (scope->parentQueryLocation != Location::kUnknownLocation) {
    return scope->parentQueryLocation;
  }
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, scope->blob);
  auto nextPipeline = relationData->pipelineStart;
  if (nextPipeline != nullptr) {
    if (nextPipeline->parentQueryLocation != Location::kUnknownLocation) {
      return nextPipeline->parentQueryLocation;
    }
  }
  return Location::kUnknownLocation;
}

// TODO -- Refactor this to return the symbol for later display decisions.
std::string PlanPrinterVisitor::lookupFieldReference(
    uint32_t fieldReference,
    const SymbolInfo* currentScope,
    uint32_t stepsOut,
    bool needFullyQualified) {
  if (currentScope == nullptr || *currentScope_ == SymbolInfo::kUnknown) {
    errorListener_->addError(
        "Field number " + std::to_string(fieldReference) +
        " mysteriously requested outside of a relation.");
    return "field#" + std::to_string(fieldReference);
  }
  auto actualScope = currentScope;
  if (stepsOut > 0) {
    for (auto stepsLeft = stepsOut; stepsLeft > 0; stepsLeft--) {
      auto actualParentQueryLocation = getParentQueryLocation(actualScope);
      if (actualParentQueryLocation == Location::kUnknownLocation) {
        errorListener_->addError(
            "Requested steps out of " + std::to_string(stepsOut) +
            " but not within subquery depth that high.");
        return "field#" + std::to_string(fieldReference);
      }
      actualScope = symbolTable_->lookupSymbolByLocationAndType(
          actualParentQueryLocation, SymbolType::kRelation);
      if (actualScope == nullptr) {
        errorListener_->addError(
            "Internal error: Missing previously encountered parent query symbol.");
        return "field#" + std::to_string(fieldReference);
      }
    }
  }
  auto relationData =
      ANY_CAST(std::shared_ptr<RelationData>, actualScope->blob);
  auto fieldReferencesSize = relationData->fieldReferences.size();
  const SymbolInfo* symbol{nullptr};
  if (fieldReference < fieldReferencesSize) {
    symbol = relationData->fieldReferences[fieldReference];
  } else if (
      fieldReference <
      fieldReferencesSize + relationData->generatedFieldReferences.size()) {
    symbol =
        relationData
            ->generatedFieldReferences[fieldReference - fieldReferencesSize];
  } else {
    errorListener_->addError(
        "Encountered field reference out of range: " +
        std::to_string(fieldReference));
    return "field#" + std::to_string(fieldReference);
  }
  if (!symbol->alias.empty()) {
    return symbol->alias;
  } else if (needFullyQualified && symbol->schema != nullptr) {
    return symbol->schema->name + "." + symbol->name;
  }
  return symbol->name;
}

std::string PlanPrinterVisitor::lookupFieldReferenceForEmit(
    uint32_t fieldReference,
    const SymbolInfo* currentScope,
    uint32_t stepsOut,
    bool needFullyQualified) {
  if (currentScope == nullptr || *currentScope_ == SymbolInfo::kUnknown) {
    errorListener_->addError(
        "Field number " + std::to_string(fieldReference) +
        " mysteriously requested outside of a relation.");
    return "field#" + std::to_string(fieldReference);
  }
  auto actualScope = currentScope;
  if (stepsOut > 0) {
    for (auto stepsLeft = stepsOut; stepsLeft > 0; stepsLeft--) {
      auto actualParentQueryLocation = getParentQueryLocation(actualScope);
      if (actualParentQueryLocation == Location::kUnknownLocation) {
        errorListener_->addError(
            "Requested steps out of " + std::to_string(stepsOut) +
            " but not within subquery depth that high.");
        return "field#" + std::to_string(fieldReference);
      }
      actualScope = symbolTable_->lookupSymbolByLocationAndType(
          actualParentQueryLocation, SymbolType::kRelation);
      if (actualScope == nullptr) {
        errorListener_->addError(
            "Internal error: Missing previously encountered parent query symbol.");
        return "field#" + std::to_string(fieldReference);
      }
    }
  }
  auto relationData =
      ANY_CAST(std::shared_ptr<RelationData>, actualScope->blob);
  const SymbolInfo* symbol{nullptr};
  if (isAggregate(currentScope)) {
    if (fieldReference < relationData->generatedFieldReferences.size()) {
      symbol = relationData->generatedFieldReferences[fieldReference];
    }
  } else {
    auto size = relationData->fieldReferences.size();
    if (fieldReference < size) {
      symbol = relationData->fieldReferences[fieldReference];
    } else if (
        fieldReference < size + relationData->generatedFieldReferences.size()) {
      symbol = relationData->generatedFieldReferences[fieldReference - size];
    }
  }
  if (symbol == nullptr) {
    errorListener_->addError(
        "Encountered field reference out of range: " +
        std::to_string(fieldReference));
    return "field#" + std::to_string(fieldReference);
  }
  if (!symbol->alias.empty()) {
    return symbol->alias;
  } else if (needFullyQualified && symbol->schema != nullptr) {
    return symbol->schema->name + "." + symbol->name;
  }
  return symbol->name;
}

std::string PlanPrinterVisitor::lookupFunctionReference(
    uint32_t function_reference) {
  for (const auto& symbol : symbolTable_->getSymbols()) {
    if (symbol->type != SymbolType::kFunction) {
      continue;
    }
    auto functionData = ANY_CAST(std::shared_ptr<FunctionData>, symbol->blob);
    if (functionData->anchor == function_reference) {
      return symbol->name;
    }
  }
  errorListener_->addError(
      "Reference of undefined function " + std::to_string(function_reference) +
      " encountered.");
  return "functionref#" + std::to_string(function_reference);
}

std::any PlanPrinterVisitor::visitSubqueryScalar(
    const ::substrait::proto::Expression_Subquery_Scalar& query) {
  std::stringstream result;
  result << "SUBQUERY ";
  if (query.has_input()) {
    const SymbolInfo* symbol = symbolTable_->lookupSymbolByParentQueryAndType(
        currentScope_->sourceLocation,
        ++currentScopeIndex_,
        SymbolType::kRelation);
    if (symbol != nullptr) {
      result << symbol->name;
    } else {
      errorListener_->addError(
          "Internal error -- could not find subquery symbol.");
    }
  } else {
    errorListener_->addError(
        "No subquery input provided to scalar subquery operation.");
  }
  return result.str();
}

std::any PlanPrinterVisitor::visitSubqueryInPredicate(
    const ::substrait::proto::Expression_Subquery_InPredicate& query) {
  std::stringstream result;
  result << "(";
  bool hadPreviousNeedle = false;
  for (const auto& needle : query.needles()) {
    if (hadPreviousNeedle) {
      result << ", ";
    }
    result << ANY_CAST(std::string, visitExpression(needle));
    hadPreviousNeedle = true;
  }
  result << ") IN SUBQUERY ";
  if (query.has_haystack()) {
    const SymbolInfo* symbol = symbolTable_->lookupSymbolByParentQueryAndType(
        currentScope_->sourceLocation,
        ++currentScopeIndex_,
        SymbolType::kRelation);
    if (symbol != nullptr) {
      result << symbol->name;
    } else {
      errorListener_->addError(
          "Internal error -- could not find subquery symbol.");
    }
  } else {
    errorListener_->addError("No haystack defined for in predicate subquery.");
  }
  return result.str();
}

std::any PlanPrinterVisitor::visitSubquerySetPredicate(
    const ::substrait::proto::Expression_Subquery_SetPredicate& query) {
  std::stringstream result;
  switch (query.predicate_op()) {
    case ::substrait::proto::
        Expression_Subquery_SetPredicate_PredicateOp_PREDICATE_OP_EXISTS:
      result << "EXISTS IN ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetPredicate_PredicateOp_PREDICATE_OP_UNIQUE:
      result << "UNIQUE IN ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetPredicate_PredicateOp_PREDICATE_OP_UNSPECIFIED:
    case ::substrait::proto::
        Expression_Subquery_SetPredicate_PredicateOp_Expression_Subquery_SetPredicate_PredicateOp_INT_MIN_SENTINEL_DO_NOT_USE_:
    case ::substrait::proto::
        Expression_Subquery_SetPredicate_PredicateOp_Expression_Subquery_SetPredicate_PredicateOp_INT_MAX_SENTINEL_DO_NOT_USE_:
      errorListener_->addError("Did not recognize the subquery predicate.");
      return std::string("UNSUPPORTED SUBQUERY");
  }
  result << "SUBQUERY ";
  if (query.has_tuples()) {
    const SymbolInfo* symbol = symbolTable_->lookupSymbolByParentQueryAndType(
        currentScope_->sourceLocation,
        ++currentScopeIndex_,
        SymbolType::kRelation);
    if (symbol != nullptr) {
      result << symbol->name;
    } else {
      errorListener_->addError(
          "Internal error -- could not find subquery symbol.");
    }
  } else {
    errorListener_->addError(
        "No subquery defined for set predicate operation.");
  }
  return result.str();
}

std::any PlanPrinterVisitor::visitSubquerySetComparison(
    const ::substrait::proto::Expression_Subquery_SetComparison& query) {
  std::stringstream result;
  if (query.has_left()) {
    result << ANY_CAST(std::string, visitExpression(query.left())) << " ";
  } else {
    errorListener_->addError(
        "No expression defined for set comparison operation.");
  }
  switch (query.comparison_op()) {
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_COMPARISON_OP_LE:
      result << "LE ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_COMPARISON_OP_GE:
      result << "GE ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_COMPARISON_OP_EQ:
      result << "EQ ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_COMPARISON_OP_NE:
      result << "NE ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_COMPARISON_OP_LT:
      result << "LT ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_COMPARISON_OP_GT:
      result << "GT ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_COMPARISON_OP_UNSPECIFIED:
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_Expression_Subquery_SetComparison_ComparisonOp_INT_MIN_SENTINEL_DO_NOT_USE_:
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ComparisonOp_Expression_Subquery_SetComparison_ComparisonOp_INT_MAX_SENTINEL_DO_NOT_USE_:
      errorListener_->addError("Did not recognize the subquery comparison.");
      return std::string("UNSUPPORTED SUBQUERY");
  }
  switch (query.reduction_op()) {
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ReductionOp_REDUCTION_OP_ANY:
      result << "ANY ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ReductionOp_REDUCTION_OP_ALL:
      result << "ALL ";
      break;
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ReductionOp_REDUCTION_OP_UNSPECIFIED:
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ReductionOp_Expression_Subquery_SetComparison_ReductionOp_INT_MIN_SENTINEL_DO_NOT_USE_:
    case ::substrait::proto::
        Expression_Subquery_SetComparison_ReductionOp_Expression_Subquery_SetComparison_ReductionOp_INT_MAX_SENTINEL_DO_NOT_USE_:
      errorListener_->addError("Did not recognize the subquery reduction op.");
      return std::string("UNSUPPORTED SUBQUERY");
  }
  result << "SUBQUERY ";
  if (query.has_right()) {
    const SymbolInfo* symbol = symbolTable_->lookupSymbolByParentQueryAndType(
        currentScope_->sourceLocation,
        ++currentScopeIndex_,
        SymbolType::kRelation);
    if (symbol != nullptr) {
      result << symbol->name;
    } else {
      errorListener_->addError(
          "Internal error -- could not find subquery symbol.");
    }
  } else {
    errorListener_->addError(
        "No subquery defined for set comparison operation.");
  }
  return result.str();
}

std::any PlanPrinterVisitor::visitSelect(
    const ::substrait::proto::Expression_MaskExpression_Select& select) {
  errorListener_->addError("MaskExpression Select not yet implemented.");
  return std::string("MASKSELECT_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitType(const ::substrait::proto::Type& type) {
  switch (type.kind_case()) {
    case ::substrait::proto::Type::kBool:
      if (type.bool_().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("bool?");
      }
      return std::string("bool");
    case ::substrait::proto::Type::kI8:
      if (type.i8().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("i8?");
      }
      return std::string("i8");
    case ::substrait::proto::Type::kI16:
      if (type.i16().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("i16?");
      }
      return std::string("i16");
    case ::substrait::proto::Type::kI32:
      if (type.i32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("i32?");
      }
      return std::string("i32");
    case ::substrait::proto::Type::kI64:
      if (type.i64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("i64?");
      }
      return std::string("i64");
    case ::substrait::proto::Type::kFp32:
      if (type.fp32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("fp32?");
      }
      return std::string("fp32");
    case ::substrait::proto::Type::kFp64:
      if (type.fp64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("fp64?");
      }
      return std::string("fp64");
    case ::substrait::proto::Type::kString:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("string?");
      }
      return std::string("string");
    case ::substrait::proto::Type::kDecimal: {
      std::stringstream result;
      result << "decimal";
      if (type.decimal().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        result << '?';
      }
      result << "<" << type.decimal().precision() << ",";
      result << type.decimal().scale() << ">";
      return result.str();
    }
    case ::substrait::proto::Type::kVarchar: {
      std::stringstream result;
      result << "varchar";
      if (type.varchar().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        result << "?";
      }
      result << "<" << type.varchar().length() << ">";
      return result.str();
    }
    case ::substrait::proto::Type::kFixedChar: {
      std::stringstream result;
      result << "fixedchar";
      if (type.fixed_char().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        result << "?";
      }
      result << "<" << type.fixed_char().length() << ">";
      return result.str();
    }
    case ::substrait::proto::Type::kDate:
      if (type.date().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("date?");
      }
      return std::string("date");
    case ::substrait::proto::Type::KIND_NOT_SET:
      errorListener_->addError(
          "Invalid type message -- type was not included.");
      return std::string("MISSING_TYPE");
    case ::substrait::proto::Type::kBinary:
    case ::substrait::proto::Type::kTimestamp:
    case ::substrait::proto::Type::kTime:
    case ::substrait::proto::Type::kIntervalYear:
    case ::substrait::proto::Type::kIntervalDay:
    case ::substrait::proto::Type::kTimestampTz:
    case ::substrait::proto::Type::kUuid:
    case ::substrait::proto::Type::kFixedBinary:
    case ::substrait::proto::Type::kStruct:
    case ::substrait::proto::Type::kList:
    case ::substrait::proto::Type::kMap:
    case ::substrait::proto::Type::kUserDefined:
    case ::substrait::proto::Type::kUserDefinedTypeReference:
      errorListener_->addError(
          "Unsupported type requested: " + type.ShortDebugString());
      return std::string("UNSUPPORTED_TYPE");
  }
  errorListener_->addError(
      "New unsupported type requested: " + type.ShortDebugString());
  return std::string("UNSUPPORTED_TYPE");
}

std::any PlanPrinterVisitor::visitStruct(
    const ::substrait::proto::Type_Struct& structure) {
  // TODO -- Implement.
  errorListener_->addError("Struct types are not yet supported.");
  return std::string("STRUCT_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitLiteral(
    const ::substrait::proto::Expression::Literal& literal) {
  std::stringstream text;
  switch (literal.literal_type_case()) {
    case ::substrait::proto::Expression::Literal::kBoolean:
      text << literal.boolean();
      break;
    case ::substrait::proto::Expression::Literal::kI8:
      text << literal.i8() << "_i8";
      break;
    case ::substrait::proto::Expression::Literal::kI16:
      text << literal.i16() << "_i16";
      break;
    case ::substrait::proto::Expression::Literal::kI32:
      text << literal.i32() << "_i32";
      break;
    case ::substrait::proto::Expression::Literal::kI64:
      text << literal.i64() << "_i64";
      break;
    case ::substrait::proto::Expression::Literal::kFp32:
      text << literal.fp32() << "_fp32";
      break;
    case ::substrait::proto::Expression::Literal::kFp64:
      text << literal.fp64() << "_fp64";
      break;
    case ::substrait::proto::Expression::Literal ::kDate: {
      auto refDate = date::sys_days{};
      date::sys_days newDate = refDate + date::days{literal.date()};
      text << '"' << date::year_month_day{newDate} << "\"_date";
      break;
    }
    case ::substrait::proto::Expression::Literal::kString:
      text << "\"" << stringEscape(literal.string()) << "\"_string";
      break;
    case ::substrait::proto::Expression_Literal::kBinary:
    case ::substrait::proto::Expression_Literal::kTimestamp:
    case ::substrait::proto::Expression_Literal::kTime:
      errorListener_->addError(
          "Literal of this type not yet supported: " +
          literal.ShortDebugString());
      return std::string("UNSUPPORTED_LITERAL_TYPE");
    case ::substrait::proto::Expression_Literal::kIntervalYearToMonth: {
      text << "{" << literal.interval_year_to_month().years() << "_years"
           << ", " << literal.interval_year_to_month().months() << "_months"
           << "}_interval_year"; // TODO - Change spec to better name.
      break;
    }
    case ::substrait::proto::Expression_Literal::kIntervalDayToSecond: {
      text << "{" << literal.interval_day_to_second().days() << "_days"
           << ", " << literal.interval_day_to_second().seconds() << "_seconds"
           << ", " << literal.interval_day_to_second().microseconds()
           << "_microseconds"
           << "}_interval_day"; // TODO - Change spec to better name.
      break;
    }
    case ::substrait::proto::Expression_Literal::kFixedChar:
      // TODO - Consider dropping the length in the type in favor of strlen.
      text << "\"" << stringEscape(literal.fixed_char()) << "\"_fixedchar<"
           << literal.fixed_char().length() << ">";
      break;
    case ::substrait::proto::Expression_Literal::kVarChar:
      text << "\"" << stringEscape(literal.var_char().value()) << "\"_varchar<"
           << literal.var_char().length() << ">";
      break;
    case ::substrait::proto::Expression_Literal::kDecimal: {
      auto dec = expression::DecimalLiteral::fromProto(literal.decimal());
      text << dec.toBaseString();
      text << "_decimal<" << dec.precision() << "," << dec.scale() << ">";
      break;
    }
    case ::substrait::proto::Expression_Literal::kFixedBinary:
    case ::substrait::proto::Expression_Literal::kStruct:
    case ::substrait::proto::Expression_Literal::kMap:
    case ::substrait::proto::Expression_Literal::kTimestampTz:
    case ::substrait::proto::Expression_Literal::kUuid:
    case ::substrait::proto::Expression_Literal::kNull:
    case ::substrait::proto::Expression_Literal::kList:
    case ::substrait::proto::Expression_Literal::kEmptyList:
    case ::substrait::proto::Expression_Literal::kEmptyMap:
    case ::substrait::proto::Expression_Literal::kUserDefined:
      errorListener_->addError(
          "Literal of this type not yet supported: " +
          literal.ShortDebugString());
      return std::string("UNSUPPORTED_LITERAL_TYPE");
    case ::substrait::proto::Expression::Literal::LITERAL_TYPE_NOT_SET:
      errorListener_->addError(
          "Type not specified in literal: " + literal.ShortDebugString());
      return std::string("UNSPECIFIED_LITERAL_TYPE");
  }
  if (literal.nullable()) {
    text << "?";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitFieldReference(
    const ::substrait::proto::Expression::FieldReference& ref) {
  // TODO -- Move this logic into visitDirectReference and visitMaskedReference.
  switch (ref.reference_type_case()) {
    case ::substrait::proto::Expression::FieldReference::kDirectReference: {
      uint32_t stepsOut{0};
      if (ref.has_outer_reference()) {
        stepsOut = ref.outer_reference().steps_out();
      }
      // TODO -- Figure out when fully qualified names aren't needed.
      return lookupFieldReference(
          ref.direct_reference().struct_field().field(),
          currentScope_,
          stepsOut,
          true);
    }
    case ::substrait::proto::Expression::FieldReference::kMaskedReference:
      errorListener_->addError(
          "Masked reference not yet supported: " + ref.ShortDebugString());
      return "MASKED_REF_NOT_SUPPORTED";
    case ::substrait::proto::Expression::FieldReference::REFERENCE_TYPE_NOT_SET:
      errorListener_->addError("Reference type missing from field reference.");
      return "";
  }
  errorListener_->addError(
      "Field reference type not yet supported: " + ref.ShortDebugString());
  return "FIELD_REF_TYPE_NOT_SUPPORTED";
}

std::any PlanPrinterVisitor::visitScalarFunction(
    const ::substrait::proto::Expression::ScalarFunction& function) {
  std::stringstream text;
  if (functionDepth_ > 0) {
    text << "\n";
    for (int i = 0; i <= functionDepth_; ++i) {
      text << "  ";
    }
  }
  functionDepth_++;
  auto resetFunctionDepth = finally([&]() { functionDepth_--; });
  text << lookupFunctionReference(function.function_reference()) << "(";
  // TODO -- Eventually handle options.
  bool hasPreviousText = false;
  for (const auto& arg : function.arguments()) {
    if (hasPreviousText) {
      text << ", ";
    }
    switch (arg.arg_type_case()) {
      case ::substrait::proto::FunctionArgument::kEnum:
        text << visitEnumArgument(arg.enum_());
        break;
      case ::substrait::proto::FunctionArgument::kType:
        text << ANY_CAST(std::string, visitType(arg.type()));
        break;
      case ::substrait::proto::FunctionArgument::kValue:
        text << ANY_CAST(std::string, visitExpression(arg.value()));
        break;
      case ::substrait::proto::FunctionArgument::ARG_TYPE_NOT_SET:
        errorListener_->addError("Argument type missing the type field.");
        text << "SCALAR_FUNCTION_ARGUMENT_TYPE_MISSING";
        break;
    }
    hasPreviousText = true;
  }
  for (const auto& arg : function.args()) {
    if (hasPreviousText) {
      text << ", ";
    }
    text << ANY_CAST(std::string, visitExpression(arg));
    hasPreviousText = true;
  }
  if (!hasPreviousText) {
    errorListener_->addError(
        "Function encountered without any arguments: " +
        function.ShortDebugString());
  }

  text << ")";

  // TODO -- Determine if the output type can be automatically determined.
  text << "->" << typeToText(function.output_type());

  return text.str();
}

std::any PlanPrinterVisitor::visitWindowFunction(
    const ::substrait::proto::Expression::WindowFunction& function) {
  errorListener_->addError(
      "Window functions are not yet supported: " + function.ShortDebugString());
  return std::string("WINDOWFUNC_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitIfThen(
    const ::substrait::proto::Expression::IfThen& ifthen) {
  std::stringstream text;
  text << "IFTHEN(";
  bool hasPreviousText = false;
  for (const auto& clause : ifthen.ifs()) {
    if (!clause.has_if_() || !clause.has_then()) {
      errorListener_->addError(
          "If then clauses require both an if and a then expression: " +
          clause.ShortDebugString());
      continue;
    }
    if (hasPreviousText) {
      text << ", ";
    }
    text << ANY_CAST(std::string, visitExpression(clause.if_()));
    text << ", ";
    text << ANY_CAST(std::string, visitExpression(clause.then()));
    hasPreviousText = true;
  }
  if (ifthen.has_else_()) {
    if (hasPreviousText) {
      text << ", ";
    }
    text << ANY_CAST(std::string, visitExpression(ifthen.else_()));
  }

  text << ")";
  return text.str();
}

std::any PlanPrinterVisitor::visitSwitchExpression(
    const ::substrait::proto::Expression::SwitchExpression& expression) {
  errorListener_->addError(
      "Switch expressions are not yet supported: " +
      expression.ShortDebugString());
  return std::string("SWITCH_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitSingularOrList(
    const ::substrait::proto::Expression::SingularOrList& expression) {
  errorListener_->addError(
      "Singular or list expressions are not yet supported: " +
      expression.ShortDebugString());
  return std::string("SINGORLIST_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitMultiOrList(
    const ::substrait::proto::Expression::MultiOrList& expression) {
  errorListener_->addError(
      "Multiple or list expressions are not yet supported: " +
      expression.ShortDebugString());
  return std::string("MULTIORLIST_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitCast(
    const ::substrait::proto::Expression::Cast& cast) {
  std::stringstream text;
  text << ANY_CAST(std::string, visitExpression(cast.input()));
  text << " AS " << ANY_CAST(std::string, visitType(cast.type()));
  // TODO -- Handle case.failure_behavior().
  return text.str();
}

std::any PlanPrinterVisitor::visitNested(
    const ::substrait::proto::Expression_Nested& structure) {
  errorListener_->addError(
      "Nested expressions are not yet supported: " +
      structure.ShortDebugString());
  return std::string("NESTED_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitEnum(
    const ::substrait::proto::Expression_Enum& value) {
  errorListener_->addError(
      "Enum expressions are deprecated and not supported: " +
      value.ShortDebugString());
  return std::string("ENUM_EXPRESSION_DEPRECATED");
}

std::any PlanPrinterVisitor::visitStructSelect(
    const ::substrait::proto::Expression_MaskExpression_StructSelect&
        structure) {
  errorListener_->addError(
      "Struct selects are not yet supported: " + structure.ShortDebugString());
  return std::string("STRUCTSELECT_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitListSelect(
    const ::substrait::proto::Expression_MaskExpression_ListSelect& select) {
  errorListener_->addError(
      "List selects are not yet supported: " + select.ShortDebugString());
  return std::string("LISTSELECT_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitListSelectItem(
    const ::substrait::proto::
        Expression_MaskExpression_ListSelect_ListSelectItem& item) {
  errorListener_->addError(
      "List select items are not yet supported: " + item.ShortDebugString());
  return std::string("LISTSELECTITEM_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitMapSelect(
    const ::substrait::proto::Expression_MaskExpression_MapSelect& select) {
  errorListener_->addError(
      "Map selects are not yet supported: " + select.ShortDebugString());
  return std::string("MAPSELECT_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitExpressionLiteralStruct(
    const ::substrait::proto::Expression_Literal_Struct& structure) {
  errorListener_->addError(
      "Expression literals are not yet supported: " +
      structure.ShortDebugString());
  return std::string("EXPRLIT_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitFileOrFiles(
    const ::substrait::proto::ReadRel_LocalFiles_FileOrFiles& structure) {
  errorListener_->addError(
      "Local file or files is not yet supported: " +
      structure.ShortDebugString());
  return std::string("FORF_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitRelationCommon(
    const ::substrait::proto::RelCommon& common) {
  std::stringstream text;
  if (common.emit().output_mapping_size() > 0) {
    text << "\n";
  }
  for (const auto& mapping : common.emit().output_mapping()) {
    text << "  emit "
         << lookupFieldReferenceForEmit(
                mapping, currentScope_, /* stepsOut= */ 0, true)
         << ";\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitAggregateFunction(
    const ::substrait::proto::AggregateFunction& function) {
  std::stringstream text;
  if (functionDepth_ > 0) {
    text << "\n";
    for (int i = 0; i <= functionDepth_; ++i) {
      text << "  ";
    }
  }
  functionDepth_++;
  auto resetFunctionDepth = finally([&]() { functionDepth_--; });
  text << lookupFunctionReference(function.function_reference()) << "(";
  bool hasPreviousText = false;
  for (const auto& arg : function.arguments()) {
    if (hasPreviousText) {
      text << ", ";
    }
    switch (arg.arg_type_case()) {
      case ::substrait::proto::FunctionArgument::kEnum:
        text << visitEnumArgument(arg.enum_());
        break;
      case ::substrait::proto::FunctionArgument::kType:
        text << ANY_CAST(std::string, visitType(arg.type()));
        break;
      case ::substrait::proto::FunctionArgument::kValue:
        text << ANY_CAST(std::string, visitExpression(arg.value()));
        break;
      case ::substrait::proto::FunctionArgument::ARG_TYPE_NOT_SET:
      default:
        text << "UNKNOWN_AGGREGATE_FUNCTION_ARGUMENT_TYPE";
        break;
    }
    hasPreviousText = true;
  }
  for (const auto& arg : function.args()) {
    if (hasPreviousText) {
      text << ", ";
    }
    text << ANY_CAST(std::string, visitExpression(arg));
    hasPreviousText = true;
  }
  text << ")";
  for (const auto& option : function.options()) {
    text << "#" << option.name();
    for (const auto& pref : option.preference()) {
      text << ";" << pref;
    }
  }
  text << "->" << ANY_CAST(std::string, visitType(function.output_type()));
  // TODO -- Emit the requested sort behavior here.
  // TODO -- Consider removing the AGGREGATION_PHASE_ prefix.
  text << "@" << ::substrait::proto::AggregationPhase_Name(function.phase());
  return text.str();
}

std::any PlanPrinterVisitor::visitReferenceSegment(
    const ::substrait::proto::Expression_ReferenceSegment& segment) {
  errorListener_->addError(
      "Reference segments are not yet supported: " +
      segment.ShortDebugString());
  return std::string("REFSEG_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitExpression(
    const ::substrait::proto::Expression& expression) {
  return BasePlanProtoVisitor::visitExpression(expression);
}

std::any PlanPrinterVisitor::visitMaskExpression(
    const ::substrait::proto::Expression::MaskExpression& expression) {
  errorListener_->addError(
      "Mask expressions are not yet supported: " +
      expression.ShortDebugString());
  return std::string("MASKEXPR-NOT-YET-IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitRelation(
    const ::substrait::proto::Rel& relation) {
  // Mark the current scope for any operations within this relation.
  auto previousScope = currentScope_;
  auto previousScopeIndex = currentScopeIndex_;
  auto resetCurrentScope = finally([&]() {
    currentScope_ = previousScope;
    currentScopeIndex_ = previousScopeIndex;
  });
  const SymbolInfo* symbol = symbolTable_->lookupSymbolByLocationAndType(
      PROTO_LOCATION(relation), SymbolType::kRelation);
  if (symbol != nullptr) {
    currentScope_ = symbol;
  }
  currentScopeIndex_ = -1;

  auto result = BasePlanProtoVisitor::visitRelation(relation);

  return result;
}

std::any PlanPrinterVisitor::visitReadRelation(
    const ::substrait::proto::ReadRel& relation) {
  std::stringstream text;

  const google::protobuf::Message* msg;
  switch (relation.read_type_case()) {
    case ::substrait::proto::ReadRel::ReadTypeCase::kVirtualTable:
      msg = (const google::protobuf::Message*)&relation.virtual_table();
      break;
    case ::substrait::proto::ReadRel::ReadTypeCase::kLocalFiles:
      msg = (const google::protobuf::Message*)&relation.local_files();
      break;
    case ::substrait::proto::ReadRel::ReadTypeCase::kNamedTable:
      msg = (const google::protobuf::Message*)&relation.named_table();
      break;
    case ::substrait::proto::ReadRel::ReadTypeCase::kExtensionTable:
      msg = (const google::protobuf::Message*)&relation.extension_table();
      break;
    case ::substrait::proto::ReadRel::READ_TYPE_NOT_SET:
      return "";
  }

  auto source = symbolTable_->lookupSymbolByLocationAndType(
      PROTO_LOCATION(*msg), SymbolType::kSource);
  if (source != nullptr) {
    text << "  source " << source->name << ";\n";
  }

  if (relation.has_base_schema()) {
    const auto* schemaSymbol = symbolTable_->lookupSymbolByLocationAndType(
        PROTO_LOCATION(relation.base_schema()), SymbolType::kSchema);
    if (schemaSymbol != nullptr) {
      text << "  base_schema " << schemaSymbol->name << ";\n";
    }
  }
  if (relation.has_filter()) {
    text << "  filter "
         << ANY_CAST(std::string, visitExpression(relation.filter())) + ";\n";
  }
  if (relation.has_best_effort_filter()) {
    text << "  filter "
         << ANY_CAST(
                std::string, visitExpression(relation.best_effort_filter()))
         << ";\n";
  }
  if (relation.has_projection()) {
    text << "  projection "
         << ANY_CAST(std::string, visitMaskExpression(relation.projection()))
         << ";\n";
  }

  return text.str();
}

std::any PlanPrinterVisitor::visitFilterRelation(
    const ::substrait::proto::FilterRel& relation) {
  std::stringstream text;
  if (relation.has_condition()) {
    text << "  filter "
         << ANY_CAST(std::string, visitExpression(relation.condition()))
         << ";\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitFetchRelation(
    const ::substrait::proto::FetchRel& relation) {
  std::stringstream text;
  if (relation.offset() != 0) {
    text << "  offset " << std::to_string(relation.offset()) << ";\n";
  }
  text << "  count " << std::to_string(relation.count()) << ";\n";
  return text.str();
}

std::any PlanPrinterVisitor::visitAggregateRelation(
    const ::substrait::proto::AggregateRel& relation) {
  std::stringstream text;
  for (const auto& group : relation.groupings()) {
    for (const auto& expr : group.grouping_expressions()) {
      text << "  grouping " << ANY_CAST(std::string, visitExpression(expr))
           << ";\n";
    }
  }
  for (const auto& measure : relation.measures()) {
    if (!measure.has_measure()) {
      errorListener_->addError(
          "Encountered aggregate measure without a measure function.");
      continue;
    }
    text << "  measure {\n";
    text << "    measure "
         << ANY_CAST(std::string, visitAggregateFunction(measure.measure()));
    auto symbol = symbolTable_->lookupSymbolByLocationAndType(
        PROTO_LOCATION(measure), SymbolType::kMeasure);
    if (symbol != nullptr) {
      text << " NAMED " << symbol->name;
    }
    text << ";\n";
    if (measure.has_filter()) {
      text << "    filter "
           << ANY_CAST(std::string, visitExpression(measure.filter())) << ";\n";
    }
    if (measure.measure().invocation() !=
        ::substrait::proto::
            AggregateFunction_AggregationInvocation_AGGREGATION_INVOCATION_UNSPECIFIED) {
      text << "    invocation "
           << invocationToString(measure.measure().invocation()) << ";\n";
    }
    text << "  }\n";
  }
  text << ANY_CAST(std::string, visitRelationCommon(relation.common()));
  return text.str();
}

std::any PlanPrinterVisitor::visitSortRelation(
    const ::substrait::proto::SortRel& relation) {
  std::stringstream text;
  for (const auto& sort : relation.sorts()) {
    text << "  sort " << ANY_CAST(std::string, visitExpression(sort.expr()));
    switch (sort.sort_kind_case()) {
      case ::substrait::proto::SortField::kDirection:
        switch (sort.direction()) {
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_ASC_NULLS_FIRST:
            text << " by ASC_NULLS_FIRST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_ASC_NULLS_LAST:
            text << " by ASC_NULLS_LAST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_DESC_NULLS_FIRST:
            text << " by DESC_NULLS_FIRST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_DESC_NULLS_LAST:
            text << " by DESC_NULLS_LAST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_CLUSTERED:
            text << " by CLUSTERED";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_UNSPECIFIED:
          default:
            break;
        }
        break;
      case ::substrait::proto::SortField::kComparisonFunctionReference: {
        return lookupFunctionReference(sort.comparison_function_reference());
      }
      case ::substrait::proto::SortField::SORT_KIND_NOT_SET:
        errorListener_->addError(
            "Required sort kind missing from sort relation sort.");
        break;
    }
    text << ";\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitProjectRelation(
    const ::substrait::proto::ProjectRel& relation) {
  std::stringstream text;
  auto relationData =
      ANY_CAST(std::shared_ptr<RelationData>, currentScope_->blob);
  int exprNum = 0;
  for (const auto& expr : relation.expressions()) {
    text << "  expression ";
    if (relationData->generatedFieldReferenceAlternativeExpression.find(
            exprNum) !=
        relationData->generatedFieldReferenceAlternativeExpression.end()) {
      text << relationData
                  ->generatedFieldReferenceAlternativeExpression[exprNum];
    } else if (
        exprNum < relationData->generatedFieldReferences.size() &&
        !relationData->generatedFieldReferences[exprNum]->alias.empty()) {
      text << ANY_CAST(std::string, visitExpression(expr));
      text << " NAMED "
           << relationData->generatedFieldReferences[exprNum]->alias;
    } else {
      text << ANY_CAST(std::string, visitExpression(expr));
    }

    text << ";\n";
    exprNum++;
  }
  text << ANY_CAST(std::string, visitRelationCommon(relation.common()));
  return text.str();
}

std::any PlanPrinterVisitor::visitJoinRelation(
    const ::substrait::proto::JoinRel& relation) {
  std::stringstream text;
  // TODO -- Consider removing the JOIN_TYPE_ prefix.
  text << "  type "
       << ::substrait::proto::JoinRel_JoinType_Name(relation.type()) << ";\n";
  if (relation.has_expression()) {
    text << "  expression "
         << ANY_CAST(std::string, visitExpression(relation.expression()))
         << ";\n";
  }
  if (relation.has_post_join_filter()) {
    text << "  post_join "
         << ANY_CAST(std::string, visitExpression(relation.post_join_filter()))
         << ";\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitCrossRelation(
    const ::substrait::proto::CrossRel& relation) {
  // There are no custom details in a cross relation.
  return std::string{""};
}

} // namespace io::substrait::textplan

#pragma clang diagnostic pop
#pragma gcc diagnostic pop
