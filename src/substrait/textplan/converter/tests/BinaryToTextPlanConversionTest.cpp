/* SPDX-License-Identifier: Apache-2.0 */

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/converter/InitialPlanProtoVisitor.h"
#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/ParseBinary.h"
#include "substrait/textplan/tests/ParseResultMatchers.h"

namespace io::substrait::textplan {
namespace {

class TestCase {
 public:
  std::string name;
  std::string input;
  ::testing::Matcher<const ParseResult&> expectedMatch;
};

class BinaryToTextPlanConverterTestFixture
    : public ::testing::TestWithParam<TestCase> {};

std::vector<TestCase> GetTestCases() {
  static std::vector<TestCase> cases = {
      {
          "bad proto input",
          "gibberish",
          HasErrors(
              {"1:10 → Message type \"substrait.proto.Plan\" has no field named \"gibberish\"."}),
      },
      {
          "empty plan",
          "",
          ReparsesAs(""),
      },
      {
          "empty extension space",
          "extension_uris: {\n"
          "  extension_uri_anchor: 42;\n"
          "  uri: \"http://life@everything\",\n"
          "}",
          ReparsesAs(""),
      },
      {
          "used extension space",
          "extension_uris: {\n"
          "  extension_uri_anchor: 42;\n"
          "  uri: \"http://life@everything\",\n"
          "}\n"
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 42\n"
          "    function_anchor: 5\n"
          "    name: \"sum:fp64_fp64\"\n"
          "  }\n"
          "}\n",
          ReparsesAs("extension_space http://life@everything {\n"
                     "  function sum:fp64_fp64 as sum;\n"
                     "}\n"),
      },
      {
          "seven extensions, no uris",
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 0\n"
          "    function_anchor: 4\n"
          "    name: \"lte:fp64_fp64\"\n"
          "  }\n"
          "}\n"
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 0\n"
          "    function_anchor: 5\n"
          "    name: \"sum:fp64_fp64\"\n"
          "  }\n"
          "}\n"
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 0\n"
          "    function_anchor: 3\n"
          "    name: \"lt:fp64_fp64\"\n"
          "  }\n"
          "}\n"
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 0\n"
          "    function_anchor: 0\n"
          "    name: \"is_not_null:fp64\"\n"
          "  }\n"
          "}\n"
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 0\n"
          "    function_anchor: 1\n"
          "    name: \"and:bool_bool\"\n"
          "  }\n"
          "}\n"
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 0\n"
          "    function_anchor: 2\n"
          "    name: \"gte:fp64_fp64\"\n"
          "  }\n"
          "}\n"
          "extensions: {\n"
          "  extension_function: {\n"
          "    extension_uri_reference: 0\n"
          "    function_anchor: 6\n"
          "    name: \"multiply:opt_fp64_fp64\"\n"
          "  }\n"
          "}\n",
          ReparsesAs("extension_space {\n"
                     "  function lte:fp64_fp64 as lte;\n"
                     "  function sum:fp64_fp64 as sum;\n"
                     "  function lt:fp64_fp64 as lt;\n"
                     "  function is_not_null:fp64 as is_not_null;\n"
                     "  function and:bool_bool as and;\n"
                     "  function gte:fp64_fp64 as gte;\n"
                     "  function multiply:opt_fp64_fp64 as multiply;\n"
                     "}\n"),
      },
  };
  return cases;
}

TEST_P(BinaryToTextPlanConverterTestFixture, Parse) {
  auto [name, input, matcher] = GetParam();

  const auto& planOrError = LoadFromText(input);
  if (std::holds_alternative<std::vector<std::string>>(planOrError)) {
    auto errors = std::get<std::vector<std::string>>(planOrError);
    ParseResult result(SymbolTable(), errors, {});
    ASSERT_THAT(result, matcher);
    return;
  }

  auto plan = std::get<::substrait::proto::Plan>(planOrError);
  auto result = ParseBinaryPlan(plan);

  ASSERT_THAT(result, matcher);
}

INSTANTIATE_TEST_SUITE_P(
    BinaryPlanConversionTests,
    BinaryToTextPlanConverterTestFixture,
    ::testing::ValuesIn(GetTestCases()),
    [](const testing::TestParamInfo<TestCase>& info) {
      std::string identifier = info.param.name;
      // Remove non-alphanumeric characters to make the test framework happy.
      identifier.erase(
          std::remove_if(
              identifier.begin(),
              identifier.end(),
              [](auto const& c) -> bool { return !std::isalnum(c); }),
          identifier.end());
      return identifier;
    });

class BinaryToTextPlanConversionTest : public ::testing::Test {};

TEST_F(BinaryToTextPlanConversionTest, LoadFromJSON) {
  std::string json = ReadFromFile("data/q6_first_stage.json");
  auto plan = LoadFromJSON(json);
  EXPECT_THAT(plan.extensions_size(), ::testing::Eq(7));

  auto result = ParseBinaryPlan(plan);
  auto symbols = result.GetSymbolTable()->GetSymbols();
  ASSERT_THAT(
      result,
      HasSymbols({
          "lte",
          "sum",
          "lt",
          "is_not_null",
          "and",
          "gte",
          "multiply",
      }));
}

} // namespace
} // namespace io::substrait::textplan