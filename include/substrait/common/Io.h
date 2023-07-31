/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string_view>

#include "absl/status/statusor.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait {

/*
 * \brief The four different ways plans can be represented on disk.
 */
enum class PlanFileFormat {
  kBinary = 0,
  kJson = 1,
  kProtoText = 2,
  kText = 3,
};

/*
 * \\brief Loads a Substrait plan of any format from the given file.
 *
 * loadPlan determines which file type the specified file is and then calls
 * the appropriate load/parse method to consume it preserving any error
 * messages.
 *
 * This will load the plan into memory and then convert it consuming twice the
 * amount of memory that it consumed on disk.
 *
 * \\param input_filename The filename containing the plan to convert.
 * \\return If loading was successful, returns a plan.  If loading was not
 * successful this is a status containing a list of parse errors in the status's
 * message.
 */
absl::StatusOr<::substrait::proto::Plan> loadPlan(
    std::string_view input_filename);

/*
 * \\brief Writes the provided plan to disk.
 *
 * savePlan writes the provided plan in the specified format to the specified
 * location.
 *
 * This routine will consume more memory during the conversion to the text
 * format as the original plan as well as the annotated parse tree will need to
 * reside in memory during the process.
 *
 * \\param plan
 * \\param output_filename
 * \\param format
 * \\return
 */
absl::Status savePlan(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename,
    PlanFileFormat format);

} // namespace io::substrait
