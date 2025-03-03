/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/common/Io.h"

#include <regex>
#include <string_view>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/SaveBinary.h"
#include "substrait/textplan/parser/LoadText.h"

namespace io::substrait {

namespace {

const std::regex kIsJson(
    R"(("extensionUris"|"extension_uris"|"extensions"|"relations"))");
const std::regex kIsProtoText(
    R"((^|\n)((relations|extensions|extension_uris|expected_type_urls) \{))");
const std::regex kIsText(
    R"((^|\n) *(pipelines|[a-z]+ *relation|schema|source|extension_space) *)");

PlanFileFormat detectFormat(std::string_view content) {
  if (std::regex_search(content.begin(), content.end(), kIsJson)) {
    return PlanFileFormat::kJson;
  }
  if (std::regex_search(content.begin(), content.end(), kIsProtoText)) {
    return PlanFileFormat::kProtoText;
  }
  if (std::regex_search(content.begin(), content.end(), kIsText)) {
    return PlanFileFormat::kText;
  }
  return PlanFileFormat::kBinary;
}

} // namespace

absl::StatusOr<::substrait::proto::Plan> loadPlan(
    std::string_view input_filename) {
  auto contentOrError = textplan::readFromFile(input_filename.data());
  if (!contentOrError.ok()) {
    return contentOrError.status();
  }

  auto encoding = detectFormat(*contentOrError);
  absl::StatusOr<::substrait::proto::Plan> planOrError;
  switch (encoding) {
    case PlanFileFormat::kBinary:
      return textplan::loadFromBinary(*contentOrError);
    case PlanFileFormat::kJson:
      return textplan::loadFromJson(*contentOrError);
    case PlanFileFormat::kProtoText:
      return textplan::loadFromProtoText(*contentOrError);
    case PlanFileFormat::kText:
      return textplan::loadFromText(*contentOrError);
  }
  // There are no other possibilities so this can't happen.
  return absl::UnimplementedError("Unexpected format encountered.");
}

absl::Status savePlan(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename,
    PlanFileFormat format) {
  switch (format) {
    case PlanFileFormat::kBinary:
      return textplan::savePlanToBinary(plan, output_filename);
    case PlanFileFormat::kJson:
      return textplan::savePlanToJson(plan, output_filename);
    case PlanFileFormat::kProtoText:
      return textplan::savePlanToProtoText(plan, output_filename);
    case PlanFileFormat::kText:
      return textplan::savePlanToText(plan, output_filename);
  }
  return absl::UnimplementedError("Unexpected format requested.");
}

} // namespace io::substrait
