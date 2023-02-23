/* SPDX-License-Identifier: Apache-2.0 */

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

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
          SerializesTo(""),
      },
      {
          "empty extension space",
          "extension_uris: {\n"
          "  extension_uri_anchor: 42;\n"
          "  uri: \"http://life@everything\",\n"
          "}",
          SerializesTo(""),
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
          SerializesTo("extension_space http://life@everything {\n"
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
          SerializesTo("extension_space {\n"
                       "  function lte:fp64_fp64 as lte;\n"
                       "  function sum:fp64_fp64 as sum;\n"
                       "  function lt:fp64_fp64 as lt;\n"
                       "  function is_not_null:fp64 as is_not_null;\n"
                       "  function and:bool_bool as and;\n"
                       "  function gte:fp64_fp64 as gte;\n"
                       "  function multiply:opt_fp64_fp64 as multiply;\n"
                       "}\n"),
      },
      {
          "read local files",
          R"(relations: {
            root: {
              input: {
                read: {
                  local_files {
                    items {
                      partition_index: 0
                      start: 0
                      length: 3719
                      uri_file: "/mock_lineitem.orc"
                      orc {}
                    }
                  }
                }
              }
            }
          })",
          AllOf(
              HasSymbols({"local", "read", "root"}),
              SerializesTo(
                  "read relation read {\n"
                  "}\n"
                  "\n"
                  "source local_files local {\n"
                  "  items = [\n"
                  "    {uri_file: \"/mock_lineitem.orc\" length: 3719 orc: {}}\n"
                  "  ]\n"
                  "}\n")),
      },
      {
          "read named table",
          "relations: { root: { input: { read: { base_schema {} named_table { names: \"#2\" } } } } }",
          AllOf(
              HasSymbols({"schema", "named", "read", "root"}),
              SerializesTo("read relation read {\n"
                           "}\n"
                           "\n"
                           "schema schema {\n"
                           "}\n"
                           "\n"
                           "source named_table named {\n"
                           "  names = [\n"
                           "    \"#2\",\n"
                           "  ]\n"
                           "}\n")),
      },
      {
          "single three node pipeline",
          "relations: { root: { input: { project: { input { read: { local_files {} } } } } } }",
          HasSymbols({"local", "read", "project", "root"}),
      },
      {
          "two identical three node pipelines",
          "relations: { root: { input: { project: { input { read: { local_files {} } } } } } }"
          "relations: { root: { input: { project: { input { read: { local_files {} } } } } } }",
          HasSymbols(
              {"local",
               "read",
               "project",
               "root",
               "local2",
               "read2",
               "project2",
               "root2"}),
      },
  };
  return cases;
}

TEST_P(BinaryToTextPlanConverterTestFixture, Parse) {
  auto [name, input, matcher] = GetParam();

  auto planOrError = loadFromText(input);
  if (!planOrError.ok()) {
    ParseResult result(SymbolTable(), planOrError.errors(), {});
    ASSERT_THAT(result, matcher);
    return;
  }

  auto result = parseBinaryPlan(*planOrError);
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

TEST_F(BinaryToTextPlanConversionTest, loadFromJSON) {
  std::string json = readFromFile("data/q6_first_stage.json");
  auto planOrError = loadFromJSON(json);
  ASSERT_TRUE(planOrError.ok());
  auto plan = *planOrError;
  EXPECT_THAT(plan.extensions_size(), ::testing::Eq(7));

  auto result = parseBinaryPlan(plan);
  auto symbols = result.getSymbolTable().getSymbols();
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

          "schema",
          "local",

          "read",
          "filter",
          "project",
          "aggregate",
          "root",
      }));
}

} // namespace
} // namespace io::substrait::textplan
