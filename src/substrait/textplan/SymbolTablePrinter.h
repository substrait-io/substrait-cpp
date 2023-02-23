/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include "SymbolTable.h"

namespace io::substrait::textplan {

class SymbolTablePrinter {
 public:
  static std::string OutputToText(const SymbolTable& symbolTable);
};

} // namespace io::substrait::textplan
