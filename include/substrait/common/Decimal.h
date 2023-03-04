/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/proto/algebra.pb.h"

namespace io::substrait::common {

class Decimal {
  Decimal(std::string v, int32_t p, int32_t s)
      : value_(std::move(v)), precision_(p), scale_(s) {}

 public:
  static Decimal fromProto(
      const ::substrait::proto::Expression_Literal_Decimal& proto);

  std::string toString();

 private:
  // Little-endian twos-complement integer representation of complete value
  // (ignoring precision).  Always 16 bytes in length.
  std::string value_;
  // The maximum number of digits allowed in the value.
  // the maximum precision is 38.
  int32_t precision_;
  // The declared scale of decimal literal
  int32_t scale_;
};

} // namespace io::substrait::common
