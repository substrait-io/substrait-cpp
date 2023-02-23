/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/PlanPrinterVisitor.h"

#include <iterator>
#include <string>

#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait::textplan {

void PlanPrinterVisitor::visit() {
  symbol_table_->AddCachedOutput(std::any_cast<std::string>(visitPlan()));
}

std::any PlanPrinterVisitor::visitPlan() {
  return std::string();
}

std::any PlanPrinterVisitor::visitRelationRoot(
    const ::substrait::proto::RelRoot& relation) {
  BasePlanProtoVisitor::visitRelationRoot(relation);
  return std::nullopt;
}

} // namespace io::substrait::textplan
