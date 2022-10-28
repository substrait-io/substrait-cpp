/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Type.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "../common/Exceptions.h"

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

TypePtr Type::decode(const std::string& rawType) {
  std::string matchingType = rawType;
  const auto& questionMaskPos = rawType.find_last_of('?');
  // deal with type and with a question mask like "i32?".
  if (questionMaskPos != std::string::npos) {
    matchingType = rawType.substr(0, questionMaskPos);
  }
  std::transform(
      matchingType.begin(),
      matchingType.end(),
      matchingType.begin(),
      [](unsigned char c) { return std::tolower(c); });

  const auto& leftAngleBracketPos = rawType.find('<');
  if (leftAngleBracketPos == std::string::npos) {
    const auto& scalarType = scalarTypeMapping().find(matchingType);
    if (scalarType != scalarTypeMapping().end()) {
      return scalarType->second;
    } else if (matchingType.rfind("unknown", 0) == 0) {
      return std::make_shared<const UsedDefinedType>(rawType);
    } else {
      return std::make_shared<const StringLiteralType>(rawType);
    }
  }
  const auto& rightAngleBracketPos = rawType.rfind('>');

  auto baseType = matchingType.substr(0, leftAngleBracketPos);

  std::vector<TypePtr> nestedTypes;
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
    return std::make_shared<ListType>(nestedTypes[0]);
  } else if (TypeTraits<TypeKind::kMap>::typeString == baseType) {
    return std::make_shared<MapType>(nestedTypes[0], nestedTypes[1]);
  } else if (TypeTraits<TypeKind::kDecimal>::typeString == baseType) {
    auto precision =
        std::dynamic_pointer_cast<const StringLiteralType>(nestedTypes[0]);
    auto scale =
        std::dynamic_pointer_cast<const StringLiteralType>(nestedTypes[1]);
    return std::make_shared<DecimalType>(precision, scale);
  } else if (TypeTraits<TypeKind::kVarchar>::typeString == baseType) {
    auto length =
        std::dynamic_pointer_cast<const StringLiteralType>(nestedTypes[0]);
    return std::make_shared<VarcharType>(length);
  } else if (TypeTraits<TypeKind::kFixedChar>::typeString == baseType) {
    auto length =
        std::dynamic_pointer_cast<const StringLiteralType>(nestedTypes[0]);
    return std::make_shared<FixedCharType>(length);
  } else if (TypeTraits<TypeKind::kFixedBinary>::typeString == baseType) {
    auto length =
        std::dynamic_pointer_cast<const StringLiteralType>(nestedTypes[0]);
    return std::make_shared<FixedBinaryType>(length);
  } else if (TypeTraits<TypeKind::kStruct>::typeString == baseType) {
    return std::make_shared<StructType>(nestedTypes);
  } else {
    SUBSTRAIT_UNSUPPORTED("Unsupported substrait type: " + rawType);
  }
}

#define SCALAR_TYPE_MAPPING(typeKind)                   \
  {                                                     \
    TypeTraits<TypeKind::typeKind>::typeString,         \
        std::make_shared<TypeBase<TypeKind::typeKind>>( \
            TypeBase<TypeKind::typeKind>())             \
  }

const std::unordered_map<std::string, TypePtr>& Type::scalarTypeMapping() {
  static const std::unordered_map<std::string, TypePtr> scalarTypeMap{
      SCALAR_TYPE_MAPPING(kBool),
      SCALAR_TYPE_MAPPING(kI8),
      SCALAR_TYPE_MAPPING(kI16),
      SCALAR_TYPE_MAPPING(kI32),
      SCALAR_TYPE_MAPPING(kI64),
      SCALAR_TYPE_MAPPING(kFp32),
      SCALAR_TYPE_MAPPING(kFp64),
      SCALAR_TYPE_MAPPING(kString),
      SCALAR_TYPE_MAPPING(kBinary),
      SCALAR_TYPE_MAPPING(kTimestamp),
      SCALAR_TYPE_MAPPING(kTimestampTz),
      SCALAR_TYPE_MAPPING(kDate),
      SCALAR_TYPE_MAPPING(kTime),
      SCALAR_TYPE_MAPPING(kIntervalDay),
      SCALAR_TYPE_MAPPING(kIntervalYear),
      SCALAR_TYPE_MAPPING(kUuid),
  };
  return scalarTypeMap;
}

std::string FixedBinaryType::signature() const {
  std::stringstream sign;
  sign << TypeBase::signature();
  sign << "<";
  sign << length_->value();
  sign << ">";
  return sign.str();
}

bool FixedBinaryType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type =
          std::dynamic_pointer_cast<const FixedBinaryType>(other)) {
    return true;
  }
  return false;
}

std::string DecimalType::signature() const {
  std::stringstream signature;
  signature << TypeBase::signature();
  signature << "<";
  signature << precision_->value() << "," << scale_->value();
  signature << ">";
  return signature.str();
}

bool DecimalType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type = std::dynamic_pointer_cast<const DecimalType>(other)) {
    return true;
  }
  return false;
}

std::string FixedCharType::signature() const {
  std::ostringstream sign;
  sign << TypeBase::signature();
  sign << "<";
  sign << length_->value();
  sign << ">";
  return sign.str();
}

bool FixedCharType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type =
          std::dynamic_pointer_cast<const FixedCharType>(other)) {
    return true;
  }
  return false;
}

std::string VarcharType::signature() const {
  std::ostringstream sign;
  sign << TypeBase::signature();
  sign << "<";
  sign << length_->value();
  sign << ">";
  return sign.str();
}

bool VarcharType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type = std::dynamic_pointer_cast<const VarcharType>(other)) {
    return true;
  }
  return false;
}

std::string StructType::signature() const {
  std::ostringstream signature;
  signature << TypeBase::signature();
  signature << "<";
  for (auto it = children_.begin(); it != children_.end(); ++it) {
    const auto& typeSign = (*it)->signature();
    if (it == children_.end() - 1) {
      signature << typeSign;
    } else {
      signature << typeSign << ",";
    }
  }
  signature << ">";
  return signature.str();
}

bool StructType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type = std::dynamic_pointer_cast<const StructType>(other)) {
    bool sameSize = type->children_.size() == children_.size();
    if (sameSize) {
      for (int i = 0; i < children_.size(); i++) {
        if (!children_[i]->isSameAs(type->children_[i])) {
          return false;
        }
      }
      return true;
    }
  }
  return false;
}

std::string MapType::signature() const {
  std::ostringstream signature;
  signature << TypeBase::signature();
  signature << "<";
  signature << keyType_->signature();
  signature << ",";
  signature << valueType_->signature();
  signature << ">";
  return signature.str();
}

bool MapType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type = std::dynamic_pointer_cast<const MapType>(other)) {
    return keyType_->isSameAs(type->keyType_) &&
        valueType_->isSameAs(type->valueType_);
  }
  return false;
}

std::string ListType::signature() const {
  std::ostringstream signature;
  signature << TypeBase::signature();
  signature << "<";
  signature << elementType_->signature();
  signature << ">";
  return signature.str();
}

bool ListType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type = std::dynamic_pointer_cast<const ListType>(other)) {
    return elementType_->isSameAs(type->elementType_);
  }
  return false;
}

bool UsedDefinedType::isSameAs(const std::shared_ptr<const Type>& other) const {
  if (const auto& type =
          std::dynamic_pointer_cast<const UsedDefinedType>(other)) {
    return type->value_ == value_;
  }
  return false;
}

bool StringLiteralType::isSameAs(
    const std::shared_ptr<const Type>& other) const {
  if (isWildcard()) {
    return true;
  }
  if (const auto& type =
          std::dynamic_pointer_cast<const StringLiteralType>(other)) {
    return type->value_ == value_;
  }
  return false;
}

std::shared_ptr<const ScalarType<TypeKind::kBool>> BOOL() {
  return std::make_shared<const ScalarType<TypeKind::kBool>>();
}

std::shared_ptr<const ScalarType<TypeKind::kI8>> TINYINT() {
  return std::make_shared<const ScalarType<TypeKind::kI8>>();
}

std::shared_ptr<const ScalarType<TypeKind::kI16>> SMALLINT() {
  return std::make_shared<const ScalarType<TypeKind::kI16>>();
}

std::shared_ptr<const ScalarType<TypeKind::kI32>> INTEGER() {
  return std::make_shared<const ScalarType<TypeKind::kI32>>();
}

std::shared_ptr<const ScalarType<TypeKind::kI64>> BIGINT() {
  return std::make_shared<const ScalarType<TypeKind::kI64>>();
}

std::shared_ptr<const ScalarType<TypeKind::kFp32>> FLOAT() {
  return std::make_shared<const ScalarType<TypeKind::kFp32>>();
}

std::shared_ptr<const ScalarType<TypeKind::kFp64>> DOUBLE() {
  return std::make_shared<const ScalarType<TypeKind::kFp64>>();
}

std::shared_ptr<const ScalarType<TypeKind::kString>> STRING() {
  return std::make_shared<const ScalarType<TypeKind::kString>>();
}

std::shared_ptr<const ScalarType<TypeKind::kBinary>> BINARY() {
  return std::make_shared<const ScalarType<TypeKind::kBinary>>();
}

std::shared_ptr<const ScalarType<TypeKind::kTimestamp>> TIMESTAMP() {
  return std::make_shared<const ScalarType<TypeKind::kTimestamp>>();
}

std::shared_ptr<const ScalarType<TypeKind::kDate>> DATE() {
  return std::make_shared<const ScalarType<TypeKind::kDate>>();
}

std::shared_ptr<const ScalarType<TypeKind::kTime>> TIME() {
  return std::make_shared<const ScalarType<TypeKind::kTime>>();
}

std::shared_ptr<const ScalarType<TypeKind::kIntervalYear>> INTERVAL_YEAR() {
  return std::make_shared<const ScalarType<TypeKind::kIntervalYear>>();
}

std::shared_ptr<const ScalarType<TypeKind::kIntervalDay>> INTERVAL_DAY() {
  return std::make_shared<const ScalarType<TypeKind::kIntervalDay>>();
}

std::shared_ptr<const ScalarType<TypeKind::kTimestampTz>> TIMESTAMP_TZ() {
  return std::make_shared<const ScalarType<TypeKind::kTimestampTz>>();
}

std::shared_ptr<const ScalarType<TypeKind::kUuid>> UUID() {
  return std::make_shared<const ScalarType<TypeKind::kUuid>>();
}

} // namespace io::substrait
