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

#include <iostream>
#include <gtest/gtest.h>
#include "substrait/function/FunctionLookup.h"

using namespace substrait;

class FunctionLookupTest : public ::testing::Test {
 protected:
  static std::string getExtensionAbsolutePath() {
    const std::string absolute_path = __FILE__;
    auto const pos = absolute_path.find_last_of('/');
    return absolute_path.substr(0, pos) +
        "/../../../third_party/substrait/extensions/";
  }

  void SetUp() override {
    ExtensionPtr extension_ = Extension::load(getExtensionAbsolutePath());
    scalarFunctionLookup_ =
        std::make_shared<ScalarFunctionLookup>(extension_);
    aggregateFunctionLookup_ =
        std::make_shared<AggregateFunctionLookup>(extension_);
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

TEST_F(FunctionLookupTest, compare_function) {
  testScalarFunctionLookup(
      {"lt", {TINYINT(), TINYINT()}, BOOL()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {SMALLINT(), SMALLINT()}, BOOL()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {INTEGER(), INTEGER()}, BOOL()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {BIGINT(), BIGINT()}, BOOL()}, "lt:any1_any1");

  testScalarFunctionLookup({"lt", {FLOAT(), FLOAT()}, BOOL()}, "lt:any1_any1");

  testScalarFunctionLookup(
      {"lt", {DOUBLE(), DOUBLE()}, BOOL()}, "lt:any1_any1");
  testScalarFunctionLookup(
      {"between", {TINYINT(), TINYINT(), TINYINT()}, BOOL()},
      "between:any1_any1_any1");
}

TEST_F(FunctionLookupTest, arithmetic_function) {
  testScalarFunctionLookup(
      {"add", {TINYINT(), TINYINT()}, TINYINT()}, "add:opt_i8_i8");

  testScalarFunctionLookup(
      {"divide",
       {
           FLOAT(),
           FLOAT(),
       },
       FLOAT()},
      "divide:opt_opt_opt_fp32_fp32");
}

TEST_F(FunctionLookupTest, aggregate) {
  // for intermediate type
  testAggregateFunctionLookup(
      {"avg", {ROW({DOUBLE(), BIGINT()})}, FLOAT()}, "avg:opt_fp32");
}

TEST_F(FunctionLookupTest, logical) {
  testScalarFunctionLookup({"and", {}, BOOL()}, "and:bool");
  testScalarFunctionLookup({"and", {BOOL()}, BOOL()}, "and:bool");
  testScalarFunctionLookup({"and", {BOOL(), BOOL()}, BOOL()}, "and:bool");

  testScalarFunctionLookup({"or", {BOOL(), BOOL()}, BOOL()}, "or:bool");
  testScalarFunctionLookup({"not", {BOOL()}, BOOL()}, "not:bool");
  testScalarFunctionLookup({"xor", {BOOL(), BOOL()}, BOOL()}, "xor:bool_bool");
}

TEST_F(FunctionLookupTest, string_function) {
  testScalarFunctionLookup(
      {"like", {STRING(), STRING()}, BOOL()}, "like:opt_str_str");
  testScalarFunctionLookup(
      {"like", {VARCHAR(3), VARCHAR(4)}, BOOL()},
      "like:opt_vchar<L1>_vchar<L2>");
  testScalarFunctionLookup(
      {"substring", {STRING(), INTEGER(), INTEGER()}, STRING()},
      "substring:str_i32_i32");
}
