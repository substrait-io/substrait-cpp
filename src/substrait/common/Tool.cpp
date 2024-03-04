/* SPDX-License-Identifier: Apache-2.0 */

#include <iostream>

#include "substrait/common/Io.h"

namespace io::substrait {
namespace {

PlanFileFormat planFileFormatFromText(std::string_view str) {
  std::string foo;
  foo.resize(str.size());
  std::transform(str.begin(), str.end(), foo.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  if (foo == "binary") {
    return PlanFileFormat::kBinary;
  } else if (foo == "json") {
    return PlanFileFormat::kJson;
  } else if (foo == "prototext") {
    return PlanFileFormat::kProtoText;
  } else if (foo == "text") {
    return PlanFileFormat::kText;
  }
  // If the format can't be understood, default to text.
  return PlanFileFormat::kText;
}

} // namespace
} // namespace io::substrait

int main(int argc, char* argv[]) {
  if (argc <= 3) {
    printf(
        "Usage:  plantransformer <inputfilename> <outputfilename> [BINARY|JSON|PROTOTEXT|TEXT]\n");
    return EXIT_FAILURE;
  }

  auto planOrError = io::substrait::loadPlan(argv[1]);
  if (!planOrError.ok()) {
    std::cerr << planOrError.status() << std::endl;
    return EXIT_FAILURE;
  }

  auto format = io::substrait::planFileFormatFromText(argv[3]);

  auto result = io::substrait::savePlan(*planOrError, argv[2], format);
  if (!result.ok()) {
    std::cerr << result << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
