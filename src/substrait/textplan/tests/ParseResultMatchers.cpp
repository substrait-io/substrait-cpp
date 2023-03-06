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

class ParsesOkMatcher {
 public:
  using is_gtest_matcher = void;

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

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesOk() {
  return ParsesOkMatcher();
}

class HasSymbolsMatcher {
 public:
  using is_gtest_matcher = void;

  explicit HasSymbolsMatcher(std::vector<std::string> expected_symbols)
      : expected_symbols_(std::move(expected_symbols)) {}

  bool MatchAndExplain(const ParseResult& result, std::ostream* listener)
      const {
    auto actual_symbols = symbolNames(result.getSymbolTable().getSymbols());
    if (listener != nullptr) {
      std::vector<std::string> extra_symbols(actual_symbols.size());
      auto end = std::set_difference(
          actual_symbols.begin(),
          actual_symbols.end(),
          expected_symbols_.begin(),
          expected_symbols_.end(),
          extra_symbols.begin());
      extra_symbols.resize(end - extra_symbols.begin());
      if (!extra_symbols.empty()) {
        *listener << std::endl << "          with missing symbols: ";
        for (const auto& symbol : extra_symbols) {
          *listener << " \"" << symbol << "\"";
        }
      }

      std::vector<std::string> missing_symbols(expected_symbols_.size());
      end = std::set_difference(
          expected_symbols_.begin(),
          expected_symbols_.end(),
          actual_symbols.begin(),
          actual_symbols.end(),
          missing_symbols.begin());
      missing_symbols.resize(end - missing_symbols.begin());
      if (!missing_symbols.empty()) {
        if (!extra_symbols.empty()) {
          *listener << ", and extra symbols: ";
        } else {
          *listener << " with extra symbols: ";
        }
        for (const auto& symbol : missing_symbols) {
          *listener << " \"" << symbol << "\"";
        }
      }
    }
    return actual_symbols == expected_symbols_;
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
    std::vector<std::string> expected_symbols) {
  return HasSymbolsMatcher(std::move(expected_symbols));
}

class SerializesToMatcher {
 public:
  using is_gtest_matcher = void;

  explicit SerializesToMatcher(std::string expected_result)
      : expected_result_(std::move(expected_result)) {}

  bool MatchAndExplain(const ParseResult& result, std::ostream* listener)
      const {
    std::string outputText =
        SymbolTablePrinter::outputToText(result.getSymbolTable());
    if (listener) {
      *listener << "has output text \"" << outputText << "\"";
    }
    return outputText == expected_result_;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "serializes to: " << ::testing::PrintToString(expected_result_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not serialize to: "
        << ::testing::PrintToString(expected_result_);
  }

 private:
  const std::string expected_result_;
};

::testing::Matcher<const ParseResult&> SerializesTo(
    std::string expected_symbols) {
  return SerializesToMatcher(std::move(expected_symbols));
}

class HasErrorsMatcher {
 public:
  using is_gtest_matcher = void;

  explicit HasErrorsMatcher(std::vector<std::string> expected_errors)
      : expected_errors_(std::move(expected_errors)) {}

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
    std::vector<std::string> expected_errors) {
  return HasErrorsMatcher(std::move(expected_errors));
}

} // namespace io::substrait::textplan
