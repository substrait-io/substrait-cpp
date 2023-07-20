/* SPDX-License-Identifier: Apache-2.0 */

#include <algorithm>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <protobuf-matchers/protocol-buffer-matchers.h>

#include "substrait/textplan/SymbolTablePrinter.h"
#include "substrait/textplan/converter/LoadBinary.h"
#include "substrait/textplan/converter/ParseBinary.h"
#include "substrait/textplan/converter/ReferenceNormalizer.h"
#include "substrait/textplan/parser/ParseText.h"
#include "substrait/textplan/tests/ParseResultMatchers.h"

using ::protobuf_matchers::EqualsProto;
using ::protobuf_matchers::IgnoringFieldPaths;
using ::protobuf_matchers::Partially;
using ::testing::AllOf;

namespace io::substrait::textplan {
namespace {

bool endsWith(const std::string& haystack, const std::string& needle) {
  return haystack.size() > needle.size() &&
      haystack.substr(haystack.size() - needle.size()) == needle;
}

std::string addLineNumbers(const std::string& text) {
  std::stringstream input{text};
  std::stringstream result;
  int lineNum = 0;
  std::string line;
  while (std::getline(input, line, '\n')) {
    result << std::setw(4) << ++lineNum << " " << line << std::endl;
  }
  return result.str();
}

::substrait::proto::Plan normalizePlan(const ::substrait::proto::Plan& plan) {
  ::substrait::proto::Plan newPlan = plan;
  ReferenceNormalizer::normalize(&newPlan);
  return newPlan;
}

class RoundTripBinaryToTextFixture
    : public ::testing::TestWithParam<std::string> {};

std::vector<std::string> getTestCases() {
  const std::filesystem::path currPath = std::filesystem::current_path();
  std::vector<std::string> filenames{};
  std::filesystem::path testDataPath = currPath;
  testDataPath.append("data");
  for (auto const& dirEntry :
       std::filesystem::recursive_directory_iterator{testDataPath}) {
    std::string pathName = dirEntry.path();
    if (endsWith(pathName, ".json") &&
        !endsWith(pathName, "q6_first_stage.json")) {
      filenames.push_back(pathName);
    }
  }
  std::sort(filenames.begin(), filenames.end());
  return filenames;
}

TEST_P(RoundTripBinaryToTextFixture, RoundTrip) {
  auto filename = GetParam();
  auto jsonOrError = readFromFile(filename);
  ASSERT_TRUE(jsonOrError.ok());
  auto planOrError = loadFromJson(*jsonOrError);
  ASSERT_TRUE(planOrError.ok());

  auto plan = *planOrError;

  auto textResult = parseBinaryPlan(plan);
  auto textSymbols = textResult.getSymbolTable().getSymbols();

  std::string outputText =
      SymbolTablePrinter::outputToText(textResult.getSymbolTable());

  ASSERT_THAT(textResult, AllOf(ParsesOk(), HasErrors({})))
      << std::endl
      << "Initial result:" << std::endl
      << addLineNumbers(outputText) << std::endl
      << textResult.getSymbolTable().toDebugString() << std::endl;

  auto stream = loadTextString(outputText);
  auto result = parseStream(stream);
  ASSERT_NO_THROW(auto outputBinary = SymbolTablePrinter::outputToBinaryPlan(
                      result.getSymbolTable()););

  auto normalizedPlan = normalizePlan(plan);
  ASSERT_THAT(
      result,
      ::testing::AllOf(
          ParsesOk(), HasErrors({}), AsBinaryPlan(EqualsProto(normalizedPlan))))
      << std::endl
      << "Intermediate result:" << std::endl
      << addLineNumbers(outputText) << std::endl
      << result.getSymbolTable().toDebugString() << std::endl;
}

INSTANTIATE_TEST_SUITE_P(
    RoundTripBinaryToTextTests,
    RoundTripBinaryToTextFixture,
    ::testing::ValuesIn(getTestCases()),
    [](const testing::TestParamInfo<std::string>& info) {
      std::string identifier = info.param;
      auto lastSlash = identifier.find_last_of('/');
      if (lastSlash != std::string::npos) {
        identifier = identifier.substr(lastSlash);
      }
      if (identifier.length() > 5 &&
          identifier.substr(identifier.length() - 5) == ".json") {
        identifier = identifier.substr(0, identifier.length() - 5);
      }

      // Remove non-alphanumeric characters to make the test framework happy.
      identifier.erase(
          std::remove_if(
              identifier.begin(),
              identifier.end(),
              [](auto const& c) -> bool { return !std::isalnum(c); }),
          identifier.end());
      return identifier;
    });

} // namespace
} // namespace io::substrait::textplan
