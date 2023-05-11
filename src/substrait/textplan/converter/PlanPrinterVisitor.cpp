/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/PlanPrinterVisitor.h"

#include <iterator>
#include <sstream>
#include <string>

#include "substrait/expression/DecimalLiteral.h"
#include "substrait/proto/ProtoUtils.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Finally.h"
#include "substrait/textplan/StructuredSymbolData.h"

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

} // namespace

std::string PlanPrinterVisitor::printRelation(
    const ::substrait::proto::Rel* relation) {
  std::stringstream text;
  auto symbol = symbolTable_->lookupSymbolByLocation(PROTO_LOCATION(*relation));
  text << ::substrait::proto::relTypeCaseName(relation->rel_type_case())
       << " relation " << symbol.name << " {\n";

  auto result = this->visitRelation(*relation);
  if (result.type() != typeid(std::string)) {
    errorListener_->addError(
        "ERROR:  Relation subtype " +
        ::substrait::proto::relTypeCaseName((relation->rel_type_case())) +
        " not supported!");
    return "";
  }
  text << ANY_CAST(std::string, result);
  text << "}\n";

  return text.str();
}

std::string PlanPrinterVisitor::lookupFieldReference(uint32_t field_reference) {
  if (*currentScope_ != SymbolInfo::kUnknown) {
    auto relationData =
        ANY_CAST(std::shared_ptr<RelationData>, currentScope_->blob);
    if (field_reference < relationData->fieldReferences.size()) {
      return relationData->fieldReferences[field_reference]->name;
    }
  }
  errorListener_->addError(
      "Field number " + std::to_string(field_reference) +
      " referenced but not defined.");
  return "field#" + std::to_string(field_reference);
}

std::string PlanPrinterVisitor::lookupFunctionReference(
    uint32_t function_reference) {
  for (const auto& symbol : symbolTable_->getSymbols()) {
    if (symbol->type != SymbolType::kFunction) {
      continue;
    }
    if (symbol->blob.type() !=
        typeid(const ::substrait::proto::extensions::
                   SimpleExtensionDeclaration_ExtensionFunction*)) {
      // TODO -- Implement function references for text plans.
      continue;
    }
    auto function = ANY_CAST(
        const ::substrait::proto::extensions::
            SimpleExtensionDeclaration_ExtensionFunction*,
        symbol->blob);
    if (function->function_anchor() == function_reference) {
      return symbol->name;
    }
  }
  errorListener_->addError(
      "Reference of undefined function " + std::to_string(function_reference) +
      " encountered.");
  return "functionref#" + std::to_string(function_reference);
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
        return std::string("opt_bool");
      }
      return std::string("bool");
    case ::substrait::proto::Type::kI8:
      if (type.i8().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_i8");
      }
      return std::string("i8");
    case ::substrait::proto::Type::kI16:
      if (type.i16().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_i16");
      }
      return std::string("i16");
    case ::substrait::proto::Type::kI32:
      if (type.i32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_i32");
      }
      return std::string("i32");
    case ::substrait::proto::Type::kI64:
      if (type.i64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_i64");
      }
      return std::string("i64");
    case ::substrait::proto::Type::kFp32:
      if (type.fp32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_fp32");
      }
      return std::string("fp32");
    case ::substrait::proto::Type::kFp64:
      if (type.fp64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_fp64");
      }
      return std::string("fp64");
    case ::substrait::proto::Type::kString:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_string");
      }
      return std::string("string");
    case ::substrait::proto::Type::kDecimal:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return std::string("opt_decimal");
      }
      return std::string("decimal");
    case ::substrait::proto::Type::kVarchar:
      return std::string("varchar");
    case ::substrait::proto::Type::kFixedChar:
      return std::string("fixedchar");
    case ::substrait::proto::Type::kDate:
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
      "New unspported type requested: " + type.ShortDebugString());
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
      // TODO -- Format this as a date instead of a delta since an epoch.
      if (literal.date() >= 0) {
        text << "\"epoch+" << literal.date() << " days\"_date";
      } else {
        text << "\"epoch" << literal.date() << " days\"_date";
      }
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
      text << "{";
      bool hasPreviousText = false;
      if (literal.interval_year_to_month().years() != 0) {
        text << literal.interval_year_to_month().years() << "years";
        hasPreviousText = true;
      }
      if (literal.interval_year_to_month().months() != 0) {
        if (hasPreviousText) {
          text << ", ";
        }
        text << literal.interval_year_to_month().months() << "months";
      }
      text << "}_interval_year"; // TODO - Change spec to better name.
      break;
    }
    case ::substrait::proto::Expression_Literal::kIntervalDayToSecond: {
      text << "{";
      bool hasPreviousText = false;
      if (literal.interval_day_to_second().days() != 0) {
        text << literal.interval_day_to_second().days() << "days";
        hasPreviousText = true;
      }
      if (literal.interval_day_to_second().seconds() != 0) {
        if (hasPreviousText) {
          text << ", ";
        }
        text << literal.interval_day_to_second().seconds() << "seconds";
        hasPreviousText = true;
      }
      if (literal.interval_day_to_second().microseconds() != 0) {
        if (hasPreviousText) {
          text << ", ";
        }
        text << literal.interval_day_to_second().microseconds()
             << "microseconds";
      }
      text << "}_interval_day"; // TODO - Change spec to better name.
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
    case ::substrait::proto::Expression::FieldReference::kDirectReference:
      return lookupFieldReference(
          ref.direct_reference().struct_field().field());
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

// The visitor should be tolerant of older plans.  This requires calling
// deprecated APIs.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma gcc diagnostic push
#pragma gcc diagnostic ignored "-Wdeprecated-declarations"

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
        errorListener_->addError(
            "Enum arguments not yet supported in scalar functions: " +
            arg.ShortDebugString());
        text << "ENUM_NOT_SUPPORTED";
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
  // TODO -- Determine if the output type can be automatically determined.
  // text << "->" << visitType(function.output_type());

  if (!hasPreviousText) {
    errorListener_->addError(
        "Function encountered without any arguments: " +
        function.ShortDebugString());
  }
  text << ")";
  return text.str();
}

#pragma clang diagnostic pop
#pragma gcc diagnostic pop

std::any PlanPrinterVisitor::visitWindowFunction(
    const ::substrait::proto::Expression::WindowFunction& function) {
  errorListener_->addError(
      "Window functions are not yet supported: " + function.ShortDebugString());
  return std::string("WINDOWFUNC_NOT_YET_IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitIfThen(
    const ::substrait::proto::Expression::IfThen& ifthen) {
  errorListener_->addError(
      "If then expressions are not yet supported: " +
      ifthen.ShortDebugString());
  return std::string("IFTHEN_NOT_YET_IMPLEMENTED");
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

std::any PlanPrinterVisitor::visitSubquery(
    const ::substrait::proto::Expression_Subquery& query) {
  errorListener_->addError(
      "Subquery expressions are not yet supported: " +
      query.ShortDebugString());
  return std::string("SUBQUERY_NOT_YET_IMPLEMENTED");
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
      "Enum expressions are not yet supported: " + value.ShortDebugString());
  return std::string("ENUM_NOT_YET_IMPLEMENTED");
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
        text << "ENUM_NOT_SUPPORTED";
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
  auto resetCurrentScope = finally([&]() { currentScope_ = previousScope; });
  const SymbolInfo& symbol =
      symbolTable_->lookupSymbolByLocation(PROTO_LOCATION(relation));
  if (symbol != SymbolInfo::kUnknown) {
    currentScope_ = &symbol;
  }

  auto result = BasePlanProtoVisitor::visitRelation(relation);

  return result;
}

std::any PlanPrinterVisitor::visitReadRelation(
    const ::substrait::proto::ReadRel& relation) {
  std::stringstream text;

  ::google::protobuf::Message* msg;
  switch (relation.read_type_case()) {
    case ::substrait::proto::ReadRel::ReadTypeCase::kVirtualTable:
      msg = (google::protobuf::Message*)&relation.virtual_table();
      break;
    case ::substrait::proto::ReadRel::ReadTypeCase::kLocalFiles:
      msg = (::google::protobuf::Message*)&relation.local_files();
      break;
    case ::substrait::proto::ReadRel::ReadTypeCase::kNamedTable:
      msg = (::google::protobuf::Message*)&relation.named_table();
      break;
    case ::substrait::proto::ReadRel::ReadTypeCase::kExtensionTable:
      msg = (::google::protobuf::Message*)&relation.extension_table();
      break;
    case ::substrait::proto::ReadRel::READ_TYPE_NOT_SET:
      return "";
  }
  const auto& symbol =
      symbolTable_->lookupSymbolByLocation(PROTO_LOCATION(*msg));
  if (symbol != SymbolInfo::kUnknown) {
    text << "  source " << symbol.name << ";\n";
  }

  if (relation.has_base_schema()) {
    const auto& schemaSymbol = symbolTable_->lookupSymbolByLocation(
        PROTO_LOCATION(relation.base_schema()));
    if (schemaSymbol != SymbolInfo::kUnknown) {
      text << "  base_schema " << schemaSymbol.name << ";\n";
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
    text << "  condition "
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
      continue;
    }
    text << "  measure {\n";
    text << "    measure "
         << ANY_CAST(std::string, visitAggregateFunction(measure.measure()))
         << ";\n";
    if (measure.has_filter()) {
      text << "    filter " +
              ANY_CAST(std::string, visitExpression(measure.filter()))
           << ";\n";
    }
    text << "  }\n";
  }
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
  for (const auto& expr : relation.expressions()) {
    text << "  expression " << ANY_CAST(std::string, visitExpression(expr))
         << ";\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitJoinRelation(
    const ::substrait::proto::JoinRel& relation) {
  std::stringstream text;
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

} // namespace io::substrait::textplan

#pragma clang diagnostic pop
#pragma gcc diagnostic pop
