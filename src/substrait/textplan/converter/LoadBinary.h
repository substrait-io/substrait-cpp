/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "substrait/proto/plan.pb.h"

namespace substrait::proto {
class Plan;
}

namespace io::substrait::textplan {

// PlanOrErrors behaves similarly to abseil::StatusOr.
class PlanOrErrors {
 public:
  explicit PlanOrErrors(::substrait::proto::Plan plan)
      : plan_(std::move(plan)){};
  explicit PlanOrErrors(std::vector<std::string> errors)
      : errors_(std::move(errors)){};

  bool ok() {
    return errors_.empty();
  }

  const ::substrait::proto::Plan& operator*() {
    return plan_;
  }

  const std::vector<std::string>& errors() {
    return errors_;
  }

 private:
  ::substrait::proto::Plan plan_;
  std::vector<std::string> errors_;
};

// Read the contents of a file from disk.
// Throws an exception if file cannot be read.
std::string readFromFile(std::string_view msgPath);

// Reads a plan from a json-encoded text proto.
// Returns a list of errors if the file cannot be parsed.
PlanOrErrors loadFromJson(std::string_view json);

// Reads a plan encoded as a text protobuf.
// Returns a list of errors if the file cannot be parsed.
PlanOrErrors loadFromText(const std::string& text);

} // namespace io::substrait::textplan
