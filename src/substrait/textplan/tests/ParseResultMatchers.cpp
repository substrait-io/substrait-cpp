/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/tests/ParseResultMatchers.h"

#include <memory>
#include <string>
#include <utility>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/ParseResult.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/SymbolTablePrinter.h"

namespace io::substrait::textplan {
namespace {

std::vector<std::string> symbolNames(
    const std::vector<std::shared_ptr<SymbolInfo>>& symbols) {
  std::vector<std::string> names;
  for (const auto& symbol : symbols) {
    names.push_back(symbol->name);
  }
  return names;
}

} // namespace

class ParsesSuccessfullyMatcher {
 public:
  using is_gtest_matcher = void;

  explicit ParsesSuccessfullyMatcher() = default;

  static bool MatchAndExplain(
      const ParseResult& result,
      std::ostream* /* listener */) {
    return result.successful();
  }

  static void DescribeTo(std::ostream* os) {
    *os << "parses successfully";
  }

  static void DescribeNegationTo(std::ostream* os) {
    *os << "does not parse successfully";
  }
};

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesSuccessfully() {
  return ParsesSuccessfullyMatcher();
}

class HasSymbolsMatcher {
 public:
  using is_gtest_matcher = void;

  explicit HasSymbolsMatcher(const std::vector<std::string>& expected_symbols)
      : expected_symbols_(expected_symbols) {}

  bool MatchAndExplain(const ParseResult& result, std::ostream* /* listener */)
      const {
    return symbolNames(result.getSymbolTable()->getSymbols()) ==
        expected_symbols_;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has exactly these symbols: "
        << ::testing::PrintToString(expected_symbols_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not have exactly these symbols: "
        << ::testing::PrintToString(expected_symbols_);
  }

 private:
  const std::vector<std::string> expected_symbols_;
};

::testing::Matcher<const ParseResult&> HasSymbols(
    const std::vector<std::string>& expected_symbols) {
  return HasSymbolsMatcher(expected_symbols);
}

class ReparsesAsMatcher {
 public:
  using is_gtest_matcher = void;

  explicit ReparsesAsMatcher(std::string expected_result)
      : expected_result_(std::move(expected_result)) {}

  bool MatchAndExplain(const ParseResult& result, std::ostream* listener)
      const {
    std::string outputText =
        SymbolTablePrinter::outputToText(*result.getSymbolTable());
    if (listener) {
      *listener << "has output text \"" << outputText << "\"";
    }
    return outputText == expected_result_;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "reparses to: " << ::testing::PrintToString(expected_result_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not reparse to: "
        << ::testing::PrintToString(expected_result_);
  }

 private:
  const std::string expected_result_;
};

::testing::Matcher<const ParseResult&> ReparsesAs(
    const std::string& expected_symbols) {
  return ReparsesAsMatcher(expected_symbols);
}

class HasErrorsMatcher {
 public:
  using is_gtest_matcher = void;

  explicit HasErrorsMatcher(const std::vector<std::string>& expected_errors)
      : expected_errors_(expected_errors) {}

  bool MatchAndExplain(const ParseResult& result, std::ostream* /* listener */)
      const {
    return result.getAllErrors() == expected_errors_;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has exactly these symbols: "
        << ::testing::PrintToString(expected_errors_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not have exactly these symbols: "
        << ::testing::PrintToString(expected_errors_);
  }

 private:
  const std::vector<std::string> expected_errors_;
};

::testing::Matcher<const ParseResult&> HasErrors(
    const std::vector<std::string>& expected_errors) {
  return HasErrorsMatcher(expected_errors);
}

} // namespace io::substrait::textplan