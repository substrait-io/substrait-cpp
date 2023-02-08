/* SPDX-License-Identifier: Apache-2.0 */

#include <gtest/gtest.h>
#include "substrait/type/Type.h"

using namespace io::substrait;

class TypeTest : public ::testing::Test {
 protected:
  template <TypeKind kind>
  void testDecode(const std::string& rawType, const std::string& signature) {
    const auto& type = ParameterizedType::decode(rawType);
    ASSERT_TRUE(type->kind() == kind);
    ASSERT_EQ(type->signature(), signature);
  }

  static void testType(
      const ParameterizedTypePtr& type,
      TypeKind kind,
      const std::string& signature) {
    ASSERT_EQ(type->kind(), kind);
    ASSERT_EQ(type->signature(), signature);
  }

  template <class T>
  void testDecode(
      const std::string& rawType,
      const std::function<void(const std::shared_ptr<const T>&)>&
          typeCallBack) {
    const auto& type = ParameterizedType::decode(rawType);
    if (typeCallBack) {
      typeCallBack(std::dynamic_pointer_cast<const T>(type));
    }
  }
};

TEST_F(TypeTest, typeCreator) {
  testType(BOOL(), TypeKind::kBool, "bool");
  testType(TINYINT(), TypeKind::kI8, "i8");
  testType(SMALLINT(), TypeKind::kI16, "i16");
  testType(INTEGER(), TypeKind::kI32, "i32");
  testType(BIGINT(), TypeKind::kI64, "i64");
  testType(FLOAT(), TypeKind::kFp32, "fp32");
  testType(DOUBLE(), TypeKind::kFp64, "fp64");
  testType(BINARY(), TypeKind::kBinary, "vbin");
  testType(TIMESTAMP(), TypeKind::kTimestamp, "ts");
  testType(STRING(), TypeKind::kString, "str");
  testType(TIMESTAMP_TZ(), TypeKind::kTimestampTz, "tstz");
  testType(DATE(), TypeKind::kDate, "date");
  testType(TIME(), TypeKind::kTime, "time");
  testType(INTERVAL_DAY(), TypeKind::kIntervalDay, "iday");
  testType(INTERVAL_YEAR(), TypeKind::kIntervalYear, "iyear");
  testType(UUID(), TypeKind::kUuid, "uuid");
  testType(FIXED_CHAR(12), TypeKind::kFixedChar, "fchar<12>");
  testType(FIXED_BINARY(12), TypeKind::kFixedBinary, "fbin<12>");
  testType(VARCHAR(12), TypeKind::kVarchar, "vchar<12>");
  testType(DECIMAL(12,23), TypeKind::kDecimal, "dec<12,23>");
  testType(LIST(FLOAT()), TypeKind::kList, "list<fp32>");
  testType(MAP(STRING(),FLOAT()), TypeKind::kMap, "map<str,fp32>");
  testType(STRUCT({STRING(), FLOAT()}), TypeKind::kStruct, "struct<str,fp32>");
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

  testDecode<ParameterizedFixedBinary>(
      "fixedbinary<L1>",
      [](const std::shared_ptr<const ParameterizedFixedBinary>& typePtr) {
        ASSERT_EQ(typePtr->length()->value(), "L1");
        ASSERT_EQ(typePtr->signature(), "fbin<L1>");
      });

  testDecode<ParameterizedVarchar>(
      "varchar<L1>",
      [](const std::shared_ptr<const ParameterizedVarchar>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "vchar<L1>");
        ASSERT_EQ(typePtr->length()->value(), "L1");
      });

  testDecode<ParameterizedDecimal>(
      "decimal<P,S>",
      [](const std::shared_ptr<const ParameterizedDecimal>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "dec<P,S>");
        ASSERT_EQ(typePtr->precision()->value(), "P");
        ASSERT_EQ(typePtr->scale()->value(), "S");
      });

  testDecode<ParameterizedStruct>(
      "struct<i32,i64,string>",
      [](const std::shared_ptr<const ParameterizedStruct>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,str>");
      });

  testDecode<ParameterizedStruct>(
      "struct<i32,i64,struct<string,string>>",
      [](const std::shared_ptr<const ParameterizedStruct>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,struct<str,str>>");
      });

  testDecode<ParameterizedList>(
      "list<string>",
      [](const std::shared_ptr<const ParameterizedList>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<str>");
      });
  testDecode<ParameterizedList>(
      "LIST?<any>",
      [](const std::shared_ptr<const ParameterizedList>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<any>");
        ASSERT_EQ(typePtr->nullable(),true);
      });

  testDecode<ParameterizedMap>(
      "map<string,i32>",
      [](const std::shared_ptr<const ParameterizedMap>& typePtr) {
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
        ASSERT_TRUE(typePtr->isWildcard());
      });
}
