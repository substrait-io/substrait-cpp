/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/ParseResult.h"

namespace io::substrait::textplan {

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesOk();

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasSymbols(
    std::vector<std::string> expectedSymbols);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> WhenSerialized(
    ::testing::Matcher<const std::string&> stringMatcher);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasErrors(
    std::vector<std::string> expectedErrors);

// Matches strings ignoring differences in kinds of whitespace (as long as they
// are present) and ignoring trailing whitespace as well.
[[maybe_unused]] ::testing::Matcher<const std::string&> EqSquashingWhitespace(
    std::string expectedString);

} // namespace io::substrait::textplan
