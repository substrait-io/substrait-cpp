/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/tests/ParseResultMatchers.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <memory>
#include <set>
#include <string>
#include <utility>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/ParseResult.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/SymbolTablePrinter.h"

// NOLINTBEGIN(readability-identifier-naming)

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

bool stringEqSquashingWhitespace(
    const std::string& have,
    const std::string& expected,
    std::ostream* listener) {
  auto atHave = have.begin();
  auto atExpected = expected.begin();
  while (atHave != have.end() && atExpected != expected.end()) {
    if (isspace(*atExpected)) {
      if (!isspace(*atHave)) {
        return false;
      }
      // Have a match, consume all remaining space.
      do {
        atExpected++;
      } while (atExpected != expected.end() && isspace(*atExpected));
      do {
        atHave++;
      } while (atHave != have.end() && isspace(*atHave));
      continue;
    }
    if (*atHave != *atExpected) {
      if (listener) {
        *listener << "first mismatch at character #"
                  << std::to_string(atHave - have.begin()) << " |"
                  << have.substr(atHave - have.begin(), 20) << "|"
                  << " should be |"
                  << expected.substr(atExpected - expected.begin(), 20) << "|"
                  << std::endl;
      }
      return false;
    }
    atHave++;
    atExpected++;
  }
  // For convenience consume any trailing whitespace on both sides.
  if (atExpected != expected.end()) {
    do {
      atExpected++;
    } while (atExpected != expected.end() && isspace(*atExpected));
  }
  if (atHave != have.end()) {
    do {
      atHave++;
    } while (atHave != have.end() && isspace(*atHave));
  }
  return atHave == have.end() && atExpected == expected.end();
}

std::vector<std::string> symbolNamesWithTypes(
    const std::vector<std::shared_ptr<SymbolInfo>>& symbols,
    const std::set<SymbolType>& types) {
  std::vector<std::string> names;
  for (const auto& symbol : symbols) {
    if (types.find(symbol->type) != types.end()) {
      names.push_back(symbol->name);
    }
  }
  return names;
}

std::vector<std::string> orderedSetDifference(
    const std::vector<std::string>& haystack,
    const std::vector<std::string>& source) {
  std::vector<std::string> found(haystack.size());
  std::vector<std::string> sortedHaystack = haystack;
  std::vector<std::string> sortedSource = source;
  std::sort(sortedHaystack.begin(), sortedHaystack.end());
  std::sort(sortedSource.begin(), sortedSource.end());
  auto end = std::set_difference(
      sortedHaystack.begin(),
      sortedHaystack.end(),
      sortedSource.begin(),
      sortedSource.end(),
      found.begin());
  found.resize(end - found.begin());
  std::set<std::string> items;
  items.insert(found.begin(), found.end());
  std::vector<std::string> result;
  for (const auto& item : haystack) {
    if (items.find(item) != items.end()) {
      result.push_back(item);
    }
  }
  return result;
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

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesOk() { // NOLINT
  return ParsesOkMatcher();
}

class HasSymbolsMatcher {
 public:
  using is_gtest_matcher = void;

  explicit HasSymbolsMatcher(const std::vector<std::string>& expectedSymbols)
      : expectedSymbols_(std::move(expectedSymbols)) {}

  bool MatchAndExplain(const ParseResult& result, std::ostream* listener)
      const {
    auto actualSymbols = symbolNames(result.getSymbolTable().getSymbols());
    if (listener != nullptr) {
      std::vector<std::string> extraSymbols(actualSymbols.size());
      auto end = std::set_difference(
          actualSymbols.begin(),
          actualSymbols.end(),
          expectedSymbols_.begin(),
          expectedSymbols_.end(),
          extraSymbols.begin());
      extraSymbols.resize(end - extraSymbols.begin());
      if (!extraSymbols.empty()) {
        *listener << std::endl << "          with extra symbols: ";
        for (const auto& symbol : extraSymbols) {
          *listener << " \"" << symbol << "\"";
        }
      }

      std::vector<std::string> missingSymbols(expectedSymbols_.size());
      end = std::set_difference(
          expectedSymbols_.begin(),
          expectedSymbols_.end(),
          actualSymbols.begin(),
          actualSymbols.end(),
          missingSymbols.begin());
      missingSymbols.resize(end - missingSymbols.begin());
      if (!missingSymbols.empty()) {
        if (!extraSymbols.empty()) {
          *listener << ", and missing symbols: ";
        } else {
          *listener << " with missing symbols: ";
        }
        for (const auto& symbol : missingSymbols) {
          *listener << " \"" << symbol << "\"";
        }
      }
    }
    return actualSymbols == expectedSymbols_;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has exactly these symbols: "
        << ::testing::PrintToString(expectedSymbols_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not have exactly these symbols: "
        << ::testing::PrintToString(expectedSymbols_);
  }

 private:
  const std::vector<std::string> expectedSymbols_;
};

::testing::Matcher<const ParseResult&> HasSymbols(
    const std::vector<std::string>& expectedSymbols) {
  return HasSymbolsMatcher(std::move(expectedSymbols));
}

class WhenSerializedMatcher {
 public:
  using is_gtest_matcher = void;

  explicit WhenSerializedMatcher(
      ::testing::Matcher<const std::string&> stringMatcher)
      : stringMatcher_(std::move(stringMatcher)) {}

  bool MatchAndExplain(
      const ParseResult& result,
      ::testing::MatchResultListener* listener) const {
    std::string outputText =
        SymbolTablePrinter::outputToText(result.getSymbolTable());
    return MatchPrintAndExplain(outputText, stringMatcher_, listener);
  }

  void DescribeTo(::std::ostream* os) const {
    *os << "matches after serializing ";
    stringMatcher_.DescribeTo(os);
  }

  void DescribeNegationTo(::std::ostream* os) const {
    *os << "does not match after serializing ";
    stringMatcher_.DescribeTo(os);
  }

 private:
  ::testing::Matcher<const std::string&> stringMatcher_;
};

::testing::Matcher<const ParseResult&> WhenSerialized(
    ::testing::Matcher<const std::string&> stringMatcher) {
  return WhenSerializedMatcher(std::move(stringMatcher));
}

class AsBinaryPlanMatcher {
 public:
  using is_gtest_matcher = void;

  explicit AsBinaryPlanMatcher(
      ::testing::Matcher<const ::substrait::proto::Plan&> proto_matcher)
      : proto_matcher_(std::move(proto_matcher)) {}

  bool MatchAndExplain(
      const ParseResult& result,
      ::testing::MatchResultListener* listener) const {
    auto outputPlan =
        SymbolTablePrinter::outputToBinaryPlan(result.getSymbolTable());
    return MatchPrintAndExplain(outputPlan, proto_matcher_, listener);
  }

  void DescribeTo(::std::ostream* os) const {
    *os << "matches after converting to a binary plan ";
    proto_matcher_.DescribeTo(os);
  }

  void DescribeNegationTo(::std::ostream* os) const {
    *os << "does not match after converting to a binary plan ";
    proto_matcher_.DescribeTo(os);
  }

 private:
  ::testing::Matcher<const ::substrait::proto::Plan&> proto_matcher_;
};

::testing::Matcher<const ParseResult&> AsBinaryPlan(
    ::testing::Matcher<const ::substrait::proto::Plan&> proto_matcher) {
  return AsBinaryPlanMatcher(std::move(proto_matcher));
}

class HasSymbolsWithTypesMatcher {
 public:
  using is_gtest_matcher = void;

  HasSymbolsWithTypesMatcher(
      const std::vector<std::string>& expected_symbols,
      const std::vector<SymbolType>& interesting_types)
      : expectedSymbols_(std::move(expected_symbols)) {
    interestingTypes_.insert(
        interesting_types.begin(), interesting_types.end());
  }

  void DescribeTypes(std::ostream* os) const {
    *os << " of types ";
    bool hasPreviousOutput = false;
    for (const auto& type : interestingTypes_) {
      if (hasPreviousOutput) {
        *os << ", ";
      }
      *os << symbolTypeName(type);
      hasPreviousOutput = true;
    }
  }

  bool MatchAndExplain(const ParseResult& result, std::ostream* listener)
      const {
    auto actualSymbols = symbolNamesWithTypes(
        result.getSymbolTable().getSymbols(), interestingTypes_);
    if (listener != nullptr) {
      std::vector<std::string> extraSymbols =
          orderedSetDifference(actualSymbols, expectedSymbols_);
      if (!extraSymbols.empty()) {
        *listener << std::endl << "          with extra symbols";
        DescribeTypes(listener);
        *listener << ": ";
        for (const auto& symbol : extraSymbols) {
          *listener << " \"" << symbol << "\"";
        }
      }

      std::vector<std::string> missingSymbols =
          orderedSetDifference(expectedSymbols_, actualSymbols);
      if (!missingSymbols.empty()) {
        if (!extraSymbols.empty()) {
          *listener << ", and missing symbols";
          DescribeTypes(listener);
          *listener << ": ";
        } else {
          *listener << " with missing symbols";
          DescribeTypes(listener);
          *listener << ": ";
        }
        for (const auto& symbol : missingSymbols) {
          *listener << " \"" << symbol << "\"";
        }
      }
    }
    return actualSymbols == expectedSymbols_;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has exactly these symbols";
    DescribeTypes(os);
    *os << ": " << ::testing::PrintToString(expectedSymbols_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not have exactly these symbols";
    DescribeTypes(os);
    *os << ": " << ::testing::PrintToString(expectedSymbols_);
  }

 private:
  const std::vector<std::string> expectedSymbols_;
  std::set<SymbolType> interestingTypes_;
};

::testing::Matcher<const ParseResult&> HasSymbolsWithTypes(
    const std::vector<std::string>& expected_symbols,
    const std::vector<SymbolType>& interesting_types) {
  return HasSymbolsWithTypesMatcher(
      std::move(expected_symbols), std::move(interesting_types));
}

class HasErrorsMatcher {
 public:
  using is_gtest_matcher = void;

  explicit HasErrorsMatcher(const std::vector<std::string>& expectedErrors)
      : expectedErrors_(std::move(expectedErrors)) {}

  bool MatchAndExplain(const ParseResult& result, std::ostream* listener)
      const {
    ::testing::Matcher<std::vector<std::string>> matcher =
        ::testing::ContainerEq(expectedErrors_);
    ::testing::StringMatchResultListener strListener;
    std::vector<std::string> errors = result.getAllErrors();
    bool match = MatchPrintAndExplain(errors, matcher, &strListener);
    if (listener) {
      *listener << strListener.str();
    }
    return match;
  }

  void DescribeTo(std::ostream* os) const {
    *os << "has exactly these errors: "
        << ::testing::PrintToString(expectedErrors_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not have exactly these errors: "
        << ::testing::PrintToString(expectedErrors_);
  }

 private:
  const std::vector<std::string> expectedErrors_;
};

::testing::Matcher<const ParseResult&> HasErrors(
    const std::vector<std::string>& expectedErrors) {
  return HasErrorsMatcher(std::move(expectedErrors));
}

class EqSquashingWhitespaceMatcher {
 public:
  using is_gtest_matcher = void;

  explicit EqSquashingWhitespaceMatcher(const std::string& expectedString)
      : expectedString_(std::move(expectedString)) {}

  bool MatchAndExplain(const std::string& str, std::ostream* listener) const {
    return stringEqSquashingWhitespace(str, expectedString_, listener);
  }

  void DescribeTo(std::ostream* os) const {
    *os << "equals squashing whitespace "
        << ::testing::PrintToString(expectedString_);
  }

  void DescribeNegationTo(std::ostream* os) const {
    *os << "does not equal squashing whitespace "
        << ::testing::PrintToString(expectedString_);
  }

 private:
  std::string expectedString_;
};

::testing::Matcher<const std::string&> EqSquashingWhitespace(
    const std::string& expectedString) {
  return EqSquashingWhitespaceMatcher(std::move(expectedString));
}

// NOLINTEND(readability-identifier-naming)

} // namespace io::substrait::textplan
