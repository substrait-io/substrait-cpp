/* SPDX-License-Identifier: Apache-2.0 */

#include "SubstraitPlanTypeVisitor.h"

#include <memory>
#include <string>
#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "substrait/proto/type.pb.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/type/Type.h"

namespace io::substrait::textplan {

std::any SubstraitPlanTypeVisitor::visitLiteral_basic_type(
    SubstraitPlanParser::Literal_basic_typeContext* ctx) {
  return textToTypeProto(ctx, ctx->getText());
}

std::any SubstraitPlanTypeVisitor::visitLiteral_complex_type(
    SubstraitPlanParser::Literal_complex_typeContext* ctx) {
  return textToTypeProto(ctx, ctx->getText());
}

::substrait::proto::Type SubstraitPlanTypeVisitor::textToTypeProto(
    const antlr4::ParserRuleContext* ctx,
    const std::string& typeText) {
  std::shared_ptr<const ParameterizedType> decodedType;
  try {
    decodedType = Type::decode(typeText);
  } catch (...) {
    errorListener_->addError(ctx->getStart(), "Failed to decode type.");
    return ::substrait::proto::Type{};
  }
  return typeToProto(ctx, *decodedType);
}

::substrait::proto::Type SubstraitPlanTypeVisitor::typeToProto(
    const antlr4::ParserRuleContext* ctx,
    const ParameterizedType& decodedType) {
  ::substrait::proto::Type type;
  auto nullValue = ::substrait::proto::Type_Nullability_NULLABILITY_REQUIRED;
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
        errorListener_->addError(
            ctx->getStart(), "Could not parse fixedchar length.");
      }
      type.mutable_fixed_char()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kVarchar: {
      auto varChar =
          reinterpret_cast<const ParameterizedVarchar*>(&decodedType);
      if (varChar == nullptr) {
        break;
      }
      try {
        if (!varChar->length()->isInteger()) {
          errorListener_->addError(
              ctx->getStart(), "Missing varchar length.");
          break;
        }
        int32_t length = std::stoi(varChar->length()->value());
        type.mutable_varchar()->set_length(length);
      } catch (...) {
        errorListener_->addError(
            ctx->getStart(), "Could not parse varchar length.");
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
            ctx->getStart(), "Could not parse decimal precision and scale.");
      }
      type.mutable_decimal()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kStruct: {
      auto structure =
          reinterpret_cast<const ParameterizedStruct*>(&decodedType);
      for (const auto& t : structure->children()) {
        *type.mutable_struct_()->add_types() = typeToProto(ctx, *t);
      }
      type.mutable_struct_()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kList: {
      auto list = reinterpret_cast<const ParameterizedList*>(&decodedType);
      *type.mutable_list()->mutable_type() =
          typeToProto(ctx, *list->elementType());
      type.mutable_list()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kMap: {
      auto map = reinterpret_cast<const ParameterizedMap*>(&decodedType);
      if (map->keyType() == nullptr || map->valueType() == nullptr) {
        errorListener_->addError(
            ctx->getStart(), "Maps require both a key and a value type.");
        break;
      }
      *type.mutable_map()->mutable_key() = typeToProto(ctx, *map->keyType());
      *type.mutable_map()->mutable_value() =
          typeToProto(ctx, *map->valueType());
      type.mutable_map()->set_nullability(nullValue);
      break;
    }
    case TypeKind::kKindNotSet:
      if (!insideStructLiteralWithExternalType(ctx)) {
        errorListener_->addError(
            ctx->getStart(), "Unable to recognize requested type.");
      }
      break;
  }
  return type;
}

bool SubstraitPlanTypeVisitor::insideStructLiteralWithExternalType(
    const antlr4::RuleContext* ctx) {
  if (ctx == nullptr) {
    return false;
  }
  if (ctx->getRuleIndex() == SubstraitPlanParser::RuleConstant &&
      const_cast<SubstraitPlanParser::ConstantContext*>(
          dynamic_cast<const SubstraitPlanParser::ConstantContext*>(ctx))
              ->struct_literal() != nullptr) {
    return true;
  }
  return insideStructLiteralWithExternalType(
      dynamic_cast<antlr4::RuleContext*>(ctx->parent));
}

} // namespace io::substrait::textplan
