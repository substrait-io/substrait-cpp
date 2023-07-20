/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/LoadBinary.h"

#include <fmt/format.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/StringManipulation.h"

namespace io::substrait::textplan {

namespace {

class StringErrorCollector : public google::protobuf::io::ErrorCollector {
 public:
  void AddError(int line, int column, const std::string& message) override {
    errors_.push_back(
        std::to_string(line + 1) + ":" + std::to_string(column + 1) + " → " +
        message);
  }

  [[nodiscard]] std::vector<std::string> getErrors() const {
    return errors_;
  }

 private:
  std::vector<std::string> errors_;
};

} // namespace

absl::StatusOr<std::string> readFromFile(std::string_view msgPath) {
  std::ifstream textFile(std::string{msgPath});
  if (textFile.fail()) {
    auto currDir = std::filesystem::current_path().string();
    return absl::ErrnoToStatus(
        errno,
        fmt::format(
            "Failed to open file {} when running in {}", msgPath, currDir));
  }
  std::stringstream buffer;
  buffer << textFile.rdbuf();
  return buffer.str();
}

absl::StatusOr<::substrait::proto::Plan> loadFromJson(const std::string& json) {
  if (json.empty()) {
    return absl::InternalError("Provided JSON string was empty.");
  }
  std::string_view usableJson = json;
  if (json[0] == '#') {
    int idx = 0;
    while (idx < json.size() && json[idx] != '\n') {
      idx++;
    }
    usableJson.remove_prefix(idx);
  }
  ::substrait::proto::Plan plan;
  auto status = google::protobuf::util::JsonStringToMessage(
      std::string{usableJson}, &plan);
  if (!status.ok()) {
    std::string msg{status.message()};
    return absl::InternalError(
        fmt::format("Failed to parse Substrait JSON: {}", msg));
  }
  return plan;
}

absl::StatusOr<::substrait::proto::Plan> loadFromProtoText(
    const std::string& text) {
  ::substrait::proto::Plan plan;
  ::google::protobuf::TextFormat::Parser parser;
  StringErrorCollector collector;
  parser.RecordErrorsTo(&collector);
  if (!parser.ParseFromString(text, &plan)) {
    auto errors = collector.getErrors();
    return absl::InternalError(joinLines(errors));
  }
  return plan;
}

absl::StatusOr<::substrait::proto::Plan> loadFromBinary(
    const std::string& bytes) {
  ::substrait::proto::Plan plan;
  if (!plan.ParseFromString(bytes)) {
    return absl::InternalError("Failed to parse as a binary Substrait plan.");
  }
  return plan;
}

} // namespace io::substrait::textplan
