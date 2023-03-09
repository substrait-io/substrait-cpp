/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/textplan/ParseResult.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/parser/SubstraitPlanVisitor.h"

namespace antlr4 {
class ANTLRInputStream;
} // namespace antlr4

namespace io::substrait::textplan {

std::optional<antlr4::ANTLRInputStream> loadTextFile(std::string_view filename);
antlr4::ANTLRInputStream loadTextString(std::string_view text);

ParseResult parseStream(antlr4::ANTLRInputStream stream);

} // namespace io::substrait::textplan
