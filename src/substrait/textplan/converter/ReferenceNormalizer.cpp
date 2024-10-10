/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/ReferenceNormalizer.h"

#include <string>

#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"

namespace io::substrait::textplan {

namespace {

bool compareExtensionFunctions(
    const ::substrait::proto::extensions::SimpleExtensionDeclaration& a,
    const ::substrait::proto::extensions::SimpleExtensionDeclaration& b) {
  auto ord = [](const auto& decl) {
    return make_tuple(
        // First sort so that extension functions precede any other kind of
        // extension.
        !decl.has_extension_function(),
        // Next sort by space.
        decl.extension_function().extension_uri_reference(),
        // Finally sort by name within a space.
        std::string_view{decl.extension_function().name()});
  };

  // Now let the default tuple compare do the rest of the work.
  return ord(a) < ord(b);
}

// Forward references
void normalizeFunctionsForExpression(
    ::substrait::proto::Expression* expr,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping);

void normalizeFunctionsForRelation(
    ::substrait::proto::Rel* relation,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping);

void normalizeFunctionsForArgument(
    ::substrait::proto::FunctionArgument& argument,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping) {
  if (argument.has_value()) {
    normalizeFunctionsForExpression(
        argument.mutable_value(), functionReferenceMapping);
  }
}

void normalizeFunctionsForMeasure(
    ::substrait::proto::AggregateRel_Measure& measure,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping) {
  measure.mutable_measure()->set_function_reference(
      functionReferenceMapping.at(measure.measure().function_reference()));
}

void normalizeFunctionsForExpression(
    ::substrait::proto::Expression* expr,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping) {
  if (expr->has_scalar_function()) {
    expr->mutable_scalar_function()->set_function_reference(
        functionReferenceMapping.at(
            expr->scalar_function().function_reference()));
    for (auto& arg : *expr->mutable_scalar_function()->mutable_arguments()) {
      normalizeFunctionsForArgument(arg, functionReferenceMapping);
    }
  } else if (expr->has_cast()) {
    normalizeFunctionsForExpression(
        expr->mutable_cast()->mutable_input(), functionReferenceMapping);
  } else if (expr->has_if_then()) {
    for (auto& ifthen : *expr->mutable_if_then()->mutable_ifs()) {
      normalizeFunctionsForExpression(
          ifthen.mutable_if_(), functionReferenceMapping);
      normalizeFunctionsForExpression(
          ifthen.mutable_then(), functionReferenceMapping);
    }
    if (expr->if_then().has_else_()) {
      normalizeFunctionsForExpression(
          expr->mutable_if_then()->mutable_else_(), functionReferenceMapping);
    }
  } else if (expr->has_subquery()) {
    switch (expr->subquery().subquery_type_case()) {
      case ::substrait::proto::Expression_Subquery::kScalar:
        normalizeFunctionsForRelation(
            expr->mutable_subquery()->mutable_scalar()->mutable_input(),
            functionReferenceMapping);
        break;
      case ::substrait::proto::Expression_Subquery::kInPredicate:
        for (auto& needle : *expr->mutable_subquery()
                                 ->mutable_in_predicate()
                                 ->mutable_needles()) {
          normalizeFunctionsForExpression(&needle, functionReferenceMapping);
        }
        normalizeFunctionsForRelation(
            expr->mutable_subquery()
                ->mutable_in_predicate()
                ->mutable_haystack(),
            functionReferenceMapping);
        break;
      case ::substrait::proto::Expression_Subquery::kSetPredicate:
        normalizeFunctionsForRelation(
            expr->mutable_subquery()->mutable_set_predicate()->mutable_tuples(),
            functionReferenceMapping);
        break;
      case ::substrait::proto::Expression_Subquery::kSetComparison:
        normalizeFunctionsForExpression(
            expr->mutable_subquery()->mutable_set_comparison()->mutable_left(),
            functionReferenceMapping);
        normalizeFunctionsForRelation(
            expr->mutable_subquery()->mutable_set_comparison()->mutable_right(),
            functionReferenceMapping);
        break;
      case ::substrait::proto::Expression_Subquery::SUBQUERY_TYPE_NOT_SET:
        break;
    }
  }
}

void normalizeFunctionsForRelation(
    ::substrait::proto::Rel* relation,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping) {
  if (relation->has_read()) {
    if (relation->read().has_filter()) {
      normalizeFunctionsForExpression(
          relation->mutable_read()->mutable_filter(), functionReferenceMapping);
    }
    if (relation->read().has_best_effort_filter()) {
      normalizeFunctionsForExpression(
          relation->mutable_read()->mutable_best_effort_filter(),
          functionReferenceMapping);
    }
  } else if (relation->has_filter()) {
    normalizeFunctionsForRelation(
        relation->mutable_filter()->mutable_input(), functionReferenceMapping);
    if (relation->filter().has_condition()) {
      normalizeFunctionsForExpression(
          relation->mutable_filter()->mutable_condition(),
          functionReferenceMapping);
    }
  } else if (relation->has_fetch()) {
    normalizeFunctionsForRelation(
        relation->mutable_fetch()->mutable_input(), functionReferenceMapping);
  } else if (relation->has_aggregate()) {
    normalizeFunctionsForRelation(
        relation->mutable_aggregate()->mutable_input(),
        functionReferenceMapping);
    for (auto& measure : *relation->mutable_aggregate()->mutable_measures()) {
      normalizeFunctionsForMeasure(measure, functionReferenceMapping);
    }
  } else if (relation->has_sort()) {
    normalizeFunctionsForRelation(
        relation->mutable_sort()->mutable_input(), functionReferenceMapping);
    for (auto& sort : *relation->mutable_sort()->mutable_sorts()) {
      normalizeFunctionsForExpression(
          sort.mutable_expr(), functionReferenceMapping);
    }
  } else if (relation->has_join()) {
    if (relation->join().has_expression()) {
      normalizeFunctionsForExpression(
          relation->mutable_join()->mutable_expression(),
          functionReferenceMapping);
    }
    if (relation->join().has_post_join_filter()) {
      normalizeFunctionsForExpression(
          relation->mutable_join()->mutable_post_join_filter(),
          functionReferenceMapping);
    }
    normalizeFunctionsForRelation(
        relation->mutable_join()->mutable_left(), functionReferenceMapping);
    normalizeFunctionsForRelation(
        relation->mutable_join()->mutable_right(), functionReferenceMapping);
  } else if (relation->has_project()) {
    normalizeFunctionsForRelation(
        relation->mutable_project()->mutable_input(), functionReferenceMapping);
    for (auto& expr : *relation->mutable_project()->mutable_expressions()) {
      normalizeFunctionsForExpression(&expr, functionReferenceMapping);
    }
  } else if (relation->has_set()) {
    for (auto& input : *relation->mutable_set()->mutable_inputs()) {
      normalizeFunctionsForRelation(&input, functionReferenceMapping);
    }
  } else if (relation->has_extension_single()) {
    if (relation->extension_single().has_input()) {
      normalizeFunctionsForRelation(
          relation->mutable_extension_single()->mutable_input(),
          functionReferenceMapping);
    }
  } else if (relation->has_extension_multi()) {
    for (auto& input : *relation->mutable_extension_multi()->mutable_inputs()) {
      normalizeFunctionsForRelation(&input, functionReferenceMapping);
    }
  } else if (relation->has_extension_leaf()) {
    // Nothing to do here.
  } else if (relation->has_cross()) {
    if (relation->cross().has_left()) {
      normalizeFunctionsForRelation(
          relation->mutable_cross()->mutable_left(), functionReferenceMapping);
    }
    if (relation->cross().has_right()) {
      normalizeFunctionsForRelation(
          relation->mutable_cross()->mutable_right(), functionReferenceMapping);
    }
  } else if (relation->has_hash_join()) {
    if (relation->hash_join().has_left()) {
      normalizeFunctionsForRelation(
          relation->mutable_hash_join()->mutable_left(),
          functionReferenceMapping);
    }
    if (relation->hash_join().has_right()) {
      normalizeFunctionsForRelation(
          relation->mutable_hash_join()->mutable_right(),
          functionReferenceMapping);
    }
    if (relation->hash_join().has_post_join_filter()) {
      normalizeFunctionsForExpression(
          relation->mutable_hash_join()->mutable_post_join_filter(),
          functionReferenceMapping);
    }
  } else if (relation->has_merge_join()) {
    if (relation->merge_join().has_left()) {
      normalizeFunctionsForRelation(
          relation->mutable_merge_join()->mutable_left(),
          functionReferenceMapping);
    }
    if (relation->merge_join().has_right()) {
      normalizeFunctionsForRelation(
          relation->mutable_merge_join()->mutable_right(),
          functionReferenceMapping);
    }
    if (relation->merge_join().has_post_join_filter()) {
      normalizeFunctionsForExpression(
          relation->mutable_merge_join()->mutable_post_join_filter(),
          functionReferenceMapping);
    }
  }
}

void normalizeFunctionsForRootRelation(
    ::substrait::proto::RelRoot* relation,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping) {
  if (relation->has_input()) {
    normalizeFunctionsForRelation(
        relation->mutable_input(), functionReferenceMapping);
  }
}

void normalizeFunctionsForPlanRelation(
    ::substrait::proto::PlanRel& relation,
    const std::map<uint32_t, uint32_t>& functionReferenceMapping) {
  if (relation.has_root()) {
    normalizeFunctionsForRootRelation(
        relation.mutable_root(), functionReferenceMapping);
  }
  if (relation.has_rel()) {
    normalizeFunctionsForRelation(
        relation.mutable_rel(), functionReferenceMapping);
  }
}

} // namespace

void ReferenceNormalizer::normalizeSpaces(::substrait::proto::Plan* plan) {
  std::map<uint32_t, uint32_t> extensionSpaceReferenceMapping;

  // Reorder the extension spaces and remember what we changed.
  std::sort(
      plan->mutable_extension_uris()->begin(),
      plan->mutable_extension_uris()->end(),
      [](const ::substrait::proto::extensions::SimpleExtensionURI& a,
         const ::substrait::proto::extensions::SimpleExtensionURI& b) {
        return a.uri() < b.uri();
      });

  // Now renumber the spaces.
  uint32_t uriNum = 0;
  for (auto& extensionUri : *plan->mutable_extension_uris()) {
    extensionSpaceReferenceMapping[extensionUri.extension_uri_anchor()] =
        ++uriNum;
    extensionUri.set_extension_uri_anchor(uriNum);
  }

  // Apply the space numbering changes to the functions.
  for (auto& function : *plan->mutable_extensions()) {
    if (function.has_extension_function()) {
      auto newSpace = extensionSpaceReferenceMapping.find(
          function.extension_function().extension_uri_reference());
      if (newSpace != extensionSpaceReferenceMapping.end()) {
        function.mutable_extension_function()->set_extension_uri_reference(
            newSpace->second);
      }
    }
  }
}

void ReferenceNormalizer::normalizeFunctions(::substrait::proto::Plan* plan) {
  std::map<uint32_t, uint32_t> functionReferenceMapping;

  // First sort the functions alphabetically by space.
  std::sort(
      plan->mutable_extensions()->begin(),
      plan->mutable_extensions()->end(),
      compareExtensionFunctions);

  // Now renumber the functions starting with zero.
  uint32_t functionNum = 0;
  for (auto& function : *plan->mutable_extensions()) {
    functionReferenceMapping[function.extension_function().function_anchor()] =
        functionNum;
    function.mutable_extension_function()->set_function_anchor(functionNum);
    functionNum++;
  }

  // Now apply that reordering to the rest of the protobuf.
  for (auto& relation : *plan->mutable_relations()) {
    normalizeFunctionsForPlanRelation(relation, functionReferenceMapping);
  }
}

void ReferenceNormalizer::normalize(::substrait::proto::Plan* plan) {
  normalizeSpaces(plan);
  normalizeFunctions(plan);
}

} // namespace io::substrait::textplan
