/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/ParseResult.h"

namespace io::substrait::textplan {

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesOk(); // NOLINT

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasSymbols( // NOLINT
    std::vector<std::string> expectedSymbols);

[[maybe_unused]] ::testing::Matcher<const ParseResult&>
HasSymbolsWithTypes( // NOLINT
    std::vector<std::string> expectedSymbols,
    std::vector<SymbolType> interestingTypes);

[[maybe_unused]] ::testing::Matcher<const ParseResult&>
WhenSerialized( // NOLINT
    ::testing::Matcher<const std::string&> stringMatcher);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasErrors( // NOLINT
    std::vector<std::string> expectedErrors);

// Matches strings ignoring differences in kinds of whitespace (as long as they
// are present) and ignoring trailing whitespace as well.
[[maybe_unused]] ::testing::Matcher<const std::string&>
EqSquashingWhitespace( // NOLINT
    std::string expectedString);

} // namespace io::substrait::textplan
