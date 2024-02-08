/* SPDX-License-Identifier: Apache-2.0 */

#include <substrait/common/Io.h>

extern "C" {

using SerializedPlan = struct {
  // If set, contains a serialized ::substrait::proto::Plan object.
  char *buffer;
  // If buffer is set, this is the size of the buffer.
  int32_t size;
  // If null the buffer is valid, otherwise this points to an error message.
  char *errorMessage;
};

// Since this is actually C code, stick to C style names for exporting.
// NOLINTBEGIN(readability-identifier-naming)

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

// NOLINTEND(readability-identifier-naming)

}  // extern "C"
