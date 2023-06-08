/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "SubstraitPlanParser/SubstraitPlanParserBaseVisitor.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/parser/SubstraitParserErrorListener.h"
#include "substrait/type/Type.h"

namespace substrait::proto {
class Type;
}

namespace io::substrait::textplan {

class SubstraitPlanTypeVisitor : public SubstraitPlanParserBaseVisitor {
 public:
  SubstraitPlanTypeVisitor(
      const SymbolTable& symbolTable,
      std::shared_ptr<SubstraitParserErrorListener> errorListener) {
    symbolTable_ = std::make_shared<SymbolTable>(symbolTable);
    errorListener_ = std::move(errorListener);
  }

  std::any visitLiteral_basic_type(
      SubstraitPlanParser::Literal_basic_typeContext* ctx) override;
  std::any visitLiteral_complex_type(
      SubstraitPlanParser::Literal_complex_typeContext* ctx) override;

 protected:
  ::substrait::proto::Type textToTypeProto(
      const antlr4::ParserRuleContext* ctx,
      const std::string& typeText);

  ::substrait::proto::Type typeToProto(
      const antlr4::ParserRuleContext* ctx,
      const ParameterizedType& decodedType);

  // Identifies whether the given context has a parent node of a constant
  // including a struct.  This allows {3years, 1month,
  // 2days}_interval_year_month_day to have the optional label tags which are
  // not real types.
  bool insideStructLiteralWithExternalType(const antlr4::RuleContext* ctx);

  std::shared_ptr<SymbolTable> symbolTable_;
  std::shared_ptr<SubstraitParserErrorListener> errorListener_;
};

} // namespace io::substrait::textplan
