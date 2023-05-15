/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/SubstraitPlanRelationVisitor.h"

#include <chrono>
#include <memory>
#include <sstream>
#include <string>

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "absl/strings/numbers.h"
#include "datetime/tz.h"
#include "substrait/expression/DecimalLiteral.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/type.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/type/Type.h"

namespace io::substrait::textplan {

namespace {

enum RelationFilterBehavior {
  kDefault = 0,
  kBestEffort = 1,
  kPostJoin = 2,
};

std::string toLower(const std::string& str) {
  std::string s = str;
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  return s;
}

// Yields true if the string 'haystack' starts with the string 'needle'.
bool startsWith(const std::string& haystack, const std::string& needle) {
  return strncmp(haystack.c_str(), needle.c_str(), needle.size()) == 0;
}

void setNullable(::substrait::proto::Type* type) {
  switch (type->kind_case()) {
    case ::substrait::proto::Type::kBool:
      type->mutable_bool_()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI8:
      type->mutable_i8()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI16:
      type->mutable_i16()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI32:
      type->mutable_i32()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI64:
      type->mutable_i64()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFp32:
      type->mutable_fp32()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFp64:
      type->mutable_fp64()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kString:
      type->mutable_string()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kBinary:
      type->mutable_binary()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kTimestamp:
      type->mutable_timestamp()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kDate:
      type->mutable_date()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kTime:
      type->mutable_time()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kIntervalYear:
      type->mutable_interval_year()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kIntervalDay:
      type->mutable_interval_day()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kTimestampTz:
      type->mutable_timestamp_tz()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kUuid:
      type->mutable_uuid()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFixedChar:
      type->mutable_fixed_char()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kVarchar:
      type->mutable_varchar()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFixedBinary:
      type->mutable_fixed_binary()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kDecimal:
      type->mutable_decimal()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kStruct:
      type->mutable_struct_()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kList:
      type->mutable_list()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kMap:
      type->mutable_map()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kUserDefined:
      type->mutable_user_defined()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kUserDefinedTypeReference:
    case ::substrait::proto::Type::KIND_NOT_SET:
      break;
  }
}

} // namespace

std::any SubstraitPlanRelationVisitor::aggregateResult(
    std::any aggregate,
    std::any nextResult) {
  if (!nextResult.has_value()) {
    // No point returning an unspecified result over whatever we already have.
    return aggregate;
  }
  return nextResult;
}

std::any SubstraitPlanRelationVisitor::visitRelation(
    SubstraitPlanParser::RelationContext* ctx) {
  // Create the relation before visiting our children, so they can update it.
  auto symbol = symbolTable_->lookupSymbolByLocation(Location(ctx));
  if (symbol == SymbolInfo::kUnknown) {
    // This error has been previously dealt with thus we can safely skip it.
    return defaultResult();
  }
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol.blob);
  ::substrait::proto::Rel relation;

  // Validate that we have the right details for our type.
  auto relationType = ANY_CAST(RelationType, symbol.subtype);
  switch (relationType) {
    case RelationType::kRead:
      relation.mutable_read()->clear_common();
      break;
    case RelationType::kProject:
      relation.mutable_project()->clear_common();
      break;
    case RelationType::kJoin:
      relation.mutable_join()->clear_common();
      break;
    case RelationType::kCross:
      relation.mutable_cross()->clear_common();
      break;
    case RelationType::kFetch:
      relation.mutable_fetch()->clear_common();
      break;
    case RelationType::kAggregate:
      relation.mutable_aggregate()->clear_common();
      break;
    case RelationType::kSort:
      relation.mutable_sort()->clear_common();
      break;
    case RelationType::kFilter:
      relation.mutable_filter()->clear_common();
      break;
    case RelationType::kSet:
      relation.mutable_set()->clear_common();
      break;
    case RelationType::kExchange:
    case RelationType::kDdl:
    case RelationType::kWrite:
      break;
    case RelationType::kHashJoin:
      relation.mutable_hash_join()->clear_common();
      break;
    case RelationType::kMergeJoin:
      relation.mutable_merge_join()->clear_common();
      break;
    case RelationType::kExtensionLeaf:
      relation.mutable_extension_leaf()->clear_common();
      break;
    case RelationType::kExtensionSingle:
      relation.mutable_extension_single()->clear_common();
      break;
    case RelationType::kExtensionMulti:
      relation.mutable_extension_multi()->clear_common();
      break;
    case RelationType::kUnknown:
      break;
  }

  relationData->relation = relation;

  return visitChildren(ctx);
}

std::any SubstraitPlanRelationVisitor::visitRelation_filter_behavior(
    SubstraitPlanParser::Relation_filter_behaviorContext* ctx) {
  std::string text = toLower(ctx->getText());
  // Only look at alphabetic characters for this comparison.
  text.erase(
      std::remove_if(
          text.begin(),
          text.end(),
          [](auto const& c) -> bool { return !std::isalnum(c); }),
      text.end());
  if (text == "besteffort") {
    return kBestEffort;
  } else if (text == "postjoin") {
    return kPostJoin;
  }
  errorListener_->addError(
      ctx->getStart(),
      "Best effort and post join are the only two legal filter behavior "
      "choices.  You may also not provide one which will result to the default "
      "filter behavior.");
  return kDefault;
}

std::any SubstraitPlanRelationVisitor::visitRelationFilter(
    SubstraitPlanParser::RelationFilterContext* ctx) {
  RelationFilterBehavior behavior = kDefault;
  if (ctx->relation_filter_behavior() != nullptr) {
    behavior = ANY_CAST(
        RelationFilterBehavior,
        visitRelation_filter_behavior(ctx->relation_filter_behavior()));
  }

  auto parentSymbol = symbolTable_->lookupSymbolByLocation(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)));
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol.blob);
  auto result = SubstraitPlanRelationVisitor::visitChildren(ctx);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol.subtype);
  switch (parentRelationType) {
    case RelationType::kRead:
      switch (behavior) {
        case kDefault:
          if (parentRelationData->relation.read().has_filter()) {
            errorListener_->addError(
                ctx->getStart(),
                "A filter has already been specified for this read relation.  "
                "Only one filter and one best effort filter may be specified "
                "for a read relation.");
            break;
          }
          *parentRelationData->relation.mutable_read()->mutable_filter() =
              ANY_CAST(::substrait::proto::Expression, result);
          break;
        case kBestEffort:
          if (parentRelationData->relation.read().has_best_effort_filter()) {
            errorListener_->addError(
                ctx->getStart(),
                "A best effort filter has already been specified for this read "
                "relation.  Only one filter and one best effort filter may be "
                "specified for a read relation.");
            break;
          }
          *parentRelationData->relation.mutable_read()
               ->mutable_best_effort_filter() =
              ANY_CAST(::substrait::proto::Expression, result);
          break;
        case kPostJoin:
        default:
          errorListener_->addError(
              ctx->getStart(),
              "Default and best effort filters are the only legal filter for "
              "read relations.");
          break;
      }
      break;
    case RelationType::kJoin:
      if (behavior == kPostJoin) {
        if (parentRelationData->relation.join().has_post_join_filter()) {
          errorListener_->addError(
              ctx->getStart(),
              "A post join filter has already been specified for this join "
              "relation.  Only one may be specified.");
          break;
        }
        *parentRelationData->relation.mutable_join()
             ->mutable_post_join_filter() =
            ANY_CAST(::substrait::proto::Expression, result);
      } else {
        errorListener_->addError(
            ctx->getStart(),
            "A post join filter is the only legal option for a join relation.");
      }
      break;
    case RelationType::kFilter:
      if (behavior == kDefault) {
        if (parentRelationData->relation.filter().has_condition()) {
          errorListener_->addError(
              ctx->getStart(),
              "A filter condition has already been specified.  Only one may be "
              "specified.");
          break;
        }
        *parentRelationData->relation.mutable_filter()->mutable_condition() =
            ANY_CAST(::substrait::proto::Expression, result);
      } else {
        errorListener_->addError(
            ctx->getStart(),
            "No filter behavior options are permissible for a filter join.");
      }
      break;
    default:
      errorListener_->addError(
          ctx->getStart(),
          "Filters are not permitted for this kind of relation.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationExpression(
    SubstraitPlanParser::RelationExpressionContext* ctx) {
  auto parentSymbol = symbolTable_->lookupSymbolByLocation(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)));
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol.blob);
  auto result = SubstraitPlanRelationVisitor::visitChildren(ctx);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol.subtype);
  switch (parentRelationType) {
    case RelationType::kJoin:
      if (parentRelationData->relation.join().has_expression()) {
        errorListener_->addError(
            ctx->getStart(),
            "A join expression has already been specified.  Only one may be "
            "specified.");
        break;
      }
      *parentRelationData->relation.mutable_join()->mutable_expression() =
          ANY_CAST(::substrait::proto::Expression, result);
      break;
    case RelationType::kProject:
      *parentRelationData->relation.mutable_project()->add_expressions() =
          ANY_CAST(::substrait::proto::Expression, result);
      break;
    default:
      errorListener_->addError(
          ctx->getStart(),
          "Expressions are not permitted for this kind of relation.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitExpressionFunctionUse(
    SubstraitPlanParser::ExpressionFunctionUseContext* ctx) {
  ::substrait::proto::Expression expr;
  visitChildren(ctx);
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitExpressionConstant(
    SubstraitPlanParser::ExpressionConstantContext* ctx) {
  ::substrait::proto::Expression expr;
  *expr.mutable_literal() =
      ANY_CAST(::substrait::proto::Expression_Literal, visitChildren(ctx));
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitExpressionColumn(
    SubstraitPlanParser::ExpressionColumnContext* ctx) {
  ::substrait::proto::Expression expr;
  visitChildren(ctx);
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitConstant(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Expression_Literal literal;
  if (ctx->literal_basic_type() != nullptr) {
    auto literalType = ANY_CAST(
        ::substrait::proto::Type,
        visitLiteral_basic_type(ctx->literal_basic_type()));
    return visitConstantWithType(ctx, literalType);
  } else if (ctx->literal_complex_type() != nullptr) {
    auto literalType = ANY_CAST(
        ::substrait::proto::Type,
        visitLiteral_complex_type(ctx->literal_complex_type()));
    return visitConstantWithType(ctx, literalType);
  } else {
    // Try to figure out this literal without a type.
    if (ctx->NULLVAL() != nullptr) {
      errorListener_->addError(
          ctx->NULLVAL()->getSymbol(), "Null literals require type.");
    } else if (ctx->TRUEVAL() != nullptr) {
      literal.set_boolean(true);
    } else if (ctx->FALSEVAL() != nullptr) {
      literal.set_boolean(false);
    } else if (ctx->STRING() != nullptr) {
      auto literalType = ::substrait::proto::Type();
      literalType.mutable_string();
      literal = visitString(ctx->STRING(), literalType);
    } else {
      errorListener_->addError(
          ctx->getStart(), "Literals should include a type.");
    }
  }
  return literal;
}

std::any SubstraitPlanRelationVisitor::visitLiteral_specifier(
    SubstraitPlanParser::Literal_specifierContext* ctx) {
  // Provides detail for the width of the type.
  return visitChildren(ctx);
}

std::any SubstraitPlanRelationVisitor::visitLiteral_basic_type(
    SubstraitPlanParser::Literal_basic_typeContext* ctx) {
  std::shared_ptr<const ParameterizedType> decodedType;
  try {
    decodedType = Type::decode(ctx->getText());
  } catch (...) {
    errorListener_->addError(ctx->getStart(), "Failed to decode type.");
    return ::substrait::proto::Type{};
  }
  return typeToProto(ctx->getStart(), *decodedType);
}

std::any SubstraitPlanRelationVisitor::visitLiteral_complex_type(
    SubstraitPlanParser::Literal_complex_typeContext* ctx) {
  std::shared_ptr<const ParameterizedType> decodedType;
  try {
    decodedType = Type::decode(ctx->getText());
  } catch (...) {
    errorListener_->addError(ctx->getStart(), "Failed to decode type.");
    return ::substrait::proto::Type{};
  }
  return typeToProto(ctx->getStart(), *decodedType);
}

std::any SubstraitPlanRelationVisitor::visitMap_literal(
    SubstraitPlanParser::Map_literalContext* ctx) {
  ::substrait::proto::Expression_Literal literal;
  literal.mutable_map()->clear_key_values();
  for (auto pair : ctx->map_literal_value()) {
    auto item = ANY_CAST(
        ::substrait::proto::Expression_Literal_Map_KeyValue,
        visitMap_literal_value(pair));
    *literal.mutable_map()->add_key_values() = item;
  }
  return literal;
}

std::any SubstraitPlanRelationVisitor::visitMap_literal_value(
    SubstraitPlanParser::Map_literal_valueContext* ctx) {
  ::substrait::proto::Expression_Literal_Map_KeyValue keyValue;
  auto key = ANY_CAST(
      ::substrait::proto::Expression_Literal, visitConstant(ctx->constant(0)));
  auto value = ANY_CAST(
      ::substrait::proto::Expression_Literal, visitConstant(ctx->constant(1)));
  *keyValue.mutable_key() = key;
  *keyValue.mutable_value() = value;
  return keyValue;
}

std::any SubstraitPlanRelationVisitor::visitStruct_literal(
    SubstraitPlanParser::Struct_literalContext* ctx) {
  ::substrait::proto::Expression_Literal literal;
  for (auto constant : ctx->constant()) {
    auto item = ANY_CAST(
        ::substrait::proto::Expression_Literal, visitConstant(constant));
    *literal.mutable_struct_()->add_fields() = item;
  }
  return literal;
}

std::any SubstraitPlanRelationVisitor::visitColumn_name(
    SubstraitPlanParser::Column_nameContext* ctx) {
  return visitChildren(ctx);
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitConstantWithType(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->NUMBER() != nullptr) {
    literal = visitNumber(ctx->NUMBER(), literalType);
  } else if (ctx->STRING() != nullptr) {
    switch (literalType.kind_case()) {
      case ::substrait::proto::Type::kTimestamp:
        literal = visitTimestamp(ctx);
        break;
      case ::substrait::proto::Type::kTimestampTz:
        literal = visitTimestampTz(ctx);
        break;
      case ::substrait::proto::Type::kDate:
        literal = visitDate(ctx);
        break;
      case ::substrait::proto::Type::kTime:
        literal = visitTime(ctx);
        break;
      default:
        literal = visitString(ctx->STRING(), literalType);
        break;
    }
  } else if (ctx->TRUEVAL() != nullptr) {
    if (literalType.has_bool_()) {
      literal.set_boolean(true);
    } else {
      errorListener_->addError(
          ctx->TRUEVAL()->getSymbol(),
          "Provided value does not match the provided type.");
    }
  } else if (ctx->FALSEVAL() != nullptr) {
    if (literalType.has_bool_()) {
      literal.set_boolean(false);
    } else {
      errorListener_->addError(
          ctx->FALSEVAL()->getSymbol(),
          "Provided value does not match the provided type.");
    }
  } else if (ctx->NULLVAL()) {
    *literal.mutable_null() = literalType;
    setNullable(literal.mutable_null());
  } else {
    switch (literalType.kind_case()) {
      case ::substrait::proto::Type::kStruct:
        literal = visitStruct(ctx, literalType);
        break;
      case ::substrait::proto::Type::kList:
        literal = visitList(ctx, literalType);
        break;
      case ::substrait::proto::Type::kMap:
        literal = visitMap(ctx, literalType);
        break;
      case ::substrait::proto::Type::kIntervalYear:
        literal = visitIntervalYear(ctx);
        break;
      case ::substrait::proto::Type::kIntervalDay:
        literal = visitIntervalDay(ctx);
        break;
      default:
        errorListener_->addError(
            ctx->getStart(),
            "Unsupported type " + std::to_string(literalType.kind_case()) +
                ".");
        break;
    }
  }

  return literal;
}

::substrait::proto::Expression_Literal_Map_KeyValue
SubstraitPlanRelationVisitor::visitMapLiteralValueWithType(
    SubstraitPlanParser::Map_literal_valueContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal_Map_KeyValue keyValue;
  *keyValue.mutable_key() =
      visitConstantWithType(ctx->constant(0), literalType.map().key());
  *keyValue.mutable_value() =
      visitConstantWithType(ctx->constant(1), literalType.map().value());
  return keyValue;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitMapLiteralWithType(
    SubstraitPlanParser::Map_literalContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;
  if (ctx->map_literal_value().empty()) {
    *literal.mutable_empty_map() = literalType.map();
    return literal;
  }
  for (const auto& keyValue : ctx->map_literal_value()) {
    *literal.mutable_map()->add_key_values() =
        visitMapLiteralValueWithType(keyValue, literalType);
  }
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitStructLiteralWithType(
    SubstraitPlanParser::Struct_literalContext* ctx,
    const ::substrait::proto::Type_Struct& literalType) {
  ::substrait::proto::Expression_Literal literal;
  if (ctx->constant().empty()) {
    return literal;
  }
  for (int i = 0; i < ctx->constant().size(); ++i) {
    if (i >= literalType.types().size()) {
      break;
    }
    *literal.mutable_struct_()->add_fields() =
        visitConstantWithType(ctx->constant(i), literalType.types(i));
  }
  if (ctx->constant().size() != literalType.types().size()) {
    errorListener_->addError(
        ctx->getStart(),
        "The number of fields does not match the number of types in this "
        "struct.");
  }
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitString(
    antlr4::tree::TerminalNode* node,
    const ::substrait::proto::Type& literalType) {
  std::string input = node->getText();

  // First remove the surrounding quote marks.
  std::string str;
  if (startsWith(input, "```")) {
    str = input.substr(3, input.length() - 6);
  } else if (startsWith(input, "``")) {
    str = input.substr(2, input.length() - 4);
  } else if (input[0] == '"' || input[0] == '`') {
    str = input.substr(1, input.length() - 2);
  } else {
    str = input;
  }

  // Perform escapes if necessary.
  std::string resultStr;
  if (startsWith(input, "`")) {
    // Don't perform escapes on raw strings.
    resultStr = str;
  } else {
    resultStr = escapeText(node, str);
  }

  ::substrait::proto::Expression_Literal literal;
  switch (literalType.kind_case()) {
    case ::substrait::proto::Type::kString: {
      literal.set_string(resultStr);
      break;
    }
    case ::substrait::proto::Type::kFixedChar: {
      literal.set_fixed_char(resultStr);
      break;
    }
    case ::substrait::proto::Type::kVarchar: {
      literal.mutable_var_char()->set_value(resultStr);
      literal.mutable_var_char()->set_length(literalType.varchar().length());
      break;
    }
    case ::substrait::proto::Type::kBinary: {
      literal.set_binary(resultStr);
      break;
    }
    case ::substrait::proto::Type::kFixedBinary: {
      literal.set_fixed_binary(resultStr);
      break;
    }
    case ::substrait::proto::Type::kUuid: {
      // Remove all dashes for consistency.
      std::string modifiedStr = resultStr;
      modifiedStr.erase(
          std::remove_if(
              modifiedStr.begin(),
              modifiedStr.end(),
              [](unsigned char c) -> bool { return c == '-'; }),
          modifiedStr.end());
      if (std::find_if(
              modifiedStr.begin(), modifiedStr.end(), [](unsigned char c) {
                return !std::isxdigit(c);
              }) != modifiedStr.end()) {
        errorListener_->addError(
            node->getSymbol(),
            "UUIDs should be be specified with hexadecimal characters with "
            "optional dashes only.");
        modifiedStr = "0";
      } else if (modifiedStr.length() != 32) {
        errorListener_->addError(
            node->getSymbol(),
            "UUIDs are 128 bits long and thus should be specified with exactly "
            "32 hexadecimal digits.");
        modifiedStr = modifiedStr.substr(0, 32);
      }
      literal.set_uuid(modifiedStr);
      break;
    }
    case ::substrait::proto::Type::kTimestampTz:
    case ::substrait::proto::Type::kTimestamp:
    case ::substrait::proto::Type::kDate:
    case ::substrait::proto::Type::kTime:
      literal.set_string(resultStr);
      break;
    case ::substrait::proto::Type::KIND_NOT_SET:
      // Use of an unrecognized symbol is handled more generally elsewhere.
      break;
    default:
      errorListener_->addError(
          node->getSymbol(),
          "Unexpected string type: " + std::to_string(literalType.kind_case()));
      break;
  }

  return literal;
}

std::string SubstraitPlanRelationVisitor::escapeText(
    const antlr4::tree::TerminalNode* node,
    const std::string& str) {
  std::stringstream result;

  for (std::size_t i = 0; i < str.length(); ++i) {
    if (str[i] == '\\' && i < str.length() - 1) {
      switch (str[i + 1]) {
        case '\\':
          result << '\\';
          break;
        case '\'':
          result << '\'';
          break;
        case '"':
          result << '"';
          break;
        case 'b':
          result << '\b';
          break;
        case 'f':
          result << '\f';
          break;
        case 'n':
          result << '\n';
          break;
        case 'r':
          result << '\r';
          break;
        case 't':
          result << '\t';
          break;
        case 'x':
          if (i < str.length() - 3) {
            int32_t hex;
            if (absl::SimpleHexAtoi(str.substr(i + 2, 2), &hex)) {
              result << static_cast<char>(hex & 0xff);
              i += 3;
              continue;
            }
            // This is an invalid sequence - leave escaped.
          }
          // This is an unrecognized sequence - leave escaped.
          result << str[i];
          result << str[i + 1];
          break;
        case 'u':
          if (i < str.length() - 3) {
            if (str[i + 2] != '{') {
              break;
            }
            auto substr = str.substr(i + 3);
            size_t x = substr.find('}');
            int64_t hex;
            if (x > 0 && absl::SimpleHexAtoi(substr.substr(0, x), &hex)) {
              if (x <= 2) {
                result << static_cast<char>(hex & 0xff);
              } else if (x <= 4) {
                result << static_cast<char>(hex >> 8 & 0xff);
                result << static_cast<char>(hex & 0xff);
              } else if (x <= 6) {
                result << static_cast<char>(hex >> 16 & 0xff);
                result << static_cast<char>(hex >> 8 & 0xff);
                result << static_cast<char>(hex & 0xff);
              }
              i += x + 3;
              continue;
            }
            // Drop through to handle the invalid sequence.
          }
        default:
          this->errorListener_->SubstraitErrorListener::addError(
              node->getSymbol()->getLine(),
              node->getSymbol()->getCharPositionInLine() + i + 1,
              "Unknown slash escape sequence.");
          result << str[i];
          result << str[i + 1];
          break;
      }
      ++i; // Increments for the character after the slash.
    } else {
      result << str[i];
    }
  }

  return result.str();
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitNumber(
    antlr4::tree::TerminalNode* node,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;
  try {
    switch (literalType.kind_case()) {
      case ::substrait::proto::Type::kI8: {
        int32_t val = std::stoi(node->getText());
        literal.set_i8(val);
        if (literalType.i8().nullability()) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kI16: {
        int32_t val = std::stoi(node->getText());
        literal.set_i16(val);
        if (literalType.i16().nullability()) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kI32: {
        int32_t val = std::stoi(node->getText());
        literal.set_i32(val);
        if (literalType.i32().nullability()) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kI64: {
        int64_t val = std::stol(node->getText());
        literal.set_i64(val);
        if (literalType.i64().nullability()) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kFp32: {
        float val = std::stof(node->getText());
        literal.set_fp32(val);
        if (literalType.fp32().nullability()) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kFp64: {
        double val = std::stod(node->getText());
        literal.set_fp64(val);
        if (literalType.fp64().nullability()) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kDecimal: {
        auto decimal = ::io::substrait::expression::DecimalLiteral::fromString(
            node->getText(),
            literalType.decimal().precision(),
            literalType.decimal().scale());
        if (!decimal.isValid()) {
          errorListener_->addError(
              node->getSymbol(), "Could not parse literal as decimal.");
          break;
        }
        *literal.mutable_decimal() = decimal.toProto();
        if (literalType.decimal().nullability()) {
          literal.set_nullable(true);
        }
        break;
      }
      default:
        break;
    }
  } catch (...) {
    errorListener_->addError(
        node->getSymbol(), "Could not parse as a numeric literal.");
  }

  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitList(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->map_literal() != nullptr) {
    auto mapLiteral = ANY_CAST(
        ::substrait::proto::Expression_Literal,
        visitMap_literal(ctx->map_literal()));
    if (!mapLiteral.map().key_values().empty()) {
      errorListener_->addError(
          ctx->getStart(), "Lists only accept literals separated by commas.");
    }
    *literal.mutable_empty_list()->mutable_type() = literalType.list().type();
    literal.mutable_empty_list()->set_nullability(
        literalType.list().nullability());
  } else if (ctx->struct_literal() != nullptr) {
    for (const auto& constant : ctx->struct_literal()->constant()) {
      *literal.mutable_list()->add_values() =
          visitConstantWithType(constant, literalType.list().type());
    }
  }

  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitMap(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->map_literal() != nullptr) {
    literal = ANY_CAST(
        ::substrait::proto::Expression_Literal,
        visitMapLiteralWithType(ctx->map_literal(), literalType));
  } else if (ctx->struct_literal() != nullptr) {
    errorListener_->addError(
        ctx->getStart(),
        "Map literals require pairs of values separated by colons.");

    *literal.mutable_empty_map() = literalType.map();
  } else {
    errorListener_->addError(ctx->getStart(), "Unrecognized map construction.");
    *literal.mutable_empty_map() = literalType.map();
  }

  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitStruct(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->map_literal() != nullptr) {
    // We know that there this is an empty literal, treat as such.
    literal.mutable_struct_()->clear_fields();
  } else if (ctx->struct_literal() != nullptr) {
    literal = visitStructLiteralWithType(
        ctx->struct_literal(), literalType.struct_());
    // Validate that the literal matches the type.
  }

  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitIntervalYear(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_struct_()->add_types()->mutable_i32();
  literalType.mutable_struct_()->add_types()->mutable_i32();

  auto s = visitStruct(ctx, literalType);

  ::substrait::proto::Expression_Literal literal;
  literal.mutable_interval_year_to_month()->set_years(
      s.struct_().fields(0).i32());
  literal.mutable_interval_year_to_month()->set_months(
      s.struct_().fields(1).i32());
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitIntervalDay(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_struct_()->add_types()->mutable_i32();
  literalType.mutable_struct_()->add_types()->mutable_i32();
  literalType.mutable_struct_()->add_types()->mutable_i32();

  auto s = visitStruct(ctx, literalType);

  ::substrait::proto::Expression_Literal literal;
  literal.mutable_interval_day_to_second()->set_days(
      s.struct_().fields(0).i32());
  literal.mutable_interval_day_to_second()->set_seconds(
      s.struct_().fields(1).i32());
  literal.mutable_interval_day_to_second()->set_microseconds(
      s.struct_().fields(2).i32());
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitTimestamp(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_timestamp();

  auto s = visitString(ctx->STRING(), literalType);

  date::sys_seconds timePoint;
  std::istringstream ss(s.string());
  ss >> date::parse("%Y-%m-%d %H:%M:%S", timePoint);
  if (ss.fail()) {
    errorListener_->addError(ctx->getStart(), "Could not parse timestamp.");
    return {};
  }

  ::substrait::proto::Expression_Literal literal;
  literal.set_timestamp(std::chrono::system_clock::to_time_t(timePoint));
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitTimestampTz(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_timestamp_tz();

  auto s = visitString(ctx->STRING(), literalType);

  date::local_time<std::chrono::microseconds> localTimePoint;
  std::string tzName;
  std::istringstream ss{s.string()};
  ss >> date::parse("%Y-%m-%d %H:%M:%S %Z", localTimePoint, tzName);
  if (ss.fail()) {
    errorListener_->addError(
        ctx->getStart(), "Could not parse timestamp with timezone.");
    return {};
  }
  // TODO -- Support timezones like -0100 using %z.  #60

  ::substrait::proto::Expression_Literal literal;
  try {
    auto timePoint = date::locate_zone(tzName)->to_sys(localTimePoint);
    literal.set_timestamp_tz(std::chrono::system_clock::to_time_t(timePoint));
  } catch (...) {
    errorListener_->addError(
        ctx->getStart(), "Failed to locate timezone in the TZ database.");
  }
  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitDate(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_date();

  auto s = visitString(ctx->STRING(), literalType);

  date::sys_days timePoint;
  std::istringstream ss(s.string());
  ss >> date::parse("%Y-%m-%d", timePoint);
  if (ss.fail()) {
    errorListener_->addError(ctx->getStart(), "Could not parse date.");
    return {};
  }

  ::substrait::proto::Expression_Literal literal;
  auto duration = timePoint - date::sys_days();
  literal.set_date(duration.count());
  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitTime(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_time();

  auto s = visitString(ctx->STRING(), literalType);

  std::chrono::microseconds timeOfDay;
  std::istringstream ss(s.string());
  ss >> date::parse("%H:%M:%S", timeOfDay);
  if (ss.fail()) {
    ss = std::istringstream(s.string());
    ss >> date::parse("%H:%M", timeOfDay);
    if (ss.fail()) {
      errorListener_->addError(ctx->getStart(), "Could not parse time.");
      return {};
    }
  }

  ::substrait::proto::Expression_Literal literal;
  literal.set_time(timeOfDay.count());
  return literal;
}

::substrait::proto::Type SubstraitPlanRelationVisitor::typeToProto(
    const antlr4::Token* token,
    const ParameterizedType& decodedType) {
  ::substrait::proto::Type type;
  auto nullValue = ::substrait::proto::Type_Nullability_NULLABILITY_UNSPECIFIED;
  if (decodedType.nullable()) {
    nullValue = ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE;
  }
  switch (decodedType.kind()) {
    case TypeKind::kBool:
      type.mutable_bool_()->set_nullability(nullValue);
      break;
    case TypeKind::kI8:
      type.mutable_i8()->set_nullability(nullValue);
      break;
    case TypeKind::kI16:
      type.mutable_i16()->set_nullability(nullValue);
      break;
    case TypeKind::kI32:
      type.mutable_i32()->set_nullability(nullValue);
      break;
    case TypeKind::kI64:
      type.mutable_i64()->set_nullability(nullValue);
      break;
    case TypeKind::kFp32:
      type.mutable_fp32()->set_nullability(nullValue);
      break;
    case TypeKind::kFp64:
      type.mutable_fp64()->set_nullability(nullValue);
      break;
    case TypeKind::kString:
      type.mutable_string()->set_nullability(nullValue);
      break;
    case TypeKind::kBinary:
      type.mutable_binary()->set_nullability(nullValue);
      break;
    case TypeKind::kTimestamp:
      type.mutable_timestamp()->set_nullability(nullValue);
      break;
    case TypeKind::kDate:
      type.mutable_date()->set_nullability(nullValue);
      break;
    case TypeKind::kTime:
      type.mutable_time()->set_nullability(nullValue);
      break;
    case TypeKind::kIntervalYear:
      type.mutable_interval_year()->set_nullability(nullValue);
      break;
    case TypeKind::kIntervalDay:
      type.mutable_interval_day()->set_nullability(nullValue);
      break;
    case TypeKind::kTimestampTz:
      type.mutable_timestamp_tz()->set_nullability(nullValue);
      break;
    case TypeKind::kUuid:
      type.mutable_uuid()->set_nullability(nullValue);
      break;
    case TypeKind::kFixedChar: {
      auto fixedChar =
          reinterpret_cast<const ParameterizedFixedChar*>(&decodedType);
      if (fixedChar == nullptr) {
        break;
      }
      try {
        int32_t length = std::stoi(fixedChar->length()->value());
        type.mutable_fixed_char()->set_length(length);
      } catch (...) {
        errorListener_->addError(token, "Could not parse fixedchar length.");
      }
      type.mutable_fixed_char()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kVarchar: {
      auto varChar =
          reinterpret_cast<const ParameterizedFixedChar*>(&decodedType);
      if (varChar == nullptr) {
        break;
      }
      try {
        int32_t length = std::stoi(varChar->length()->value());
        type.mutable_varchar()->set_length(length);
      } catch (...) {
        errorListener_->addError(token, "Could not parse varchar length.");
      }
      type.mutable_varchar()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kFixedBinary:
      type.mutable_fixed_binary()->set_nullability(nullValue);
      break;
    case TypeKind::kDecimal: {
      auto dec = reinterpret_cast<const ParameterizedDecimal*>(&decodedType);
      if (dec == nullptr) {
        break;
      }
      try {
        int32_t precision = std::stoi(dec->precision()->value());
        int32_t scale = std::stoi(dec->scale()->value());
        type.mutable_decimal()->set_precision(precision);
        type.mutable_decimal()->set_scale(scale);
      } catch (...) {
        errorListener_->addError(
            token, "Could not parse decimal precision and scale.");
      }
      type.mutable_decimal()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kStruct: {
      auto structure =
          reinterpret_cast<const ParameterizedStruct*>(&decodedType);
      for (const auto& t : structure->children()) {
        *type.mutable_struct_()->add_types() = typeToProto(token, *t);
      }
      type.mutable_struct_()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kList: {
      auto list = reinterpret_cast<const ParameterizedList*>(&decodedType);
      *type.mutable_list()->mutable_type() =
          typeToProto(token, *list->elementType());
      type.mutable_list()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kMap: {
      auto map = reinterpret_cast<const ParameterizedMap*>(&decodedType);
      *type.mutable_map()->mutable_key() = typeToProto(token, *map->keyType());
      *type.mutable_map()->mutable_value() =
          typeToProto(token, *map->valueType());
      type.mutable_map()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kKindNotSet:
      errorListener_->addError(token, "Unable to recognize requested type.");
      break;
  }
  return type;
}

} // namespace io::substrait::textplan
