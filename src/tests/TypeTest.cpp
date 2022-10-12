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

#include "../Type.h"
#include "iostream"
#include <gtest/gtest.h>

using namespace io::substrait;

class TypeTest : public ::testing::Test {
 protected:
  template <TypeKind kind>
  void testDecode(const std::string& rawType, const std::string& signature) {
    const auto& type = Type::decode(rawType);
    ASSERT_TRUE(type->kind() == kind);
    ASSERT_EQ(type->signature(), signature);
  }

  template <class T>
  void testDecode(
      const std::string& rawType,
      const std::function<void(const std::shared_ptr<const T>&)>&
          typeCallBack) {
    const auto& type = Type::decode(rawType);
    if (typeCallBack) {
      typeCallBack(std::dynamic_pointer_cast<const T>(type));
    }
  }
};

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
  testDecode<TypeKind::kDate>("date", "date");
  testDecode<TypeKind::kTime>("time", "time");
  testDecode<TypeKind::kIntervalDay>("interval_day", "iday");
  testDecode<TypeKind::kIntervalYear>("interval_year", "iyear");
  testDecode<TypeKind::kTimestampTz>("timestamp_tz", "tstz");
  testDecode<TypeKind::kUuid>("uuid", "uuid");

  testDecode<FixedCharType>(
      "fixedchar<L1>",
      [](const std::shared_ptr<const FixedCharType>& typePtr) {
        ASSERT_EQ(typePtr->length()->value(), "L1");
        ASSERT_EQ(typePtr->signature(), "fchar<L1>");
      });

  testDecode<FixedBinaryType>(
      "fixedbinary<L1>",
      [](const std::shared_ptr<const FixedBinaryType>& typePtr) {
        ASSERT_EQ(typePtr->length()->value(), "L1");
        ASSERT_EQ(typePtr->signature(), "fbin<L1>");
      });

  testDecode<VarcharType>(
      "varchar<L1>",
      [](const std::shared_ptr<const VarcharType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "vchar<L1>");
        ASSERT_EQ(typePtr->length()->value(), "L1");
      });

  testDecode<DecimalType>(
      "decimal<P,S>",
      [](const std::shared_ptr<const DecimalType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "dec<P,S>");
        ASSERT_EQ(typePtr->precision(), "P");
        ASSERT_EQ(typePtr->scale(), "S");
      });

  testDecode<StructType>(
      "struct<i32,i64,string>",
      [](const std::shared_ptr<const StructType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,str>");
      });

  testDecode<StructType>(
      "struct<i32,i64,struct<string,string>>",
      [](const std::shared_ptr<const StructType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "struct<i32,i64,struct<str,str>>");
      });

  testDecode<ListType>(
      "list<string>",
      [](const std::shared_ptr<const ListType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "list<str>");
      });

  testDecode<MapType>(
      "map<string,i32>",
      [](const std::shared_ptr<const MapType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "map<str,i32>");
      });

  testDecode<StringLiteralType>(
      "any1",
      [](const std::shared_ptr<const StringLiteralType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "any1");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<StringLiteralType>(
      "any",
      [](const std::shared_ptr<const StringLiteralType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "any");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<StringLiteralType>(
      "T",
      [](const std::shared_ptr<const StringLiteralType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "T");
        ASSERT_TRUE(typePtr->isWildcard());
      });

  testDecode<UsedDefinedType>(
      "unknown",
      [](const std::shared_ptr<const UsedDefinedType>& typePtr) {
        ASSERT_EQ(typePtr->signature(), "u!name");
      });
}
