/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/TypeConversion.h"

#include "substrait/type/Type.h"

namespace io::substrait::textplan {

namespace {

std::string kNullSignature = "?";

} // namespace

// Converts a Type proto directly into its text signature form.
std::string typeToText(const ::substrait::proto::Type& type) {
  switch (type.kind_case()) {
    case ::substrait::proto::Type::kBool:
      if (type.bool_().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kBool>::kSignature + kNullSignature;
      }
      return TypeTraits<TypeKind::kBool>::kSignature;
    case ::substrait::proto::Type::kI8:
      if (type.i8().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kI8>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kI8>::kTypeString;
    case ::substrait::proto::Type::kI16:
      if (type.i16().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kI16>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kI16>::kTypeString;
    case ::substrait::proto::Type::kI32:
      if (type.i32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kI32>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kI32>::kTypeString;
    case ::substrait::proto::Type::kI64:
      if (type.i64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kI64>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kI64>::kTypeString;
    case ::substrait::proto::Type::kFp32:
      if (type.fp32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kFp32>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kFp32>::kTypeString;
    case ::substrait::proto::Type::kFp64:
      if (type.fp64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kFp64>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kFp64>::kTypeString;
    case ::substrait::proto::Type::kString:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kString>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kString>::kTypeString;
    case ::substrait::proto::Type::kBinary:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kBinary>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kBinary>::kTypeString;
    case ::substrait::proto::Type::kTimestamp:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kTimestamp>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kTimestamp>::kTypeString;
    case ::substrait::proto::Type::kDate:
      if (type.date().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kDate>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kDate>::kTypeString;
    case ::substrait::proto::Type::kTime:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kTime>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kTime>::kTypeString;
    case ::substrait::proto::Type::kIntervalYear:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kIntervalYear>::kTypeString +
            kNullSignature;
      }
      return TypeTraits<TypeKind::kIntervalYear>::kTypeString;
    case ::substrait::proto::Type::kIntervalDay:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kIntervalDay>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kIntervalDay>::kTypeString;
    case ::substrait::proto::Type::kTimestampTz:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kTimestampTz>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kTimestampTz>::kTypeString;
    case ::substrait::proto::Type::kUuid:
      if (type.fixed_char().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kUuid>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kUuid>::kTypeString;
    case ::substrait::proto::Type::kFixedChar:
      if (type.fixed_char().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kFixedChar>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kFixedChar>::kTypeString;
    case ::substrait::proto::Type::kVarchar:
      if (type.varchar().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kVarchar>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kVarchar>::kTypeString;
    case ::substrait::proto::Type::kFixedBinary:
      if (type.varchar().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kFixedBinary>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kFixedBinary>::kTypeString;
    case ::substrait::proto::Type::kDecimal:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return TypeTraits<TypeKind::kDecimal>::kTypeString + kNullSignature;
      }
      return TypeTraits<TypeKind::kDecimal>::kTypeString;
    case ::substrait::proto::Type::kStruct:
    case ::substrait::proto::Type::kList:
    case ::substrait::proto::Type::kMap:
      return "UNSUPPORTED_TYPE";
    case ::substrait::proto::Type::kUserDefined:
    case ::substrait::proto::Type::kUserDefinedTypeReference:
    case ::substrait::proto::Type::KIND_NOT_SET:
      break;
  }
  return "UNSUPPORTED_TYPE";
};

} // namespace io::substrait::textplan
