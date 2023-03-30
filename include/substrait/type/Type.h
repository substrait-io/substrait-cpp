/* SPDX-License-Identifier: Apache-2.0 */

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
  kI32 = 4,
  kI64 = 5,
  kFp32 = 6,
  kFp64 = 7,
  kString = 8,
  kBinary = 9,
  kTimestamp = 10,
  kDate = 11,
  kTime = 12,
  kIntervalYear = 13,
  kIntervalDay = 14,
  kTimestampTz = 15,
  kUuid = 16,
  kFixedChar = 17,
  kVarchar = 18,
  kFixedBinary = 19,
  kDecimal = 20,
  kStruct = 21,
  kList = 22,
  kMap = 23,
  kKindNotSet = 0,
};

template <TypeKind KIND>
struct TypeTraits {};

template <>
struct TypeTraits<TypeKind::kBool> {
  static constexpr const char* kSignature = "bool";
  static constexpr const char* kTypeString = "boolean";
};

template <>
struct TypeTraits<TypeKind::kI8> {
  static constexpr const char* kSignature = "i8";
  static constexpr const char* kTypeString = "i8";
};

template <>
struct TypeTraits<TypeKind::kI16> {
  static constexpr const char* kSignature = "i16";
  static constexpr const char* kTypeString = "i16";
};

template <>
struct TypeTraits<TypeKind::kI32> {
  static constexpr const char* kSignature = "i32";
  static constexpr const char* kTypeString = "i32";
};

template <>
struct TypeTraits<TypeKind::kI64> {
  static constexpr const char* kSignature = "i64";
  static constexpr const char* kTypeString = "i64";
};

template <>
struct TypeTraits<TypeKind::kFp32> {
  static constexpr const char* kSignature = "fp32";
  static constexpr const char* kTypeString = "fp32";
};

template <>
struct TypeTraits<TypeKind::kFp64> {
  static constexpr const char* kSignature = "fp64";
  static constexpr const char* kTypeString = "fp64";
};

template <>
struct TypeTraits<TypeKind::kString> {
  static constexpr const char* kSignature = "str";
  static constexpr const char* kTypeString = "string";
};

template <>
struct TypeTraits<TypeKind::kBinary> {
  static constexpr const char* kSignature = "vbin";
  static constexpr const char* kTypeString = "binary";
};

template <>
struct TypeTraits<TypeKind::kTimestamp> {
  static constexpr const char* kSignature = "ts";
  static constexpr const char* kTypeString = "timestamp";
};

template <>
struct TypeTraits<TypeKind::kTimestampTz> {
  static constexpr const char* kSignature = "tstz";
  static constexpr const char* kTypeString = "timestamp_tz";
};

template <>
struct TypeTraits<TypeKind::kDate> {
  static constexpr const char* kSignature = "date";
  static constexpr const char* kTypeString = "date";
};

template <>
struct TypeTraits<TypeKind::kTime> {
  static constexpr const char* kSignature = "time";
  static constexpr const char* kTypeString = "time";
};

template <>
struct TypeTraits<TypeKind::kIntervalYear> {
  static constexpr const char* kSignature = "iyear";
  static constexpr const char* kTypeString = "interval_year";
};

template <>
struct TypeTraits<TypeKind::kIntervalDay> {
  static constexpr const char* kSignature = "iday";
  static constexpr const char* kTypeString = "interval_day";
};

template <>
struct TypeTraits<TypeKind::kUuid> {
  static constexpr const char* kSignature = "uuid";
  static constexpr const char* kTypeString = "uuid";
};

template <>
struct TypeTraits<TypeKind::kFixedChar> {
  static constexpr const char* kSignature = "fchar";
  static constexpr const char* kTypeString = "fixedchar";
};

template <>
struct TypeTraits<TypeKind::kVarchar> {
  static constexpr const char* kSignature = "vchar";
  static constexpr const char* kTypeString = "varchar";
};

template <>
struct TypeTraits<TypeKind::kFixedBinary> {
  static constexpr const char* kSignature = "fbin";
  static constexpr const char* kTypeString = "fixedbinary";
};

template <>
struct TypeTraits<TypeKind::kDecimal> {
  static constexpr const char* kSignature = "dec";
  static constexpr const char* kTypeString = "decimal";
};

template <>
struct TypeTraits<TypeKind::kStruct> {
  static constexpr const char* kSignature = "struct";
  static constexpr const char* kTypeString = "struct";
};

template <>
struct TypeTraits<TypeKind::kList> {
  static constexpr const char* kSignature = "list";
  static constexpr const char* kTypeString = "list";
};

template <>
struct TypeTraits<TypeKind::kMap> {
  static constexpr const char* kSignature = "map";
  static constexpr const char* kTypeString = "map";
};

class ParameterizedType {
 public:
  explicit ParameterizedType(bool nullable = false) : nullable_(nullable) {}

  [[nodiscard]] virtual std::string signature() const = 0;

  [[nodiscard]] virtual TypeKind kind() const = 0;

  /// Deserialize substrait raw type string into Substrait extension  type.
  /// @param rawType - substrait extension raw string type
  static std::shared_ptr<const ParameterizedType> decode(
      const std::string& rawType) {
    return decode(rawType, true);
  }

  static std::shared_ptr<const ParameterizedType> decode(
      const std::string& rawType,
      bool isParameterized);

  [[nodiscard]] const bool& nullable() const {
    return nullable_;
  }

  [[nodiscard]] bool nullMatch(
      const std::shared_ptr<const ParameterizedType>& type) const {
    return nullable() || nullable() == type->nullable();
  }

  /// Test whether a type is a Wildcard type or not, always false for non
  /// StringLiteral type.
  [[nodiscard]] virtual bool isWildcard() const {
    return false;
  }

  /// Test whether a Type is a placeholder type or not, always false for non
  /// StringLiteral type.
  [[nodiscard]] virtual bool isPlaceholder() const {
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
    return TypeTraits<Kind>::kSignature;
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

class ParameterizedTypeBase : public ParameterizedType {
 public:
  explicit ParameterizedTypeBase(bool nullable = false)
      : ParameterizedType(nullable) {}
};

/// A string literal type can present the 'any1' or 'T','P1'.
class StringLiteral : public ParameterizedTypeBase {
 public:
  StringLiteral(const std::string& value, bool wildcard, bool placeholder)
      : ParameterizedTypeBase(false),
        value_(std::move(value)),
        wildcard_(wildcard),
        placeholder_(placeholder) {}

  [[nodiscard]] std::string signature() const override {
    return value_;
  }

  [[nodiscard]] TypeKind kind() const override {
    return TypeKind::kKindNotSet;
  }

  [[nodiscard]] const std::string& value() const {
    return value_;
  }

  [[nodiscard]] bool isWildcard() const override {
    return wildcard_;
  }

  [[nodiscard]] bool isPlaceholder() const override {
    return placeholder_;
  }

  /// Return true if value is a integer, false otherwise.
  [[nodiscard]] bool isInteger() const;

  [[nodiscard]] bool isMatch(
      const std::shared_ptr<const ParameterizedType>& type) const override;

 private:
  const std::string value_;
  const bool wildcard_;
  const bool placeholder_;
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

std::shared_ptr<const ScalarType<TypeKind::kBool>> boolean();

std::shared_ptr<const ScalarType<TypeKind::kI8>> tinyint();

std::shared_ptr<const ScalarType<TypeKind::kI16>> smallint();

std::shared_ptr<const ScalarType<TypeKind::kI32>> integer();

std::shared_ptr<const ScalarType<TypeKind::kI64>> bigint();

std::shared_ptr<const ScalarType<TypeKind::kFp32>> float4();

std::shared_ptr<const ScalarType<TypeKind::kFp64>> float8();

std::shared_ptr<const ScalarType<TypeKind::kString>> string();

std::shared_ptr<const ScalarType<TypeKind::kBinary>> binary();

std::shared_ptr<const ScalarType<TypeKind::kTimestamp>> timestamp();

std::shared_ptr<const ScalarType<TypeKind::kTimestampTz>> timestampTz();

std::shared_ptr<const ScalarType<TypeKind::kDate>> date();

std::shared_ptr<const ScalarType<TypeKind::kTime>> time();

std::shared_ptr<const ScalarType<TypeKind::kIntervalYear>> intervalYear();

std::shared_ptr<const ScalarType<TypeKind::kIntervalDay>> intervalDay();

std::shared_ptr<const ScalarType<TypeKind::kUuid>> uuid();

std::shared_ptr<const Decimal> decimal(int precision, int scale);

std::shared_ptr<const Varchar> varchar(int len);

std::shared_ptr<const FixedChar> fixedChar(int len);

std::shared_ptr<const FixedBinary> fixedBinary(int len);

std::shared_ptr<const List> list(const TypePtr& elementType);

std::shared_ptr<const Map> map(
    const TypePtr& keyType,
    const TypePtr& valueType);

std::shared_ptr<const Struct> row(const std::vector<TypePtr>& children);

} // namespace io::substrait
