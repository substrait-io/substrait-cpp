/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

namespace substrait::proto {
class Expression_Literal_Decimal;
}

namespace io::substrait::expression {

class DecimalLiteral {
  DecimalLiteral(std::string v, int32_t p, int32_t s)
      : value_(std::move(v)), precision_(p), scale_(s) {}

 public:
  static DecimalLiteral fromProto(
      const ::substrait::proto::Expression_Literal_Decimal& proto);

  static DecimalLiteral
  fromString(const std::string& str, int32_t precision, int32_t scale);

  // Validates that the constructed decimal has an exactly 16 byte value with
  // a stated precision between 1 and 38.
  bool isValid();

  // Converts the value portion of the decimal to a string only.
  std::string toBaseString();

  // Converts the entirety of the decimal (including precision and scale) to
  // a string.
  std::string toString();

  // Emits a proto version of the corresponding decimal.
  ::substrait::proto::Expression_Literal_Decimal toProto();

  [[nodiscard]] int32_t precision() const {
    return precision_;
  }

  [[nodiscard]] int32_t scale() const {
    return scale_;
  }

 private:
  // Little-endian twos-complement integer representation of complete value
  // (ignoring precision).  Always 16 bytes in length.
  std::string value_;
  // The maximum number of digits allowed in the value.
  // the maximum precision is 38.
  int32_t precision_;
  // The number of digits after the decimal point (may be negative).
  int32_t scale_;
};

} // namespace io::substrait::expression
