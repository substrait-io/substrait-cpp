/* SPDX-License-Identifier: Apache-2.0 */

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/parser/ParseText.h"
#include "substrait/textplan/tests/ParseResultMatchers.h"

using ::testing::AllOf;

namespace io::substrait::textplan {
namespace {

class TestCase {
 public:
  std::string name;
  std::string input;
  ::testing::Matcher<const ParseResult&> expectedMatch;
};

class TextPlanParserTestFixture : public ::testing::TestWithParam<TestCase> {};

std::vector<TestCase> getTestCases() {
  static std::vector<TestCase> cases = {
      {
          "test1-unused-extension-space",
          "extension_space blah.yaml {}",
          AllOf(
              HasSymbols({"blah.yaml"}),
              ParsesOk(),
              WhenSerialized(::testing::Eq(""))),
      },
      {
          "test1-used-extension-space",
          "extension_space blah.yaml { function concat:str as concat; }",
          AllOf(
              HasSymbols({"blah.yaml", "concat"}),
              ParsesOk(),
              WhenSerialized(EqSquashingWhitespace(
                  R"(extension_space blah.yaml {
                       function concat:str as concat;
                     })"))),
      },
      {
          "test1-errors",
          "extension_space blah.yaml",
          HasErrors({"1:25 → mismatched input '<EOF>' expecting '{'"}),
      },
      {
          "test2-pipelines",
          R"(pipelines {
            root -> project -> read;
          })",
          AllOf(HasSymbols({"root", "project", "read"}), ParsesOk()),
      },
      {
          "test2-duplicate-within-pipeline",
          R"(pipelines {
            myself -> myself;
          })",
          // TODO -- Add detection for duplicates.
          AllOf(HasSymbols({"myself", "myself"})),
      },
      {
          "test2-impossible-node-reuse-in-pipelines",
          R"(pipelines {
            root1 -> middle -> end1;
            root2 -> middle -> end2;
          })",
          // TODO -- Detect this situation.
          ParsesOk(),
      },
      {
          "test3-schema",
          R"(schema schema {
            r_regionkey UNKNOWN;
            r_name nullable UNKNOWN;
            r_comment UNKNOWN;
          })",
          AllOf(
              HasSymbols({"schema", "r_regionkey", "r_name", "r_comment"}),
              ParsesOk()),
      },
      {
          "test4-source",
          R"(source named_table named {
            names = [
              "#2",
            ]
          })",
          AllOf(HasSymbols({"named", "#2"}), ParsesOk()),
      },
      {
          "test4-multiple-sources",
          R"(source named_table named {
            names = [
              "a",
              "b",
            ]
          }

          source named_table named2 {
            names = [
              "a",
              "b",
              "c",
            ]
          })",
          AllOf(
              HasSymbols({"named", "a", "b", "named2", "a", "b", "c"}),
              ParsesOk()),
      },
      {
          "test5-project-relation",
          R"(project relation myproject {
            expression r_regionkey;
            expression r_name;
            expression r_comment;
            expression add(r_regionkey, 1);
            expression subtract(r_regionkey, 1);
            expression concat(r_name, r_name);
          })",
          AllOf(
              HasSymbolsWithTypes({"myproject"}, {SymbolType::kRelation}),
              WhenSerialized(
                  ::testing::Eq("not-yet-implemented\nnot-yet-implemented"))),
      },
      {
          "test6-read-relation",
          R"(read relation myread {
            base_schema schemaone;
            source mynamedtable;
          })",
          AllOf(HasSymbols({"myread"}), ParsesOk()),
      },
      {
          "test6b-capital-read-relation",
          R"(READ relation myread {
            base_schema schemaone;
            source mynamedtable;
          })",
          AllOf(HasSymbols({"myread"}), ParsesOk()),
      },
      {
          "test6.5-literals",
          R"(project relation literalexamples {
            expression true;
            expression false_bool;
            expression 12_i8;
            expression -223_i16;
            expression 12345_i32;
            expression 9999_i64;
            expression 1.23_fp32;
            expression 1.2345_fp64;
            expression 1.2345E-2_fp64;
            expression "simple text";
            expression "123"_string;
            expression "embedded bytes: \xA9\x72";
            expression "unicode char: \u{023B}";
            expression `raw string with a Windows path: C:\file.txt`;
            expression ``string with a backtick (`) in it``;
            expression "0123456789abcde"_binary;
            expression "1000-01-01 00:00:00.000000"_timestamp;
            expression "2020-12-20"_date;
            expression "13:21"_time;
            expression "13:21:12.012345"_time;
            expression {5_years, 1_month}_interval_year;
            expression {5_year, 1_months}_interval_year;
            expression {4_days, 1_second, 13_microseconds}_interval_day;
            expression "1000-01-01 00:00:00.000000 UTC"_timestamp_tz;
            expression "ddb287e8-7d4c-4fad-b2e7-07428be043e5"_uuid;
            expression "12345"_varchar<5>;
            expression "two\nlines with \"escapes\""_varchar<80>;
            expression "abcde"_fixedchar<5>;
            expression "1234"_fixedbinary<2>;
            expression 42_decimal<5,-4>;
            expression {"a", {"b", "c"}}_struct<string, struct<string, string>>;
            expression {"a", "b", "c"}_list<string>;
            expression {null, "a", "b"}_list<string?>;
            expression {42 : "life", 32 : "everything"}_map<int, string>;
            expression {}_list<string>;
            expression {}_list<string?>;
            expression {}_list?<string?>;
            expression {}_map<int, string>;
          })",
          ParsesOk(),
      },
      {
          "test6.5-bad-literals",
          R"(project relation literalexamples {
            expression 1;
            expression 1.5;
            expression "data"_potato;
          })",
          ParsesOk(),
      },
      {
          "test6.6-invalid-literals",
          R"(project relation literalexamples {
            expression {}_list<string?>?;
          })",
          HasErrors({"2:39 → extraneous input '?' expecting ';'"}),
      },
      {
          "test7-relation-without-type",
          R"(relation notyperelation {
          })",
          // TODO -- Replace this error message with something user-friendly.
          HasErrors({
              "1:0 → extraneous input 'relation' expecting {<EOF>, "
              "'EXTENSION_SPACE', 'SCHEMA', 'PIPELINES', 'FILTER', "
              "'SOURCE', COLUMN_TYPE, IDENTIFIER}",
              "1:24 → mismatched input '{' expecting 'RELATION'",
              "1:9 → Unrecognized relation type: notyperelation",
          }),
      },
      {
          "test8-relation-invalid-type",
          R"(weird relation notyperelation {
          })",
          HasErrors({"1:0 → Unrecognized relation type: weird"}),
      },
  };
  return cases;
}

TEST(TextPlanParser, LoadFromFile) {
  auto stream = loadTextFile("data/provided_sample1.splan");
  ASSERT_TRUE(stream.has_value()) << "Test input file missing.";
  auto result = parseStream(*stream);
  ASSERT_TRUE(result.successful());
}

TEST_P(TextPlanParserTestFixture, Parse) {
  auto [name, input, matcher] = GetParam();

  auto stream = loadTextString(input);
  auto result = parseStream(stream);

  ASSERT_THAT(result, matcher);
}

INSTANTIATE_TEST_SUITE_P(
    TextPlanParsingTests,
    TextPlanParserTestFixture,
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

} // namespace
} // namespace io::substrait::textplan
