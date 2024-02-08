/* SPDX-License-Identifier: Apache-2.0 */

#include <gtest/gtest.h>
#include <functional>

#include "../planloader.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait::textplan {
namespace {

TEST(PlanLoaderTest, LoadAndSave) {
  auto serializedPlan = load_substrait_plan("data/q6_first_stage.json");
  ASSERT_EQ(serializedPlan->errorMessage, nullptr);

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

} // namespace
} // namespace io::substrait::textplan
