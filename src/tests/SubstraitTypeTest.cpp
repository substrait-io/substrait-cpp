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

#include <gtest/gtest.h>
#include "iostream"
#include "../SubstraitType.h"

using namespace io::substrait;

class SubstraitTypeTest : public ::testing::Test {
 protected:
  template <SubstraitTypeKind kind>
  void testDecode(const std::string& rawType, const std::string& signature) {
    const auto& type = SubstraitType::decode(rawType);
    ASSERT_TRUE(type->kind() == kind);
    ASSERT_EQ(type->signature(), signature);
  }

  template <class T>
  void testDecode(
      const std::string& rawType,
      const std::function<void(const std::shared_ptr<const T>&)>&
          typeCallBack) {
    const auto& type = SubstraitType::decode(rawType);
    if (typeCallBack) {
      typeCallBack(std::dynamic_pointer_cast<const T>(type));
    }
  }
};

TEST_F(SubstraitTypeTest, decodeTest) {
  testDecode<SubstraitTypeKind::kI32>("i32?", "i32");
  testDecode<SubstraitTypeKind::kBool>("BOOLEAN", "bool");
  testDecode<SubstraitTypeKind::kBool>("boolean", "bool");
  testDecode<SubstraitTypeKind::kI8>("i8", "i8");
  testDecode<SubstraitTypeKind::kI16>("i16", "i16");
  testDecode<SubstraitTypeKind::kI32>("i32", "i32");
  testDecode<SubstraitTypeKind::kI64>("i64", "i64");
  testDecode<SubstraitTypeKind::kFp32>("fp32", "fp32");
  testDecode<SubstraitTypeKind::kFp64>("fp64", "fp64");
  testDecode<SubstraitTypeKind::kBinary>("binary", "vbin");
  testDecode<SubstraitTypeKind::kTimestamp>("timestamp", "ts");
  testDecode<SubstraitTypeKind::kDate>("date", "date");
  testDecode<SubstraitTypeKind::kTime>("time", "time");
  testDecode<SubstraitTypeKind::kIntervalDay>("interval_day", "iday");
  testDecode<SubstraitTypeKind::kIntervalYear>("interval_year", "iyear");
  testDecode<SubstraitTypeKind::kTimestampTz>("timestamp_tz", "tstz");
  testDecode<SubstraitTypeKind::kUuid>("uuid", "uuid");

  testDecode<SubstraitFixedCharType>(
      "fixedchar<L1>",
      [](const std::shared_ptr<const SubstraitFixedCharType>& typePtr) {
        ASSERT_EQ(typePtr->length()->value(), "L1");
        ASSERT_EQ(typePtr->signature(), "fchar<L1>");
      });

  testDecode<SubstraitFixedBinaryType>(
      "fixedbinary<L1>",
      [](const std::shared_ptr<const SubstraitFixedBinaryType>& typePtr) {
        ASSERT_EQ(typePtr->length()->value(), "L1");
        ASSERT_EQ(typePtr->signature(), "fbin<L1>");
      });

  testDecode<SubstraitVarcharType>(
      "varchar<L1>",
      [](const std::shared_ptr<const SubstraitVarcharType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "vchar<L1>");
        ASSERT_EQ(typePtr->length()->value(), "L1");
      });

  testDecode<SubstraitDecimalType>(
      "decimal<P,S>",
      [](const std::shared_ptr<const SubstraitDecimalType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "dec<P,S>");
        ASSERT_EQ(typePtr->precision(), "P");
        ASSERT_EQ(typePtr->scale(), "S");
      });

  testDecode<SubstraitStructType>(
      "struct<i32,i64,string>",
      [](const std::shared_ptr<const SubstraitStructType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,str>");
      });

  testDecode<SubstraitStructType>(
      "struct<i32,i64,struct<string,string>>",
      [](const std::shared_ptr<const SubstraitStructType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,struct<str,str>>");
      });

  testDecode<SubstraitListType>(
      "list<string>",
      [](const std::shared_ptr<const SubstraitListType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<str>");
      });

  testDecode<SubstraitMapType>(
      "map<string,i32>",
      [](const std::shared_ptr<const SubstraitMapType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "map<str,i32>");
      });

  testDecode<SubstraitStringLiteralType>(
      "any1",
      [](const std::shared_ptr<const SubstraitStringLiteralType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "any1");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<SubstraitStringLiteralType>(
      "any",
      [](const std::shared_ptr<const SubstraitStringLiteralType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "any");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<SubstraitStringLiteralType>(
      "T",
      [](const std::shared_ptr<const SubstraitStringLiteralType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "T");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<SubstraitUsedDefinedType>(
      "unknown",
      [](const std::shared_ptr<const SubstraitUsedDefinedType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "u!name");
      });
}
