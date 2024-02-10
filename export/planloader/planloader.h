/* SPDX-License-Identifier: Apache-2.0 */

#include <substrait/common/Io.h>

extern "C" {

// Since this is actually C code, stick to C style names for exporting.
// NOLINTBEGIN(readability-identifier-naming)

using SerializedPlan = struct {
  // If set, contains a serialized ::substrait::proto::Plan object.
  unsigned char *buffer;
  // If buffer is set, this is the size of the buffer.
  int32_t size;
  // If null the buffer is valid, otherwise this points to a null terminated
  // error string.
  char *error_message;
};

// Load a Substrait plan (in any format) from disk.
//
// Accepts filename as a null-terminated C string.
// Returns a SerializedPlan structure containing either the serialized plan or
// an error message.  This SerializedPlan should be freed using
// free_substrait_plan.
SerializedPlan* load_substrait_plan(const char* filename);

// Frees a SerializedPlan that was returned from load_substrait_plan.
void free_substrait_plan(SerializedPlan* plan);

// Write a serialized Substrait plan to disk in the specified format.
//
// plan_data is a Substrait Plan serialized into a byte array with length
// plan_data_length.
// Filename is a null-terminated C string.
// On error returns a non-empty error message.
// On success an empty string is returned.
const char* save_substrait_plan(
    const unsigned char* plan_data,
    int32_t plan_data_length,
    const char* filename,
    io::substrait::PlanFileFormat format);

// NOLINTEND(readability-identifier-naming)

}  // extern "C"
