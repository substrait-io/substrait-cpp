/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/LoadText.h"

#include <absl/strings/str_join.h>

#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/StringManipulation.h"
#include "substrait/textplan/SymbolTablePrinter.h"
#include "substrait/textplan/parser/ParseText.h"

namespace io::substrait::textplan {

absl::StatusOr<::substrait::proto::Plan> loadFromText(const std::string& text) {
  auto stream = loadTextString(text);
  auto parseResult = io::substrait::textplan::parseStream(stream);
  if (!parseResult.successful()) {
    auto errors = parseResult.getAllErrors();
    return absl::UnknownError(absl::StrJoin(errors, ""));
  }

  return SymbolTablePrinter::outputToBinaryPlan(parseResult.getSymbolTable());
}

} // namespace io::substrait::textplan
