/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/ParseBinary.h"

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/converter/InitialPlanProtoVisitor.h"
#include "substrait/textplan/converter/PlanPrinterVisitor.h"

namespace io::substrait::textplan {

ParseResult ParseBinaryPlan(const ::substrait::proto::Plan& plan) {
  InitialPlanProtoVisitor visitor(plan);
  visitor.visit();
  auto symbols = visitor.GetSymbolTable();
  auto syntaxErrors = visitor.GetErrorListener()->GetErrorMessages();
  std::vector<std::string> semanticErrors;

  PlanPrinterVisitor printer(plan, *symbols);
  printer.visit();
  auto moreErrors = printer.GetErrorListener()->GetErrorMessages();
  semanticErrors.insert(
      semanticErrors.end(), moreErrors.begin(), moreErrors.end());

  return {*printer.GetSymbolTable(), syntaxErrors, semanticErrors};
}

} // namespace io::substrait::textplan
