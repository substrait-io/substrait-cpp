/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include "SubstraitErrorListener.h"
#include "SymbolTable.h"

namespace substrait::proto {
class Expression;
class Plan;
class Rel;
} // namespace substrait::proto

namespace io::substrait::textplan {

class SymbolTablePrinter {
 public:
  static std::string outputToText(
      const SymbolTable& symbolTable,
      SubstraitErrorListener* errorListener);

  static ::substrait::proto::Plan outputToBinaryPlan(
      const SymbolTable& symbolTable);

 private:
  static void addInputsToRelation(
      const SymbolTable& symbolTable,
      const SymbolInfo& symbolInfo,
      ::substrait::proto::Rel* relation);

  static void addInputsToExpression(
      const SymbolTable& symbolTable,
      const std::vector<const SymbolInfo*>& symbolInfos,
      ::substrait::proto::Expression* expression,
      int* consumedPipelines);
};

} // namespace io::substrait::textplan
