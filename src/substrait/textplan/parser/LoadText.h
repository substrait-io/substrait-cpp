/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <absl/status/statusor.h>

namespace substrait::proto {
class Plan;
}

namespace io::substrait::textplan {

// Reads a plan encoded as a text protobuf.
// Returns a list of errors if the text cannot be parsed.
absl::StatusOr<::substrait::proto::Plan> loadFromText(const std::string& text);

} // namespace io::substrait::textplan
