/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <absl/status/statusor.h>
#include <string>
#include <string_view>

namespace substrait::proto {
class Plan;
}

namespace io::substrait::textplan {

// Read the contents of a file from disk. 'forceBinary' enables file reading in
// binary mode.
absl::StatusOr<std::string> readFromFile(std::string_view msgPath);

// Reads a plan from a json-encoded text proto.
// Returns a list of errors if the file cannot be parsed.
absl::StatusOr<::substrait::proto::Plan> loadFromJson(const std::string& json);

// Reads a plan encoded as a text protobuf.
// Returns a list of errors if the file cannot be parsed.
absl::StatusOr<::substrait::proto::Plan> loadFromProtoText(
    const std::string& text);

// Reads a plan serialized as a binary protobuf.
// Returns a list of errors if the file cannot be parsed.
absl::StatusOr<::substrait::proto::Plan> loadFromBinary(
    const std::string& bytes);

} // namespace io::substrait::textplan
