/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/textplan/ParseResult.h"

namespace io::substrait::textplan {

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ParsesSuccessfully();

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasSymbols(
    const std::vector<std::string>& expected_symbols);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> ReparsesAs(
    const std::string& expected_result);

[[maybe_unused]] ::testing::Matcher<const ParseResult&> HasErrors(
    const std::vector<std::string>& expected_errors);

} // namespace io::substrait::textplan
