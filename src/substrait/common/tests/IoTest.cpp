/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/common/Io.h"

#include <filesystem>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <protobuf-matchers/protocol-buffer-matchers.h>

#ifndef _WIN32
#include <unistd.h>
#endif

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
  void SetUp() override {
    testFileDirectory_ = (std::filesystem::temp_directory_path() /
                          std::filesystem::path("my_temp_dir"))
                             .string();

    std::filesystem::create_directory(testFileDirectory_);
    if (!std::filesystem::exists(testFileDirectory_)) {
      ASSERT_TRUE(false) << "Failed to create temporary directory.";
      testFileDirectory_.clear();
    }
  }

  void TearDown() override {
    if (!testFileDirectory_.empty()) {
      std::error_code err;
      std::filesystem::remove_all(testFileDirectory_, err);
      ASSERT_FALSE(err) << err.message();
    }
  }

  static std::string makeTempFileName() {
    static int tempFileNum = 0;
    return "testfile" + std::to_string(++tempFileNum);
  }

 protected:
  std::string testFileDirectory_;
};

TEST_P(SaveAndLoadTestFixture, SaveAndLoad) {
  auto tempFilename = testFileDirectory_ + "/" + makeTempFileName();
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

static auto getFormats() {
  return testing::Values(
      PlanFileFormat::kBinary,
      PlanFileFormat::kJson,
      PlanFileFormat::kProtoText

#ifndef _WIN32
      // Text format is currently not supported on Windows
      ,
      PlanFileFormat::kText
#endif
  );
}

INSTANTIATE_TEST_SUITE_P(
    SaveAndLoadTests,
    SaveAndLoadTestFixture,
    getFormats(),
    [](const testing::TestParamInfo<SaveAndLoadTestFixture::ParamType>& info) {
      return planFileEncodingToString(info.param);
    });

} // namespace io::substrait
