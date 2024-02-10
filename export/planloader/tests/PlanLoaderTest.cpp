/* SPDX-License-Identifier: Apache-2.0 */

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <functional>

#include "../planloader.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait::textplan {
namespace {

TEST(PlanLoaderTest, LoadAndSave) {
  auto serializedPlan = load_substrait_plan("data/q6_first_stage.json");
  ASSERT_EQ(serializedPlan->error_message, nullptr);

  ::substrait::proto::Plan plan;
  bool parseStatus =
      plan.ParseFromArray(serializedPlan->buffer, serializedPlan->size);
  ASSERT_TRUE(parseStatus) << "Failed to parse the plan.";

  const char* saveStatus = save_substrait_plan(
      serializedPlan->buffer,
      serializedPlan->size,
      "outfile.splan",
      PlanFileFormat::kText);
  ASSERT_EQ(saveStatus, nullptr);

  free_substrait_plan(serializedPlan);
}

TEST(PlanLoaderTest, LoadMissingFile) {
  auto serializedPlan = load_substrait_plan("no_such_file.json");
  ASSERT_THAT(
      serializedPlan->error_message,
      ::testing::StartsWith("Failed to open file no_such_file.json"));
}

} // namespace
} // namespace io::substrait::textplan
