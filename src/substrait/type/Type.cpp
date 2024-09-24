/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/type/Type.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "substrait/common/Exceptions.h"
#include "substrait/common/NumberUtils.h"
#include "substrait/common/StringUtils.h"

namespace io::substrait {

namespace {

size_t findNextComma(std::string_view str, size_t start) {
  int cnt = 0;
  for (auto i = start; i < str.size(); i++) {
    if (str[i] == '<') {
      cnt++;
    } else if (str[i] == '>') {
      cnt--;
    } else if (cnt == 0 && str[i] == ',') {
      return i;
    }
  }

  return std::string::npos;
}

template <TypeKind Kind>
ParameterizedTypePtr decodeType(bool nullable) {
  return std::make_shared<const ScalarType<Kind>>(nullable);
}

template <TypeKind Kind>
ParameterizedTypePtr decodeType(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  SUBSTRAIT_UNSUPPORTED(
      std::string("Unsupported parameter type: ") +
      TypeTraits<Kind>::kTypeString);
}

template <>
ParameterizedTypePtr decodeType<TypeKind::kList>(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  if (isParameterized) {
    return std::make_shared<ParameterizedList>(parameterTypes[0], nullable);
  } else {
    return std::make_shared<List>(
        std::dynamic_pointer_cast<const Type>(parameterTypes[0]), nullable);
  }
}

template <>
ParameterizedTypePtr decodeType<TypeKind::kMap>(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  if (isParameterized) {
    if (parameterTypes.size() >= 2) {
      return std::make_shared<ParameterizedMap>(
          parameterTypes[0], parameterTypes[1], nullable);
    } else if (!parameterTypes.empty()) {
      return std::make_shared<ParameterizedMap>(
          parameterTypes[0], nullptr, nullable);
    } else {
      return std::make_shared<ParameterizedMap>(nullptr, nullptr, nullable);
    }
  } else {
    return std::make_shared<Map>(
        std::dynamic_pointer_cast<const Type>(parameterTypes[0]),
        std::dynamic_pointer_cast<const Type>(parameterTypes[1]),
        nullable);
  }
}

template <>
ParameterizedTypePtr decodeType<TypeKind::kStruct>(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  if (isParameterized) {
    return std::make_shared<const ParameterizedStruct>(
        parameterTypes, nullable);
  } else {
    std::vector<TypePtr> types;
    types.reserve(parameterTypes.size());
    for (const auto& parameterType : parameterTypes) {
      types.emplace_back(std::dynamic_pointer_cast<const Type>(parameterType));
    }
    return std::make_shared<Struct>(types, nullable);
  }
}

template <>
ParameterizedTypePtr decodeType<TypeKind::kDecimal>(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  if (parameterTypes.size() != 2) {
    SUBSTRAIT_FAIL(
        "Decimal types should have both precision and scale specifiers.")
  }
  auto precision =
      std::dynamic_pointer_cast<const StringLiteral>(parameterTypes[0]);
  auto scale =
      std::dynamic_pointer_cast<const StringLiteral>(parameterTypes[1]);

  if (isParameterized) {
    return std::make_shared<ParameterizedDecimal>(precision, scale, nullable);
  } else {
    if (common::NumberUtils::isNonNegativeInteger(precision->value()) &&
        common::NumberUtils::isNonNegativeInteger(scale->value())) {
      return std::make_shared<Decimal>(
          std::stoi(precision->value()), std::stoi(scale->value()), nullable);
    } else {
      SUBSTRAIT_FAIL(
          "Fail decode to Decimal type, precision or scale parameter must be a "
          "positive number")
    }
  }
}

template <class ParameterizedTypeTag, class TypeTag, TypeKind Kind>
ParameterizedTypePtr decodeLengthBaseType(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  auto length =
      std::dynamic_pointer_cast<const StringLiteral>(parameterTypes[0]);
  if (isParameterized) {
    return std::make_shared<ParameterizedTypeTag>(length, nullable);
  } else {
    if (common::NumberUtils::isNonNegativeInteger(length->value())) {
      return std::make_shared<TypeTag>(std::stoi(length->value()), nullable);
    } else {
      SUBSTRAIT_FAIL(
          "Fail decode to {} type, length parameter must be a positive integer",
          TypeTraits<Kind>::kTypeString);
    }
  }
}

template <>
ParameterizedTypePtr decodeType<TypeKind::kVarchar>(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  return decodeLengthBaseType<
      ParameterizedVarchar,
      Varchar,
      TypeKind::kVarchar>(isParameterized, nullable, parameterTypes);
}

template <>
ParameterizedTypePtr decodeType<TypeKind::kFixedChar>(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  return decodeLengthBaseType<
      ParameterizedFixedChar,
      class FixedChar,
      TypeKind::kFixedChar>(isParameterized, nullable, parameterTypes);
}

template <>
ParameterizedTypePtr decodeType<TypeKind::kFixedBinary>(
    bool isParameterized,
    bool nullable,
    const std::vector<ParameterizedTypePtr>& parameterTypes) {
  return decodeLengthBaseType<
      ParameterizedFixedBinary,
      class FixedBinary,
      TypeKind::kBinary>(isParameterized, nullable, parameterTypes);
}

} // namespace

ParameterizedTypePtr ParameterizedType::decode(
    const std::string& value,
    bool isParameterized) {
  std::string rawType =
      static_cast<std::string>(common::StringUtils::trim(value));
  std::string matchingType = rawType;
  std::transform(
      matchingType.begin(),
      matchingType.end(),
      matchingType.begin(),
      [](unsigned char c) { return std::tolower(c); });

  const auto& questionMaskPos = matchingType.find_last_of('?');

  const auto& leftAngleBracketPos = matchingType.find('<');
  if (leftAngleBracketPos == std::string::npos) {
    bool nullable;
    if (matchingType.empty()) {
      nullable = false;
    } else {
      nullable = matchingType.back() == '?';
    }
    // deal with type and with a question mask like "i32?".
    const auto& baseType = nullable
        ? matchingType = matchingType.substr(0, questionMaskPos)
        : matchingType;

    if (TypeTraits<TypeKind::kBool>::kTypeString == baseType ||
        TypeTraits<TypeKind::kBool>::kSignature == baseType) {
      return decodeType<TypeKind::kBool>(nullable);
    } else if (TypeTraits<TypeKind::kI8>::kTypeString == baseType) {
      return decodeType<TypeKind::kI8>(nullable);
    } else if (TypeTraits<TypeKind::kI16>::kTypeString == baseType) {
      return decodeType<TypeKind::kI16>(nullable);
    } else if (TypeTraits<TypeKind::kI32>::kTypeString == baseType) {
      return decodeType<TypeKind::kI32>(nullable);
    } else if (TypeTraits<TypeKind::kI64>::kTypeString == baseType) {
      return decodeType<TypeKind::kI64>(nullable);
    } else if (TypeTraits<TypeKind::kFp32>::kTypeString == baseType) {
      return decodeType<TypeKind::kFp32>(nullable);
    } else if (TypeTraits<TypeKind::kFp64>::kTypeString == baseType) {
      return decodeType<TypeKind::kFp64>(nullable);
    } else if (
        TypeTraits<TypeKind::kString>::kTypeString == baseType ||
        TypeTraits<TypeKind::kString>::kSignature == baseType) {
      return decodeType<TypeKind::kString>(nullable);
    } else if (TypeTraits<TypeKind::kBinary>::kTypeString == baseType) {
      return decodeType<TypeKind::kBinary>(nullable);
    } else if (TypeTraits<TypeKind::kUuid>::kTypeString == baseType) {
      return decodeType<TypeKind::kUuid>(nullable);
    } else if (TypeTraits<TypeKind::kIntervalYear>::kTypeString == baseType) {
      return decodeType<TypeKind::kIntervalYear>(nullable);
    } else if (TypeTraits<TypeKind::kIntervalDay>::kTypeString == baseType) {
      return decodeType<TypeKind::kIntervalDay>(nullable);
    } else if (TypeTraits<TypeKind::kTimestamp>::kTypeString == baseType) {
      return decodeType<TypeKind::kTimestamp>(nullable);
    } else if (TypeTraits<TypeKind::kTimestampTz>::kTypeString == baseType) {
      return decodeType<TypeKind::kTimestampTz>(nullable);
    } else if (TypeTraits<TypeKind::kDate>::kTypeString == baseType) {
      return decodeType<TypeKind::kDate>(nullable);
    } else if (TypeTraits<TypeKind::kTime>::kTypeString == baseType) {
      return decodeType<TypeKind::kTime>(nullable);
    } else {
      bool wildcard = matchingType.rfind("any", 0) == 0;
      bool placeholder = !wildcard && !common::NumberUtils::isInteger(rawType);
      return std::make_shared<const StringLiteral>(
          rawType, wildcard, placeholder);
    }
  } else {
    bool nullable =
        leftAngleBracketPos > 1 && matchingType[leftAngleBracketPos - 1] == '?';
    const auto& rightAngleBracketPos = rawType.rfind('>');
    const auto& baseTypePos = nullable
        ? std::min(leftAngleBracketPos, questionMaskPos)
        : leftAngleBracketPos;

    const auto& baseType = matchingType.substr(0, baseTypePos);

    std::vector<ParameterizedTypePtr> nestedTypes;
    auto prevPos = leftAngleBracketPos + 1;
    auto commaPos = findNextComma(rawType, prevPos);
    while (commaPos != std::string::npos) {
      auto token = rawType.substr(prevPos, commaPos - prevPos);
      nestedTypes.emplace_back(decode(token, isParameterized));
      prevPos = commaPos + 1;
      commaPos = findNextComma(rawType, prevPos);
    }
    auto token = rawType.substr(prevPos, rightAngleBracketPos - prevPos);
    nestedTypes.emplace_back(decode(token, isParameterized));

    if (TypeTraits<TypeKind::kList>::kTypeString == baseType) {
      return decodeType<TypeKind::kList>(
          isParameterized, nullable, nestedTypes);
    } else if (TypeTraits<TypeKind::kMap>::kTypeString == baseType) {
      return decodeType<TypeKind::kMap>(isParameterized, nullable, nestedTypes);
    } else if (TypeTraits<TypeKind::kStruct>::kTypeString == baseType) {
      return decodeType<TypeKind::kStruct>(
          isParameterized, nullable, nestedTypes);
    } else if (TypeTraits<TypeKind::kDecimal>::kTypeString == baseType) {
      return decodeType<TypeKind::kDecimal>(
          isParameterized, nullable, nestedTypes);
    } else if (TypeTraits<TypeKind::kVarchar>::kTypeString == baseType) {
      return decodeType<TypeKind::kVarchar>(
          isParameterized, nullable, nestedTypes);
    } else if (TypeTraits<TypeKind::kFixedChar>::kTypeString == baseType) {
      return decodeType<TypeKind::kFixedChar>(
          isParameterized, nullable, nestedTypes);
    } else if (TypeTraits<TypeKind::kFixedBinary>::kTypeString == baseType) {
      return decodeType<TypeKind::kFixedBinary>(
          isParameterized, nullable, nestedTypes);
    } else {
      SUBSTRAIT_UNSUPPORTED("Unsupported type: " + rawType);
    }
  }
}

std::string Decimal::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<" << precision_ << "," << scale_ << ">";
  return sign.str();
}

bool Decimal::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto decimalType = std::dynamic_pointer_cast<const Decimal>(type)) {
    return TypeBase::isMatch(type) && precision_ == decimalType->precision() &&
        scale_ == decimalType->scale();
  }

  return false;
}

std::string FixedBinary::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<" << length() << ">";
  return sign.str();
}

bool FixedBinary::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto fBinaryType = std::dynamic_pointer_cast<const FixedBinary>(type)) {
    return TypeBase::isMatch(type) && length_ == fBinaryType->length();
  }

  return false;
}

std::string FixedChar::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<" << length() << ">";
  return sign.str();
}

bool FixedChar::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto fBinaryType = std::dynamic_pointer_cast<const FixedChar>(type)) {
    return TypeBase::isMatch(type) && length_ == fBinaryType->length();
  }
  return false;
}

std::string Varchar::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<" << length() << ">";
  return sign.str();
}

bool Varchar::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto varcharType = std::dynamic_pointer_cast<const Varchar>(type)) {
    return TypeBase::isMatch(type) && length_ == varcharType->length();
  }
  return false;
}

std::string List::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<" << elementType_->signature() << ">";
  return sign.str();
}

bool List::isMatch(const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto listType = std::dynamic_pointer_cast<const List>(type)) {
    return TypeBase::isMatch(type) &&
        elementType()->isMatch(listType->elementType());
  }
  return false;
}

std::string Struct::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<";
  for (auto it = children_.begin(); it != children_.end(); ++it) {
    const auto& typeSign = (*it)->signature();
    if (it == children_.end() - 1) {
      sign << typeSign;
    } else {
      sign << typeSign << ",";
    }
  }
  sign << ">";
  return sign.str();
}

bool Struct::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto structType = std::dynamic_pointer_cast<const Struct>(type)) {
    bool sameSize = structType->children_.size() == children_.size();
    if (sameSize) {
      for (int i = 0; i < children_.size(); i++) {
        if (!children_[i]->isMatch(structType->children_[i])) {
          return false;
        }
      }
      return true;
    }
  }
  return false;
}

std::string Map::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<";
  sign << keyType()->signature();
  sign << ",";
  sign << valueType()->signature();
  sign << ">";
  return sign.str();
}

bool Map::isMatch(const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto mapType = std::dynamic_pointer_cast<const Map>(type)) {
    return TypeBase::isMatch(type) && keyType()->isMatch(mapType->keyType()) &&
        valueType()->isMatch(mapType->valueType());
  }
  return false;
}

std::string ParameterizedFixedBinary::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kFixedBinary>::kSignature;
  sign << "<" << length_->value() << ">";
  return sign.str();
}

bool ParameterizedFixedBinary::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto parameterizedFixedBinary =
          std::dynamic_pointer_cast<const ParameterizedFixedBinary>(type)) {
    return length()->isMatch(parameterizedFixedBinary->length()) &&
        nullMatch(type);
  }

  return false;
}

std::string ParameterizedDecimal::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kDecimal>::kSignature;
  sign << "<" << precision_->value() << "," << scale_->value() << ">";
  return sign.str();
}

bool ParameterizedDecimal::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto decimal = std::dynamic_pointer_cast<const Decimal>(type)) {
    return nullMatch(type);
  }

  return false;
}

std::string ParameterizedFixedChar::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kFixedChar>::kSignature;
  sign << "<" << length_->value() << ">";
  return sign.str();
}

bool ParameterizedFixedChar::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto fixedChar = std::dynamic_pointer_cast<const class FixedChar>(type)) {
    return nullMatch(type);
  }

  return false;
}

std::string ParameterizedVarchar::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kVarchar>::kSignature;
  sign << "<" << length_->value() << ">";
  return sign.str();
}

bool ParameterizedVarchar::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto varchar = std::dynamic_pointer_cast<const Varchar>(type)) {
    return nullMatch(type);
  }

  return false;
}

std::string ParameterizedList::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kList>::kSignature;
  sign << "<" << elementType()->signature() << ">";
  return sign.str();
}

bool ParameterizedList::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto list = std::dynamic_pointer_cast<const List>(type)) {
    return elementType()->isMatch(list->elementType()) && nullMatch(type);
  }

  return false;
}

std::string ParameterizedStruct::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kStruct>::kSignature;
  sign << "<";
  for (auto it = children_.begin(); it != children_.end(); ++it) {
    const auto& typeSign = (*it)->signature();
    if (it == children_.end() - 1) {
      sign << typeSign;
    } else {
      sign << typeSign << ",";
    }
  }
  sign << ">";
  return sign.str();
}

bool ParameterizedStruct::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto structType = std::dynamic_pointer_cast<const Struct>(type)) {
    bool sameSize = structType->children().size() == children_.size();
    if (sameSize) {
      for (int i = 0; i < children_.size(); i++) {
        if (!children_[i]->isMatch(structType->children()[i])) {
          return false;
        }
      }
      return nullMatch(type);
    }
  }
  return false;
}

std::string ParameterizedMap::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kMap>::kSignature;
  sign << "<";
  if (keyType() != nullptr) {
    sign << keyType()->signature();
  }
  sign << ",";
  if (valueType() != nullptr) {
    sign << valueType()->signature();
  }
  sign << ">";
  return sign.str();
}

bool ParameterizedMap::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto mapType = std::dynamic_pointer_cast<const Map>(type)) {
    return keyType()->isMatch(mapType->keyType()) &&
        valueType()->isMatch(mapType->valueType()) && nullMatch(type);
  }
  return false;
}

std::shared_ptr<const ScalarType<TypeKind::kBool>> boolean() {
  return std::make_shared<const ScalarType<TypeKind::kBool>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI8>> tinyint() {
  return std::make_shared<const ScalarType<TypeKind::kI8>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI16>> smallint() {
  return std::make_shared<const ScalarType<TypeKind::kI16>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI32>> integer() {
  return std::make_shared<const ScalarType<TypeKind::kI32>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI64>> bigint() {
  return std::make_shared<const ScalarType<TypeKind::kI64>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kFp32>> float4() {
  return std::make_shared<const ScalarType<TypeKind::kFp32>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kFp64>> float8() {
  return std::make_shared<const ScalarType<TypeKind::kFp64>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kString>> string() {
  return std::make_shared<const ScalarType<TypeKind::kString>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kBinary>> binary() {
  return std::make_shared<const ScalarType<TypeKind::kBinary>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kTimestamp>> timestamp() {
  return std::make_shared<const ScalarType<TypeKind::kTimestamp>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kDate>> date() {
  return std::make_shared<const ScalarType<TypeKind::kDate>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kTime>> time() {
  return std::make_shared<const ScalarType<TypeKind::kTime>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kIntervalYear>> intervalYear() {
  return std::make_shared<const ScalarType<TypeKind::kIntervalYear>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kIntervalDay>> intervalDay() {
  return std::make_shared<const ScalarType<TypeKind::kIntervalDay>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kTimestampTz>> timestampTz() {
  return std::make_shared<const ScalarType<TypeKind::kTimestampTz>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kUuid>> uuid() {
  return std::make_shared<const ScalarType<TypeKind::kUuid>>(false);
}

std::shared_ptr<const Decimal> decimal(int precision, int scale) {
  return std::make_shared<const Decimal>(precision, scale, false);
}

std::shared_ptr<const Varchar> varchar(int len) {
  return std::make_shared<const Varchar>(len, false);
}

std::shared_ptr<const class FixedChar> fchar(int len) {
  return std::make_shared<const class FixedChar>(len, false);
}

std::shared_ptr<const class FixedBinary> fixedBinary(int len) {
  return std::make_shared<const class FixedBinary>(len, false);
}

std::shared_ptr<const List> list(const TypePtr& elementType) {
  return std::make_shared<const List>(elementType, false);
}

std::shared_ptr<const Map> map(
    const TypePtr& keyType,
    const TypePtr& valueType) {
  return std::make_shared<const Map>(keyType, valueType, false);
}

std::shared_ptr<const Struct> row(const std::vector<TypePtr>& children) {
  return std::make_shared<const Struct>(children, false);
}

std::shared_ptr<const class FixedChar> fixedChar(int len) {
  return std::make_shared<const class FixedChar>(len);
}

bool StringLiteral::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (isWildcard()) {
    return true;
  } else {
    if (auto stringLiteral =
            std::dynamic_pointer_cast<const StringLiteral>(type)) {
      return value_ == stringLiteral->value_;
    }
    return false;
  }
}

bool StringLiteral::isInteger() const {
  return common::NumberUtils::isInteger(value_);
}

} // namespace io::substrait
