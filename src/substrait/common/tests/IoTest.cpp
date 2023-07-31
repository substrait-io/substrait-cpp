/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/common/Io.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <protobuf-matchers/protocol-buffer-matchers.h>
#include <unistd.h>

using ::protobuf_matchers::EqualsProto;
using ::protobuf_matchers::Partially;

namespace io::substrait {

namespace {

constexpr const char* planFileEncodingToString(PlanFileFormat e) noexcept {
  switch (e) {
    case PlanFileFormat::kBinary:
      return "kBinary";
    case PlanFileFormat::kJson:
      return "kJson";
    case PlanFileFormat::kProtoText:
      return "kProtoText";
    case PlanFileFormat::kText:
      return "kText";
  }
  return "IMPOSSIBLE";
}

} // namespace

class IoTest : public ::testing::Test {};

TEST_F(IoTest, LoadMissingFile) {
  auto result = ::io::substrait::loadPlan("non-existent-file");
  ASSERT_FALSE(result.ok());
  ASSERT_THAT(
      result.status().message(),
      ::testing::ContainsRegex("Failed to open file non-existent-file"));
}

class SaveAndLoadTestFixture : public ::testing::TestWithParam<PlanFileFormat> {
 public:
  ~SaveAndLoadTestFixture() override {
    for (const auto& filename : testFiles_) {
      unlink(filename.c_str());
    }
  }

  void registerCleanup(const char* filename) {
    testFiles_.emplace_back(filename);
  }

 private:
  std::vector<std::string> testFiles_;
};

TEST_P(SaveAndLoadTestFixture, SaveAndLoad) {
  auto tempFilename = std::tmpnam(nullptr);
  registerCleanup(tempFilename);
  PlanFileFormat encoding = GetParam();

  ::substrait::proto::Plan plan;
  auto root = plan.add_relations()->mutable_root();
  auto read = root->mutable_input()->mutable_read();
  read->mutable_common()->mutable_direct();
  read->mutable_named_table()->add_names("table_name");
  auto status = ::io::substrait::savePlan(plan, tempFilename, encoding);
  ASSERT_TRUE(status.ok()) << "Save failed.\n" << status;

  auto result = ::io::substrait::loadPlan(tempFilename);
  ASSERT_TRUE(result.ok()) << "Load failed.\n" << result.status();
  ASSERT_THAT(
      *result,
      Partially(EqualsProto<::substrait::proto::Plan>(
          R"(relations {
            root {
              input {
                read {
                  common {
                    direct {
                    }
                  }
                  named_table {
                    names: "table_name"
                  }
                }
              }
            }
          })")));
}

INSTANTIATE_TEST_SUITE_P(
    SaveAndLoadTests,
    SaveAndLoadTestFixture,
    testing::Values(
        PlanFileFormat::kBinary,
        PlanFileFormat::kJson,
        PlanFileFormat::kProtoText,
        PlanFileFormat::kText),
    [](const testing::TestParamInfo<SaveAndLoadTestFixture::ParamType>& info) {
      return planFileEncodingToString(info.param);
    });

} // namespace io::substrait
