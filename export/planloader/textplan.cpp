/* SPDX-License-Identifier: Apache-2.0 */

#include <substrait/common/Io.h>

extern "C" {

// Load a Substrait plan (in any format) from disk.
// Stores the Substrait plan in planBuffer in serialized form.
// On error returns a non-empty error message.
// On success an empty string is returned.
const char* load_substrait_plan(
    char* planBuffer,
    int32_t planBufferLen,
    const char* filename) {
  auto planOrError = io::substrait::loadPlan(filename);
  if (!planOrError.ok()) {
    return planOrError.status().message().data();
  }
  ::substrait::proto::Plan plan = *planOrError;
  std::string text = plan.SerializeAsString();
  if (text.length() > planBufferLen) {
    return "Actual plan size is longer than the provided buffer.";
  }
  strncpy(planBuffer, text.c_str(), text.length());
  return "";
}

// Write a serialized Substrait plan to disk in the specified format.
// On error returns a non-empty error message.
// On success an empty string is returned.
const char* save_substrait_plan(
    const char* planData,
    const char* filename,
    io::substrait::PlanFileFormat format) {
  ::substrait::proto::Plan plan;
  plan.ParseFromString(planData);
  auto result = io::substrait::savePlan(plan, filename, format);
  if (!result.ok()) {
    return result.message().data();
  }
  return "";
}

}  // extern "C"
