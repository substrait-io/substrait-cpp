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

#include "proto/substrait/algebra.pb.h"
#include "proto/substrait/type.pb.h"
#include <iostream>
#include <sstream>

namespace io::substrait {

using TypeKind = ::substrait::Type::KindCase;

template <TypeKind KIND> struct TypeTraits {};

template <> struct TypeTraits<TypeKind::kBool> {
  static constexpr const char *signature = "bool";
  static constexpr const char *typeString = "boolean";
};

template <> struct TypeTraits<TypeKind::kI8> {
  static constexpr const char *signature = "i8";
  static constexpr const char *typeString = "i8";
};

template <> struct TypeTraits<TypeKind::kI16> {
  static constexpr const char *signature = "i16";
  static constexpr const char *typeString = "i16";
};

template <> struct TypeTraits<TypeKind::kI32> {
  static constexpr const char *signature = "i32";
  static constexpr const char *typeString = "i32";
};

template <> struct TypeTraits<TypeKind::kI64> {
  static constexpr const char *signature = "i64";
  static constexpr const char *typeString = "i64";
};

template <> struct TypeTraits<TypeKind::kFp32> {
  static constexpr const char *signature = "fp32";
  static constexpr const char *typeString = "fp32";
};

template <> struct TypeTraits<TypeKind::kFp64> {
  static constexpr const char *signature = "fp64";
  static constexpr const char *typeString = "fp64";
};

template <> struct TypeTraits<TypeKind::kString> {
  static constexpr const char *signature = "str";
  static constexpr const char *typeString = "string";
};

template <> struct TypeTraits<TypeKind::kBinary> {
  static constexpr const char *signature = "vbin";
  static constexpr const char *typeString = "binary";
};

template <> struct TypeTraits<TypeKind::kTimestamp> {
  static constexpr const char *signature = "ts";
  static constexpr const char *typeString = "timestamp";
};

template <> struct TypeTraits<TypeKind::kTimestampTz> {
  static constexpr const char *signature = "tstz";
  static constexpr const char *typeString = "timestamp_tz";
};

template <> struct TypeTraits<TypeKind::kDate> {
  static constexpr const char *signature = "date";
  static constexpr const char *typeString = "date";
};

template <> struct TypeTraits<TypeKind::kTime> {
  static constexpr const char *signature = "time";
  static constexpr const char *typeString = "time";
};

template <> struct TypeTraits<TypeKind::kIntervalYear> {
  static constexpr const char *signature = "iyear";
  static constexpr const char *typeString = "interval_year";
};

template <> struct TypeTraits<TypeKind::kIntervalDay> {
  static constexpr const char *signature = "iday";
  static constexpr const char *typeString = "interval_day";
};

template <> struct TypeTraits<TypeKind::kUuid> {
  static constexpr const char *signature = "uuid";
  static constexpr const char *typeString = "uuid";
};

template <> struct TypeTraits<TypeKind::kFixedChar> {
  static constexpr const char *signature = "fchar";
  static constexpr const char *typeString = "fixedchar";
};

template <> struct TypeTraits<TypeKind::kVarchar> {
  static constexpr const char *signature = "vchar";
  static constexpr const char *typeString = "varchar";
};

template <> struct TypeTraits<TypeKind::kFixedBinary> {
  static constexpr const char *signature = "fbin";
  static constexpr const char *typeString = "fixedbinary";
};

template <> struct TypeTraits<TypeKind::kDecimal> {
  static constexpr const char *signature = "dec";
  static constexpr const char *typeString = "decimal";
};

template <> struct TypeTraits<TypeKind::kStruct> {
  static constexpr const char *signature = "struct";
  static constexpr const char *typeString = "struct";
};

template <> struct TypeTraits<TypeKind::kList> {
  static constexpr const char *signature = "list";
  static constexpr const char *typeString = "list";
};

template <> struct TypeTraits<TypeKind::kMap> {
  static constexpr const char *signature = "map";
  static constexpr const char *typeString = "map";
};

template <> struct TypeTraits<TypeKind::kUserDefined> {
  static constexpr const char *signature = "u!name";
  static constexpr const char *typeString = "user defined type";
};

class Type {
public:
  /// deserialize substrait raw type string into Substrait extension  type.
  /// @param rawType - substrait extension raw string type
  static std::shared_ptr<const Type> decode(const std::string &rawType);

  /// signature name.
  virtual const std::string signature() const = 0;

  /// test type is a Wildcard type or not.
  virtual const bool isWildcard() const { return false; }

  /// a known substrait type kind
  virtual const TypeKind kind() const = 0;

  virtual const std::string typeString() const = 0;

  /// whether two types are same as each other
  virtual bool isSameAs(const std::shared_ptr<const Type> &other) const {
    return kind() == other->kind();
  }

private:
  /// A map store the raw type string and corresponding Substrait Type
  static const std::unordered_map<std::string, std::shared_ptr<const Type>> &
  scalarTypeMapping();
};

using TypePtr = std::shared_ptr<const Type>;

/// Types used in function argument declarations.
template <TypeKind Kind> class TypeBase : public Type {
public:
  const std::string signature() const override {
    return TypeTraits<Kind>::signature;
  }

  virtual const TypeKind kind() const override { return Kind; }

  const std::string typeString() const override {
    return TypeTraits<Kind>::typeString;
  }
};

template <TypeKind Kind> class ScalarType : public TypeBase<Kind> {};

/// A string literal type can present the 'any1'.
class StringLiteralType : public Type {
public:
  StringLiteralType(const std::string &value) : value_(value) {}

  const std::string &value() const { return value_; }

  const std::string signature() const override { return value_; }

  const std::string typeString() const override { return value_; }
  const bool isWildcard() const override {
    return value_.find("any") == 0 || value_ == "T";
  }

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const TypeKind kind() const override { return TypeKind ::KIND_NOT_SET; }

private:
  /// Raw string of wildcard type.
  const std::string value_;
};

using StringLiteralTypePtr = std::shared_ptr<const StringLiteralType>;

class DecimalType : public TypeBase<TypeKind ::kDecimal> {
public:
  DecimalType(const StringLiteralTypePtr &precision,
              const StringLiteralTypePtr &scale)
      : precision_(precision), scale_(scale) {}

  DecimalType(const std::string &precision, const std::string &scale)
      : precision_(std::make_shared<StringLiteralType>(precision)),
        scale_(std::make_shared<StringLiteralType>(scale)) {}

  DecimalType(const int precision, const int scale)
      : DecimalType(std::to_string(precision), std::to_string(scale)) {}

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const std::string signature() const override;

  const std::string precision() const { return precision_->value(); }

  const std::string scale() const { return scale_->value(); }

private:
  StringLiteralTypePtr precision_;
  StringLiteralTypePtr scale_;
};

class FixedBinaryType : public TypeBase<TypeKind ::kFixedBinary> {
public:
  FixedBinaryType(const StringLiteralTypePtr &length) : length_(length) {}

  FixedBinaryType(const int length)
      : FixedBinaryType(
            std::make_shared<StringLiteralType>(std::to_string(length))) {}

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const StringLiteralTypePtr &length() const { return length_; }

  const std::string signature() const override;

protected:
  StringLiteralTypePtr length_;
};

class FixedCharType : public TypeBase<TypeKind ::kFixedChar> {
public:
  FixedCharType(const StringLiteralTypePtr &length) : length_(length) {}

  FixedCharType(const int length)
      : FixedCharType(
            std::make_shared<StringLiteralType>(std::to_string(length))) {}

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const StringLiteralTypePtr &length() const { return length_; }

  const std::string signature() const override;

protected:
  StringLiteralTypePtr length_;
};

class VarcharType : public TypeBase<TypeKind ::kVarchar> {
public:
  VarcharType(const StringLiteralTypePtr &length) : length_(length) {}

  VarcharType(const int length)
      : VarcharType(
            std::make_shared<StringLiteralType>(std::to_string(length))) {}

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const StringLiteralTypePtr &length() const { return length_; }

  const std::string signature() const override;

protected:
  StringLiteralTypePtr length_;
};

class ListType : public TypeBase<TypeKind ::kList> {
public:
  ListType(const TypePtr &elementType) : elementType_(elementType){};

  const TypePtr elementType() const { return elementType_; }

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const std::string signature() const override;

private:
  TypePtr elementType_;
};

class StructType : public TypeBase<TypeKind ::kStruct> {
public:
  StructType(const std::vector<TypePtr> &types) : children_(types) {}

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const std::string signature() const override;

  const std::vector<TypePtr> &children() const { return children_; }

private:
  std::vector<TypePtr> children_;
};

class MapType : public TypeBase<TypeKind ::kMap> {
public:
  MapType(const TypePtr &keyType, const TypePtr &valueType)
      : keyType_(keyType), valueType_(valueType) {}

  const TypePtr keyType() const { return keyType_; }

  const TypePtr valueType() const { return valueType_; }

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

  const std::string signature() const override;

private:
  TypePtr keyType_;
  TypePtr valueType_;
};

class UsedDefinedType : public TypeBase<TypeKind ::kUserDefined> {
public:
  UsedDefinedType(const std::string &value) : value_(value) {}

  const std::string &value() const { return value_; }

  bool isSameAs(const std::shared_ptr<const Type> &other) const override;

private:
  /// raw string of wildcard type.
  const std::string value_;
};

#define SCALAR_ACCESSOR(KIND)                                                  \
  std::shared_ptr<const ScalarType<TypeKind::KIND>> KIND()

SCALAR_ACCESSOR(kBool);
SCALAR_ACCESSOR(kI8);
SCALAR_ACCESSOR(kI16);
SCALAR_ACCESSOR(kI32);
SCALAR_ACCESSOR(kI64);
SCALAR_ACCESSOR(kFp32);
SCALAR_ACCESSOR(kFp64);
SCALAR_ACCESSOR(kString);
SCALAR_ACCESSOR(kBinary);
SCALAR_ACCESSOR(kTimestamp);
SCALAR_ACCESSOR(kDate);
SCALAR_ACCESSOR(kTime);
SCALAR_ACCESSOR(kIntervalYear);
SCALAR_ACCESSOR(kIntervalDay);
SCALAR_ACCESSOR(kTimestampTz);
SCALAR_ACCESSOR(kUuid);

} // namespace io::substrait
