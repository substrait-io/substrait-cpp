/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include "SymbolTable.h"

namespace substrait::proto {
class Plan;
class Rel;
} // namespace substrait::proto

namespace io::substrait::textplan {

class SymbolTablePrinter {
 public:
  static std::string outputToText(const SymbolTable& symbolTable);

  static ::substrait::proto::Plan outputToBinaryPlan(
      const SymbolTable& symbolTable);

 private:
  static void addInputsToRelation(
      const SymbolInfo& symbolInfo,
      ::substrait::proto::Rel* relation);
};

} // namespace io::substrait::textplan
