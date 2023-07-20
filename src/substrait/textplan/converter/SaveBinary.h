/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "absl/status/status.h"

namespace substrait::proto {
class Plan;
}

namespace io::substrait::textplan {

// Serializes a plan to disk as a binary protobuf.
absl::Status savePlanToBinary(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename);

// Serializes a plan to disk as a JSON-encoded protobuf.
absl::Status savePlanToJson(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename);

// Calls the converter to store a plan on disk as a text-based substrait plan.
absl::Status savePlanToText(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename);

// Serializes a plan to disk as a text-encoded protobuf.
absl::Status savePlanToProtoText(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename);

} // namespace io::substrait::textplan
