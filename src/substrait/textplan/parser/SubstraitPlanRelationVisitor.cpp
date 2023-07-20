/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/parser/SubstraitPlanRelationVisitor.h"

#include <chrono>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

#include "SubstraitPlanParser/SubstraitPlanParser.h"
#include "SubstraitPlanTypeVisitor.h"
#include "absl/strings/ascii.h"
#include "absl/strings/numbers.h"
#include "absl/strings/strip.h"
#include "date/tz.h"
#include "substrait/expression/DecimalLiteral.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/type.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/Finally.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/StringManipulation.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

namespace {

const std::string kAggregationPhasePrefix = "aggregationphase";
const std::string kAggregationInvocationPrefix = "aggregationinvocation";
const std::string kJoinTypePrefix = "jointype";
const std::string kSortDirectionPrefix = "sortdirection";

const std::string kIntermediateNodeName = "intermediate";

enum RelationFilterBehavior {
  kDefault = 0,
  kBestEffort = 1,
  kPostJoin = 2,
};

std::string toLower(const std::string& str) {
  std::string s = str;
  std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
  return s;
}

void setNullable(::substrait::proto::Type* type) {
  switch (type->kind_case()) {
    case ::substrait::proto::Type::kBool:
      type->mutable_bool_()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI8:
      type->mutable_i8()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI16:
      type->mutable_i16()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI32:
      type->mutable_i32()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kI64:
      type->mutable_i64()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFp32:
      type->mutable_fp32()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFp64:
      type->mutable_fp64()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kString:
      type->mutable_string()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kBinary:
      type->mutable_binary()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kTimestamp:
      type->mutable_timestamp()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kDate:
      type->mutable_date()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kTime:
      type->mutable_time()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kIntervalYear:
      type->mutable_interval_year()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kIntervalDay:
      type->mutable_interval_day()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kTimestampTz:
      type->mutable_timestamp_tz()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kUuid:
      type->mutable_uuid()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFixedChar:
      type->mutable_fixed_char()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kVarchar:
      type->mutable_varchar()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kFixedBinary:
      type->mutable_fixed_binary()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kDecimal:
      type->mutable_decimal()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kStruct:
      type->mutable_struct_()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kList:
      type->mutable_list()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kMap:
      type->mutable_map()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kUserDefined:
      type->mutable_user_defined()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE);
      break;
    case ::substrait::proto::Type::kUserDefinedTypeReference:
    case ::substrait::proto::Type::KIND_NOT_SET:
      // We are guaranteed to have a valid type so no need to emit an error.
      break;
  }
}

void setRelationType(
    RelationType relationType,
    ::substrait::proto::Rel* relation) {
  switch (relationType) {
    case RelationType::kRead:
      relation->mutable_read()->clear_common();
      break;
    case RelationType::kProject:
      relation->mutable_project()->clear_common();
      break;
    case RelationType::kJoin:
      relation->mutable_join()->clear_common();
      break;
    case RelationType::kCross:
      relation->mutable_cross()->clear_common();
      break;
    case RelationType::kFetch:
      relation->mutable_fetch()->clear_common();
      break;
    case RelationType::kAggregate:
      relation->mutable_aggregate()->clear_common();
      break;
    case RelationType::kSort:
      relation->mutable_sort()->clear_common();
      break;
    case RelationType::kFilter:
      relation->mutable_filter()->clear_common();
      break;
    case RelationType::kSet:
      relation->mutable_set()->clear_common();
      break;
    case RelationType::kExchange:
    case RelationType::kDdl:
    case RelationType::kWrite:
      break;
    case RelationType::kHashJoin:
      relation->mutable_hash_join()->clear_common();
      break;
    case RelationType::kMergeJoin:
      relation->mutable_merge_join()->clear_common();
      break;
    case RelationType::kExtensionLeaf:
      relation->mutable_extension_leaf()->clear_common();
      break;
    case RelationType::kExtensionSingle:
      relation->mutable_extension_single()->clear_common();
      break;
    case RelationType::kExtensionMulti:
      relation->mutable_extension_multi()->clear_common();
      break;
    case RelationType::kUnknown:
      break;
  }
}

::substrait::proto::RelCommon* findCommonRelation(
    RelationType relationType,
    ::substrait::proto::Rel* relation) {
  switch (relationType) {
    case RelationType::kRead:
      return relation->mutable_read()->mutable_common();
    case RelationType::kProject:
      return relation->mutable_project()->mutable_common();
    case RelationType::kJoin:
      return relation->mutable_join()->mutable_common();
    case RelationType::kCross:
      return relation->mutable_cross()->mutable_common();
    case RelationType::kFetch:
      return relation->mutable_fetch()->mutable_common();
    case RelationType::kAggregate:
      return relation->mutable_aggregate()->mutable_common();
    case RelationType::kSort:
      return relation->mutable_sort()->mutable_common();
    case RelationType::kFilter:
      return relation->mutable_filter()->mutable_common();
    case RelationType::kSet:
      return relation->mutable_set()->mutable_common();
    case RelationType::kExtensionLeaf:
      return relation->mutable_extension_leaf()->mutable_common();
    case RelationType::kExtensionMulti:
      return relation->mutable_extension_multi()->mutable_common();
    case RelationType::kExtensionSingle:
      return relation->mutable_extension_single()->mutable_common();
    case RelationType::kHashJoin:
      return relation->mutable_hash_join()->mutable_common();
    case RelationType::kMergeJoin:
      return relation->mutable_merge_join()->mutable_common();
    case RelationType::kExchange:
    case RelationType::kDdl:
    case RelationType::kWrite:
    case RelationType::kUnknown:
      break;
  }
  return nullptr;
}

std::string normalizeProtoEnum(std::string_view text, std::string_view prefix) {
  std::string result{text};
  // Remove non-alphabetic characters.
  result.erase(
      std::remove_if(
          result.begin(),
          result.end(),
          [](auto const& c) -> bool { return !std::isalpha(c); }),
      result.end());
  // Lowercase.
  std::transform(
      result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
      });
  // Remove the prefix if it exists.
  if (startsWith(result, prefix)) {
    result = result.substr(prefix.length());
  }
  return result;
}

void addInputFieldsToSchema(
    RelationType relationType,
    std::shared_ptr<RelationData>& relationData) {
  if (relationData->continuingPipeline != nullptr) {
    auto continuingRelationData = ANY_CAST(
        std::shared_ptr<RelationData>, relationData->continuingPipeline->blob);
    if (!continuingRelationData->outputFieldReferences.empty()) {
      // There is an emit sequence so use that.
      for (auto field : continuingRelationData->outputFieldReferences) {
        relationData->fieldReferences.push_back(field);
      }
    } else {
      // There was no emit so just access all the field references.
      for (auto field : continuingRelationData->fieldReferences) {
        relationData->fieldReferences.push_back(field);
      }
      for (auto field : continuingRelationData->generatedFieldReferences) {
        relationData->fieldReferences.push_back(field);
      }
    }
  }

  for (auto pipeline : relationData->newPipelines) {
    auto pipelineRelationData =
        ANY_CAST(std::shared_ptr<RelationData>, pipeline->blob);
    if (!pipelineRelationData->outputFieldReferences.empty()) {
      for (auto field : pipelineRelationData->outputFieldReferences) {
        relationData->fieldReferences.push_back(field);
      }
    } else {
      for (auto field : pipelineRelationData->fieldReferences) {
        relationData->fieldReferences.push_back(field);
      }
      for (auto field : pipelineRelationData->generatedFieldReferences) {
        relationData->fieldReferences.push_back(field);
      }
    }
  }
}

bool isRelationEmitDetail(SubstraitPlanParser::Relation_detailContext* ctx) {
  return dynamic_cast<SubstraitPlanParser::RelationEmitContext*>(ctx) !=
      nullptr;
}

} // namespace

std::any SubstraitPlanRelationVisitor::aggregateResult(
    std::any aggregate,
    std::any nextResult) {
  if (!nextResult.has_value()) {
    // No point returning an unspecified result over whatever we already have.
    return aggregate;
  }
  return nextResult;
}

std::any SubstraitPlanRelationVisitor::visitRelation(
    SubstraitPlanParser::RelationContext* ctx) {
  // First find the relation created in a previous step.
  auto* symbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(ctx), SymbolType::kRelation);
  if (symbol == nullptr) {
    // This error has been previously dealt with thus we can safely skip it.
    return defaultResult();
  }
  // Create the relation data before visiting children, so they can update it.
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbol->blob);
  ::substrait::proto::Rel relation;

  auto relationType = ANY_CAST(RelationType, symbol->subtype);
  setRelationType(relationType, &relation);

  relationData->relation = relation;
  symbolTable_->updateLocation(*symbol, PROTO_LOCATION(relationData->relation));

  // Mark the current scope for any operations within this relation.
  auto previousScope = currentRelationScope_;
  auto resetCurrentScope =
      finally([&]() { currentRelationScope_ = previousScope; });
  currentRelationScope_ = symbol;

  addInputFieldsToSchema(relationType, relationData);

  // Visit everything but the emit details to gather necessary information.
  for (auto detail : ctx->relation_detail()) {
    if (!isRelationEmitDetail(detail)) {
      visitRelationDetail(detail);
    }
  }

  addExpressionsToSchema(relationData);

  // Now visit the emit details.
  for (auto detail : ctx->relation_detail()) {
    if (isRelationEmitDetail(detail)) {
      visitRelationDetail(detail);
    }
  }

  // Aggregate relations are different in that they alter the emitted fields
  // by default.
  if (relationType == RelationType::kAggregate) {
    relationData->outputFieldReferences.insert(
        relationData->outputFieldReferences.end(),
        relationData->generatedFieldReferences.begin(),
        relationData->generatedFieldReferences.end());
  }

  applyOutputMappingToSchema(ctx->getStart(), relationType, relationData);

  // Emit one empty grouping for an aggregation relation not specifying any.
  if (relationType == RelationType::kAggregate &&
      relationData->relation.aggregate().groupings_size() == 0) {
    relationData->relation.mutable_aggregate()->add_groupings();
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationDetail(
    SubstraitPlanParser::Relation_detailContext* ctx) {
  if (auto* commonCtx =
          dynamic_cast<SubstraitPlanParser::RelationCommonContext*>(ctx)) {
    return visitRelationCommon(commonCtx);
  } else if (
      auto* usesSchemaCtx =
          dynamic_cast<SubstraitPlanParser::RelationUsesSchemaContext*>(ctx)) {
    return visitRelationUsesSchema(usesSchemaCtx);
  } else if (
      auto* filterCtx =
          dynamic_cast<SubstraitPlanParser::RelationFilterContext*>(ctx)) {
    return visitRelationFilter(filterCtx);
  } else if (
      auto* exprCtx =
          dynamic_cast<SubstraitPlanParser::RelationExpressionContext*>(ctx)) {
    return visitRelationExpression(exprCtx);
  } else if (
      auto* advExtensionCtx =
          dynamic_cast<SubstraitPlanParser::RelationAdvancedExtensionContext*>(
              ctx)) {
    return visitRelationAdvancedExtension(advExtensionCtx);
  } else if (
      auto* sourceRefCtx =
          dynamic_cast<SubstraitPlanParser::RelationSourceReferenceContext*>(
              ctx)) {
    return visitRelationSourceReference(sourceRefCtx);
  } else if (
      auto* groupingCtx =
          dynamic_cast<SubstraitPlanParser::RelationGroupingContext*>(ctx)) {
    return visitRelationGrouping(groupingCtx);
  } else if (
      auto* measureCtx =
          dynamic_cast<SubstraitPlanParser::RelationMeasureContext*>(ctx)) {
    return visitRelationMeasure(measureCtx);
  } else if (
      auto* sortCtx =
          dynamic_cast<SubstraitPlanParser::RelationSortContext*>(ctx)) {
    return visitRelationSort(sortCtx);
  } else if (
      auto* countCtx =
          dynamic_cast<SubstraitPlanParser::RelationCountContext*>(ctx)) {
    return visitRelationCount(countCtx);
  } else if (
      auto* joinTypeCtx =
          dynamic_cast<SubstraitPlanParser::RelationJoinTypeContext*>(ctx)) {
    return visitRelationJoinType(joinTypeCtx);
  } else if (
      auto* emitCtx =
          dynamic_cast<SubstraitPlanParser::RelationEmitContext*>(ctx)) {
    return visitRelationEmit(emitCtx);
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelation_filter_behavior(
    SubstraitPlanParser::Relation_filter_behaviorContext* ctx) {
  std::string text = toLower(ctx->getText());
  // Only look at alphabetic characters for this comparison.
  text.erase(
      std::remove_if(
          text.begin(),
          text.end(),
          [](auto const& c) -> bool { return !std::isalnum(c); }),
      text.end());
  if (text == "besteffort") {
    return kBestEffort;
  } else if (text == "postjoin") {
    return kPostJoin;
  }
  errorListener_->addError(
      ctx->getStart(),
      "Best effort and post join are the only two legal filter behavior "
      "choices.  You may also not provide one which will result to the default "
      "filter behavior.");
  return kDefault;
}

std::any SubstraitPlanRelationVisitor::visitRelationFilter(
    SubstraitPlanParser::RelationFilterContext* ctx) {
  RelationFilterBehavior behavior = kDefault;
  if (ctx->relation_filter_behavior() != nullptr) {
    behavior = ANY_CAST(
        RelationFilterBehavior,
        visitRelation_filter_behavior(ctx->relation_filter_behavior()));
  }

  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      PARSER_LOCATION(ctx->parent), SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto result = visitChildren(ctx);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  switch (parentRelationType) {
    case RelationType::kRead:
      switch (behavior) {
        case kDefault:
          if (parentRelationData->relation.read().has_filter()) {
            errorListener_->addError(
                ctx->getStart(),
                "A filter has already been specified for this read relation.  "
                "Only one filter and one best effort filter may be specified "
                "for a read relation.");
            break;
          }
          *parentRelationData->relation.mutable_read()->mutable_filter() =
              ANY_CAST(::substrait::proto::Expression, result);
          break;
        case kBestEffort:
          if (parentRelationData->relation.read().has_best_effort_filter()) {
            errorListener_->addError(
                ctx->getStart(),
                "A best effort filter has already been specified for this read "
                "relation.  Only one filter and one best effort filter may be "
                "specified for a read relation.");
            break;
          }
          *parentRelationData->relation.mutable_read()
               ->mutable_best_effort_filter() =
              ANY_CAST(::substrait::proto::Expression, result);
          break;
        case kPostJoin:
        default:
          errorListener_->addError(
              ctx->getStart(),
              "Default and best effort filters are the only legal filter for "
              "read relations.");
          break;
      }
      break;
    case RelationType::kJoin:
      if (behavior == kPostJoin) {
        if (parentRelationData->relation.join().has_post_join_filter()) {
          errorListener_->addError(
              ctx->getStart(),
              "A post join filter has already been specified for this join "
              "relation.  Only one may be specified.");
          break;
        }
        *parentRelationData->relation.mutable_join()
             ->mutable_post_join_filter() =
            ANY_CAST(::substrait::proto::Expression, result);
      } else {
        errorListener_->addError(
            ctx->getStart(),
            "A post join filter is the only legal option for a join relation.");
      }
      break;
    case RelationType::kFilter:
      if (behavior == kDefault) {
        if (parentRelationData->relation.filter().has_condition()) {
          errorListener_->addError(
              ctx->getStart(),
              "A filter condition has already been specified.  Only one may be "
              "specified.");
          break;
        }
        if (result.type() != typeid(::substrait::proto::Expression)) {
          errorListener_->addError(
              ctx->getStart(), "Could not parse as an expression.");
          return defaultResult();
        }
        *parentRelationData->relation.mutable_filter()->mutable_condition() =
            ANY_CAST(::substrait::proto::Expression, result);
      } else {
        errorListener_->addError(
            ctx->getStart(),
            "No filter behavior options are permissible for a filter join.");
      }
      break;
    default:
      errorListener_->addError(
          ctx->getStart(),
          "Filters are not permitted for this kind of relation.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationUsesSchema(
    SubstraitPlanParser::RelationUsesSchemaContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);

  if (parentRelationType == RelationType::kRead) {
    auto schemaName = ctx->id()->getText();
    auto* symbol = symbolTable_->lookupSymbolByName(schemaName);
    if (symbol != nullptr) {
      auto* schema =
          parentRelationData->relation.mutable_read()->mutable_base_schema();
      for (const auto& sym : *symbolTable_) {
        if (sym.type != SymbolType::kSchemaColumn) {
          continue;
        }
        if (sym.location != symbol->location) {
          continue;
        }
        parentRelationData->outputFieldReferences.push_back(&sym);
        schema->add_names(sym.name);
        auto typeProto = ANY_CAST(::substrait::proto::Type, sym.blob);
        if (typeProto.kind_case() != ::substrait::proto::Type::KIND_NOT_SET) {
          *schema->mutable_struct_()->add_types() = typeProto;
          // If the schema contains any types, the struct is required.
          schema->mutable_struct_()->set_nullability(
              ::substrait::proto::Type_Nullability_NULLABILITY_REQUIRED);
        }
      }
    }
  } else {
    errorListener_->addError(
        ctx->getStart(),
        "Schema references are not defined for this kind of relation.");
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationExpression(
    SubstraitPlanParser::RelationExpressionContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto result = visitChildren(ctx);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  switch (parentRelationType) {
    case RelationType::kJoin:
      if (parentRelationData->relation.join().has_expression()) {
        errorListener_->addError(
            ctx->getStart(),
            "A join expression has already been specified.  Only one may be "
            "specified.");
        break;
      }
      *parentRelationData->relation.mutable_join()->mutable_expression() =
          ANY_CAST(::substrait::proto::Expression, result);
      break;
    case RelationType::kProject:
      *parentRelationData->relation.mutable_project()->add_expressions() =
          ANY_CAST(::substrait::proto::Expression, result);
      break;
    default:
      errorListener_->addError(
          ctx->getStart(),
          "Expressions are not permitted for this kind of relation.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationGrouping(
    SubstraitPlanParser::RelationGroupingContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto result = visitChildren(ctx);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  switch (parentRelationType) {
    case RelationType::kAggregate: {
      if (parentRelationData->relation.aggregate().groupings_size() == 0) {
        parentRelationData->relation.mutable_aggregate()->add_groupings();
      }
      // Always add new expressions to the first groupings group.
      auto newExpr = parentRelationData->relation.mutable_aggregate()
                         ->mutable_groupings(0)
                         ->add_grouping_expressions();
      *newExpr = ANY_CAST(::substrait::proto::Expression, result);
      if (newExpr->has_selection()) {
        newExpr->mutable_selection()->mutable_root_reference();
        if (newExpr->selection().direct_reference().has_struct_field()) {
          parentRelationData->generatedFieldReferences.push_back(
              parentRelationData->fieldReferences[newExpr->selection()
                                                      .direct_reference()
                                                      .struct_field()
                                                      .field()]);
        }
      }
      break;
    }
    default:
      errorListener_->addError(
          ctx->getStart(),
          "Groupings are not permitted for this kind of relation.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationMeasure(
    SubstraitPlanParser::RelationMeasureContext* ctx) {
  // Construct the measure.
  ::substrait::proto::AggregateRel_Measure measure;
  auto invocation = ::substrait::proto::
      AggregateFunction_AggregationInvocation_AGGREGATION_INVOCATION_UNSPECIFIED;
  std::vector<::substrait::proto::SortField> sorts;
  for (auto detail : ctx->measure_detail()) {
    auto detailItem = ANY_CAST(
        ::substrait::proto::AggregateRel_Measure, visitMeasure_detail(detail));
    if (detail->getStart()->getType() == SubstraitPlanParser::MEASURE) {
      if (measure.has_measure()) {
        errorListener_->addError(
            detail->getStart(),
            "A measure expression has already been provided for this measure.");
        break;
      }
      *measure.mutable_measure() = detailItem.measure();
    } else if (detail->getStart()->getType() == SubstraitPlanParser::FILTER) {
      if (measure.has_filter()) {
        errorListener_->addError(
            detail->getStart(),
            "A filter has already been provided for this measure.");
        break;
      }
      *measure.mutable_filter() = detailItem.filter();
    } else if (
        detail->getStart()->getType() == SubstraitPlanParser::INVOCATION) {
      invocation = detailItem.measure().invocation();
    } else if (detail->getStart()->getType() == SubstraitPlanParser::SORT) {
      auto newSorts = detailItem.measure().sorts();
      sorts.insert(sorts.end(), newSorts.begin(), newSorts.end());
    }
  }
  if (invocation !=
      ::substrait::proto::
          AggregateFunction_AggregationInvocation_AGGREGATION_INVOCATION_UNSPECIFIED) {
    measure.mutable_measure()->set_invocation(invocation);
  }
  for (const auto& sort : sorts) {
    *measure.mutable_measure()->add_sorts() = sort;
  }

  // Add it to our relation.
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  switch (parentRelationType) {
    case RelationType::kAggregate:
      *parentRelationData->relation.mutable_aggregate()->add_measures() =
          measure;
      break;
    default:
      errorListener_->addError(
          ctx->getStart(),
          "Measures are not permitted for this kind of relation.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationJoinType(
    SubstraitPlanParser::RelationJoinTypeContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  if (parentRelationType == RelationType::kJoin) {
    std::string text =
        normalizeProtoEnum(ctx->id()->getText(), kJoinTypePrefix);
    ::substrait::proto::JoinRel_JoinType joinType;
    if (text == "unspecified") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_UNSPECIFIED;
    } else if (text == "inner") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_INNER;
    } else if (text == "outer") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_OUTER;
    } else if (text == "left") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_LEFT;
    } else if (text == "right") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_RIGHT;
    } else if (text == "semi") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_SEMI;
    } else if (text == "anti") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_ANTI;
    } else if (text == "single") {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_SINGLE;
    } else {
      joinType = ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_UNSPECIFIED;
    }
    if (joinType ==
        ::substrait::proto::JoinRel_JoinType_JOIN_TYPE_UNSPECIFIED) {
      this->errorListener_->addError(
          ctx->getStart(),
          "Unsupported join type direction: " + ctx->id()->getText());
    }
    parentRelationData->relation.mutable_join()->set_type(joinType);

    // TODO -- Add support for HashJoin/MergeJoin which have different enums.
  } else {
    errorListener_->addError(
        ctx->getStart(),
        "Join types are not supported for this relation type.");
    return defaultResult();
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationEmit(
    SubstraitPlanParser::RelationEmitContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto result = visitChildren(ctx);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  auto common =
      findCommonRelation(parentRelationType, &parentRelationData->relation);
  if (common == nullptr) {
    errorListener_->addError(
        ctx->getStart(), "Emits do not make sense for this kind of relation.");
    return defaultResult();
  }
  common->mutable_emit()->add_output_mapping(ANY_CAST(int32_t, result));
  return defaultResult();
}

int32_t SubstraitPlanRelationVisitor::visitAggregationInvocation(
    SubstraitPlanParser::IdContext* ctx) {
  std::string text =
      normalizeProtoEnum(ctx->getText(), kAggregationInvocationPrefix);
  if (text == "unspecified") {
    return ::substrait::proto::AggregateFunction::
        AGGREGATION_INVOCATION_UNSPECIFIED;
  } else if (text == "all") {
    return ::substrait::proto::AggregateFunction::AGGREGATION_INVOCATION_ALL;
  } else if (text == "distinct") {
    return ::substrait::proto::AggregateFunction::
        AGGREGATION_INVOCATION_DISTINCT;
  }
  this->errorListener_->addError(
      ctx->getStart(),
      "Unrecognized aggregation invocation: " + ctx->getText());
  return ::substrait::proto::AggregateFunction::
      AGGREGATION_INVOCATION_UNSPECIFIED;
}

int32_t SubstraitPlanRelationVisitor::visitAggregationPhase(
    SubstraitPlanParser::IdContext* ctx) {
  std::string text =
      normalizeProtoEnum(ctx->getText(), kAggregationPhasePrefix);
  if (text == "unspecified") {
    return ::substrait::proto::AGGREGATION_PHASE_UNSPECIFIED;
  } else if (text == "initialtointermediate") {
    return ::substrait::proto::AGGREGATION_PHASE_INITIAL_TO_INTERMEDIATE;
  } else if (text == "intermediatetointermediate") {
    return ::substrait::proto::AGGREGATION_PHASE_INTERMEDIATE_TO_INTERMEDIATE;
  } else if (text == "initialtoresult") {
    return ::substrait::proto::AGGREGATION_PHASE_INITIAL_TO_RESULT;
  } else if (text == "intermediatetoresult") {
    return ::substrait::proto::AGGREGATION_PHASE_INTERMEDIATE_TO_RESULT;
  }
  this->errorListener_->addError(
      ctx->getStart(), "Unrecognized aggregation phase: " + ctx->getText());
  return ::substrait::proto::AGGREGATION_PHASE_UNSPECIFIED;
}

std::any SubstraitPlanRelationVisitor::visitMeasure_detail(
    SubstraitPlanParser::Measure_detailContext* ctx) {
  ::substrait::proto::AggregateRel_Measure measure;
  switch (ctx->getStart()->getType()) {
    case SubstraitPlanParser::MEASURE: {
      auto function = measure.mutable_measure();
      auto result = visitExpression(ctx->expression());
      auto expr = ANY_CAST(::substrait::proto::Expression, result);
      if (expr.has_scalar_function()) {
        const auto& scalarFunc = expr.scalar_function();
        function->set_function_reference(scalarFunc.function_reference());
        for (const auto& arg : scalarFunc.arguments()) {
          *function->add_arguments() = arg;
        }
        for (const auto& option : scalarFunc.options()) {
          *function->add_options() = option;
        }
        if (scalarFunc.has_output_type()) {
          *function->mutable_output_type() = scalarFunc.output_type();
        }
        if (ctx->literal_complex_type() != nullptr) {
          // The version here overrides any that might be in the function.
          *function->mutable_output_type() = ANY_CAST(
              ::substrait::proto::Type,
              visitLiteral_complex_type(ctx->literal_complex_type()));
        }
        if (ctx->id(0) != nullptr) {
          measure.mutable_measure()->set_phase(
              static_cast<::substrait::proto::AggregationPhase>(
                  visitAggregationPhase(ctx->id(0))));
        }
      } else {
        errorListener_->addError(
            ctx->id(0)->getStart(),
            "Expected an expression utilizing a function here.");
      }
      // If we have a NAMED clause, add a symbol reference.
      if (ctx->id().size() > 1) {
        auto symbol = symbolTable_->defineSymbol(
            ctx->id(1)->getText(),
            PROTO_LOCATION(measure),
            SymbolType::kMeasure,
            std::nullopt,
            std::nullopt);

        // Add it to our generated field mapping.
        auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
            Location(
                dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent->parent)),
            SymbolType::kRelation);
        auto parentRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
        parentRelationData->generatedFieldReferences.push_back(symbol);
      }
      return measure;
    }
    case SubstraitPlanParser::FILTER:
      *measure.mutable_filter() = ANY_CAST(
          ::substrait::proto::Expression, visitExpression(ctx->expression()));
      return measure;
    case SubstraitPlanParser::INVOCATION:
      measure.mutable_measure()->set_invocation(
          static_cast<
              ::substrait::proto::AggregateFunction_AggregationInvocation>(
              visitAggregationInvocation(ctx->id(0))));
      return measure;
    case SubstraitPlanParser::SORT:
      *measure.mutable_measure()->add_sorts() = ANY_CAST(
          ::substrait::proto::SortField, visitSort_field(ctx->sort_field()));
      return measure;
    default:
      // Alert that this kind of measure detail is not in the grammar.
      return measure;
  }
}

std::any SubstraitPlanRelationVisitor::visitRelationSourceReference(
    SubstraitPlanParser::RelationSourceReferenceContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);

  if (parentRelationType != RelationType::kRead) {
    errorListener_->addError(
        ctx->getStart(),
        "Source references are not defined for this kind of relation.");
    return defaultResult();
  }

  auto sourceName = ctx->source_reference()->id()->getText();
  auto* symbol = symbolTable_->lookupSymbolByName(sourceName);
  if (symbol == nullptr) {
    return defaultResult();
  }
  switch (ANY_CAST(SourceType, symbol->subtype)) {
    case SourceType::kLocalFiles: {
      auto* source =
          parentRelationData->relation.mutable_read()->mutable_local_files();
      for (const auto& sym : *symbolTable_) {
        if (sym.type != SymbolType::kSourceDetail) {
          continue;
        }
        if (sym.location != symbol->location) {
          continue;
        }
        *source->add_items() = *ANY_CAST(
            std::shared_ptr<::substrait::proto::ReadRel_LocalFiles_FileOrFiles>,
            sym.blob);
      }
      break;
    }
    case SourceType::kNamedTable: {
      auto* source =
          parentRelationData->relation.mutable_read()->mutable_named_table();
      for (const auto& sym :
           symbolTable_->lookupSymbolsByLocation(symbol->location)) {
        if (sym->type != SymbolType::kSourceDetail) {
          continue;
        }
        source->add_names(sym->name);
      }
      break;
    }
    case SourceType::kVirtualTable:
      // TODO -- Implement.
      break;
    case SourceType::kExtensionTable:
      // TODO -- Implement.
      break;
    case SourceType::kUnknown:
      break;
  }

  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationSort(
    SubstraitPlanParser::RelationSortContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  switch (parentRelationType) {
    case RelationType::kSort:
      *parentRelationData->relation.mutable_sort()->add_sorts() = ANY_CAST(
          ::substrait::proto::SortField, visitSort_field(ctx->sort_field()));
      break;
    default:
      errorListener_->addError(
          ctx->getStart(),
          "Sorts are not permitted for this kind of relation.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitRelationCount(
    SubstraitPlanParser::RelationCountContext* ctx) {
  auto* parentSymbol = symbolTable_->lookupSymbolByLocationAndType(
      Location(dynamic_cast<antlr4::ParserRuleContext*>(ctx->parent)),
      SymbolType::kRelation);
  auto parentRelationData =
      ANY_CAST(std::shared_ptr<RelationData>, parentSymbol->blob);
  auto parentRelationType = ANY_CAST(RelationType, parentSymbol->subtype);
  switch (parentRelationType) {
    case RelationType::kFetch: {
      ::substrait::proto::Type type;
      type.mutable_i64()->set_nullability(
          ::substrait::proto::Type_Nullability_NULLABILITY_REQUIRED);
      auto number = visitNumber(ctx->NUMBER(), type);
      parentRelationData->relation.mutable_fetch()->set_count(number.i64());
      break;
    }
    default:
      errorListener_->addError(
          ctx->getStart(), "Count only applies to fetch relations.");
      break;
  }
  return defaultResult();
}

std::any SubstraitPlanRelationVisitor::visitExpression(
    SubstraitPlanParser::ExpressionContext* ctx) {
  if (auto* funcUseCtx =
          dynamic_cast<SubstraitPlanParser::ExpressionFunctionUseContext*>(
              ctx)) {
    return visitExpressionFunctionUse(funcUseCtx);
  } else if (
      auto* constantCtx =
          dynamic_cast<SubstraitPlanParser::ExpressionConstantContext*>(ctx)) {
    return visitExpressionConstant(constantCtx);
  } else if (
      auto* columnCtx =
          dynamic_cast<SubstraitPlanParser::ExpressionColumnContext*>(ctx)) {
    return visitExpressionColumn(columnCtx);
  } else if (
      auto* castCtx =
          dynamic_cast<SubstraitPlanParser::ExpressionCastContext*>(ctx)) {
    return visitExpressionCast(castCtx);
  }
  return defaultResult();
}

::substrait::proto::Expression
SubstraitPlanRelationVisitor::visitExpressionIfThenUse(
    SubstraitPlanParser::ExpressionFunctionUseContext* ctx) {
  ::substrait::proto::Expression expr;
  size_t currExprNum = 0;
  size_t totalExprCount = ctx->expression().size();
  while (currExprNum + 2 <= totalExprCount) {
    // Peel off an if/then pair.
    auto ifThen = expr.mutable_if_then()->add_ifs();
    *ifThen->mutable_if_() = ANY_CAST(
        ::substrait::proto::Expression,
        visitExpression(ctx->expression(currExprNum)));
    *ifThen->mutable_then() = ANY_CAST(
        ::substrait::proto::Expression,
        visitExpression(ctx->expression(currExprNum + 1)));
    currExprNum += 2;
  }
  if (currExprNum + 1 <= totalExprCount) {
    // Use the last expression as the else clause.
    *expr.mutable_if_then()->mutable_else_() = ANY_CAST(
        ::substrait::proto::Expression,
        visitExpression(ctx->expression(currExprNum)));
  }
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitExpressionFunctionUse(
    SubstraitPlanParser::ExpressionFunctionUseContext* ctx) {
  ::substrait::proto::Expression expr;
  std::string funcName = ctx->id()->getText();
  if (absl::AsciiStrToLower(funcName) == "ifthen") {
    return visitExpressionIfThenUse(ctx);
  }
  uint32_t funcReference = 0;
  auto symbol = symbolTable_->lookupSymbolByName(funcName);
  if (symbol == nullptr || symbol->type != SymbolType::kFunction) {
    errorListener_->addError(
        ctx->id()->getStart(),
        ctx->id()->getText() + " is not a function reference.");
  } else {
    auto functionData = ANY_CAST(std::shared_ptr<FunctionData>, symbol->blob);
    funcReference = functionData->anchor;
  }

  expr.mutable_scalar_function()->set_function_reference(funcReference);
  for (const auto& exp : ctx->expression()) {
    if (endsWith(exp->getText(), "_enum")) {
      std::string str{absl::StripSuffix(exp->getText(), "_enum")};
      expr.mutable_scalar_function()->add_arguments()->set_enum_(str);
      continue;
    }

    auto result = visitExpression(exp);
    if (result.type() != typeid(::substrait::proto::Expression)) {
      errorListener_->addError(
          ctx->id()->getStart(), "Could not parse as an expression.");
      return expr;
    }
    auto newExpr = ANY_CAST(::substrait::proto::Expression, result);
    *expr.mutable_scalar_function()->add_arguments()->mutable_value() = newExpr;
  }
  if (ctx->literal_complex_type() != nullptr) {
    auto literalType = ANY_CAST(
        ::substrait::proto::Type,
        visitLiteral_complex_type(ctx->literal_complex_type()));
    *expr.mutable_scalar_function()->mutable_output_type() = literalType;
  }
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitExpressionConstant(
    SubstraitPlanParser::ExpressionConstantContext* ctx) {
  ::substrait::proto::Expression expr;
  *expr.mutable_literal() =
      ANY_CAST(::substrait::proto::Expression_Literal, visitChildren(ctx));
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitExpressionColumn(
    SubstraitPlanParser::ExpressionColumnContext* ctx) {
  auto relationData =
      ANY_CAST(std::shared_ptr<RelationData>, currentRelationScope_->blob);

  std::string symbolName = ctx->getText();
  int32_t fieldReference =
      findFieldReferenceByName(ctx->getStart(), relationData, symbolName);

  ::substrait::proto::Expression expr;
  if (fieldReference != -1) {
    expr.mutable_selection()
        ->mutable_direct_reference()
        ->mutable_struct_field()
        ->set_field(fieldReference);
    // TODO -- Update the following when non-direct references are implemented.
    expr.mutable_selection()->mutable_root_reference();
  }
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitExpressionCast(
    SubstraitPlanParser::ExpressionCastContext* ctx) {
  ::substrait::proto::Expression expr;
  auto origExpression = ANY_CAST(
      ::substrait::proto::Expression, visitExpression(ctx->expression()));
  auto literalType = ANY_CAST(
      ::substrait::proto::Type,
      visitLiteral_complex_type(ctx->literal_complex_type()));
  *expr.mutable_cast()->mutable_type() = literalType;
  *expr.mutable_cast()->mutable_input() = origExpression;
  return expr;
}

std::any SubstraitPlanRelationVisitor::visitConstant(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Expression_Literal literal;
  if (ctx->literal_basic_type() != nullptr) {
    auto literalType = ANY_CAST(
        ::substrait::proto::Type,
        visitLiteral_basic_type(ctx->literal_basic_type()));
    return visitConstantWithType(ctx, literalType);
  } else if (ctx->literal_complex_type() != nullptr) {
    auto literalType = ANY_CAST(
        ::substrait::proto::Type,
        visitLiteral_complex_type(ctx->literal_complex_type()));
    return visitConstantWithType(ctx, literalType);
  } else {
    // Try to figure out this literal without a type.
    if (ctx->NULLVAL() != nullptr) {
      errorListener_->addError(
          ctx->NULLVAL()->getSymbol(), "Null literals require type.");
    } else if (ctx->TRUEVAL() != nullptr) {
      literal.set_boolean(true);
    } else if (ctx->FALSEVAL() != nullptr) {
      literal.set_boolean(false);
    } else if (ctx->STRING() != nullptr) {
      auto literalType = ::substrait::proto::Type();
      literalType.mutable_string();
      literal = visitString(ctx->STRING(), literalType);
    } else {
      errorListener_->addError(
          ctx->getStart(), "Literals should include a type.");
    }
  }
  return literal;
}

std::any SubstraitPlanRelationVisitor::visitMap_literal(
    SubstraitPlanParser::Map_literalContext* ctx) {
  ::substrait::proto::Expression_Literal literal;
  literal.mutable_map()->clear_key_values();
  for (auto pair : ctx->map_literal_value()) {
    auto item = ANY_CAST(
        ::substrait::proto::Expression_Literal_Map_KeyValue,
        visitMap_literal_value(pair));
    *literal.mutable_map()->add_key_values() = item;
  }
  return literal;
}

std::any SubstraitPlanRelationVisitor::visitMap_literal_value(
    SubstraitPlanParser::Map_literal_valueContext* ctx) {
  ::substrait::proto::Expression_Literal_Map_KeyValue keyValue;
  auto key = ANY_CAST(
      ::substrait::proto::Expression_Literal, visitConstant(ctx->constant(0)));
  auto value = ANY_CAST(
      ::substrait::proto::Expression_Literal, visitConstant(ctx->constant(1)));
  *keyValue.mutable_key() = key;
  *keyValue.mutable_value() = value;
  return keyValue;
}

std::any SubstraitPlanRelationVisitor::visitStruct_literal(
    SubstraitPlanParser::Struct_literalContext* ctx) {
  ::substrait::proto::Expression_Literal literal;
  for (auto constant : ctx->constant()) {
    auto item = ANY_CAST(
        ::substrait::proto::Expression_Literal, visitConstant(constant));
    *literal.mutable_struct_()->add_fields() = item;
  }
  return literal;
}

std::any SubstraitPlanRelationVisitor::visitColumn_name(
    SubstraitPlanParser::Column_nameContext* ctx) {
  auto relationData =
      ANY_CAST(std::shared_ptr<RelationData>, currentRelationScope_->blob);
  return findFieldReferenceByName(
      ctx->getStart(), relationData, ctx->getText());
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitConstantWithType(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->NUMBER() != nullptr) {
    literal = visitNumber(ctx->NUMBER(), literalType);
  } else if (ctx->STRING() != nullptr) {
    switch (literalType.kind_case()) {
      case ::substrait::proto::Type::kTimestamp:
        literal = visitTimestamp(ctx);
        break;
      case ::substrait::proto::Type::kTimestampTz:
        literal = visitTimestampTz(ctx);
        break;
      case ::substrait::proto::Type::kDate:
        literal = visitDate(ctx);
        break;
      case ::substrait::proto::Type::kTime:
        literal = visitTime(ctx);
        break;
      default:
        literal = visitString(ctx->STRING(), literalType);
        break;
    }
  } else if (ctx->TRUEVAL() != nullptr) {
    if (literalType.has_bool_()) {
      literal.set_boolean(true);
    } else {
      errorListener_->addError(
          ctx->TRUEVAL()->getSymbol(),
          "Provided value does not match the provided type.");
    }
  } else if (ctx->FALSEVAL() != nullptr) {
    if (literalType.has_bool_()) {
      literal.set_boolean(false);
    } else {
      errorListener_->addError(
          ctx->FALSEVAL()->getSymbol(),
          "Provided value does not match the provided type.");
    }
  } else if (ctx->NULLVAL()) {
    *literal.mutable_null() = literalType;
    setNullable(literal.mutable_null());
  } else {
    switch (literalType.kind_case()) {
      case ::substrait::proto::Type::kStruct:
        literal = visitStruct(ctx, literalType);
        break;
      case ::substrait::proto::Type::kList:
        literal = visitList(ctx, literalType);
        break;
      case ::substrait::proto::Type::kMap:
        literal = visitMap(ctx, literalType);
        break;
      case ::substrait::proto::Type::kIntervalYear:
        literal = visitIntervalYear(ctx);
        break;
      case ::substrait::proto::Type::kIntervalDay:
        literal = visitIntervalDay(ctx);
        break;
      default:
        errorListener_->addError(
            ctx->getStart(),
            "Unsupported type " + std::to_string(literalType.kind_case()) +
                ".");
        break;
    }
  }

  return literal;
}

::substrait::proto::Expression_Literal_Map_KeyValue
SubstraitPlanRelationVisitor::visitMapLiteralValueWithType(
    SubstraitPlanParser::Map_literal_valueContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal_Map_KeyValue keyValue;
  *keyValue.mutable_key() =
      visitConstantWithType(ctx->constant(0), literalType.map().key());
  *keyValue.mutable_value() =
      visitConstantWithType(ctx->constant(1), literalType.map().value());
  return keyValue;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitMapLiteralWithType(
    SubstraitPlanParser::Map_literalContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;
  if (ctx->map_literal_value().empty()) {
    *literal.mutable_empty_map() = literalType.map();
    return literal;
  }
  for (const auto& keyValue : ctx->map_literal_value()) {
    *literal.mutable_map()->add_key_values() =
        visitMapLiteralValueWithType(keyValue, literalType);
  }
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitStructLiteralWithType(
    SubstraitPlanParser::Struct_literalContext* ctx,
    const ::substrait::proto::Type_Struct& literalType) {
  ::substrait::proto::Expression_Literal literal;
  if (ctx->constant().empty()) {
    return literal;
  }
  for (int i = 0; i < ctx->constant().size(); ++i) {
    if (i >= literalType.types().size()) {
      break;
    }
    *literal.mutable_struct_()->add_fields() =
        visitConstantWithType(ctx->constant(i), literalType.types(i));
  }
  if (ctx->constant().size() != literalType.types().size()) {
    errorListener_->addError(
        ctx->getStart(),
        "The number of fields does not match the number of types in this "
        "struct.");
  }
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitString(
    antlr4::tree::TerminalNode* node,
    const ::substrait::proto::Type& literalType) {
  std::string input = node->getText();

  // First remove the surrounding quote marks.
  std::string str;
  if (startsWith(input, "```")) {
    str = input.substr(3, input.length() - 6);
  } else if (startsWith(input, "``")) {
    str = input.substr(2, input.length() - 4);
  } else if (input[0] == '"' || input[0] == '`') {
    str = input.substr(1, input.length() - 2);
  } else {
    str = input;
  }

  // Perform escapes if necessary.
  std::string resultStr;
  if (startsWith(input, "`")) {
    // Don't perform escapes on raw strings.
    resultStr = str;
  } else {
    resultStr = escapeText(node, str);
  }

  ::substrait::proto::Expression_Literal literal;
  switch (literalType.kind_case()) {
    case ::substrait::proto::Type::kString: {
      literal.set_string(resultStr);
      break;
    }
    case ::substrait::proto::Type::kFixedChar: {
      literal.set_fixed_char(resultStr);
      break;
    }
    case ::substrait::proto::Type::kVarchar: {
      literal.mutable_var_char()->set_value(resultStr);
      literal.mutable_var_char()->set_length(literalType.varchar().length());
      break;
    }
    case ::substrait::proto::Type::kBinary: {
      literal.set_binary(resultStr);
      break;
    }
    case ::substrait::proto::Type::kFixedBinary: {
      literal.set_fixed_binary(resultStr);
      break;
    }
    case ::substrait::proto::Type::kUuid: {
      // Remove all dashes for consistency.
      std::string modifiedStr = resultStr;
      modifiedStr.erase(
          std::remove_if(
              modifiedStr.begin(),
              modifiedStr.end(),
              [](unsigned char c) -> bool { return c == '-'; }),
          modifiedStr.end());
      if (std::find_if(
              modifiedStr.begin(), modifiedStr.end(), [](unsigned char c) {
                return !std::isxdigit(c);
              }) != modifiedStr.end()) {
        errorListener_->addError(
            node->getSymbol(),
            "UUIDs should be be specified with hexadecimal characters with "
            "optional dashes only.");
        modifiedStr = "0";
      } else if (modifiedStr.length() != 32) {
        errorListener_->addError(
            node->getSymbol(),
            "UUIDs are 128 bits long and thus should be specified with exactly "
            "32 hexadecimal digits.");
        modifiedStr = modifiedStr.substr(0, 32);
      }
      literal.set_uuid(modifiedStr);
      break;
    }
    case ::substrait::proto::Type::kTimestampTz:
    case ::substrait::proto::Type::kTimestamp:
    case ::substrait::proto::Type::kDate:
    case ::substrait::proto::Type::kTime:
      literal.set_string(resultStr);
      break;
    case ::substrait::proto::Type::KIND_NOT_SET:
      // Use of an unrecognized symbol is handled more generally elsewhere.
      break;
    default:
      errorListener_->addError(
          node->getSymbol(),
          "Unexpected string type: " + std::to_string(literalType.kind_case()));
      break;
  }

  return literal;
}

std::string SubstraitPlanRelationVisitor::escapeText(
    const antlr4::tree::TerminalNode* node,
    const std::string& str) {
  std::stringstream result;

  for (std::size_t i = 0; i < str.length(); ++i) {
    if (str[i] == '\\' && i < str.length() - 1) {
      switch (str[i + 1]) {
        case '\\':
          result << '\\';
          break;
        case '\'':
          result << '\'';
          break;
        case '"':
          result << '"';
          break;
        case 'b':
          result << '\b';
          break;
        case 'f':
          result << '\f';
          break;
        case 'n':
          result << '\n';
          break;
        case 'r':
          result << '\r';
          break;
        case 't':
          result << '\t';
          break;
        case 'x':
          if (i < str.length() - 3) {
            int32_t hex{0};
            if (absl::SimpleHexAtoi(str.substr(i + 2, 2), &hex)) {
              result << static_cast<char>(hex & 0xff);
              i += 3;
              continue;
            }
            // This is an invalid sequence - leave escaped.
          }
          // This is an unrecognized sequence - leave escaped.
          result << str[i];
          result << str[i + 1];
          break;
        case 'u':
          if (i < str.length() - 3) {
            if (str[i + 2] != '{') {
              break;
            }
            auto substr = str.substr(i + 3);
            size_t x = substr.find('}');
            int64_t hex;
            if (x > 0 && absl::SimpleHexAtoi(substr.substr(0, x), &hex)) {
              if (x <= 2) {
                result << static_cast<char>(hex & 0xff);
              } else if (x <= 4) {
                result << static_cast<char>(hex >> 8 & 0xff);
                result << static_cast<char>(hex & 0xff);
              } else if (x <= 6) {
                result << static_cast<char>(hex >> 16 & 0xff);
                result << static_cast<char>(hex >> 8 & 0xff);
                result << static_cast<char>(hex & 0xff);
              }
              i += x + 3;
              continue;
            }
            // Drop through to handle the invalid sequence.
          }
        default:
          this->errorListener_->SubstraitErrorListener::addError(
              node->getSymbol()->getLine(),
              node->getSymbol()->getCharPositionInLine() + i + 1,
              "Unknown slash escape sequence.");
          result << str[i];
          result << str[i + 1];
          break;
      }
      ++i; // Increments for the character after the slash.
    } else {
      result << str[i];
    }
  }

  return result.str();
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitNumber(
    antlr4::tree::TerminalNode* node,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;
  try {
    switch (literalType.kind_case()) {
      case ::substrait::proto::Type::kI8: {
        int32_t val = std::stoi(node->getText());
        literal.set_i8(val);
        if (literalType.i8().nullability() ==
            ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kI16: {
        int32_t val = std::stoi(node->getText());
        literal.set_i16(val);
        if (literalType.i16().nullability() ==
            ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kI32: {
        int32_t val = std::stoi(node->getText());
        literal.set_i32(val);
        if (literalType.i32().nullability() ==
            ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kI64: {
        int64_t val = std::stol(node->getText());
        literal.set_i64(val);
        if (literalType.i64().nullability() ==
            ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kFp32: {
        float val = std::stof(node->getText());
        literal.set_fp32(val);
        if (literalType.fp32().nullability() ==
            ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kFp64: {
        double val = std::stod(node->getText());
        literal.set_fp64(val);
        if (literalType.fp64().nullability() ==
            ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE) {
          literal.set_nullable(true);
        }
        break;
      }
      case ::substrait::proto::Type::kDecimal: {
        auto decimal = ::io::substrait::expression::DecimalLiteral::fromString(
            node->getText(),
            literalType.decimal().precision(),
            literalType.decimal().scale());
        if (!decimal.isValid()) {
          errorListener_->addError(
              node->getSymbol(), "Could not parse literal as decimal.");
          break;
        }
        *literal.mutable_decimal() = decimal.toProto();
        if (literalType.decimal().nullability() ==
            ::substrait::proto::Type_Nullability_NULLABILITY_NULLABLE) {
          literal.set_nullable(true);
        }
        break;
      }
      default:
        break;
    }
  } catch (...) {
    errorListener_->addError(
        node->getSymbol(), "Could not parse as a numeric literal.");
  }

  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitList(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->map_literal() != nullptr) {
    auto mapLiteral = ANY_CAST(
        ::substrait::proto::Expression_Literal,
        visitMap_literal(ctx->map_literal()));
    if (!mapLiteral.map().key_values().empty()) {
      errorListener_->addError(
          ctx->getStart(), "Lists only accept literals separated by commas.");
    }
    *literal.mutable_empty_list()->mutable_type() = literalType.list().type();
    literal.mutable_empty_list()->set_nullability(
        literalType.list().nullability());
  } else if (ctx->struct_literal() != nullptr) {
    for (const auto& constant : ctx->struct_literal()->constant()) {
      *literal.mutable_list()->add_values() =
          visitConstantWithType(constant, literalType.list().type());
    }
  }

  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitMap(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->map_literal() != nullptr) {
    literal = ANY_CAST(
        ::substrait::proto::Expression_Literal,
        visitMapLiteralWithType(ctx->map_literal(), literalType));
  } else if (ctx->struct_literal() != nullptr) {
    errorListener_->addError(
        ctx->getStart(),
        "Map literals require pairs of values separated by colons.");

    *literal.mutable_empty_map() = literalType.map();
  } else {
    errorListener_->addError(ctx->getStart(), "Unrecognized map construction.");
    *literal.mutable_empty_map() = literalType.map();
  }

  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitStruct(
    SubstraitPlanParser::ConstantContext* ctx,
    const ::substrait::proto::Type& literalType) {
  ::substrait::proto::Expression_Literal literal;

  if (ctx->map_literal() != nullptr) {
    // We know that there this is an empty literal, treat as such.
    literal.mutable_struct_()->clear_fields();
  } else if (ctx->struct_literal() != nullptr) {
    literal = visitStructLiteralWithType(
        ctx->struct_literal(), literalType.struct_());
    // Validate that the literal matches the type.
  }

  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitIntervalYear(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_struct_()->add_types()->mutable_i32();
  literalType.mutable_struct_()->add_types()->mutable_i32();

  auto s = visitStruct(ctx, literalType);

  ::substrait::proto::Expression_Literal literal;
  literal.mutable_interval_year_to_month()->set_years(
      s.struct_().fields(0).i32());
  literal.mutable_interval_year_to_month()->set_months(
      s.struct_().fields(1).i32());
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitIntervalDay(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_struct_()->add_types()->mutable_i32();
  literalType.mutable_struct_()->add_types()->mutable_i32();
  literalType.mutable_struct_()->add_types()->mutable_i32();

  auto s = visitStruct(ctx, literalType);

  ::substrait::proto::Expression_Literal literal;
  literal.mutable_interval_day_to_second()->set_days(
      s.struct_().fields(0).i32());
  literal.mutable_interval_day_to_second()->set_seconds(
      s.struct_().fields(1).i32());
  literal.mutable_interval_day_to_second()->set_microseconds(
      s.struct_().fields(2).i32());
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitTimestamp(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_timestamp();

  auto s = visitString(ctx->STRING(), literalType);

  date::sys_seconds timePoint;
  std::istringstream ss(s.string());
  ss >> date::parse("%Y-%m-%d %H:%M:%S", timePoint);
  if (ss.fail()) {
    errorListener_->addError(ctx->getStart(), "Could not parse timestamp.");
    return {};
  }

  ::substrait::proto::Expression_Literal literal;
  literal.set_timestamp(std::chrono::system_clock::to_time_t(timePoint));
  return literal;
}

::substrait::proto::Expression_Literal
SubstraitPlanRelationVisitor::visitTimestampTz(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_timestamp_tz();

  auto s = visitString(ctx->STRING(), literalType);

  date::local_time<std::chrono::microseconds> localTimePoint;
  std::string tzName;
  std::istringstream ss{s.string()};
  ss >> date::parse("%Y-%m-%d %H:%M:%S %Z", localTimePoint, tzName);
  if (ss.fail()) {
    errorListener_->addError(
        ctx->getStart(), "Could not parse timestamp with timezone.");
    return {};
  }
  // TODO -- Support timezones like -0100 using %z.  #60

  ::substrait::proto::Expression_Literal literal;
  try {
    auto timePoint = date::locate_zone(tzName)->to_sys(localTimePoint);
    literal.set_timestamp_tz(std::chrono::system_clock::to_time_t(timePoint));
  } catch (...) {
    errorListener_->addError(
        ctx->getStart(), "Failed to locate timezone in the TZ database.");
  }
  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitDate(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_date();

  auto s = visitString(ctx->STRING(), literalType);

  date::sys_days timePoint;
  std::istringstream ss(s.string());
  ss >> date::parse("%Y-%m-%d", timePoint);
  if (ss.fail()) {
    errorListener_->addError(ctx->getStart(), "Could not parse date.");
    return {};
  }

  ::substrait::proto::Expression_Literal literal;
  auto duration = timePoint - date::sys_days();
  literal.set_date(duration.count());
  return literal;
}

::substrait::proto::Expression_Literal SubstraitPlanRelationVisitor::visitTime(
    SubstraitPlanParser::ConstantContext* ctx) {
  ::substrait::proto::Type literalType;
  literalType.mutable_time();

  auto s = visitString(ctx->STRING(), literalType);

  std::chrono::microseconds timeOfDay;
  std::istringstream ss(s.string());
  ss >> date::parse("%H:%M:%S", timeOfDay);
  if (ss.fail()) {
    ss = std::istringstream(s.string());
    ss >> date::parse("%H:%M", timeOfDay);
    if (ss.fail()) {
      errorListener_->addError(ctx->getStart(), "Could not parse time.");
      return {};
    }
  }

  ::substrait::proto::Expression_Literal literal;
  literal.set_time(timeOfDay.count());
  return literal;
}

std::any SubstraitPlanRelationVisitor::visitSort_field(
    SubstraitPlanParser::Sort_fieldContext* ctx) {
  ::substrait::proto::SortField sort;
  *sort.mutable_expr() = ANY_CAST(
      ::substrait::proto::Expression, visitExpression(ctx->expression()));
  if (ctx->id() != nullptr) {
    sort.set_direction(static_cast<::substrait::proto::SortField_SortDirection>(
        visitSortDirection(ctx->id())));
  }
  return sort;
}

int32_t SubstraitPlanRelationVisitor::visitSortDirection(
    SubstraitPlanParser::IdContext* ctx) {
  std::string text = normalizeProtoEnum(ctx->getText(), kSortDirectionPrefix);
  if (text == "unspecified") {
    return ::substrait::proto::SortField::SORT_DIRECTION_UNSPECIFIED;
  } else if (text == "ascnullsfirst") {
    return ::substrait::proto::SortField::SORT_DIRECTION_ASC_NULLS_FIRST;
  } else if (text == "ascnullslast") {
    return ::substrait::proto::SortField::SORT_DIRECTION_ASC_NULLS_LAST;
  } else if (text == "descnullsfirst") {
    return ::substrait::proto::SortField::SORT_DIRECTION_DESC_NULLS_FIRST;
  } else if (text == "descnullslast") {
    return ::substrait::proto::SortField::SORT_DIRECTION_DESC_NULLS_LAST;
  } else if (text == "clustered") {
    return ::substrait::proto::SortField::SORT_DIRECTION_CLUSTERED;
  }
  this->errorListener_->addError(
      ctx->getStart(), "Unrecognized sort direction: " + ctx->getText());
  return ::substrait::proto::SortField::SORT_DIRECTION_UNSPECIFIED;
}

void SubstraitPlanRelationVisitor::addExpressionsToSchema(
    std::shared_ptr<RelationData>& relationData) {
  const auto& relation = relationData->relation;
  switch (relation.rel_type_case()) {
    case ::substrait::proto::Rel::kProject:
      for (const auto& expr : relation.project().expressions()) {
        if (expr.selection().direct_reference().has_struct_field()) {
          if (expr.selection().direct_reference().struct_field().field() <
              relationData->fieldReferences.size()) {
            relationData->generatedFieldReferences.push_back(
                relationData->fieldReferences[expr.selection()
                                                  .direct_reference()
                                                  .struct_field()
                                                  .field()]);
          }
        } else {
          const auto& uniqueName =
              symbolTable_->getUniqueName(kIntermediateNodeName);
          auto newSymbol = symbolTable_->defineSymbol(
              uniqueName,
              PROTO_LOCATION(expr),
              SymbolType::kUnknown,
              std::nullopt,
              std::nullopt);
          relationData->generatedFieldReferences.push_back(newSymbol);
        }
      }
      break;
    default:
      // Only project and aggregate relations affect the output mapping.
      break;
  }
}

std::string SubstraitPlanRelationVisitor::fullyQualifiedReference(
    const SymbolInfo* fieldReference) {
  for (const auto& symbol : symbolTable_->getSymbols()) {
    if (symbol->type == SymbolType::kSchema &&
        symbol->location == fieldReference->location) {
      auto fqn = symbol->name + "." + fieldReference->name;
      return fqn;
    }
  }
  // Shouldn't happen, but return no schema if we can't find one.
  return fieldReference->name;
}

int SubstraitPlanRelationVisitor::findFieldReferenceByName(
    antlr4::Token* token,
    std::shared_ptr<RelationData>& relationData,
    const std::string& name) {
  auto fieldReferencesSize = relationData->fieldReferences.size();

  auto generatedField = std::find_if(
      relationData->generatedFieldReferences.rbegin(),
      relationData->generatedFieldReferences.rend(),
      [&](auto ref) {
        return (!ref->alias.empty() && ref->alias == name || ref->name == name);
      });
  if (generatedField != relationData->generatedFieldReferences.rend()) {
    auto fieldPlacement =
        generatedField - relationData->generatedFieldReferences.rbegin();
    return static_cast<int32_t>(
        (fieldReferencesSize + relationData->generatedFieldReferences.size() -
         fieldPlacement - 1) &
        std::numeric_limits<int32_t>::max());
  }

  auto field = std::find_if(
      relationData->fieldReferences.rbegin(),
      relationData->fieldReferences.rend(),
      [&](auto ref) {
        return (
            !ref->alias.empty() && ref->alias == name || ref->name == name ||
            fullyQualifiedReference(ref) == name);
      });

  if (field != relationData->fieldReferences.rend()) {
    auto fieldPlacement = field - relationData->fieldReferences.rbegin();
    return static_cast<int32_t>(
        (fieldReferencesSize - fieldPlacement - 1) &
        std::numeric_limits<int32_t>::max());
  }

  errorListener_->addError(token, "Reference " + name + " does not exist.");
  return -1;
}

void SubstraitPlanRelationVisitor::applyOutputMappingToSchema(
    antlr4::Token* token,
    RelationType relationType,
    std::shared_ptr<RelationData>& relationData) {
  auto common = findCommonRelation(relationType, &relationData->relation);
  if (common == nullptr) {
    return;
  }
  if (common->emit().output_mapping_size() == 0) {
    common->mutable_direct();
  } else {
    if (!relationData->outputFieldReferences.empty()) {
      // TODO -- Add support for aggregate relations.
      errorListener_->addError(
          token, "Aggregate relations do not yet support emit sections.");
      return;
    }
    for (auto mapping : common->emit().output_mapping()) {
      auto fieldReferencesSize = relationData->fieldReferences.size();
      if (mapping < fieldReferencesSize) {
        relationData->outputFieldReferences.push_back(
            relationData->fieldReferences[mapping]);
      } else if (
          mapping <
          fieldReferencesSize + relationData->generatedFieldReferences.size()) {
        relationData->outputFieldReferences.push_back(
            relationData
                ->generatedFieldReferences[mapping - fieldReferencesSize]);
      } else {
        errorListener_->addError(
            token,
            "Field #" + std::to_string(mapping) + " requested but only " +
                std::to_string(
                    fieldReferencesSize +
                    relationData->generatedFieldReferences.size()) +
                " are available.");
      }
    }
  }
}

} // namespace io::substrait::textplan
