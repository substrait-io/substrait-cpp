/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/LoadBinary.h"

#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "substrait/common/Exceptions.h"
#include "substrait/proto/plan.pb.h"

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

std::string readFromFile(std::string_view msgPath) {
  std::ifstream textFile(std::string{msgPath});
  if (textFile.fail()) {
    auto currdir = std::filesystem::current_path().string();
    SUBSTRAIT_FAIL(
        "Failed to open file {} when running in {}: {}",
        msgPath,
        currdir,
        strerror(errno));
  }
  std::stringstream buffer;
  buffer << textFile.rdbuf();
  return buffer.str();
}

PlanOrErrors loadFromJson(std::string_view json) {
  if (json.empty()) {
    return PlanOrErrors({"Provided JSON string was empty."});
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
    return PlanOrErrors({fmt::format(
        "Failed to parse Substrait JSON: {}", status.message())});
  }
  return PlanOrErrors(plan);
}

PlanOrErrors loadFromText(const std::string& text) {
  ::substrait::proto::Plan plan;
  ::google::protobuf::TextFormat::Parser parser;
  StringErrorCollector collector;
  parser.RecordErrorsTo(&collector);
  if (!parser.ParseFromString(text, &plan)) {
    return PlanOrErrors(collector.getErrors());
  }
  return PlanOrErrors(plan);
}

} // namespace io::substrait::textplan
