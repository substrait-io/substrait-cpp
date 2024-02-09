/* SPDX-License-Identifier: Apache-2.0 */

#include "planloader.h"

#include <limits>
#include <substrait/common/Io.h>

extern "C" {

// Load a Substrait plan (in any format) from disk.
// Stores the Substrait plan in planBuffer in serialized form.
// Returns a SerializedPlan structure containing either the serialized plan or
// an error message.  errorMessage is nullptr upon success.
SerializedPlan* load_substrait_plan(const char* filename) {
  auto newPlan = new SerializedPlan();
  newPlan->buffer = nullptr;
  newPlan->size = 0;
  newPlan->errorMessage = nullptr;

  auto planOrError = io::substrait::loadPlan(filename);
  if (!planOrError.ok()) {
    auto errMsg = planOrError.status().message();
    newPlan->errorMessage = new char[errMsg.length()+1];
    strncpy(newPlan->errorMessage, errMsg.data(), errMsg.length()+1);
    return newPlan;
  }
  ::substrait::proto::Plan plan = *planOrError;
  std::string text = plan.SerializeAsString();
  newPlan->buffer = new char[text.length()+1];
  memcpy(newPlan->buffer, text.data(), text.length()+1);
  newPlan->size = static_cast<int32_t>(
            text.length() &
            std::numeric_limits<int32_t>::max());
  return newPlan;
}

void free_substrait_plan(SerializedPlan* plan) {
  delete[] plan->buffer;
  delete[] plan->errorMessage;
  delete plan;
}

// Write a serialized Substrait plan to disk in the specified format.
// On error returns a non-empty error message.
// On success a nullptr is returned.
const char* save_substrait_plan(
    const char* planData,
    uint32_t planDataLength,
    const char* filename,
    io::substrait::PlanFileFormat format) {
  ::substrait::proto::Plan plan;
  std::string data(planData, planDataLength);
  plan.ParseFromString(data);
  auto result = io::substrait::savePlan(plan, filename, format);
  if (!result.ok()) {
    return result.message().data();
  }
  return nullptr;
}

}  // extern "C"
