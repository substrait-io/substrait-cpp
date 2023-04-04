/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/expression/DecimalLiteral.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "substrait/proto/algebra.pb.h"

namespace io::substrait::expression {

class TestCase {
 public:
  std::string name;
  DecimalLiteral input;

  bool expectedValidity;
  std::string expectedValue;
};

static DecimalLiteral
createDecimal(const std::string& value, int32_t scale, int32_t precision) {
  ::substrait::proto::Expression_Literal_Decimal proto;
  proto.set_value(value);
  proto.set_precision(precision);
  proto.set_scale(scale);
  return DecimalLiteral::fromProto(proto);
}

class DecimalTest : public ::testing::TestWithParam<TestCase> {};

std::vector<TestCase> getTestCases() {
  static std::vector<TestCase> cases = {
      {"42noscale",
       createDecimal("\x2A", 0, 3), // NOLINT
       false,
       "42@precision=3"},
      {"42positivescale",
       createDecimal("\x2A", +2, 3), // NOLINT
       false,
       "42E-2@precision=3"},
      {"42negativescale",
       createDecimal("\x2A", -2, 3), // NOLINT
       false,
       "42E+2@precision=3"},

      {"8bit, 0", createDecimal({"\x00", 1}, 0, 4), false, "0@precision=4"},
      {"8bit, 1", createDecimal("\x01", 0, 4), false, "1@precision=4"},
      {"8bit, 2", createDecimal("\x02", 0, 4), false, "2@precision=4"},
      {"8bit, 126",
       createDecimal("\x7E", 0, 4), // NOLINT
       false,
       "126@precision=4"},
      {"8bit, 127", createDecimal("\x7F", 0, 4), false, "127@precision=4"},
      {"8bit, 128", createDecimal("\x80", 0, 4), false, "-128@precision=4"},
      {"8bit, 129", createDecimal("\x81", 0, 4), false, "-127@precision=4"},
      {"8bit, 130", createDecimal("\x82", 0, 4), false, "-126@precision=4"},
      {"8bit, 254", createDecimal("\xFE", 0, 4), false, "-2@precision=4"},
      {"8bit, 255", createDecimal("\xFF", 0, 4), false, "-1@precision=4"},

      {"16bit, 2", createDecimal("\x02", 0, 4), false, "2@precision=4"},
      {"16bit, 127",
       createDecimal({"\x7F\x00", 2}, 0, 4),
       false,
       "127@precision=4"},
      {"16bit, 128",
       createDecimal({"\x80\x00", 2}, 0, 4),
       false,
       "128@precision=4"},
      {"16bit, 33333",
       createDecimal("\x39\x30", 0, 4), // NOLINT
       false,
       "12345@precision=4"},
      {"16bit, 53191",
       createDecimal("\xC7\xCF", 0, 4),
       false,
       "-12345@precision=4"},
      {"16bit, 65534",
       createDecimal("\xFE\xFF", 0, 4),
       false,
       "-2@precision=4"},
      {"16bit, 65535",
       createDecimal("\xFF\xFF", 0, 4),
       false,
       "-1@precision=4"},

      {"propersize",
       createDecimal(
           "\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78",
           -2,
           31),
       true,
       "159954953172672629770948536149615195154E+2@precision=31"},
      {"zeroprecision",
       createDecimal(
           "\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78",
           -2,
           0),
       false,
       "159954953172672629770948536149615195154E+2@precision=0"},
      {"negativeprecision",
       createDecimal(
           "\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78",
           -2,
           -2),
       false,
       "159954953172672629770948536149615195154E+2@precision=-2"},
      {"largeprecision",
       createDecimal(
           "\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78\x12\x34\x56\x78",
           -2,
           42),
       false,
       "159954953172672629770948536149615195154E+2@precision=42"},

      {"zero",
       createDecimal(
           {"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
            16},
           2,
           31),
       true,
       "0E-2@precision=31"},
      {"minint",
       createDecimal(
           {"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80",
            16},
           0,
           38),
       true,
       "-170141183460469231731687303715884105728@precision=38"},
      {"maxint",
       createDecimal(
           {"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x7f",
            16},
           0,
           38),
       true,
       "170141183460469231731687303715884105727@precision=38"},
      {"negativeone",
       createDecimal(
           {"\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff",
            16},
           2,
           31),
       true,
       "-1E-2@precision=31"},
  };
  return cases;
}

TEST_P(DecimalTest, isValid) {
  auto [name, decimal, expectedValidity, _] = GetParam();
  ASSERT_THAT(decimal.isValid(), ::testing::Eq(expectedValidity));
}

TEST_P(DecimalTest, toString) {
  auto [name, decimal, _, expectedValue] = GetParam();
  ASSERT_THAT(decimal.toString(), ::testing::Eq(expectedValue));
}

INSTANTIATE_TEST_SUITE_P(
    DecimalTests,
    DecimalTest,
    ::testing::ValuesIn(getTestCases()),
    [](const testing::TestParamInfo<TestCase>& info) {
      std::string identifier = info.param.name;
      // Remove non-alphanumeric characters to make the test framework happy.
      identifier.erase(
          std::remove_if(
              identifier.begin(),
              identifier.end(),
              [](auto const& c) -> bool { return !std::isalnum(c); }),
          identifier.end());
      return identifier;
    });

} // namespace io::substrait::expression
