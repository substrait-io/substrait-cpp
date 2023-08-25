/* SPDX-License-Identifier: Apache-2.0 */

#include <algorithm>
#include <memory>
#include <string>
#include <utility>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <protobuf-matchers/protocol-buffer-matchers.h>

#include "substrait/textplan/parser/ParseText.h"
#include "substrait/textplan/tests/ParseResultMatchers.h"

using ::protobuf_matchers::EqualsProto;
using ::protobuf_matchers::IgnoringFieldPaths;
using ::protobuf_matchers::Partially;
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
          "test2-pipelines-no-relations",
          R"(pipelines {
            root -> project -> read;
          })",
          AllOf(
              HasSymbols({"read", "project", "root"}),
              ParsesOk(),
              WhenSerialized(testing::HasSubstr("pipelines {\n"
                                                "  root -> project -> read;\n"
                                                "}\n"))),
      },
      {
          "test2-duplicate-within-pipeline-no-relation",
          R"(pipelines {
            myself -> myself;
          })",
          HasErrors({"2:12 → Relation myself is already in this pipeline."}),
      },
      {
          "test2-duplicate-within-pipeline-with-relation",
          R"(pipelines {
            myself -> myself;
          }

          read relation myself {
            base_schema schemaone;
            source mynamedtable;
          })",
          HasErrors({"2:12 → Relation myself is already in this pipeline."}),
      },
      {
          "test2-duplicate-within-pipeline-with-intervening-node",
          R"(pipelines {
            myself -> other -> myself -> other2;
          })",
          HasErrors({"2:12 → Relation myself is already in this pipeline."}),
      },
      {
          "test2-duplicate-end-node-within-pipeline-with-intervening-node",
          R"(pipelines {
            myself -> other -> myself;
          })",
          HasErrors({"2:12 → Relation myself is already in this pipeline."}),
      },
      {
          "test2-duplicate-node-within-pipeline-with-intervening-nodes",
          R"(pipelines {
            starter -> myself -> other -> myself;
          })",
          HasErrors({"2:23 → Relation myself is already in this pipeline."}),
      },
      {
          "test2-legal-two-pipeline-participant",
          R"(pipelines {
            myself -> a;
            b -> myself;
          })",
          AllOf(
              HasSymbols({"a", "myself", "b"}),
              WhenSerialized(EqSquashingWhitespace(R"(pipelines {
                myself -> a;
                b -> myself;
              })")),
              AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
                  R"(relations {
                    root {
                      input {
                        # No relation node means this will be empty.
                      }
                    }
                   })"))),
      },
      {
          "test2-legal-two-pipeline-participant-alternate-order",
          R"(pipelines {
            b -> myself;
            myself -> a;
          })",
          AllOf(
              HasSymbols({"myself", "b", "a"}),
              WhenSerialized(EqSquashingWhitespace(R"(pipelines {
                b -> myself;
                myself -> a;
              })")),
              AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
                  R"(relations {
                    root {
                      input {
                        # No relation node means this will be empty.
                      }
                    }
                  })"))),
      },
      {
          "test2-impossible-node-reuse-in-pipelines",
          R"(pipelines {
            root1 -> middle -> end1;
            root2 -> middle -> end2;
          })",
          HasErrors({"3:21 → Relation middle is already a non-terminating "
                     "participant in a pipeline."}),
      },
      {
          "test3-schema",
          R"(schema schema {
            r_regionkey i32;
            r_name string?;
            r_comment string;
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
          R"(pipelines {
            read -> myproject -> root;
          }

          extension_space blah.yaml {
            function add:i8 as add;
            function subtract:i8 AS subtract;
            function concat:str AS concat;
          }

          read relation read {
            base_schema schema;
          }

          project relation myproject {
            expression r_regionkey;
            expression r_name;
            expression r_comment;
            expression add(r_regionkey, 1_i8)->i8;
            expression subtract(r_regionkey, 1_i8)->i8;
            expression concat(r_name, r_name)->str;
          }

          schema schema {
            r_regionkey i32;
            r_name string;
            r_comment string?;
          })",
          AllOf(
              HasSymbolsWithTypes(
                  {"read", "myproject", "root"}, {SymbolType::kRelation}),
              HasErrors({}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                    read -> myproject -> root;
                  }

                  project relation myproject {
                    expression schema.r_regionkey;
                    expression schema.r_name;
                    expression schema.r_comment;
                    expression add(schema.r_regionkey, 1_i8)->i8;
                    expression subtract(schema.r_regionkey, 1_i8)->i8;
                    expression concat(schema.r_name, schema.r_name)->string;
                  }

                  extension_space blah.yaml {
                    function add:i8 as add;
                    function concat:str as concat;
                    function subtract:i8 as subtract;
                  })")),
              AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
                  R"(extension_uris {
                    extension_uri_anchor: 1 uri: "blah.yaml"
                  }
                  extensions {
                    extension_function {
                      extension_uri_reference: 1 name: "add:i8" }
                  }
                  extensions {
                    extension_function {
                      extension_uri_reference: 1 function_anchor: 1
                      name: "subtract:i8" }
                  }
                  extensions {
                    extension_function {
                      extension_uri_reference: 1 function_anchor: 2
                      name: "concat:str" }
                  }
                  relations { root { input { project {
                    common { direct { } }
                    input {
                      read { common { direct { } }
                      base_schema {
                        names: "r_regionkey"
                        names: "r_name"
                        names: "r_comment"
                        struct { types { i32 {
                          nullability: NULLABILITY_REQUIRED } }
                          types { string { nullability: NULLABILITY_REQUIRED } }
                          types { string { nullability: NULLABILITY_NULLABLE } }
                          nullability: NULLABILITY_REQUIRED } } }
                    }
                    expressions {
                      selection {
                        direct_reference {
                          struct_field {
                            field: 0
                          }
                        }
                        root_reference: { }
                      }
                    }
                    expressions {
                      selection {
                        direct_reference {
                          struct_field {
                            field: 1
                          }
                        } root_reference: { }
                      }
                    }
                    expressions {
                      selection {
                        direct_reference {
                          struct_field {
                            field: 2
                          }
                        } root_reference: { }
                      }
                    }
                    expressions { scalar_function {
                      function_reference: 0 arguments { value { selection {
                        direct_reference { struct_field { } } root_reference: { } } } }
                        arguments { value { literal { i8: 1 } } }
                        output_type {
                          i8 { nullability: NULLABILITY_REQUIRED} } } }
                    expressions { scalar_function {
                      function_reference: 1 arguments { value { selection {
                        direct_reference { struct_field { } } root_reference: { } } } }
                        arguments { value { literal { i8: 1 } } }
                        output_type {
                          i8 { nullability: NULLABILITY_REQUIRED } } } }
                    expressions { scalar_function {
                      function_reference: 2 arguments { value { selection {
                        direct_reference { struct_field { field: 1 } } root_reference: { } } } }
                        arguments { value { selection { direct_reference {
                          struct_field { field: 1 } } root_reference: { } } } }
                        output_type {
                          string { nullability: NULLABILITY_REQUIRED } } } }
                  } } } })"))),
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
          "test7-bad-filter-relation",
          R"(filter relation filter {
            condition true_bool;
          })",
          HasErrors(
              {"2:22 → missing 'FILTER' at 'true'",
               "2:12 → Best effort and post join are the only two legal filter behavior choices.  You may also not provide one which will result to the default filter behavior.",
               "2:12 → Best effort and post join are the only two legal filter behavior choices.  You may also not provide one which will result to the default filter behavior."}),
      },
      {
          "test10-literals-boolean",
          R"(project relation literalexamples {
            expression true;
            expression true_bool?;
            expression false_bool;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { boolean: true } }
                expressions { literal { boolean: true } }
                expressions { literal { boolean: false } }
              } } } })"))),
      },
      {
          "test10-literals-boolean-nulls",
          R"(project relation literalexamples {
            expression null_bool;
            expression null_bool?;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { null: { bool { nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { bool { nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-integers",
          R"(project relation literalexamples {
            expression 12_i8;
            expression -223_i16;
            expression 12345_i32;
            expression 9999_i64?;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { i8: 12 } }
                expressions { literal { i16: -223 } }
                expressions { literal { i32: 12345 } }
                expressions { literal { i64: 9999 nullable: true } }
              } } } })"))),
      },
      {
          "test10-literals-integers-nulls",
          R"(project relation literalexamples {
            expression null_i8;
            expression null_i16?;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { null: { i8 { nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { i16 { nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-floats",
          R"(project relation literalexamples {
            expression 1.23_fp32;
            expression -1.2345_fp64;
            expression 1.2345E-2_fp64?;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { fp32: 1.23 } }
                expressions { literal { fp64: -1.2345 } }
                expressions { literal { fp64: .012345 nullable: true } }
              } } } })"))),
      },
      {
          "test10-literals-floats-nulls",
          R"(project relation literalexamples {
            expression null_fp32;
            expression null_fp64?;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { null: { fp32 { nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { fp64 { nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-decimal",
          R"(project relation literalexamples {
            expression 42_decimal<5,4>;
            expression 42_decimal<5,-4>;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { decimal: { value:
                  "\052\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
                  precision: 5 scale: 4 } } }
                expressions { literal { decimal: { value:
                  "\052\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
                  precision: 5 scale: -4 } } }
              } } } })"))),
      },
      {
          "test10-literals-decimal-nulls",
          R"(project relation literalexamples {
            expression null_decimal<3, 2>?;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { null: { decimal { scale: 2 precision: 3
                  nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-strings",
          R"(project relation literalexamples {
            expression "simple text";
            expression "123"_string;
            expression "basic escapes: \\\'\"\b\f\n\r\t"_string;
            expression "embedded bytes: \xA9\x72";
            expression "unicode char: \u{02}";
            expression "unicode char: \u{023B}";
            expression "unicode char: \u{023B41}";
            expression `raw string with a Windows path: C:\file.txt`;
            expression ``string with a backtick (`) in it``;
            expression "12345"_varchar<5>;
            expression "two\nlines with \"escapes\""_varchar<80>;
            expression "abcde"_fixedchar<5>;
          })",
          AllOf(
              HasErrors({}),
              AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
                  R"(relations { root { input { project {
                expressions { literal { string: "simple text" } }
                expressions { literal { string: "123" } }
                expressions { literal {
                  string: "basic escapes: \\\'\"\b\f\n\r\t" } }
                expressions { literal { string: "embedded bytes: \xA9\x72" } }
                expressions { literal { string: "unicode char: \x02" } }
                expressions { literal { string: "unicode char: \x02\x3B" } }
                expressions { literal { string: "unicode char: \x02\x3B\x41" } }
                expressions { literal {
                  string: "raw string with a Windows path: C:\\file.txt" } }
                expressions { literal {
                  string: "string with a backtick (`) in it" } }
                expressions { literal {
                  var_char: { value: "12345" length: 5 } } }
                expressions { literal {
                  var_char: { value: "two\nlines with \"escapes\""
                              length: 80 } } }
                expressions { literal { fixed_char: "abcde" } }
              } } } })")))),
      },
      {
          "test10-literals-strings-nulls",
          R"(project relation literalexamples {
            expression null_str;
            expression null_string;
            expression null_varchar<5>;
            expression null_fixedchar<5>?;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { null: { string {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { string {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: {
                  varchar { length: 5 nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: {
                  fixed_char { length: 5 nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-binary",
          R"(project relation literalexamples {
            expression "0123456789abcde"_binary;
            expression "ddb287e8-7d4c-4fad-b2e7-07428be043e5"_uuid;
            expression "ddb287e87d4c4fadb2e707428be043e5"_uuid;
            expression "1234"_fixedbinary<2>;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { binary: "0123456789abcde" } }
                expressions { literal { uuid: "ddb287e87d4c4fadb2e707428be043e5" } }
                expressions { literal { uuid: "ddb287e87d4c4fadb2e707428be043e5" } }
                expressions { literal { fixed_binary: "1234" } }
              } } } })"))),
      },
      {
          "test10-literals-binary-nulls",
          R"(project relation literalexamples {
            expression null_binary;
            expression null_uuid?;
            expression null_fixedbinary<2>;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { null: { binary {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { uuid {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { fixed_binary {
                  nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-time",
          R"(project relation literalexamples {
            expression "2000-01-01 00:00:00"_timestamp;
            expression "2000-01-01 00:00:00.000000"_timestamp;
            expression "2020-12-20"_date;
            expression "13:21"_time;
            expression "13:21:03"_time;
            expression "13:21:12.012345"_time;
            expression {5_years, 1_month}_interval_year;
            expression {5_year, 1_months}_interval_year;
            expression {4_days, 1_second, 13_microseconds}_interval_day;
            expression "2000-01-01 00:00:00 US/Central"_timestamp_tz;
            expression "2000-01-01 00:00:00.000000 UTC"_timestamp_tz;
          })",
          AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                common { direct { } }
                expressions { literal { timestamp: 946684800 } }
                expressions { literal { timestamp: 946684800 } }
                expressions { literal { date: 18616 } }
                expressions { literal { time: 48060000000 } }
                expressions { literal { time: 48063000000 } }
                expressions { literal { time: 48072012345 } }
                expressions { literal {
                  interval_year_to_month { years: 5 months: 1 } } }
                expressions { literal {
                  interval_year_to_month { years: 5 months: 1 } } }
                expressions { literal {
                  interval_day_to_second {
                    days: 4 seconds: 1 microseconds: 13 } } }
                expressions { literal { timestamp_tz: 946706400 } }
                expressions { literal { timestamp_tz: 946684800 } }
              } } } })")),
      },
      {
          "test10-literals-time-nulls",
          R"(project relation literalexamples {
            expression null_timestamp;
            expression null_date;
            expression null_time;
            expression null_interval_year;
            expression null_interval_day?;
            expression null_timestamp_tz;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { null: {
                  timestamp { nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { date {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { time {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { interval_year {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { interval_day {
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null: { timestamp_tz {
                  nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-lists",
          R"(project relation literalexamples {
            expression {"a", "b", "c"}_list<string>;
            expression {null, "a", "b"}_list<string?>;
            expression {{"a", "b"}, {"1", "2"}}_list?<list<string>>;
            expression {}_list<string>;
            expression {}_list<string?>;
            expression {}_list?<string>;
          })",
          AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                common { direct { } }
                expressions { literal { list {
                  values { string: "a" }
                  values { string: "b" }
                  values { string: "c" }
                } } }
                expressions { literal { list {
                  values { null { string { nullability: NULLABILITY_NULLABLE } } }
                  values { string: "a" }
                  values { string: "b" }
                } } }
                expressions { literal { list { values { list {
                  values { string: "a" }
                  values { string: "b" }
                } } values { list {
                  values { string: "1" }
                  values { string: "2" }
                } } } } }
                expressions { literal { empty_list { type { string {
                    nullability: NULLABILITY_REQUIRED } }
                  nullability: NULLABILITY_REQUIRED } } }
                expressions { literal { empty_list { type { string {
                    nullability: NULLABILITY_NULLABLE } }
                  nullability: NULLABILITY_REQUIRED } } }
                expressions { literal { empty_list { type { string {
                    nullability: NULLABILITY_REQUIRED } }
                  nullability: NULLABILITY_NULLABLE } } }
              } } } })")),
      },
      {
          "test10-literals-lists-nulls",
          R"(project relation literalexamples {
            expression null_list<string>;
            expression null_list<list<string?>>;
            expression null_list?<list<string>>;
          })",
          AsBinaryPlan((EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                common { direct { } }
                expressions { literal { null { list { type { string { nullability: NULLABILITY_REQUIRED } }
                  nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null { list { type {
                  list { type { string {
                    nullability: NULLABILITY_NULLABLE } } nullability: NULLABILITY_REQUIRED } }
                      nullability: NULLABILITY_NULLABLE } } } }
                expressions { literal { null { list { type {
                  list { type { string { nullability: NULLABILITY_REQUIRED } } nullability: NULLABILITY_REQUIRED } }
                   nullability: NULLABILITY_NULLABLE } } } }
              } } } })"))),
      },
      {
          "test10-literals-maps",
          R"(project relation literalexamples {
            expression {42 : "life", 32 : "everything"}_map<i16, string>;
            expression {}_map<fp32, string>;
          })",
          AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                common { direct { } }
                expressions { literal { map {
                  key_values { key { i16: 42 } value { string: "life" } }
                  key_values { key { i16: 32 } value { string: "everything" } }
                } } }
                expressions { literal {
                  empty_map { key { fp32 { nullability: NULLABILITY_REQUIRED } } value { string {nullability: NULLABILITY_REQUIRED  } } nullability: NULLABILITY_REQUIRED} } }
              } } } })")),
      },
      {
          "test10-literals-maps-nulls",
          R"(project relation literalexamples {
            expression null_map<i8, string>;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal {
                  null { map { key { i8 {} } value { string {} } } } } }
              } } } })"))),
      },
      {
          "test10-literals-struct",
          R"(project relation literalexamples {
            expression {"a", {"b", "c"}}_struct<string, struct<string, string>>;
          })",
          AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                common { direct { } }
                expressions { literal { struct {
                  fields { string: "a" }
                  fields { struct {
                    fields { string: "b" }
                    fields { string: "c" }
                  } }
                } } }
              } } } })")),
      },
      {
          "test10-literals-struct-nulls",
          R"(project relation literalexamples {
            expression null_struct<string, struct<string, string>>;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
              expressions { literal { null { struct {
                types { string {} }
                types { struct {
                  types { string {} }
                  types { string {} } } } } } } }
              } } } })"))),
      },
      {
          "test10-literals-lists-of-structs",
          R"(project relation literalexamples {
            expression {{"a", 12}, {"b", -13}, {"c", 0}}_list<struct<string, i32>>;
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { literal { list {
                  values { struct { fields { string: "a" } fields { i32: 12 } } }
                  values { struct { fields { string: "b" } fields { i32: -13 } } }
                  values { struct { fields { string: "c" } fields { i32: 0 } } }
                } } }
              }} } })"))),
      },
      {
          "test11-bad-numeric-literals",
          R"(project relation literalexamples {
            expression 1;
            expression 1.5;
            expression 1_potato;
            expression null;
            expression 42_decimal<r5,-4>;
            expression 42_decimal<r,-4>;
            expression 42_decimal<-5,-4>;
          })",
          HasErrors({
              "6:34 → mismatched input 'r5' expecting NUMBER",
              "6:36 → mismatched input ',' expecting 'FILTER'",
              "7:34 → mismatched input 'r' expecting NUMBER",
              "7:35 → mismatched input ',' expecting 'FILTER'",
              "2:23 → Literals should include a type.",
              "3:23 → Literals should include a type.",
              "4:25 → Unable to recognize requested type.",
              "5:23 → Null literals require type.",
              "6:26 → Failed to decode type.",
              "6:34 → Best effort and post join are the only two legal filter behavior choices.  You may also not provide one which will result to the default filter behavior.",
              "6:34 → Best effort and post join are the only two legal filter behavior choices.  You may also not provide one which will result to the default filter behavior.",
              "6:34 → Filters are not permitted for this kind of relation.",
              "7:26 → Failed to decode type.",
              "7:34 → Best effort and post join are the only two legal filter behavior choices.  You may also not provide one which will result to the default filter behavior.",
              "7:34 → Best effort and post join are the only two legal filter behavior choices.  You may also not provide one which will result to the default filter behavior.",
              "7:34 → Filters are not permitted for this kind of relation.",
              "8:23 → Could not parse literal as decimal.",
          }),
      },
      {
          "test11-bad-stringlike-literals",
          R"(project relation literalexamples {
            expression "data"_potato;
            expression "ddb287e8"_uuid;
            expression "nothex"_uuid;
            expression "unknown\escape"_string;
            expression "abcde"_fixedchar;
          })",
          HasErrors({
              "2:30 → Unable to recognize requested type.",
              "3:23 → UUIDs are 128 bits long and thus should be specified with exactly 32 hexadecimal digits.",
              "4:23 → UUIDs should be be specified with hexadecimal characters with optional dashes only.",
              "5:31 → Unknown slash escape sequence.",
              "6:31 → Unable to recognize requested type.",
          }),
      },
      {
          "test11-bad-complex-literals",
          R"(project relation literalexamples {
            expression {}_list<string>?;
            expression {}_struct<a>;
            expression {}_struct<>;
            expression {}_map<>;
            expression {}_map<string>;
            expression {}_map<,string>;
            expression {}_map<,>;
            expression {}_list<>;
            expression {123_i8}_map<i8, bool>;
            expression {123}_map<i8, bool>;
          })",
          HasErrors({
              "2:38 → extraneous input '?' expecting {'NAMED', ';'}",
              "3:26 → Unable to recognize requested type.",
              "4:26 → Unable to recognize requested type.",
              "5:26 → Maps require both a key and a value type.",
              "5:23 → Unsupported type 0.",
              "6:26 → Maps require both a key and a value type.",
              "6:23 → Unsupported type 0.",
              "7:26 → Unable to recognize requested type.",
              "8:26 → Unable to recognize requested type.",
              "8:26 → Unable to recognize requested type.",
              "9:26 → Unable to recognize requested type.",
              "10:23 → Map literals require pairs of values separated by colons.",
              "11:23 → Map literals require pairs of values separated by colons.",
          }),
      },
      {
          "test12-casts",
          R"(project relation literalexamples {
            expression 123_i8 AS i32;
            expression 123_i8 AS i32 AS i64;
          })",
          AllOf(
              HasErrors({}),
              AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
                  R"(relations { root { input { project {
                    common { direct { } }
                    expressions { cast { type { i32 {
                      nullability: NULLABILITY_REQUIRED } }
                      input { literal { i8: 123 } } } }
                    expressions { cast { type { i64 {
                      nullability: NULLABILITY_REQUIRED } }
                      input { cast { type { i32 {
                        nullability: NULLABILITY_REQUIRED } }
                        input { literal { i8: 123 } } } } } }
                  } } } })"))),
      },
      {
          "test13-functions",
          R"(extension_space blah.yaml {
            function myfunc:str as myfunc;
            function otherfunc:str AS otherfunc;
          }

          project relation literalexamples {
            expression myfunc(otherfunc(1_i32, -2_i32), "foo"_string);
          })",
          AsBinaryPlan(Partially(EqualsProto<::substrait::proto::Plan>(
              R"(relations { root { input { project {
                expressions { scalar_function { function_reference: 0
                  arguments { value { scalar_function { function_reference: 1
                    arguments { value { literal { i32: 1 } } }
                    arguments { value { literal { i32: -2 } } }
                  } } }
                  arguments { value { literal { string: "foo" } } }
                } }
              } } } })"))),
      },
      {
          "test13-bad-functions",
          R"(extension_space blah.yaml {
            function sum: as sum;
            function sum as sum;
            function sum;
          })",
          HasErrors({
              "3:25 → missing ':' at 'as'",
              "4:24 → missing ':' at ';'",
              "2:12 → Functions should have an associated type.",
          }),
      },
      {
          "test14-three-node-pipeline-with-fields",
          R"(pipelines {
            read -> join;
            read2 -> join;
            join -> join2;
            read3 -> join2;
            join2 -> root;
          }

          read relation read {
            source named;
            base_schema schema;
          }

          read relation read2 {
            source named2;
            base_schema schema2;
          }

          join relation join {
            type UNSPECIFIED;
            expression product_id;
            post_join filter count;
          }

          read relation read3 {
            source named3;
            base_schema schema3;
          }

          join relation join2 {
            type UNSPECIFIED;
            expression order_id;
          }

          schema schema {
            order_id i32;
            product_id i32;
            count i64;
          }

          schema schema2 {
            product_id i32;
            cost fp32;
          }

          schema schema3 {
            company string;
            order_id i32;
          }

          source named_table named {
            names = [
              "#1",
            ]
          }

          source named_table named2 {
            names = [
              "#2",
            ]
          }

          source named_table named3 {
            names = [
              "#3",
            ]
          })",
          AsBinaryPlan(IgnoringFieldPaths(
              {"relations[0].root.input.join.left.join.expression.selection.direct_reference.struct_field.field",
               "relations[0].root.input.join.left.join.post_join_filter.selection.direct_reference.struct_field.field",
               "relations[0].root.input.join.expression.selection.direct_reference.struct_field.field"},
              EqualsProto<::substrait::proto::Plan>(
                  R"(relations: {
                root: {
                  input: {
                    join: {
                      common { direct { } }
                      left: {
                        join: {
                          common { direct { } }
                          left: {
                            read: {
                              common { direct { } }
                              base_schema {
                                names: "order_id"
                                names: "product_id"
                                names: "count"
                                struct {
                                  types { i32 { nullability: NULLABILITY_REQUIRED } }
                                  types { i32 { nullability: NULLABILITY_REQUIRED } }
                                  types { i64 { nullability: NULLABILITY_REQUIRED } }
                                  nullability: NULLABILITY_REQUIRED }
                              }
                              named_table { names: "#1" }
                            }
                          }
                          right: {
                            read: {
                              common { direct { } }
                              base_schema {
                                names: "product_id"
                                names: "cost"
                                struct {
                                  types { i32 { nullability: NULLABILITY_REQUIRED } }
                                  types { fp32 { nullability: NULLABILITY_REQUIRED } }
                                  nullability: NULLABILITY_REQUIRED
                                }
                              }
                              named_table { names: "#2" }
                            }
                          }
                          expression: {
                            selection: {
                              direct_reference: {
                                struct_field: {
                                  field: 1
                                }
                              }
                              root_reference: { }
                            }
                          }
                          post_join_filter: {
                            selection: {
                              direct_reference: {
                                struct_field: {
                                  field: 2
                                }
                              }
                              root_reference: { }
                            }
                          }
                        }
                      }
                      right: {
                        read: {
                        common { direct { } }
                        base_schema {
                          names: "company"
                          names: "order_id"
                          struct {
                            types { string { nullability: NULLABILITY_REQUIRED } }
                            types { i32 { nullability: NULLABILITY_REQUIRED } }
                            nullability: NULLABILITY_REQUIRED
                          }
                        }
                        named_table { names: "#3" }
                      }
                    }
                    expression: {
                       selection: {
                         direct_reference: {
                           struct_field: {
                             field: 6
                           }
                         }
                         root_reference: { }
                       }
                     }
                    }
                  }
                }
              })"))),
      },
      {
          "test15-relation-without-type",
          R"(relation notyperelation {
          })",
          // TODO -- Replace this error message with something user-friendly.
          HasErrors({
              "1:0 → extraneous input 'relation' expecting {<EOF>, "
              "'EXTENSION_SPACE', 'NAMED', 'SCHEMA', 'PIPELINES', 'FILTER', "
              "'GROUPING', 'MEASURE', 'SORT', 'COUNT', 'TYPE', 'EMIT', "
              "'OR', 'ALL', COMPARISON, "
              "'SOURCE', 'ROOT', 'NULL', IDENTIFIER}",
              "1:24 → mismatched input '{' expecting 'RELATION'",
              "1:9 → Unrecognized relation type: notyperelation",
              "1:9 → Internal error:  Previously encountered symbol "
              "went missing.",
          }),
      },
      {
          "test16-relation-invalid-type",
          R"(weird relation notyperelation {
          })",
          HasErrors({"1:0 → Unrecognized relation type: weird"}),
      },
      {
          "test17-pipelines-with-relations",
          R"(pipelines {
            read -> project -> root;
          }

          read relation read {
            base_schema schemaone;
            source mynamedtable;
          }

          project relation project {
            expression r_regionkey;
          }

          schema schemaone {
            r_regionkey i32;
            r_name string;
            r_comment string?;
          })",
          AllOf(
              HasSymbolsWithTypes(
                  {"read", "project", "root"}, {SymbolType::kRelation}),
              ParsesOk()),

      },
      {
          "test18-root-and-read",
          R"(pipelines {
            read -> root;
          }

          read relation read {
            base_schema schemaone;
            source mynamedtable;
          }

          root {
            names = [
              apple,
            ]
          })",
          AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
              R"(relations: {
                root { names: "apple" input { read { common { direct { } } } } }
              })")),
      },
      {
          "test19-emit",
          R"(pipelines {
            read -> project -> root;
          }

          read relation read {
            base_schema schemaone;
          }

          project relation project {
            expression r_region_key;

            emit r_region_key;
          }

          schema schemaone {
            r_region_key i32;
          })",
          AllOf(
              HasSymbolsWithTypes(
                  {"read", "project", "root"}, {SymbolType::kRelation}),
              HasErrors({}),
              AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(R"(
                relations {
                  root {
                    input {
                      project {
                        common {
                          emit {
                            output_mapping: 1
                          }
                        }
                        input {
                          read: {
                            common { direct { } }
                            base_schema {
                              names: "r_region_key"
                              struct { types { i32 {
                                nullability: NULLABILITY_REQUIRED } }
                                nullability: NULLABILITY_REQUIRED } } }
                        }
                        expressions {
                          selection {
                            direct_reference {
                              struct_field {
                              }
                            }
                            root_reference: { }
                          }
                        }
                      }
                    }
                  }
                })"))),
      },
  };
  return cases;
}

TEST(TextPlanParser, LoadFromFile) {
  auto stream = loadTextFile("data/provided_sample1.splan");
  ASSERT_TRUE(stream.has_value()) << "Test input file missing.";
  auto result = parseStream(&*stream);
  ASSERT_THAT(result, ParsesOk());
}

TEST_P(TextPlanParserTestFixture, Parse) {
  auto [name, input, matcher] = GetParam();

  auto stream = loadTextString(input);
  auto result = parseStream(&stream);

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
