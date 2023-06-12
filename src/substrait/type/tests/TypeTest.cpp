/* SPDX-License-Identifier: Apache-2.0 */

#include <gtest/gtest.h>

#include <memory>
#include "substrait/type/Type.h"

using namespace io::substrait;

class TypeTest : public ::testing::Test {
 protected:
  template <TypeKind Kind>
  void testDecode(const std::string& rawType, const std::string& signature) {
    const auto& type = ParameterizedType::decode(rawType);
    ASSERT_TRUE(type->kind() == Kind);
    ASSERT_EQ(type->signature(), signature);
  }

  static void testType(
      const ParameterizedTypePtr& type,
      TypeKind kind,
      const std::string& signature) {
    ASSERT_EQ(type->kind(), kind);
    ASSERT_EQ(type->signature(), signature);
  }

  template <class T, bool Param = true>
  void testDecode(
      const std::string& rawType,
      const std::function<void(const std::shared_ptr<const T>&)>&
          typeCallBack) {
    const auto& type = ParameterizedType::decode(rawType, Param);
    if (typeCallBack) {
      typeCallBack(std::dynamic_pointer_cast<const T>(type));
    }
  }
};

TEST_F(TypeTest, typeCreator) {
  testType(boolean(), TypeKind::kBool, "bool");
  testType(tinyint(), TypeKind::kI8, "i8");
  testType(smallint(), TypeKind::kI16, "i16");
  testType(integer(), TypeKind::kI32, "i32");
  testType(bigint(), TypeKind::kI64, "i64");
  testType(float4(), TypeKind::kFp32, "fp32");
  testType(float8(), TypeKind::kFp64, "fp64");
  testType(binary(), TypeKind::kBinary, "vbin");
  testType(timestamp(), TypeKind::kTimestamp, "ts");
  testType(string(), TypeKind::kString, "str");
  testType(timestampTz(), TypeKind::kTimestampTz, "tstz");
  testType(date(), TypeKind::kDate, "date");
  testType(time(), TypeKind::kTime, "time");
  testType(intervalDay(), TypeKind::kIntervalDay, "iday");
  testType(intervalYear(), TypeKind::kIntervalYear, "iyear");
  testType(uuid(), TypeKind::kUuid, "uuid");
  testType(fixedChar(12), TypeKind::kFixedChar, "fchar<12>");
  testType(fixedBinary(12), TypeKind::kFixedBinary, "fbin<12>");
  testType(varchar(12), TypeKind::kVarchar, "vchar<12>");
  testType(decimal(12, 23), TypeKind::kDecimal, "dec<12,23>");
  testType(list(float4()), TypeKind::kList, "list<fp32>");
  testType(map(string(), float4()), TypeKind::kMap, "map<str,fp32>");
  testType(row({string(), float4()}), TypeKind::kStruct, "struct<str,fp32>");
}

TEST_F(TypeTest, decodeTest) {
  testDecode<TypeKind::kI32>("i32?", "i32");
  testDecode<TypeKind::kBool>("BOOLEAN", "bool");
  testDecode<TypeKind::kBool>("boolean", "bool");
  testDecode<TypeKind::kI8>("i8", "i8");
  testDecode<TypeKind::kI16>("i16", "i16");
  testDecode<TypeKind::kI32>("i32", "i32");
  testDecode<TypeKind::kI64>("i64", "i64");
  testDecode<TypeKind::kFp32>("fp32", "fp32");
  testDecode<TypeKind::kFp64>("fp64", "fp64");
  testDecode<TypeKind::kBinary>("binary", "vbin");
  testDecode<TypeKind::kTimestamp>("timestamp", "ts");
  testDecode<TypeKind::kString>("string", "str");
  testDecode<TypeKind::kTimestampTz>("timestamp_tz", "tstz");
  testDecode<TypeKind::kDate>("date", "date");
  testDecode<TypeKind::kTime>("time", "time");
  testDecode<TypeKind::kIntervalDay>("interval_day", "iday");
  testDecode<TypeKind::kIntervalYear>("interval_year", "iyear");
  testDecode<TypeKind::kUuid>("uuid", "uuid");

  testDecode<ParameterizedFixedChar>(
      "fixedchar<L1>",
      [](const std::shared_ptr<const ParameterizedFixedChar>& typePtr) {
        ASSERT_EQ(typePtr->length()->value(), "L1");
        ASSERT_EQ(typePtr->signature(), "fchar<L1>");
      });

  testDecode<class FixedChar, false>(
      "fixedchar<12>",
      [](const std::shared_ptr<const class FixedChar>& typePtr) {
        ASSERT_EQ(typePtr->length(), 12);
        ASSERT_EQ(typePtr->signature(), "fchar<12>");
      });

  testDecode<ParameterizedFixedBinary>(
      "fixedbinary<L1>",
      [](const std::shared_ptr<const ParameterizedFixedBinary>& typePtr) {
        ASSERT_EQ(typePtr->length()->value(), "L1");
        ASSERT_EQ(typePtr->signature(), "fbin<L1>");
      });

  testDecode<class FixedBinary, false>(
      "fixedbinary<10>",
      [](const std::shared_ptr<const class FixedBinary>& typePtr) {
        ASSERT_EQ(typePtr->length(), 10);
        ASSERT_EQ(typePtr->signature(), "fbin<10>");
      });

  testDecode<ParameterizedVarchar>(
      "varchar<L1>",
      [](const std::shared_ptr<const ParameterizedVarchar>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "vchar<L1>");
        ASSERT_EQ(typePtr->length()->value(), "L1");
      });

  testDecode<Varchar, false>(
      "varchar<5>", [](const std::shared_ptr<const Varchar>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "vchar<5>");
        ASSERT_EQ(typePtr->length(), 5);
      });

  testDecode<ParameterizedDecimal>(
      "decimal<P,S>",
      [](const std::shared_ptr<const ParameterizedDecimal>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "dec<P,S>");
        ASSERT_EQ(typePtr->precision()->value(), "P");
        ASSERT_EQ(typePtr->scale()->value(), "S");
        ASSERT_TRUE(typePtr->precision()->isPlaceholder());
        ASSERT_TRUE(typePtr->scale()->isPlaceholder());
      });

  testDecode<ParameterizedDecimal>(
      "decimal<18 ,S1>",
      [](const std::shared_ptr<const ParameterizedDecimal>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "dec<18,S1>");
        ASSERT_TRUE(typePtr->precision()->isInteger());
        ASSERT_TRUE(typePtr->scale()->isPlaceholder());
      });

  testDecode<Decimal, false>(
      "decimal<18,2>", [](const std::shared_ptr<const Decimal>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "dec<18,2>");
        ASSERT_EQ(typePtr->precision(), 18);
        ASSERT_EQ(typePtr->scale(), 2);
      });

  testDecode<Decimal, false>(
      "decimal?<18,2>", [](const std::shared_ptr<const Decimal>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "dec<18,2>");
        ASSERT_EQ(typePtr->precision(), 18);
        ASSERT_EQ(typePtr->scale(), 2);
        ASSERT_TRUE(typePtr->nullable());
      });

  testDecode<ParameterizedStruct>(
      "struct<i32,i64,string>",
      [](const std::shared_ptr<const ParameterizedStruct>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,str>");
      });

  testDecode<Struct, false>(
      "struct<i32,i64,string>",
      [](const std::shared_ptr<const Struct>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,str>");
      });

  testDecode<ParameterizedStruct>(
      "struct<i32,i64,struct<string,string>>",
      [](const std::shared_ptr<const ParameterizedStruct>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,struct<str,str>>");
      });

  testDecode<ParameterizedStruct>(
      "STRUCT<DECIMAL<38,S>,i64>",
      [](const std::shared_ptr<const ParameterizedStruct>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<dec<38,S>,i64>");
        ASSERT_EQ(typePtr->children().size(), 2);
        ASSERT_EQ(typePtr->children().at(0)->kind(), TypeKind::kDecimal);
        auto decimalType =
            std::dynamic_pointer_cast<const ParameterizedDecimal>(
                typePtr->children().at(0));

        ASSERT_TRUE(decimalType != nullptr);
        ASSERT_TRUE(decimalType->precision()->value() == "38");
        ASSERT_TRUE(decimalType->scale()->value() == "S");
        ASSERT_EQ(typePtr->children().at(1)->kind(), TypeKind::kI64);
      });

  testDecode<ParameterizedList>(
      "list<string>",
      [](const std::shared_ptr<const ParameterizedList>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<str>");
      });

  testDecode<List, false>(
      "list<string>", [](const std::shared_ptr<const List>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<str>");
      });

  testDecode<ParameterizedList>(
      "LIST?<any>",
      [](const std::shared_ptr<const ParameterizedList>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<any>");
        ASSERT_EQ(typePtr->nullable(), true);
        ASSERT_EQ(typePtr->elementType()->isWildcard(), true);
      });

  testDecode<ParameterizedList>(
      "LIST<T>", [](const std::shared_ptr<const ParameterizedList>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<T>");
        ASSERT_TRUE(typePtr->elementType()->isPlaceholder());
      });

  testDecode<ParameterizedList>(
      "list<string?>",
      [](const std::shared_ptr<const ParameterizedList>& typePtr) {
        ASSERT_FALSE(typePtr->nullable());
        ASSERT_TRUE(typePtr->elementType()->nullable());
        ASSERT_EQ(typePtr->signature(), "list<str>");
      });

  testDecode<ParameterizedMap>(
      "map<string,i32>",
      [](const std::shared_ptr<const ParameterizedMap>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "map<str,i32>");
      });

  testDecode<ParameterizedMap>(
      "map<string,any>",
      [](const std::shared_ptr<const ParameterizedMap>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "map<str,any>");
        ASSERT_EQ(typePtr->valueType()->isWildcard(), true);
      });

  testDecode<ParameterizedMap>(
      "map<>", [](const std::shared_ptr<const ParameterizedMap>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "map<,>");
        ASSERT_EQ(typePtr->valueType(), nullptr);
      });

  testDecode<ParameterizedMap>(
      "map<string,map<string,any>",
      [](const std::shared_ptr<const ParameterizedMap>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "map<str,map<str,any>>");
        auto mapValue = std::dynamic_pointer_cast<const ParameterizedMap>(
            typePtr->valueType());
        ASSERT_NE(mapValue, nullptr);
        ASSERT_EQ(mapValue->valueType()->isWildcard(), true);
      });

  testDecode<Map, false>(
      "map<string,i32>", [](const std::shared_ptr<const Map>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "map<str,i32>");
      });

  testDecode<StringLiteral>(
      "any1", [](const std::shared_ptr<const StringLiteral>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "any1");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<StringLiteral>(
      "any", [](const std::shared_ptr<const StringLiteral>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "any");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<StringLiteral>(
      "T", [](const std::shared_ptr<const StringLiteral>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "T");
        ASSERT_TRUE(typePtr->isPlaceholder());
      });

  ASSERT_TRUE(ParameterizedType::decode("T")->isPlaceholder());

  ASSERT_TRUE(ParameterizedType::decode("any")->isWildcard());
  ASSERT_TRUE(ParameterizedType::decode("ANY")->isWildcard());
  ASSERT_TRUE(ParameterizedType::decode("aNy")->isWildcard());
  ASSERT_TRUE(ParameterizedType::decode("any1")->isWildcard());
  ASSERT_TRUE(ParameterizedType::decode("any2")->isWildcard());

  ASSERT_ANY_THROW(ParameterizedType::decode("decimal<P1,S1>", false));
  ASSERT_ANY_THROW(ParameterizedType::decode("fixedbinary<P>", false));
  ASSERT_ANY_THROW(ParameterizedType::decode("varchar<P>", false));
  ASSERT_ANY_THROW(ParameterizedType::decode("fixedchar<P>", false));
}
