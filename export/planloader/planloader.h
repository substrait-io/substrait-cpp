/* SPDX-License-Identifier: Apache-2.0 */

#include <substrait/common/Io.h>

extern "C" {

typedef struct {
  char *buffer;
  uint32_t size;
  char *errorMessage;
} SerializedPlan;

// Load a Substrait plan (in any format) from disk.
// Stores the Substrait plan in planBuffer in serialized form.
// Returns a SerializedPlan structure containing either the serialized plan or
// an error message.
SerializedPlan* load_substrait_plan(const char* filename);

void free_substrait_plan(SerializedPlan* plan);

// Write a serialized Substrait plan to disk in the specified format.
// On error returns a non-empty error message.
// On success an empty string is returned.
const char* save_substrait_plan(
    const char* planData,
    uint32_t planDataLength,
    const char* filename,
    io::substrait::PlanFileFormat format);

}  // extern "C"
