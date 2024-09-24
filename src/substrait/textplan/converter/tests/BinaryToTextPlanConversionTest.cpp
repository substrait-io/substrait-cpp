/* SPDX-License-Identifier: Apache-2.0 */

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <protobuf-matchers/protocol-buffer-matchers.h>

#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/ParseBinary.h"
#include "substrait/textplan/tests/ParseResultMatchers.h"

namespace io::substrait::textplan {

using ::protobuf_matchers::EqualsProto;
using ::protobuf_matchers::Partially;
using ::testing::AllOf;
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
                   function and:bool_bool as and;
                   function gte:fp64_fp64 as gte;
                   function is_not_null:fp64 as is_not_null;
                   function lt:fp64_fp64 as lt;
                   function lte:fp64_fp64 as lte;
                   function multiply:opt_fp64_fp64 as multiply;
                   function sum:fp64_fp64 as sum;
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
              HasSymbols({"root.names", "local", "read", "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                    read -> root;
                  }

                  read relation read {
                    source local;
                  }

                  source local_files local {
                    items = [
                      {uri_file: "/mock_lineitem.orc" start: 0 length: 3719 orc: {}}
                    ]
                  })")),
              AsBinaryPlan(EqualsProto<::substrait::proto::Plan>(
                  R"(relations { root { input { read {
                    local_files
                      { items { uri_file: "/mock_lineitem.orc" length: 3719 orc { } } }
                    }
                  } } })"))),
      },
      {
          "read named table",
          R"(relations: {
            root: {
              input: {
                read: {
                  base_schema {
                    names: "cost"
                    names: "count"
                    struct {
                      types { fp32 { } }
                      types { i64 { } } }
                  }
                  named_table { names: "#2" }
                }
              }
            }
          })",
          AllOf(
              HasSymbols(
                  {"root.names",
                   "schema",
                   "cost",
                   "count",
                   "named",
                   "#2",
                   "cost",
                   "count",
                   "read",
                   "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                        read -> root;
                     }

                     read relation read {
                        source named;
                        base_schema schema;
                     }

                     schema schema {
                       cost fp32;
                       count i64;
                     }

                     source named_table named {
                       names = [
                         "#2",
                       ]
                     }
                   })"))),
      },
      {
          "read three named tables with two joins",
          R"(relations: {
            root: {
              input: {
                join: {
                  left: {
                    join: {
                      left: {
                        read: {
                          base_schema {
                            names: "order_id"
                            names: "product_id"
                            names: "count"
                            struct {
                              types { i32 { } }
                              types { i32 { } }
                              types { i64 { } } }
                          }
                          named_table { names: "#1" }
                        }
                      }
                      right: {
                        read: {
                          base_schema {
                            names: "product_id"
                            names: "cost"
                            struct {
                              types { i32 { } }
                              types { fp32 { } } }
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
                        }
                      }
                      post_join_filter: {
                        selection: {
                          direct_reference: {
                            struct_field: {
                              field: 2
                            }
                          }
                        }
                      }
                    }
                  }
                  right: {
                    read: {
                    base_schema {
                      names: "company"
                      names: "order_id"
                      struct {
                        types { string { } }
                        types { i32 { } }
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
                   }
                 }
                }
              }
            }
          })",
          WhenSerialized(EqSquashingWhitespace(
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
                type JOIN_TYPE_UNSPECIFIED;
                expression schema.product_id;
                post_join schema.count;
              }

              read relation read3 {
                source named3;
                base_schema schema3;
              }

              join relation join2 {
                type JOIN_TYPE_UNSPECIFIED;
                expression schema3.order_id;
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
              })")),
      },
      {
          "simple expression with deprecated args",
          R"(relations: {
            root: {
              input: {
                filter: {
                  condition: {
                    scalar_function: {
                      function_reference: 4,
                      args: [
                        {
                          selection: {
                            direct_reference: {
                              struct_field: {
                                field: 2
                              }
                            }
                          }
                        },
                        {
                          literal: {
                            nullable: false,
                            fp64: 0.07
                          }
                        }
                      ],
                      output_type: {
                        bool: {
                          type_variation_reference: 0,
                          nullability: NULLABILITY_NULLABLE
                        }
                      }
                    }
                  }
                }
              }
            }
          })",
          AllOf(
              HasSymbols({"root.names", "filter", "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                       filter -> root;
                     }

                     filter relation filter {
                       filter functionref#4(field#2, 0.07_fp64)->bool?;
                     })"))),
      },
      {
          "simple expression with function arguments",
          R"(relations: {
            root: {
              input: {
                filter: {
                  condition: {
                    scalar_function: {
                      function_reference: 4,
                      arguments: [
                        {
                          value: {
                            selection: {
                              direct_reference: {
                                struct_field: {
                                  field: 2
                                }
                              }
                            }
                          }
                        },
                        {
                          value: {
                            literal: {
                              nullable: false,
                              fp64: 0.07
                            }
                          }
                        }
                      ],
                      output_type: {
                        bool: {
                          type_variation_reference: 0,
                          nullability: NULLABILITY_NULLABLE
                        }
                      }
                    }
                  }
                }
              }
            }
          })",
          AllOf(
              HasSymbols({"root.names", "filter", "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                       filter -> root;
                     }

                     filter relation filter {
                       filter functionref#4(field#2, 0.07_fp64)->bool?;
                     })"))),
      },
      {
          "ifthen expression missing then",
          R"(relations: {
            root: {
              input: {
                filter: {
                  condition: {
                    if_then: {
                      ifs: {
                        if: {
                          literal: {
                            nullable: false,
                            fp64: 0.07
                          }
                        }
                      }
                      else: {
                        literal: {
                          nullable: false,
                          fp64: 0.07
                        }
                      }
                    }
                  }
                }
              }
            }
          })",
          HasErrors(
              {"If then clauses require both an if and a then expression: "
               "if { literal { fp64: 0.07 } }"}),
      },
      {
          "cast expression",
          R"(relations: {
            root: {
              input: {
                filter: {
                  condition: {
                    cast: {
                      type: {
                        fixed_char: {
                          length: 10,
                          type_variation_reference: 0,
                          nullability: NULLABILITY_REQUIRED
                        }
                      },
                      input: {
                        literal: {
                        fixed_char: "HOUSEHOLD",
                          nullable: false,
                          type_variation_reference: 0
                        }
                      },
                      failure_behavior: FAILURE_BEHAVIOR_UNSPECIFIED
                    }
                  }
                }
              }
            }
          })",
          AllOf(
              HasSymbols({"root.names", "filter", "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                       filter -> root;
                     }

                     filter relation filter {
                       filter "HOUSEHOLD"_fixedchar<9> AS fixedchar<10>;
                     })"))),
      },
      {
          "single three node pipeline",
          "relations: { root: { input: { project: { input { read: { local_files {} } } } } } }",
          HasSymbols({"root.names", "local", "read", "project", "root"}),
      },
      {
          "two identical three node pipelines",
          "relations: { root: { input: { project: { input { read: { local_files {} } } } } } }"
          "relations: { root: { input: { project: { input { read: { local_files {} } } } } } }",
          AllOf(
              HasSymbols(
                  {"root.names",
                   "local",
                   "read",
                   "project",
                   "root",
                   "root.names2",
                   "local2",
                   "read2",
                   "project2",
                   "root2"}),
              WhenSerialized(
                  ::testing::HasSubstr("pipelines {\n"
                                       "  read -> project -> root;\n"
                                       "  read2 -> project2 -> root2;\n"
                                       "}\n"))),
      },
      {
          "pipeline with a hash join",
          "relations: { root: { input: { hash_join: { left { read: { local_files {} } } right { read: { local_files {} } } } } } }",
          AllOf(
              HasSymbols(
                  {"root.names",
                   "local",
                   "read",
                   "local2",
                   "read2",
                   "hashjoin",
                   "root"}),
              WhenSerialized(::testing::HasSubstr("pipelines {\n"
                                                  "  read -> hashjoin;\n"
                                                  "  read2 -> hashjoin;\n"
                                                  "  hashjoin -> root;\n"
                                                  "}\n"))),
      },
      {
          "aggregate with emits",
          R"(extensions {
             extension_function {
               function_anchor: 0
               name: "sum:i32"
             }
          }
          relations: { root: { input: {
              aggregate: { common { emit { output_mapping: 1 } } input {
                  read: { base_schema { names: 'a' names: 'b'
                  struct { types { string {} } types { i32 {} } } }
                  local_files { items { uri_file: 'x.parquet' parquet { } } }
                  } }
                  measures { measure { output_type { i32 {} } arguments { value { selection { direct_reference { struct_field { field: 1 } } } } } } }
                  measures { measure { output_type { i32 {} } arguments { value { selection { direct_reference { struct_field { field: 0 } } } } } } }
          } } } })",
          AllOf(WhenSerialized(EqSquashingWhitespace(
              R"(pipelines {
                read -> aggregate -> root;
              }

              read relation read {
                source local;
                base_schema schema;
              }

              aggregate relation aggregate {
                measure {
                  measure sum(schema.b)->i32@AGGREGATION_PHASE_UNSPECIFIED NAMED measurename;
                }
                measure {
                  measure sum(schema.a)->i32@AGGREGATION_PHASE_UNSPECIFIED NAMED measurename2;
                }

                emit measurename2;
              }

              schema schema {
                a string;
                b i32;
              }

              source local_files local {
                items = [
                  {uri_file: "x.parquet" parquet: {}}
                ]
              }

              extension_space {
                function sum:i32 as sum;
              )"))),
      },
  };
  return cases;
}

TEST_P(BinaryToTextPlanConverterTestFixture, Parse) {
  auto [name, input, matcher] = GetParam();

  auto planOrError = loadFromProtoText(input);
  if (!planOrError.ok()) {
    ParseResult result(
        SymbolTable(), {std::string(planOrError.status().message())}, {});
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

TEST_F(BinaryToTextPlanConversionTest, FullSample) {
  auto jsonOrError = readFromFile("data/q6_first_stage.json");
  ASSERT_TRUE(jsonOrError.ok());
  auto planOrError = loadFromJson(*jsonOrError);
  ASSERT_TRUE(planOrError.ok());
  auto plan = *planOrError;
  EXPECT_THAT(plan.extensions_size(), ::testing::Eq(7));

  auto expectedOutputOrError = readFromFile("data/q6_first_stage.golden.splan");
  ASSERT_TRUE(expectedOutputOrError.ok());

  auto result = parseBinaryPlan(plan);
  auto symbols = result.getSymbolTable().getSymbols();
  ASSERT_THAT(
      result,
      AllOf(
          HasSymbolsWithTypes(
              {
                  "lte",
                  "sum",
                  "lt",
                  "is_not_null",
                  "and",
                  "gte",
                  "multiply",
              },
              {SymbolType::kFunction}),
          HasSymbolsWithTypes(
              {
                  "schema",
                  "local",

                  "read",
                  "filter",
                  "project",
                  "aggregate",
                  "root",
              },
              {
                  SymbolType::kPlanRelation,
                  SymbolType::kRelation,
                  SymbolType::kSource,
                  SymbolType::kSchema,
              }),
          WhenSerialized(EqSquashingWhitespace(*expectedOutputOrError))))
      << result.getSymbolTable().toDebugString();
}

} // namespace
} // namespace io::substrait::textplan
