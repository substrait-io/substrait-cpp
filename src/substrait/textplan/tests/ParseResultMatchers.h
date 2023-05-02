/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/ParseResult.h"

// NOLINTBEGIN(readability-identifier-naming)

namespace io::substrait::textplan {

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesOk();

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasSymbols(
    const std::vector<std::string>& expectedSymbols);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasSymbolsWithTypes(
    const std::vector<std::string>& expected_symbols,
    const std::vector<SymbolType>& interesting_types);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> WhenSerialized(
    ::testing::Matcher<const std::string&> stringMatcher);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasErrors(
    const std::vector<std::string>& expectedErrors);

// Matches strings ignoring differences in kinds of whitespace (as long as they
// are present) and ignoring trailing whitespace as well.
[[maybe_unused]] ::testing::Matcher<const std::string&> EqSquashingWhitespace(
    const std::string& expectedString);

// NOLINTEND(readability-identifier-naming)

} // namespace io::substrait::textplan
