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
                  })"))),
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
                  {"schema", "cost", "count", "named", "#2", "read", "root"}),
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
                expression product_id;
                post_join count;
              }

              read relation read3 {
                source named3;
                base_schema schema3;
              }

              join relation join2 {
                type JOIN_TYPE_UNSPECIFIED;
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
              HasSymbols({"filter", "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                       filter -> root;
                     }

                     filter relation filter {
                       condition functionref#4(field#2, 0.07_fp64);
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
              HasSymbols({"filter", "root"}),
              WhenSerialized(EqSquashingWhitespace(
                  R"(pipelines {
                       filter -> root;
                     }

                     filter relation filter {
                       condition functionref#4(field#2, 0.07_fp64);
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
          AllOf(
              HasSymbols(
                  {"local",
                   "read",
                   "project",
                   "root",
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
                  {"local", "read", "local2", "read2", "hashjoin", "root"}),
              WhenSerialized(::testing::HasSubstr("pipelines {\n"
                                                  "  read -> hashjoin;\n"
                                                  "  read2 -> hashjoin;\n"
                                                  "  hashjoin -> root;\n"
                                                  "}\n"))),
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
              })));
}

} // namespace
} // namespace io::substrait::textplan
