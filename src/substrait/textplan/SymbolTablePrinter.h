/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include "SymbolTable.h"

namespace substrait::proto {
class Plan;
}

namespace io::substrait::textplan {

class SymbolTablePrinter {
 public:
  static std::string outputToText(const SymbolTable& symbolTable);

  static ::substrait::proto::Plan outputToBinaryPlan(
      const SymbolTable& symbolTable);
};

} // namespace io::substrait::textplan
