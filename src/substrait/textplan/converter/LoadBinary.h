/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

namespace substrait::proto {
class Plan;
}

namespace io::substrait::textplan {

// Read the contents of a file from disk.
std::string ReadFromFile(const std::string& msgPath);

// Reads a plan from a json-encoded text proto.
// Throws an exception if the JSON can not be loaded.
::substrait::proto::Plan LoadFromJSON(const std::string& json);

// Reads a plan encoded as a text protobuf.
// Returns a list of errors if the file cannot be parsed.
std::variant<::substrait::proto::Plan, std::vector<std::string>> LoadFromText(
    const std::string& text);

} // namespace io::substrait::textplan
