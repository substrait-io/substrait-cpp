/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/ParseBinary.h"

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/converter/InitialPlanProtoVisitor.h"
#include "substrait/textplan/converter/PlanPrinterVisitor.h"

namespace io::substrait::textplan {

ParseResult ParseBinaryPlan(const ::substrait::proto::Plan& plan) {
  InitialPlanProtoVisitor visitor;
  visitor.visit(plan);
  auto symbols = visitor.getSymbolTable();
  auto syntaxErrors = visitor.getErrorListener()->getErrorMessages();
  std::vector<std::string> semanticErrors;

  PlanPrinterVisitor printer(*symbols);
  printer.visit(plan);
  auto moreErrors = printer.getErrorListener()->getErrorMessages();
  semanticErrors.insert(
      semanticErrors.end(), moreErrors.begin(), moreErrors.end());

  return {*printer.getSymbolTable(), syntaxErrors, semanticErrors};
}

} // namespace io::substrait::textplan
