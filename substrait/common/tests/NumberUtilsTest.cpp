/* SPDX-License-Identifier: Apache-2.0 */

#include <gtest/gtest.h>
#include "substrait/common/NumberUtils.h"

using namespace io::substrait::common;

class NumberUtilsTest : public ::testing::Test {};

TEST_F(NumberUtilsTest, isNonNegativeInteger) {
  ASSERT_TRUE(NumberUtils::isNonNegativeInteger("1"));
  ASSERT_TRUE(NumberUtils::isNonNegativeInteger("0"));
  ASSERT_FALSE(NumberUtils::isNonNegativeInteger("-1"));
  ASSERT_FALSE(NumberUtils::isNonNegativeInteger("L1"));
  ASSERT_FALSE(NumberUtils::isNonNegativeInteger("1L"));
  ASSERT_FALSE(NumberUtils::isNonNegativeInteger(""));
}

TEST_F(NumberUtilsTest, isNegativeInteger) {
  ASSERT_TRUE(NumberUtils::isNegativeInteger("-1"));
  ASSERT_FALSE(NumberUtils::isNegativeInteger("0"));
  ASSERT_FALSE(NumberUtils::isNegativeInteger("-1L"));
  ASSERT_FALSE(NumberUtils::isNegativeInteger("1"));
  ASSERT_FALSE(NumberUtils::isNegativeInteger("1L"));
  ASSERT_FALSE(NumberUtils::isNegativeInteger(""));
}

TEST_F(NumberUtilsTest, isInteger) {
  ASSERT_TRUE(NumberUtils::isInteger("1"));
  ASSERT_TRUE(NumberUtils::isInteger("0"));
  ASSERT_TRUE(NumberUtils::isInteger("-1"));
  ASSERT_FALSE(NumberUtils::isInteger("L1"));
  ASSERT_FALSE(NumberUtils::isInteger("1L"));
  ASSERT_FALSE(NumberUtils::isInteger("-L1"));
  ASSERT_FALSE(NumberUtils::isInteger("-1L"));
  ASSERT_FALSE(NumberUtils::isInteger(""));
}
