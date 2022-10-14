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

#include "../Extension.h"
#include "iostream"

using namespace io::substrait;

class ExtensionTest : public ::testing::Test {
protected:
  void testLookupScalarFunction(const std::string &name,
                                const std::vector<TypePtr> &arguments,
                                const std::string &matchedSignature) {
    const auto &functionOption =
        registry_->lookupScalarFunction(name, arguments);

    ASSERT_TRUE(functionOption != nullptr);
    ASSERT_EQ(functionOption->signature(), matchedSignature);
  }

  /// Load registry from substrait core extension YAML files.
  ExtensionPtr registry_ = Extension::load();
};

TEST_F(ExtensionTest, comparison_function) {
  testLookupScalarFunction("lt", {kI8(), kI8()}, "lt:any1_any1");
  testLookupScalarFunction("lt", {kI16(), kI16()}, "lt:any1_any1");
  testLookupScalarFunction("lt", {kI32(), kI32()}, "lt:any1_any1");
  testLookupScalarFunction("lt", {kI64(), kI64()}, "lt:any1_any1");
  testLookupScalarFunction("lt", {kFp32(), kFp32()}, "lt:any1_any1");
  testLookupScalarFunction("lt", {kFp64(), kFp64()}, "lt:any1_any1");

  testLookupScalarFunction("between", {kI8(), kI8(), kI8()},
                           "between:any1_any1_any1");
}

TEST_F(ExtensionTest, arithmetic_function) {
  testLookupScalarFunction("add", {kI8(), kI8()}, "add:opt_i8_i8");
  testLookupScalarFunction("divide",
                           {
                               kFp32(),
                               kFp32(),
                           },
                           "divide:opt_opt_opt_fp32_fp32");

  testLookupScalarFunction("avg", {Type::decode("struct<fp64,i64>")},
                           "avg:opt_fp32");
}

TEST_F(ExtensionTest, boolean_function) {
  testLookupScalarFunction("and", {kBool(), kBool()}, "and:bool");
  testLookupScalarFunction("or", {kBool(), kBool()}, "or:bool");
  testLookupScalarFunction("not", {kBool()}, "not:bool");
  testLookupScalarFunction("xor", {kBool(), kBool()}, "xor:bool_bool");
}

TEST_F(ExtensionTest, string_function) {
  testLookupScalarFunction("like", {kString(), kString()}, "like:opt_str_str");
}

TEST_F(ExtensionTest, unknowLookup) {
  auto unknown = registry_->lookupType("unknown");
  ASSERT_TRUE(unknown);
  ASSERT_EQ(unknown->name, "unknown");
}
