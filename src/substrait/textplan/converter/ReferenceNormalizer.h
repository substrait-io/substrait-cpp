/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/proto/plan.pb.h"

namespace io::substrait::textplan {

// ReferenceNormalizer renumbers the extension space uri references
// and function references in a consistent manner.  This makes it easier
// for differencing tools to compare two similar binary plans.  The behavior
// of this tool is undefined on invalid plans.
class ReferenceNormalizer {
 public:
  ReferenceNormalizer() = default;

  static void normalize(::substrait::proto::Plan* plan);

 private:
  static void normalizeSpaces(::substrait::proto::Plan* plan);
  static void normalizeFunctions(::substrait::proto::Plan* plan);
};

} // namespace io::substrait::textplan
