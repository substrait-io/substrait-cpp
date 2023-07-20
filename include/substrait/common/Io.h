/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string_view>

#include "absl/status/statusor.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait {

enum PlanFileEncoding {
  kBinary = 0,
  kJson = 1,
  kProtoText = 2,
  kText = 3,
};

// Loads a Substrait plan consisting of any encoding type from the given file.
absl::StatusOr<::substrait::proto::Plan> loadPlanWithUnknownEncoding(
    std::string_view input_filename);

// Writes the provided plan to the specified location with the specified
// encoding type.
[[maybe_unused]] absl::Status savePlan(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename,
    PlanFileEncoding encoding);

} // namespace io::substrait
