/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <ANTLRInputStream.h>

#include <optional>
#include <string_view>

#include "substrait/textplan/ParseResult.h"

namespace io::substrait::textplan {

std::optional<antlr4::ANTLRInputStream> loadTextFile(std::string_view filename);
antlr4::ANTLRInputStream loadTextString(std::string_view text);

ParseResult parseStream(antlr4::ANTLRInputStream* stream);

} // namespace io::substrait::textplan
