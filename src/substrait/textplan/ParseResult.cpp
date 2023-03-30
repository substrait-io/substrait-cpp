/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/ParseResult.h"

#include <iosfwd>
#include <iostream>

namespace io::substrait::textplan {

std::ostream& operator<<(std::ostream& os, const ParseResult& result) {
  if (result.successful()) {
    os << *result.symbolTable_;
  }
  auto msgs = result.getSyntaxErrors();
  if (!msgs.empty()) {
    os << "{" << std::endl;
    for (const std::string& msg : msgs) {
      os << "  \"" << msg << "\"," << std::endl;
    }
    os << "}";
  }
  msgs = result.getSemanticErrors();
  if (!msgs.empty()) {
    os << "{" << std::endl;
    for (const std::string& msg : msgs) {
      os << "  \"" << msg << "\"," << std::endl;
    }
    os << "}";
  }
  return os;
}

} // namespace io::substrait::textplan
