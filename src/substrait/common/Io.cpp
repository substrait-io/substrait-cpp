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

PlanFileEncoding detectEncoding(std::string_view content) {
  if (std::regex_search(content.begin(), content.end(), kIsJson)) {
    return PlanFileEncoding::kJson;
  }
  if (std::regex_search(content.begin(), content.end(), kIsProtoText)) {
    return PlanFileEncoding::kProtoText;
  }
  if (std::regex_search(content.begin(), content.end(), kIsText)) {
    return PlanFileEncoding::kText;
  }
  return PlanFileEncoding::kBinary;
}

} // namespace

absl::StatusOr<::substrait::proto::Plan> loadPlan(
    std::string_view input_filename) {
  auto contentOrError = textplan::readFromFile(input_filename.data());
  if (!contentOrError.ok()) {
    return contentOrError.status();
  }

  auto encoding = detectEncoding(*contentOrError);
  absl::StatusOr<::substrait::proto::Plan> planOrError;
  switch (encoding) {
    case PlanFileEncoding::kBinary:
      return textplan::loadFromBinary(*contentOrError);
    case PlanFileEncoding::kJson:
      return textplan::loadFromJson(*contentOrError);
    case PlanFileEncoding::kProtoText:
      return textplan::loadFromProtoText(*contentOrError);
    case PlanFileEncoding::kText:
      return textplan::loadFromText(*contentOrError);
  }
  return absl::UnimplementedError("Unexpected encoding requested.");
}

absl::Status savePlan(
    const ::substrait::proto::Plan& plan,
    std::string_view output_filename,
    PlanFileEncoding encoding) {
  switch (encoding) {
    case PlanFileEncoding::kBinary:
      return textplan::savePlanToBinary(plan, output_filename);
    case PlanFileEncoding::kJson:
      return textplan::savePlanToJson(plan, output_filename);
    case PlanFileEncoding::kProtoText:
      return textplan::savePlanToProtoText(plan, output_filename);
    case PlanFileEncoding::kText:
      return textplan::savePlanToText(plan, output_filename);
  }
  return absl::UnimplementedError("Unexpected encoding requested.");
}

} // namespace io::substrait
