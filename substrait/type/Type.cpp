/* SPDX-License-Identifier: Apache-2.0 */

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "substrait/type/Type.h"
#include "substrait/common/Exceptions.h"

namespace io::substrait {

namespace {

size_t findNextComma(const std::string& str, size_t start) {
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

} // namespace

ParameterizedTypePtr ParameterizedType::decode(const std::string& rawType) {
  std::string matchingType = rawType;
  std::transform(
      matchingType.begin(),
      matchingType.end(),
      matchingType.begin(),
      [](unsigned char c) { return std::tolower(c); });

  const auto& questionMaskPos = matchingType.find_last_of('?');

  bool nullable = questionMaskPos != std::string::npos;

  const auto& leftAngleBracketPos = matchingType.find('<');
  if (leftAngleBracketPos == std::string::npos) {
    // deal with type and with a question mask like "i32?".
    const auto& baseType = nullable
        ? matchingType = matchingType.substr(0, questionMaskPos)
        : matchingType;

    if (TypeTraits<TypeKind::kBool>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kBool>>(nullable);
    } else if (TypeTraits<TypeKind::kI8>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kI8>>(nullable);
    } else if (TypeTraits<TypeKind::kI16>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kI16>>(nullable);
    } else if (TypeTraits<TypeKind::kI32>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kI32>>(nullable);
    } else if (TypeTraits<TypeKind::kI64>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kI64>>(nullable);
    } else if (TypeTraits<TypeKind::kFp32>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kFp32>>(nullable);
    } else if (TypeTraits<TypeKind::kFp64>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kFp64>>(nullable);
    } else if (TypeTraits<TypeKind::kString>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kString>>(nullable);
    } else if (TypeTraits<TypeKind::kBinary>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kBinary>>(nullable);
    } else if (TypeTraits<TypeKind::kUuid>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kUuid>>(nullable);
    } else if (TypeTraits<TypeKind::kIntervalYear>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kIntervalYear>>(
          nullable);
    } else if (TypeTraits<TypeKind::kIntervalDay>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kIntervalDay>>(
          nullable);
    } else if (TypeTraits<TypeKind::kTimestamp>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kTimestamp>>(nullable);
    } else if (TypeTraits<TypeKind::kTimestampTz>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kTimestampTz>>(
          nullable);
    } else if (TypeTraits<TypeKind::kDate>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kDate>>(nullable);
    } else if (TypeTraits<TypeKind::kTime>::typeString == baseType) {
      return std::make_shared<const ScalarType<TypeKind::kTime>>(nullable);
    } else {
      return std::make_shared<const StringLiteral>(rawType);
    }
  } else {
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
      nestedTypes.emplace_back(decode(token));
      prevPos = commaPos + 1;
      commaPos = findNextComma(rawType, prevPos);
    }
    auto token = rawType.substr(prevPos, rightAngleBracketPos - prevPos);
    nestedTypes.emplace_back(decode(token));

    if (TypeTraits<TypeKind::kList>::typeString == baseType) {
      return std::make_shared<ParameterizedList>(nestedTypes[0], nullable);
    } else if (TypeTraits<TypeKind::kMap>::typeString == baseType) {
      return std::make_shared<ParameterizedMap>(
          nestedTypes[0], nestedTypes[1], nullable);
    } else if (TypeTraits<TypeKind::kStruct>::typeString == baseType) {
      return std::make_shared<ParameterizedStruct>(nestedTypes, nullable);
    } else if (TypeTraits<TypeKind::kDecimal>::typeString == baseType) {
      StringLiteralPtr precision =
          std::dynamic_pointer_cast<const StringLiteral>(nestedTypes[0]);
      StringLiteralPtr scale =
          std::dynamic_pointer_cast<const StringLiteral>(nestedTypes[1]);
      return std::make_shared<ParameterizedDecimal>(precision, scale, nullable);
    } else if (TypeTraits<TypeKind::kVarchar>::typeString == baseType) {
      auto length =
          std::dynamic_pointer_cast<const StringLiteral>(nestedTypes[0]);
      return std::make_shared<ParameterizedVarchar>(length, nullable);
    } else if (TypeTraits<TypeKind::kFixedChar>::typeString == baseType) {
      auto length =
          std::dynamic_pointer_cast<const StringLiteral>(nestedTypes[0]);
      return std::make_shared<ParameterizedFixedChar>(length, nullable);
    } else if (TypeTraits<TypeKind::kFixedBinary>::typeString == baseType) {
      auto length =
          std::dynamic_pointer_cast<const StringLiteral>(nestedTypes[0]);
      return std::make_shared<ParameterizedFixedBinary>(length, nullable);
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
  sign << TypeTraits<TypeKind::kFixedBinary>::signature;
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
  sign << TypeTraits<TypeKind::kDecimal>::signature;
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
  sign << TypeTraits<TypeKind::kFixedChar>::signature;
  sign << "<" << length_->value() << ">";
  return sign.str();
}
bool ParameterizedFixedChar::isMatch(
    const std::shared_ptr<const ParameterizedType>& type) const {
  if (auto fixedChar = std::dynamic_pointer_cast<const FixedChar>(type)) {
    return nullMatch(type);
  }

  return false;
}

std::string ParameterizedVarchar::signature() const {
  std::stringstream sign;
  sign << TypeTraits<TypeKind::kVarchar>::signature;
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
  sign << TypeTraits<TypeKind::kList>::signature;
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
  sign << TypeTraits<TypeKind::kStruct>::signature;
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
  sign << TypeTraits<TypeKind::kMap>::signature;
  sign << "<";
  sign << keyType()->signature();
  sign << ",";
  sign << valueType()->signature();
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

std::shared_ptr<const ScalarType<TypeKind::kBool>> BOOL() {
  return std::make_shared<const ScalarType<TypeKind::kBool>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI8>> TINYINT() {
  return std::make_shared<const ScalarType<TypeKind::kI8>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI16>> SMALLINT() {
  return std::make_shared<const ScalarType<TypeKind::kI16>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI32>> INTEGER() {
  return std::make_shared<const ScalarType<TypeKind::kI32>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kI64>> BIGINT() {
  return std::make_shared<const ScalarType<TypeKind::kI64>>(false);
}
std::shared_ptr<const ScalarType<TypeKind::kFp32>> FLOAT() {
  return std::make_shared<const ScalarType<TypeKind::kFp32>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kFp64>> DOUBLE() {
  return std::make_shared<const ScalarType<TypeKind::kFp64>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kString>> STRING() {
  return std::make_shared<const ScalarType<TypeKind::kString>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kBinary>> BINARY() {
  return std::make_shared<const ScalarType<TypeKind::kBinary>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kTimestamp>> TIMESTAMP() {
  return std::make_shared<const ScalarType<TypeKind::kTimestamp>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kDate>> DATE() {
  return std::make_shared<const ScalarType<TypeKind::kDate>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kTime>> TIME() {
  return std::make_shared<const ScalarType<TypeKind::kTime>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kIntervalYear>> INTERVAL_YEAR() {
  return std::make_shared<const ScalarType<TypeKind::kIntervalYear>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kIntervalDay>> INTERVAL_DAY() {
  return std::make_shared<const ScalarType<TypeKind::kIntervalDay>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kTimestampTz>> TIMESTAMP_TZ() {
  return std::make_shared<const ScalarType<TypeKind::kTimestampTz>>(false);
}

std::shared_ptr<const ScalarType<TypeKind::kUuid>> UUID() {
  return std::make_shared<const ScalarType<TypeKind::kUuid>>(false);
}

std::shared_ptr<const Decimal> DECIMAL(int precision, int scale) {
  return std::make_shared<const Decimal>(precision, scale, false);
}

std::shared_ptr<const Varchar> VARCHAR(int len) {
  return std::make_shared<const Varchar>(len, false);
}
std::shared_ptr<const FixedChar> FCHAR(int len) {
  return std::make_shared<const FixedChar>(len, false);
}

std::shared_ptr<const FixedBinary> FIXED_BINARY(int len) {
  return std::make_shared<const FixedBinary>(len, false);
}

std::shared_ptr<const List> LIST(const TypePtr& elementType) {
  return std::make_shared<const List>(elementType, false);
}

std::shared_ptr<const Map> MAP(
    const TypePtr& keyType,
    const TypePtr& valueType) {
  return std::make_shared<const Map>(keyType, valueType, false);
}

std::shared_ptr<const Struct> STRUCT(const std::vector<TypePtr>& children) {
  return std::make_shared<const Struct>(children, false);
}

std::shared_ptr<const FixedChar> FIXED_CHAR(int len) {
  return std::make_shared<const FixedChar>(len);
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

} // namespace io::substrait
