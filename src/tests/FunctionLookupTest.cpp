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

#include "../FunctionLookup.h"
#include "iostream"
#include <gtest/gtest.h>

using namespace io::substrait;

class VeloxFunctionMappings : public FunctionMapping {
public:
  static const std::shared_ptr<VeloxFunctionMappings> make() {
    return std::make_shared<VeloxFunctionMappings>();
  }

  /// scalar function names in difference between  velox and Substrait.
  const FunctionMap scalaMapping() const override {
    static const FunctionMap scalarMappings{
        {"plus", "add"}, {"minus", "subtract"}, {"mod", "modulus"},
        {"eq", "equal"}, {"neq", "not_equal"},  {"substr", "substring"},
    };
    return scalarMappings;
  };
};

class SubstraitFunctionLookupTest : public ::testing::Test {
protected:
  void SetUp() override {
    ExtensionPtr extension_ = Extension::load();
    FunctionMappingPtr mappings_ =
        std::make_shared<const VeloxFunctionMappings>();
    scalarFunctionLookup_ =
        std::make_shared<ScalarFunctionLookup>(extension_, mappings_);
    aggregateFunctionLookup_ =
        std::make_shared<AggregateFunctionLookup>(extension_, mappings_);
  }

  void testScalarFunctionLookup(const std::string &name,
                                const std::vector<TypePtr> &arguments,
                                const TypePtr &returnType,
                                const std::string &outputSignature) {
    const auto &functionVariant = scalarFunctionLookup_->lookupFunction(
        FunctionSignature(name, arguments, returnType));

    ASSERT_TRUE(functionVariant != nullptr);
    ASSERT_EQ(functionVariant->signature(), outputSignature);
  }

  void testAggregateFunctionLookup(const std::string &name,
                                   const std::vector<TypePtr> &arguments,
                                   const TypePtr &returnType,
                                   const std::string &outputSignature) {

    const auto &functionVariant = aggregateFunctionLookup_->lookupFunction(
        FunctionSignature(name, arguments, returnType));

    ASSERT_TRUE(functionVariant != nullptr);
    ASSERT_EQ(functionVariant->signature(), outputSignature);
  }

private:
  FunctionLookupPtr scalarFunctionLookup_;
  FunctionLookupPtr aggregateFunctionLookup_;
};

TEST_F(SubstraitFunctionLookupTest, compare_function) {
  testScalarFunctionLookup("lt", {kI8(), kI8()}, kBool(), "lt:any1_any1");

  testScalarFunctionLookup("lt", {kI16(), kI16()}, kBool(), "lt:any1_any1");

  testScalarFunctionLookup("lt", {kI32(), kI32()}, kBool(), "lt:any1_any1");

  testScalarFunctionLookup("lt", {kI64(), kI64()}, kBool(), "lt:any1_any1");

  testScalarFunctionLookup("lt", {kFp32(), kFp32()}, kBool(), "lt:any1_any1");

  testScalarFunctionLookup("lt", {kFp64(), kFp64()}, kBool(), "lt:any1_any1");
  testScalarFunctionLookup("between", {kI8(), kI8(), kI8()}, kBool(),
                           "between:any1_any1_any1");
}

TEST_F(SubstraitFunctionLookupTest, arithmetic_function) {
  testScalarFunctionLookup("add", {kI8(), kI8()}, kI8(), "add:opt_i8_i8");

  testScalarFunctionLookup("plus", {kI8(), kI8()}, kI8(), "add:opt_i8_i8");
  testScalarFunctionLookup("divide",
                           {
                               kFp32(),
                               kFp32(),
                           },
                           kFp32(), "divide:opt_opt_opt_fp32_fp32");

  testAggregateFunctionLookup("avg", {Type::decode("struct<fp64,i64>")},
                              kFp32(), "avg:opt_fp32");
}

TEST_F(SubstraitFunctionLookupTest, avg) {}

TEST_F(SubstraitFunctionLookupTest, logical) {
  testScalarFunctionLookup("and", {kBool(), kBool()}, kBool(), "and:bool");
  testScalarFunctionLookup("or", {kBool(), kBool()}, kBool(), "or:bool");
  testScalarFunctionLookup("not", {kBool()}, kBool(), "not:bool");
  testScalarFunctionLookup("xor", {kBool(), kBool()}, kBool(), "xor:bool_bool");
}

TEST_F(SubstraitFunctionLookupTest, string_function) {
  testScalarFunctionLookup("like", {kString(), kString()}, kBool(),
                           "like:opt_str_str");
  testScalarFunctionLookup(
      "like", {Type::decode("varchar<L1>"), Type::decode("varchar<L2>")},
      kBool(), "like:opt_vchar<L1>_vchar<L2>");
  testScalarFunctionLookup("substr", {kString(), kI32(), kI32()}, kString(),
                           "substring:str_i32_i32");
}
