/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/expression/DecimalLiteral.h"

#include <sstream>

#include "absl/numeric/int128.h"
#include "substrait/proto/algebra.pb.h"

namespace io::substrait::expression {

namespace {

// invertBits flips the sign of a two-complements value.
std::string invertBits(const std::string& value) {
  std::string newValue = value;
  // Flip all of the bits and add one.
  bool carryover = true;
  for (char& b : newValue) {
    uint8_t newB = ~(static_cast<uint8_t>(b));
    if (carryover) {
      newB++;
    }
    carryover = carryover && (newB == 0);
    b = static_cast<char>(newB & 0xff);
  }
  return newValue;
}

} // namespace

DecimalLiteral DecimalLiteral::fromProto(
    const ::substrait::proto::Expression_Literal_Decimal& proto) {
  return {proto.value(), proto.precision(), proto.scale()};
}

bool DecimalLiteral::isValid() {
  return value_.size() == 16 && precision_ >= 1 && precision_ <= 38;
}

std::string DecimalLiteral::toBaseString() {
  std::stringstream decimalString;
  if (value_.empty()) {
    return "0";
  }

  std::string processingValue = value_;

  // Determine the sign of the value.
  size_t numBytes = value_.size();
  bool isNegative = ((value_[numBytes - 1] & 0x80) != 0);
  if (isNegative) {
    decimalString << "-";
    processingValue = invertBits(value_);
  }

  // Collect the bytes into an unsigned integer.
  absl::uint128 value = 0; // Will only hold 16 hex chars.
  for (size_t i = numBytes; i > 0; i--) {
    value = (value << 8) | static_cast<unsigned char>(processingValue[i - 1]);
  }

  // Pull off the digits backwards.
  std::stringstream valueString;
  while (value >= 10) {
    valueString << static_cast<char>('0' + (value % 10));
    value /= 10;
  }
  valueString << static_cast<char>('0' + value);

  // Reverse the digits.
  std::string v = valueString.str();
  for (auto c = v.rbegin(); c != v.rend(); c++) {
    decimalString << *c;
  }

  return decimalString.str();
}

std::string DecimalLiteral::toString() {
  std::stringstream decimalString;

  decimalString << toBaseString();

  if (scale_ > 0) {
    decimalString << "E-" << scale_;
  } else if (scale_ < 0) {
    decimalString << "E+" << -scale_;
  }
  decimalString << "@precision=" << precision_;

  return decimalString.str();
}

} // namespace io::substrait::expression
