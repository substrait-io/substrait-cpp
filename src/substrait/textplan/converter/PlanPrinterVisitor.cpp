/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/PlanPrinterVisitor.h"

#include <iterator>
#include <optional>
#include <sstream>
#include <string>

#include "substrait/proto/ProtoUtils.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/textplan/Any.h"

namespace io::substrait::textplan {

std::string PlanPrinterVisitor::printRelation(
    const std::string& symbolName,
    const ::substrait::proto::Rel* relation) {
  std::stringstream text;

  text << RelTypeCaseName(relation->rel_type_case()) << " relation "
       << symbolName << " {\n";
  auto symbol = symbolTable_->lookupSymbolByLocation(
      Location((google::protobuf::Message*)&relation));
  if (symbol != SymbolTable::kUnknownSymbol) {
    text << "  source " << symbol.name << ";\n";
  }
  auto result = this->visitRelation(*relation);
  if (result.type() != typeid(std::string)) {
    return "ERROR:  Relation subtype " +
        RelTypeCaseName((relation->rel_type_case())) + " not supported!\n";
  }
  text << ANY_CAST(std::string, result);
  text << "}\n";

  return text.str();
}

std::any PlanPrinterVisitor::visitAggregateFunction(
    const ::substrait::proto::AggregateFunction& function) {
  return std::string("AF-NOT-YET-IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitExpression(
    const ::substrait::proto::Expression& expression) {
  return std::string("EXPR-NOT-YET-IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitMaskExpression(
    const ::substrait::proto::Expression::MaskExpression& expression) {
  return std::string("MASKEXPR-NOT-YET-IMPLEMENTED");
}

std::any PlanPrinterVisitor::visitReadRelation(
    const ::substrait::proto::ReadRel& relation) {
  std::stringstream text;
  if (relation.has_base_schema()) {
    const auto& symbol = symbolTable_->lookupSymbolByLocation(
        Location((google::protobuf::Message*)&relation));
    if (symbol != SymbolTable::kUnknownSymbol) {
      text << "  base_schema " << symbol.name << ";\n";
    }
  }
  if (relation.has_filter()) {
    text << "  filter "
         << ANY_CAST(std::string, visitExpression(relation.filter())) + ";\n";
  }
  if (relation.has_best_effort_filter()) {
    text << "  filter "
         << ANY_CAST(
                std::string, visitExpression(relation.best_effort_filter()))
         << ";\n";
  }
  if (relation.has_projection()) {
    text << "  projection "
         << ANY_CAST(std::string, visitMaskExpression(relation.projection()))
         << ";\n";
  }

  return text.str();
}

std::any PlanPrinterVisitor::visitFilterRelation(
    const ::substrait::proto::FilterRel& relation) {
  std::stringstream text;
  if (relation.has_condition()) {
    text << "  condition "
         << ANY_CAST(std::string, visitExpression(relation.condition()))
         << ";\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitFetchRelation(
    const ::substrait::proto::FetchRel& relation) {
  std::stringstream text;
  if (relation.offset() != 0) {
    text << "  offset " << std::to_string(relation.offset()) << ";\n";
  }
  text << "  count " << std::to_string(relation.count()) << ";\n";
  return text.str();
}

std::any PlanPrinterVisitor::visitAggregateRelation(
    const ::substrait::proto::AggregateRel& relation) {
  std::stringstream text;
  for (const auto& group : relation.groupings()) {
    for (const auto& expr : group.grouping_expressions()) {
      text << "  grouping " << ANY_CAST(std::string, visitExpression(expr))
           << ";\n";
    }
  }
  for (const auto& measure : relation.measures()) {
    if (!measure.has_measure()) {
      continue;
    }
    text << "  measure {\n";
    text << "    measure "
         << ANY_CAST(std::string, visitAggregateFunction(measure.measure()))
         << ";\n";
    if (measure.has_filter()) {
      text << "    filter " +
              ANY_CAST(std::string, visitExpression(measure.filter()))
           << ";\n";
    }
    text << "  }\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitSortRelation(
    const ::substrait::proto::SortRel& relation) {
  std::stringstream text;
  for (const auto& sort : relation.sorts()) {
    text << "  sort " << ANY_CAST(std::string, visitExpression(sort.expr()));
    switch (sort.sort_kind_case()) {
      case ::substrait::proto::SortField::kDirection:
        switch (sort.direction()) {
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_ASC_NULLS_FIRST:
            text << " by ASC_NULLS_FIRST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_ASC_NULLS_LAST:
            text << " by ASC_NULLS_LAST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_DESC_NULLS_FIRST:
            text << " by DESC_NULLS_FIRST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_DESC_NULLS_LAST:
            text << " by DESC_NULLS_LAST";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_CLUSTERED:
            text << " by CLUSTERED";
            break;
          case ::substrait::proto::
              SortField_SortDirection_SORT_DIRECTION_UNSPECIFIED:
          default:
            break;
        }
        break;
      case ::substrait::proto::SortField::kComparisonFunctionReference: {
        auto field = symbolTable_->nthSymbolByType(
            sort.comparison_function_reference(), SymbolType::kFunction);
        if (field == SymbolTable::kUnknownSymbol) {
          return field.name;
        } else {
          return "functionref#" +
              std::to_string(sort.comparison_function_reference());
        }
      }
      case ::substrait::proto::SortField::SORT_KIND_NOT_SET:
        break;
    }
    text << ";\n";
  }
  return text.str();
}

std::any PlanPrinterVisitor::visitProjectRelation(
    const ::substrait::proto::ProjectRel& relation) {
  std::stringstream text;
  for (const auto& expr : relation.expressions()) {
    text << "  expression " << ANY_CAST(std::string, visitExpression(expr))
         << ";\n";
  }
  return text.str();
}

} // namespace io::substrait::textplan
