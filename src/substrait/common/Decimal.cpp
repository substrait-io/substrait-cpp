/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/common/Decimal.h"

namespace io::substrait::common {

Decimal Decimal::fromProto(
    const ::substrait::proto::Expression_Literal_Decimal& proto) {
  return {proto.value(), proto.precision(), proto.scale()};
};

std::string Decimal::toString() {
  if (value_.empty()) {
    return "0";
  }

  int32_t numBytes = value_.size() & 0xFF;  // Enough for our 16 bytes.
  bool isNegative = ((value_[numBytes - 1] & 0x80) != 0);

  std::string decimalString;

  unsigned long long value = 0;
  for (int32_t i = numBytes - 1; i >= 0; i--) {
    value = (value << 8) | static_cast<unsigned char>(value_[i]);
    while (value > 10) {
      decimalString += static_cast<char>('0' + (value % 10));
      value /= 10;
    }
  }

  while (value > 0) {
    decimalString += static_cast<char>('0' + (value % 10));
    value /= 10;
  }

  if (decimalString.empty()) {
    return "0";
  }

  decimalString += "E" + std::to_string(scale_);
  decimalString += "@precision=" + std::to_string(precision_);

  if (isNegative) {
    return "-" + decimalString;
  }
  return decimalString;
}

} // namespace io::substrait::common
