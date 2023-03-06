/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/ParseResult.h"

namespace io::substrait::textplan {

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesOk();

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasSymbols(
    std::vector<std::string> expected_symbols);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> SerializesTo(
    std::string expected_result);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasErrors(
    std::vector<std::string> expected_errors);

} // namespace io::substrait::textplan
