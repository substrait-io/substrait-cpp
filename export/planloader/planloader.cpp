/* SPDX-License-Identifier: Apache-2.0 */

#include "planloader.h"

#include <substrait/common/Io.h>
#include <limits>

extern "C" {

// Load a Substrait plan (in any format) from disk.
// Stores the Substrait plan in planBuffer in serialized form.
// Returns a SerializedPlan structure containing either the serialized plan or
// an error message.  error_message is nullptr upon success.
SerializedPlan* load_substrait_plan(const char* filename) {
  auto newPlan = new SerializedPlan();
  newPlan->buffer = nullptr;
  newPlan->size = 0;
  newPlan->error_message = nullptr;

  auto planOrError = io::substrait::loadPlan(filename);
  if (!planOrError.ok()) {
    auto errMsg = planOrError.status().message();
    newPlan->error_message = new char[errMsg.length() + 1];
    strncpy(newPlan->error_message, errMsg.data(), errMsg.length() + 1);
    return newPlan;
  }
  ::substrait::proto::Plan plan = *planOrError;
  std::string text = plan.SerializeAsString();
  newPlan->buffer = new unsigned char[text.length() + 1];
  memcpy(newPlan->buffer, text.data(), text.length() + 1);
  newPlan->size =
      static_cast<int32_t>(text.length() & std::numeric_limits<int32_t>::max());
  return newPlan;
}

void free_substrait_plan(SerializedPlan* plan) {
  delete[] plan->buffer;
  delete[] plan->error_message;
  delete plan;
}

// Write a serialized Substrait plan to disk in the specified format.
// On error returns a non-empty error message.
// On success a nullptr is returned.
const char* save_substrait_plan(
    const unsigned char* plan_data,
    int32_t plan_data_length,
    const char* filename,
    io::substrait::PlanFileFormat format) {
  ::substrait::proto::Plan plan;
  std::string data((const char*)plan_data, plan_data_length);
  plan.ParseFromString(data);
  auto result = io::substrait::savePlan(plan, filename, format);
  if (!result.ok()) {
    return result.message().data();
  }
  return nullptr;
}

} // extern "C"
