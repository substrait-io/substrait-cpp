/* SPDX-License-Identifier: Apache-2.0 */

#include <gtest/gtest.h>

#include <iostream>

#include "substrait/function/FunctionLookup.h"

using namespace io::substrait;

class FunctionLookupTest : public ::testing::Test {
 protected:
  static std::string getExtensionAbsolutePath() {
    const std::string absolutePath = __FILE__;
    auto const pos = absolutePath.find_last_of('/');
    return absolutePath.substr(0, pos) +
        "/../../../../third_party/substrait/extensions/";
  }

  void SetUp() override {
    ExtensionPtr extension = Extension::load(getExtensionAbsolutePath());
    scalarFunctionLookup_ = std::make_shared<ScalarFunctionLookup>(extension);
    aggregateFunctionLookup_ =
        std::make_shared<AggregateFunctionLookup>(extension);
  }

  void testScalarFunctionLookup(
      const FunctionSignature& inputSignature,
      const std::string& outputSignature) {
    const auto& functionImpl =
        scalarFunctionLookup_->lookupFunction(inputSignature);

    ASSERT_TRUE(functionImpl != nullptr);
    ASSERT_EQ(functionImpl->signature(), outputSignature);
  }

  void testAggregateFunctionLookup(
      const FunctionSignature& inputSignature,
      const std::string& outputSignature) {
    const auto& functionImpl =
        aggregateFunctionLookup_->lookupFunction(inputSignature);

    ASSERT_TRUE(functionImpl != nullptr);
    ASSERT_EQ(functionImpl->signature(), outputSignature);
  }

 private:
  FunctionLookupPtr scalarFunctionLookup_;
  FunctionLookupPtr aggregateFunctionLookup_;
};

TEST_F(FunctionLookupTest, compareFunction) {
  testScalarFunctionLookup(
      {"lt", {tinyint(), tinyint()}, boolean()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {smallint(), smallint()}, boolean()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {integer(), integer()}, boolean()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {bigint(), bigint()}, boolean()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {float4(), float4()}, boolean()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {float8(), float8()}, boolean()}, "lt:any1_any1");
  testScalarFunctionLookup(
      {"between", {tinyint(), tinyint(), tinyint()}, boolean()},
      "between:any1_any1_any1");
}

TEST_F(FunctionLookupTest, arithmeticFunction) {
  testScalarFunctionLookup(
      {"add", {tinyint(), tinyint()}, tinyint()}, "add:i8_i8");

  testScalarFunctionLookup(
      {"divide",
       {
           float4(),
           float4(),
       },
       float4()},
      "divide:fp32_fp32");
}

TEST_F(FunctionLookupTest, aggregate) {
  // for intermediate type
  testAggregateFunctionLookup(
      {"avg", {row({float8(), bigint()})}, float4()}, "avg:fp32");
}

TEST_F(FunctionLookupTest, logical) {
  testScalarFunctionLookup({"and", {}, boolean()}, "and:bool");
  testScalarFunctionLookup({"and", {boolean()}, boolean()}, "and:bool");
  testScalarFunctionLookup(
      {"and", {boolean(), boolean()}, boolean()}, "and:bool");

  testScalarFunctionLookup(
      {"or", {boolean(), boolean()}, boolean()}, "or:bool");
  testScalarFunctionLookup({"not", {boolean()}, boolean()}, "not:bool");
  testScalarFunctionLookup(
      {"xor", {boolean(), boolean()}, boolean()}, "xor:bool_bool");
}

TEST_F(FunctionLookupTest, stringFunction) {
  testScalarFunctionLookup(
      {"like", {string(), string()}, boolean()}, "like:str_str");
  testScalarFunctionLookup(
      {"like", {varchar(3), varchar(4)}, boolean()},
      "like:vchar<L1>_vchar<L2>");
  testScalarFunctionLookup(
      {"substring", {string(), integer(), integer()}, string()},
      "substring:str_i32_i32");
}
