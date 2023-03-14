/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/SymbolTable.h"
#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Location.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

namespace io::substrait::textplan {
namespace {

class SymbolTableTest : public ::testing::Test {
 public:
  SymbolTableTest() : UnspecifiedLocation(Location::kUnknownLocation){};

 protected:
  static std::vector<std::string> symbolNames(
      const std::vector<std::shared_ptr<SymbolInfo>>& symbols) {
    std::vector<std::string> names;
    for (const auto& symbol : symbols) {
      names.push_back(symbol->name);
    }
    return names;
  }

  static SymbolTable createSimpleTable(::substrait::proto::Plan* plan) {
    auto* ptr1 = plan->add_relations();
    auto* ptr2 = plan->add_extension_uris();
    auto* ptr3 = plan->add_extensions();

    SymbolTable table;
    table.defineSymbol(
        "symbol1",
        Location(ptr1),
        SymbolType::kUnknown,
        RelationType::kUnknown,
        ptr1);
    table.defineSymbol(
        "symbol2",
        Location(ptr2),
        SymbolType::kUnknown,
        RelationType::kUnknown,
        ptr2);
    table.defineSymbol(
        "symbol3",
        Location(ptr3),
        SymbolType::kUnknown,
        RelationType::kUnknown,
        ptr3);
    return table;
  }

  const Location UnspecifiedLocation;
};

TEST_F(SymbolTableTest, DuplicateSymbolsNotDetected) {
  SymbolTable table;
  table.defineSymbol(
      "a",
      UnspecifiedLocation,
      SymbolType::kUnknown,
      RelationType::kUnknown,
      nullptr);
  table.defineSymbol(
      "a",
      UnspecifiedLocation,
      SymbolType::kUnknown,
      RelationType::kUnknown,
      nullptr);

  ASSERT_THAT(
      symbolNames(table.getSymbols()), ::testing::ElementsAre("a", "a"));
}

TEST_F(SymbolTableTest, DuplicateSymbolsHandledByUnique) {
  SymbolTable table;
  table.defineUniqueSymbol(
      "a",
      UnspecifiedLocation,
      SymbolType::kUnknown,
      RelationType::kUnknown,
      nullptr);
  table.defineUniqueSymbol(
      "a",
      UnspecifiedLocation,
      SymbolType::kUnknown,
      RelationType::kUnknown,
      nullptr);

  ASSERT_THAT(
      symbolNames(table.getSymbols()), ::testing::ElementsAre("a", "a2"));
}

TEST_F(SymbolTableTest, LocationsUnchangedAfterCopy) {
  ::substrait::proto::Plan plan;
  SymbolTable table = createSimpleTable(&plan);
  auto* ptr1 = &plan.relations(0);
  auto* ptr2 = plan.mutable_extension_uris(0);
  auto* ptr3 = &plan.extensions(0);

  SymbolTable table2 = table;
  auto symbols = table2.getSymbols();
  ASSERT_THAT(
      symbolNames(symbols),
      ::testing::ElementsAre("symbol1", "symbol2", "symbol3"));

  ASSERT_THAT(
      ANY_CAST(::substrait::proto::PlanRel*, symbols[0]->blob),
      ::testing::Eq(ptr1));
  ASSERT_THAT(
      ANY_CAST(
          ::substrait::proto::extensions::SimpleExtensionURI*,
          symbols[1]->blob),
      ::testing::Eq(ptr2));
  ASSERT_THAT(
      ANY_CAST(
          ::substrait::proto::extensions::SimpleExtensionDeclaration*,
          symbols[2]->blob),
      ::testing::Eq(ptr3));

  ASSERT_THAT(symbols[0]->location, ::testing::Eq(symbols[0]->location));
  ASSERT_THAT(
      symbols[0]->location,
      ::testing::Not(::testing::Eq(symbols[1]->location)));
  ASSERT_THAT(
      symbols[0]->location,
      ::testing::Not(::testing::Eq(symbols[2]->location)));
  ASSERT_THAT(
      symbols[1]->location,
      ::testing::Not(::testing::Eq(symbols[2]->location)));

  ASSERT_THAT(
      table.getSymbols()[0]->location, ::testing::Eq(symbols[0]->location));
  ASSERT_THAT(
      table.getSymbols()[1]->location, ::testing::Eq(symbols[1]->location));
  ASSERT_THAT(
      table.getSymbols()[2]->location, ::testing::Eq(symbols[2]->location));
}

} // namespace
} // namespace io::substrait::textplan
