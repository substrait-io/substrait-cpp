/* SPDX-License-Identifier: Apache-2.0 */

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/ParseBinary.h"
#include "substrait/textplan/tests/ParseResultMatchers.h"

namespace io::substrait::textplan {

using ::testing::Eq;

namespace {

class TestCase {
 public:
  std::string name;
  std::string input;
  ::testing::Matcher<const ParseResult&> expectedMatch;
};

class BinaryToTextPlanConverterTestFixture
    : public ::testing::TestWithParam<TestCase> {};

std::vector<TestCase> getTestCases() {
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
          WhenSerialized(Eq("")),
      },
      {
          "empty extension space",
          R"(extension_uris: {
            extension_uri_anchor: 42;
            uri: "http://life@everything",
          })",
          WhenSerialized(Eq("")),
      },
      {
          "used extension space",
          R"(extension_uris: {
            extension_uri_anchor: 42;
            uri: "http://life@everything",
          }
          extensions: {
            extension_function: {
              extension_uri_reference: 42
              function_anchor: 5
              name: "sum:fp64_fp64"
            }
          })",
          WhenSerialized(EqSquashingWhitespace(
              R"(extension_space http://life@everything {
                   function sum:fp64_fp64 as sum;
                 })")),
      },
      {
          "seven extensions, no uris",
          R"(extensions: {
            extension_function: {
              extension_uri_reference: 0
              function_anchor: 4
              name: "lte:fp64_fp64"
            }
          }
          extensions: {
            extension_function: {
              extension_uri_reference: 0
              function_anchor: 5
              name: "sum:fp64_fp64"
            }
          }
          extensions: {
            extension_function: {
              extension_uri_reference: 0
              function_anchor: 3
              name: "lt:fp64_fp64"
            }
          }
          extensions: {
            extension_function: {
              extension_uri_reference: 0
              function_anchor: 0
              name: "is_not_null:fp64"
            }
          }
          extensions: {
            extension_function: {
              extension_uri_reference: 0
              function_anchor: 1
              name: "and:bool_bool"
            }
          }
          extensions: {
            extension_function: {
              extension_uri_reference: 0
              function_anchor: 2
              name: "gte:fp64_fp64"
            }
          }
          extensions: {
            extension_function: {
              extension_uri_reference: 0
              function_anchor: 6
              name: "multiply:opt_fp64_fp64"
            }
          })",
          WhenSerialized(EqSquashingWhitespace(
              R"(extension_space {
                   function lte:fp64_fp64 as lte;
                   function sum:fp64_fp64 as sum;
                   function lt:fp64_fp64 as lt;
                   function is_not_null:fp64 as is_not_null;
                   function and:bool_bool as and;
                   function gte:fp64_fp64 as gte;
                   function multiply:opt_fp64_fp64 as multiply;
                 })")),
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
              WhenSerialized(EqSquashingWhitespace(
                  R"(read relation read {
                  }

                  source local_files local {
                    items = [
                      {uri_file: "/mock_lineitem.orc" start: 0 length: 3719 orc: {}}
                    ]
                  })"))),
      },
      {
          "read named table",
          "relations: { root: { input: { read: { base_schema {} named_table { names: \"#2\" } } } } }",
          AllOf(
              HasSymbols({"schema", "named", "read", "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(read relation read {
                     }

                     schema schema {
                     }

                     source named_table named {
                       names = [
                         "#2",
                       ]
                     })"))),
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
    ::testing::ValuesIn(getTestCases()),
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
  auto planOrError = loadFromJson(json);
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
