/* SPDX-License-Identifier: Apache-2.0 */

#include <gtest/gtest.h>

#include "substrait/common/StringUtils.h"

using io::substrait::common::StringUtils;

class StringUtilsTest : public ::testing::Test {};

TEST_F(StringUtilsTest, ltrim) {
  ASSERT_EQ(StringUtils::ltrim(" 1"), "1");
  ASSERT_EQ(StringUtils::ltrim("  1"), "1");
  ASSERT_EQ(StringUtils::ltrim(" 1 "), "1 ");
  ASSERT_EQ(StringUtils::ltrim(" 1 1 "), "1 1 ");
  ASSERT_EQ(StringUtils::ltrim(" "), "");
}

TEST_F(StringUtilsTest, rtrim) {
  ASSERT_EQ(StringUtils::rtrim("1 "), "1");
  ASSERT_EQ(StringUtils::rtrim("1 "), "1");
  ASSERT_EQ(StringUtils::rtrim("1 1 "), "1 1");
  ASSERT_EQ(StringUtils::rtrim(" "), "");
}

TEST_F(StringUtilsTest, trim) {
  ASSERT_EQ(StringUtils::trim(" 1"), "1");
  ASSERT_EQ(StringUtils::trim("1 "), "1");
  ASSERT_EQ(StringUtils::trim(" 1 "), "1");
  ASSERT_EQ(StringUtils::trim(" 1 1 "), "1 1");
  ASSERT_EQ(StringUtils::trim(" "), "");
}
