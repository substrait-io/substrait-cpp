/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/ParseResult.h"

#include <sstream>

namespace io::substrait::textplan {

std::ostream& operator<<(std::ostream& os, const ParseResult& result) {
  if (result.successful()) {
    os << *result.symbolTable_;
  }
  auto msgs = result.getSyntaxErrors();
  if (!msgs.empty()) {
    os << "{" << '\n';
    for (const std::string& msg : msgs) {
      os << "  \"" << msg << "\"," << '\n';
    }
    os << "}";
  }
  msgs = result.getSemanticErrors();
  if (!msgs.empty()) {
    os << "{" << '\n';
    for (const std::string& msg : msgs) {
      os << "  \"" << msg << "\"," << '\n';
    }
    os << "}";
  }
  return os;
}

} // namespace io::substrait::textplan
