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

#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace io::substrait {

enum class TypeKind : int8_t {
  kBool = 1,
  kI8 = 2,
  kI16 = 3,
  kI32 = 5,
  kI64 = 7,
  kFp32 = 10,
  kFp64 = 11,
  kString = 12,
  kBinary = 13,
  kTimestamp = 14,
  kDate = 16,
  kTime = 17,
  kIntervalYear = 19,
  kIntervalDay = 20,
  kTimestampTz = 29,
  kUuid = 32,
  kFixedChar = 21,
  kVarchar = 22,
  kFixedBinary = 23,
  kDecimal = 24,
  kStruct = 25,
  kList = 27,
  kMap = 28,
  kUserDefined = 30,
  KIND_NOT_SET = 0,
};

template <TypeKind KIND>
struct TypeTraits {};

template <>
struct TypeTraits<TypeKind::kBool> {
  static constexpr const char* signature = "bool";
  static constexpr const char* typeString = "boolean";
};

template <>
struct TypeTraits<TypeKind::kI8> {
  static constexpr const char* signature = "i8";
  static constexpr const char* typeString = "i8";
};

template <>
struct TypeTraits<TypeKind::kI16> {
  static constexpr const char* signature = "i16";
  static constexpr const char* typeString = "i16";
};

template <>
struct TypeTraits<TypeKind::kI32> {
  static constexpr const char* signature = "i32";
  static constexpr const char* typeString = "i32";
};

template <>
struct TypeTraits<TypeKind::kI64> {
  static constexpr const char* signature = "i64";
  static constexpr const char* typeString = "i64";
};

template <>
struct TypeTraits<TypeKind::kFp32> {
  static constexpr const char* signature = "fp32";
  static constexpr const char* typeString = "fp32";
};

template <>
struct TypeTraits<TypeKind::kFp64> {
  static constexpr const char* signature = "fp64";
  static constexpr const char* typeString = "fp64";
};

template <>
struct TypeTraits<TypeKind::kString> {
  static constexpr const char* signature = "str";
  static constexpr const char* typeString = "string";
};

template <>
struct TypeTraits<TypeKind::kBinary> {
  static constexpr const char* signature = "vbin";
  static constexpr const char* typeString = "binary";
};

template <>
struct TypeTraits<TypeKind::kTimestamp> {
  static constexpr const char* signature = "ts";
  static constexpr const char* typeString = "timestamp";
};

template <>
struct TypeTraits<TypeKind::kTimestampTz> {
  static constexpr const char* signature = "tstz";
  static constexpr const char* typeString = "timestamp_tz";
};

template <>
struct TypeTraits<TypeKind::kDate> {
  static constexpr const char* signature = "date";
  static constexpr const char* typeString = "date";
};

template <>
struct TypeTraits<TypeKind::kTime> {
  static constexpr const char* signature = "time";
  static constexpr const char* typeString = "time";
};

template <>
struct TypeTraits<TypeKind::kIntervalYear> {
  static constexpr const char* signature = "iyear";
  static constexpr const char* typeString = "interval_year";
};

template <>
struct TypeTraits<TypeKind::kIntervalDay> {
  static constexpr const char* signature = "iday";
  static constexpr const char* typeString = "interval_day";
};

template <>
struct TypeTraits<TypeKind::kUuid> {
  static constexpr const char* signature = "uuid";
  static constexpr const char* typeString = "uuid";
};

template <>
struct TypeTraits<TypeKind::kFixedChar> {
  static constexpr const char* signature = "fchar";
  static constexpr const char* typeString = "fixedchar";
};

template <>
struct TypeTraits<TypeKind::kVarchar> {
  static constexpr const char* signature = "vchar";
  static constexpr const char* typeString = "varchar";
};

template <>
struct TypeTraits<TypeKind::kFixedBinary> {
  static constexpr const char* signature = "fbin";
  static constexpr const char* typeString = "fixedbinary";
};

template <>
struct TypeTraits<TypeKind::kDecimal> {
  static constexpr const char* signature = "dec";
  static constexpr const char* typeString = "decimal";
};

template <>
struct TypeTraits<TypeKind::kStruct> {
  static constexpr const char* signature = "struct";
  static constexpr const char* typeString = "struct";
};

template <>
struct TypeTraits<TypeKind::kList> {
  static constexpr const char* signature = "list";
  static constexpr const char* typeString = "list";
};

template <>
struct TypeTraits<TypeKind::kMap> {
  static constexpr const char* signature = "map";
  static constexpr const char* typeString = "map";
};

template <>
struct TypeTraits<TypeKind::kUserDefined> {
  static constexpr const char* signature = "u!name";
  static constexpr const char* typeString = "user defined type";
};

class ParameterizedType {
 public:
  explicit ParameterizedType(bool nullable = false) : nullable_(nullable) {}

  [[nodiscard]] virtual std::string signature() const = 0;

  [[nodiscard]] virtual TypeKind kind() const = 0;

  /// Deserialize substrait raw type string into Substrait extension  type.
  /// @param rawType - substrait extension raw string type
  static std::shared_ptr<const ParameterizedType> decode(
      const std::string& rawType);

  [[nodiscard]] const bool& nullable() const {
    return nullable_;
  }

  [[nodiscard]] bool nullMatch(
      const std::shared_ptr<const ParameterizedType>& type) const {
    return nullable() || nullable() == type->nullable();
  }
  /// Test type is a Wildcard type or not.
  [[nodiscard]] virtual bool isWildcard() const {
    return false;
  }

  [[nodiscard]] virtual bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const = 0;

 private:
  const bool nullable_;
};

using ParameterizedTypePtr = std::shared_ptr<const ParameterizedType>;

class Type : public ParameterizedType {
 public:
  explicit Type(bool nullable = false) : ParameterizedType(nullable) {}
};

using TypePtr = std::shared_ptr<const Type>;

/// Types used in function argument declarations.
template <TypeKind Kind>
class TypeBase : public Type {
 public:
  explicit TypeBase(bool nullable = false) : Type(nullable) {}

  [[nodiscard]] std::string signature() const override {
    return TypeTraits<Kind>::signature;
  }

  [[nodiscard]] TypeKind kind() const override {
    return Kind;
  }

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override {
    return kind() == type->kind() && nullMatch(type);
  }
};

template <TypeKind Kind>
class ScalarType : public TypeBase<Kind> {
 public:
  explicit ScalarType(bool nullable) : TypeBase<Kind>(nullable) {}
};

class Decimal : public TypeBase<TypeKind::kDecimal> {
 public:
  Decimal(int precision, int scale, bool nullable = false)
      : TypeBase<TypeKind::kDecimal>(nullable),
        precision_(precision),
        scale_(scale) {}

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] const int& precision() const {
    return precision_;
  }

  [[nodiscard]] const int& scale() const {
    return scale_;
  }

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const int precision_;
  const int scale_;
};

class FixedBinary : public TypeBase<TypeKind::kFixedBinary> {
 public:
  explicit FixedBinary(int length, bool nullable = false)
      : TypeBase<TypeKind::kFixedBinary>(nullable), length_(length) {}

  [[nodiscard]] const int& length() const {
    return length_;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const int length_;
};

class FixedChar : public TypeBase<TypeKind::kFixedChar> {
 public:
  explicit FixedChar(int length, bool nullable = false)
      : TypeBase<TypeKind::kFixedChar>(nullable), length_(length){};

  [[nodiscard]] const int& length() const {
    return length_;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const int length_;
};

class Varchar : public TypeBase<TypeKind::kVarchar> {
 public:
  explicit Varchar(int length, bool nullable = false)
      : TypeBase<TypeKind::kVarchar>(nullable), length_(length){};

  [[nodiscard]] const int& length() const {
    return length_;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const int length_;
};

class List : public TypeBase<TypeKind::kList> {
 public:
  explicit List(TypePtr elementType, bool nullable = false)
      : TypeBase<TypeKind::kList>(nullable),
        elementType_(std::move(elementType)){};

  [[nodiscard]] const TypePtr& elementType() const {
    return elementType_;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const TypePtr elementType_;
};

class Struct : public TypeBase<TypeKind::kStruct> {
 public:
  explicit Struct(std::vector<TypePtr> types, bool nullable = false)
      : TypeBase<TypeKind::kStruct>(nullable), children_(std::move(types)) {}

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] const std::vector<TypePtr>& children() const {
    return children_;
  }

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const std::vector<TypePtr> children_;
};

class Map : public TypeBase<TypeKind::kMap> {
 public:
  Map(TypePtr keyType, TypePtr valueType, bool nullable = false)
      : TypeBase<TypeKind::kMap>(nullable),
        keyType_(std::move(keyType)),
        valueType_(std::move(valueType)) {}

  [[nodiscard]] const TypePtr& keyType() const {
    return keyType_;
  }

  [[nodiscard]] const TypePtr& valueType() const {
    return valueType_;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const TypePtr keyType_;
  const TypePtr valueType_;
};

/// ParameterizedType represent a type in
class ParameterizedTypeBase : public ParameterizedType {
 public:
  explicit ParameterizedTypeBase(bool nullable = false)
      : ParameterizedType(nullable) {}
};

class UsedDefinedType : public ParameterizedTypeBase {
 public:
  UsedDefinedType(std::string value, bool nullable)
      : ParameterizedTypeBase(nullable), value_(std::move(value)) {}

  [[nodiscard]] const std::string& value() const {
    return value_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kUserDefined;
  }

  [[nodiscard]] std::string signature() const override {
    return TypeTraits<TypeKind::kUserDefined>::signature;
  }

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  /// raw string of wildcard type.
  const std::string value_;
};

/// A string literal type can present the 'any1'.
class StringLiteral : public ParameterizedTypeBase {
 public:
  explicit StringLiteral(std::string value)
      : ParameterizedTypeBase(false), value_(std::move(value)) {}

  [[nodiscard]] std::string signature() const override {
    return value_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::KIND_NOT_SET;
  }

  [[nodiscard]] const std::string& value() const {
    return value_;
  }

  [[nodiscard]] bool isWildcard() const override {
    return value_.find("any") == 0 || value_ == "T";
  }

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const std::string value_;
};

using StringLiteralPtr = std::shared_ptr<const StringLiteral>;

class ParameterizedDecimal : public ParameterizedTypeBase {
 public:
  ParameterizedDecimal(
      StringLiteralPtr precision,
      StringLiteralPtr scale,
      bool nullable = false)
      : ParameterizedTypeBase(nullable),
        precision_(std::move(precision)),
        scale_(std::move(scale)) {}

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] const StringLiteralPtr& precision() const {
    return precision_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kDecimal;
  }

  [[nodiscard]] const StringLiteralPtr& scale() const {
    return scale_;
  }

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  StringLiteralPtr precision_;
  StringLiteralPtr scale_;
};

class ParameterizedFixedBinary : public ParameterizedTypeBase {
 public:
  explicit ParameterizedFixedBinary(
      StringLiteralPtr length,
      bool nullable = false)
      : ParameterizedTypeBase(nullable), length_(std::move(length)) {}

  [[nodiscard]] const StringLiteralPtr& length() const {
    return length_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kFixedBinary;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const StringLiteralPtr length_;
};

class ParameterizedFixedChar : public ParameterizedTypeBase {
 public:
  explicit ParameterizedFixedChar(
      StringLiteralPtr length,
      bool nullable = false)
      : ParameterizedTypeBase(nullable), length_(std::move(length)) {}

  [[nodiscard]] const StringLiteralPtr& length() const {
    return length_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kFixedChar;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const StringLiteralPtr length_;
};

class ParameterizedVarchar : public ParameterizedTypeBase {
 public:
  explicit ParameterizedVarchar(StringLiteralPtr length, bool nullable = false)
      : ParameterizedTypeBase(nullable), length_(std::move(length)) {}

  [[nodiscard]] const StringLiteralPtr& length() const {
    return length_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kVarchar;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const StringLiteralPtr length_;
};

class ParameterizedList : public ParameterizedTypeBase {
 public:
  explicit ParameterizedList(
      ParameterizedTypePtr elementType,
      bool nullable = false)
      : ParameterizedTypeBase(nullable), elementType_(std::move(elementType)){};

  [[nodiscard]] const ParameterizedTypePtr& elementType() const {
    return elementType_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kList;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const ParameterizedTypePtr elementType_;
};

class ParameterizedStruct : public ParameterizedTypeBase {
 public:
  explicit ParameterizedStruct(
      std::vector<ParameterizedTypePtr> types,
      bool nullable = false)
      : ParameterizedTypeBase(nullable), children_(std::move(types)) {}

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] const std::vector<ParameterizedTypePtr>& children() const {
    return children_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kStruct;
  }

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const std::vector<ParameterizedTypePtr> children_;
};

class ParameterizedMap : public ParameterizedTypeBase {
 public:
  ParameterizedMap(
      ParameterizedTypePtr keyType,
      ParameterizedTypePtr valueType,
      bool nullable = false)
      : ParameterizedTypeBase(nullable),
        keyType_(std::move(keyType)),
        valueType_(std::move(valueType)) {}

  [[nodiscard]] const ParameterizedTypePtr& keyType() const {
    return keyType_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kMap;
  }
  [[nodiscard]] const ParameterizedTypePtr& valueType() const {
    return valueType_;
  }

  [[nodiscard]] std::string signature() const override;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const ParameterizedTypePtr keyType_;
  const ParameterizedTypePtr valueType_;
};

std::shared_ptr<const ScalarType<TypeKind::kBool>> BOOL();

std::shared_ptr<const ScalarType<TypeKind::kI8>> TINYINT();

std::shared_ptr<const ScalarType<TypeKind::kI16>> SMALLINT();

std::shared_ptr<const ScalarType<TypeKind::kI32>> INTEGER();

std::shared_ptr<const ScalarType<TypeKind::kI64>> BIGINT();

std::shared_ptr<const ScalarType<TypeKind::kFp32>> FLOAT();

std::shared_ptr<const ScalarType<TypeKind::kFp64>> DOUBLE();

std::shared_ptr<const ScalarType<TypeKind::kString>> STRING();

std::shared_ptr<const ScalarType<TypeKind::kBinary>> BINARY();

std::shared_ptr<const ScalarType<TypeKind::kTimestamp>> TIMESTAMP();

std::shared_ptr<const ScalarType<TypeKind::kTimestampTz>> TIMESTAMP_TZ();

std::shared_ptr<const ScalarType<TypeKind::kDate>> DATE();

std::shared_ptr<const ScalarType<TypeKind::kTime>> TIME();

std::shared_ptr<const ScalarType<TypeKind::kIntervalYear>> INTERVAL_YEAR();

std::shared_ptr<const ScalarType<TypeKind::kIntervalDay>> INTERVAL_DAY();

std::shared_ptr<const ScalarType<TypeKind::kUuid>> UUID();

std::shared_ptr<const Decimal> DECIMAL(int precision, int scale);

std::shared_ptr<const Varchar> VARCHAR(int len);

std::shared_ptr<const FixedChar> FChar(int len);

std::shared_ptr<const FixedBinary> FBinary(int len);

std::shared_ptr<const List> LIST(const TypePtr& elementType);

std::shared_ptr<const Map> MAP(
    const TypePtr& keyType,
    const TypePtr& valueType);

std::shared_ptr<const Struct> ROW(const std::vector<TypePtr>& children);

} // namespace io::substrait
