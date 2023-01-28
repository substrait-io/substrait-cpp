/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <google/protobuf/util/json_util.h>

// MEGAHACK -- Should these not be referenced from the root?
#include "PlanConverter.h"
#include "SymbolTable.h"

#include "substrait/algebra.pb.h"
#include "substrait/common/Exceptions.h"
#include "substrait/plan.pb.h"

namespace substrait {
namespace {

std::string shortName(std::string str) {
  auto loc = str.find(':');
  if (loc != std::string::npos) {
    return str.substr(0, loc);
  }
  return str;
}

} // namespace

std::string readFromFile(const std::string& msgPath) {
  // Read json file and resume the Substrait plan.
  std::ifstream msgJson(msgPath);
  if (msgJson.fail()) {
    auto currdir = std::filesystem::current_path().string();
    SUBSTRAIT_FAIL(
        "Failed to open file {} when running in {}: {}",
        msgPath,
        currdir,
        strerror(errno));
  }
  std::stringstream buffer;
  buffer << msgJson.rdbuf();
  return buffer.str();
}

void PlanConverter::loadFromJSON(const std::string& json) {
  auto status = google::protobuf::util::JsonStringToMessage(json, &plan_);
  if (!status.ok()) {
    SUBSTRAIT_FAIL(
        "Failed to parse Substrait JSON: {}", status.message().ToString());
  }
}

void PlanConverter::loadFromText(const std::string& text) {
  SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
}

void PlanConverter::loadFromBinary(const std::string& blob) {
  SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
}

std::string PlanConverter::functionsToText(const substrait::Plan& plan) {
  std::string text;
  // MEGAHACK -- Deal with grouping extensions by extension space
  // (extension_uri_reference).

  // MEGAHACK -- Deal with the fact that there may be no extension space provided.
  if (plan.extensions_size() == 0) {
    return text;
  }
  text += "extension_space {\n";
  auto extensions = plan.extensions();
  std::sort(
      extensions.begin(),
      extensions.end(),
      [](const substrait::extensions::SimpleExtensionDeclaration& a,
         const substrait::extensions::SimpleExtensionDeclaration& b) {
        return a.extension_function().function_anchor() <
            b.extension_function().function_anchor();
      });
  for (const auto& ext : extensions) {
    const auto& unique_name = symbol_table_.getUniqueName(shortName(ext.extension_function().name()));
    text += "  function " + ext.extension_function().name() + " as " +
        shortName(unique_name) + ";\n";
    symbol_table_.defineSymbol(
        unique_name,
        Location(),
        SymbolType::kFunction,
        substrait::Rel::RelTypeCase::REL_TYPE_NOT_SET,
        ext);
  }
  text += "}\n";
  return text;
}

namespace {
std::string extractLocalFile(
    const substrait::ReadRel::LocalFiles::FileOrFiles& item) {
  std::string text;
  switch (item.path_type_case()) {
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriPath:
      text += "uri_path: \"" + item.uri_path() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriPathGlob:
      text += "uri_path_glob: \"" + item.uri_path_glob() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriFile:
      text += "uri_file: \"" + item.uri_file() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kUriFolder:
      text += "uri_folder: \"" + item.uri_folder() + "\"";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::PATH_TYPE_NOT_SET:
    default:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
      return "";
  }
  if (item.partition_index() != 0) {
    text += " partition_index: " + std::to_string(item.partition_index());
  }
  if (item.start() != 0) {
    text += " start: " + std::to_string(item.start());
  }
  if (item.length() != 0) {
    text += " length: " + std::to_string(item.length());
  }
  switch (item.file_format_case()) {
    case substrait::ReadRel::LocalFiles::FileOrFiles::kParquet:
      text += " parquest: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kArrow:
      text += " arrow: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kOrc:
      text += " orc: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::kExtension:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
      return "";
    case substrait::ReadRel::LocalFiles::FileOrFiles::kDwrf:
      text += " dwrf: {}";
      break;
    case substrait::ReadRel::LocalFiles::FileOrFiles::FILE_FORMAT_NOT_SET:
    default:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
      return "";
  }
  return text;
}

std::string extractReadType(const substrait::ReadRel& relation) {
  switch (relation.read_type_case()) {
    case substrait::ReadRel::kLocalFiles: {
      std::string text = "source local_files {\n";
      text += "  items = [\n";
      for (const auto& item : relation.local_files().items()) {
        text += "    {" + extractLocalFile(item) + "}\n";
      }
      text += "  ]\n";
      text += "}\n";
      // MEGAHACK -- Add support for advanced extensions.
      return text;
    }
    case substrait::ReadRel::kVirtualTable:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
    case substrait::ReadRel::kNamedTable:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
    case substrait::ReadRel::kExtensionTable:
      SUBSTRAIT_UNSUPPORTED("MEGAHACK -- Not yet implemented.");
    case substrait::ReadRel::READ_TYPE_NOT_SET:
    default:
      return "";
  }
}

std::string extractSources(const substrait::Rel& relation) {
  switch (relation.rel_type_case()) {
    case substrait::Rel::RelTypeCase::kRead:
      return extractReadType(relation.read());
    case substrait::Rel::RelTypeCase::kFilter:
      return extractSources(relation.filter().input());
    case substrait::Rel::RelTypeCase::kFetch:
      return extractSources(relation.fetch().input());
    case substrait::Rel::RelTypeCase::kAggregate:
      return extractSources(relation.aggregate().input());
    case substrait::Rel::RelTypeCase::kSort:
      return extractSources(relation.sort().input());
    case substrait::Rel::RelTypeCase::kJoin:
      return extractSources(relation.join().left()) +
          extractSources(relation.join().right());
    case substrait::Rel::RelTypeCase::kProject:
      return extractSources(relation.project().input());
    case substrait::Rel::RelTypeCase::kSet: {
      std::string text;
      for (const auto& rel : relation.set().inputs()) {
        text += extractSources(rel);
      }
      return text;
    }
    case substrait::Rel::RelTypeCase::kExtensionSingle:
      return extractSources(relation.extension_single().input());
    case substrait::Rel::RelTypeCase::kExtensionMulti: {
      std::string text;
      for (const auto& rel : relation.extension_multi().inputs()) {
        text += extractSources(rel);
      }
      return text;
    }
    case substrait::Rel::RelTypeCase::kExtensionLeaf:
      return "";
    case substrait::Rel::RelTypeCase::kCross:
      return extractSources(relation.cross().left()) +
          extractSources(relation.cross().right());
#if 0
    case substrait::Rel::RelTypeCase::kHashJoin:
      return extractSources(relation.hash_join().left()) + extractSources(relation.hash_join().right());
    case substrait::Rel::RelTypeCase::kMergeJoin:
      return extractSources(relation.merge_join().left()) + extractSources(relation.merge_join().right());
#endif
    case substrait::Rel::REL_TYPE_NOT_SET:
    default:
      return "";
  }
}

std::string extractSources(const substrait::PlanRel& relation) {
  // MEGAHACK -- Use rel_type_case here.
  if (relation.has_root()) {
    return extractSources(relation.root().input());
  } else if (relation.has_rel()) {
    return extractSources(relation.rel());
  }
  return "";
}

} // namespace

void Pipeline::add(const std::string& node) {
  nodes_.push_back(node);
}

std::string Pipeline::toString() {
  std::string text;
  for (auto it = nodes_.rbegin(); it != nodes_.rend(); ++it) {
    if (!text.empty()) {
      text += " -> ";
    }
    text += *it;
  }
  text += ";\n";
  return text;
}

Pipeline* PipelineCollector::add(const std::string& name) {
  auto pipeline = std::make_shared<Pipeline>();
  pipelines_.emplace_back(pipeline);
  pipeline->add(name);
  return pipeline.get();
}

std::string PipelineCollector::toString() {
  std::string text = "pipelines {\n";
  for (auto it = pipelines_.rbegin(); it != pipelines_.rend(); ++it) {
    const auto& pipeline = *it;
    text += "  " + pipeline->toString();
  }
  text += "}\n";
  return text;
}

std::string PlanConverter::sourcesToText(const substrait::Plan& plan) {
  std::string text;
  for (const auto& relation : plan.relations()) {
    text += extractSources(relation);
  }
  return text;
}

std::string PlanConverter::schemaToText(const SymbolInfo& info) {
  std::string text;
  if (info.blob.has_value()) {
    const auto& schema = std::any_cast<substrait::NamedStruct>(info.blob);
    text += "schema " + info.name + " {\n";
    int name_idx = 0;
    int types_idx = 0;
    while (name_idx < schema.names_size() &&
           types_idx < schema.struct_().types_size()) {
      text += "  " + schema.names(name_idx);
      // Interpret the type.
      if (schema.struct_().types(types_idx).kind_case() ==
          substrait::Type::kFp64) {
        if (schema.struct_().types(types_idx).fp64().nullability()) {
          text += " nullable";
        }
        text += " fp64";
      } else {
        text += " UNKNOWN";
      }
      text += ";\n";
      ++name_idx;
      ++types_idx;
      symbol_table_.defineSymbol(
          info.name,
          Location(),
          SymbolType::kField,
          substrait::Rel::RelTypeCase::REL_TYPE_NOT_SET,
          schema);
    }
    text += "}\n";
  }
  return text;
}

std::string PlanConverter::schemasToText() {
  std::string text;
  // MEGAHACK -- Need to figure out what the proper order for these symbols are
  // (it is easy to argue that at a minimum they appear in reverse order).
  for (const SymbolInfo& info : symbol_table_) {
    if (info.type != SymbolType::kSchema)
      continue;
    text += schemaToText(info);
  }
  return text;
}

std::string PlanConverter::literalToText(
    const substrait::Expression::Literal& literal) {
  switch (literal.literal_type_case()) {
    case substrait::Expression::Literal::kBoolean:
      return std::to_string(literal.boolean());
    case substrait::Expression::Literal::kI8:
      return std::to_string(literal.i8());
    case substrait::Expression::Literal::kI16:
      return std::to_string(literal.i16());
    case substrait::Expression::Literal::kI32:
      return std::to_string(literal.i32());
    case substrait::Expression::Literal::kI64:
      return std::to_string(literal.i64());
    case substrait::Expression::Literal::kFp32:
      return std::to_string(literal.fp32());
    case substrait::Expression::Literal::kFp64:
      return std::to_string(literal.fp64());
    case substrait::Expression::Literal::kString:
      return "\"" + literal.string() + "\""; // MEGAHACK -- Deal with escapes.
    default:
      return "UNSUPPORTED_LITERAL_TYPE";
  }
};

std::string PlanConverter::fieldReferenceToText(
    const substrait::Expression::FieldReference& ref) {
  switch (ref.reference_type_case()) {
    case substrait::Expression::FieldReference::kDirectReference: {
      auto field = symbol_table_.nthSymbolByType(
          ref.direct_reference().struct_field().field(), SymbolType::kField);
      if (field != nullptr) {
        return field->name;
      } else {
        return "field#" +
            std::to_string(ref.direct_reference().struct_field().field());
      }
    }
    case substrait::Expression::FieldReference::kMaskedReference:
      return "MASKED_REF_NOT_SUPPORTED";
    case substrait::Expression::FieldReference::REFERENCE_TYPE_NOT_SET:
    default:
      return "";
  }
}

std::string PlanConverter::typeToText(const substrait::Type& type) {
  switch (type.kind_case()) {
    case substrait::Type::kBool:
      if (type.bool_().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_bool";
      return "bool";
    case substrait::Type::kI8:
      if (type.i8().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_i8";
      return "i8";
    case substrait::Type::kI16:
      if (type.i16().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_i16";
      return "i16";
    case substrait::Type::kI32:
      if (type.i32().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_i32";
      return "i32";
    case substrait::Type::kI64:
      if (type.i64().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_i64";
      return "i64";
    case substrait::Type::kFp32:
      if (type.fp32().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_fp32";
      return "fp32";
    case substrait::Type::kFp64:
      if (type.fp64().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_fp64";
      return "fp64";
    case substrait::Type::kString:
      if (type.string().nullability() == Type::NULLABILITY_NULLABLE)
        return "opt_string";
      return "string";
    default:
      return "UNSUPPORTED_TYPE";
  }
};

std::string PlanConverter::scalarFunctionToText(
    const substrait::Expression::ScalarFunction& function) {
  std::string text;
  auto func = symbol_table_.nthSymbolByType(
      function.function_reference(), SymbolType::kFunction);
  if (func != nullptr) {
    text += func->name + "(";
  } else {
    text +=
        "functionref#" + std::to_string(function.function_reference()) + "(";
  }
  // MEGAHACK -- Eventually handle options
  // MEGAHACK -- Also handle output_type
  bool first = true;
  for (const auto& arg : function.arguments()) {
    if (!first)
      text += ", ";
    switch (arg.arg_type_case()) {
      case substrait::FunctionArgument::kEnum:
        text += "ENUM_NOT_SUPPORTED";
        break;
      case substrait::FunctionArgument::kType:
        text += typeToText(arg.type());
        break;
      case substrait::FunctionArgument::kValue:
        text += expressionToText(arg.value());
        break;
      case substrait::FunctionArgument::ARG_TYPE_NOT_SET:
      default:
        text += "UNKNOWN_ARGUMENT_TYPE";
        break;
    }
    first = false;
  }
  for (const auto& arg : function.args()) {
    if (!first)
      text += ", ";
    text += expressionToText(arg);
    first = false;
  }
  // if (first) {
  //   // Had nothing, debug.
  //   text += "MEGAHACK: " + function.ShortDebugString();
  // }
  text += ")";
  return text;
};

std::string PlanConverter::expressionToText(const substrait::Expression& exp) {
  std::string text;
  switch (exp.rex_type_case()) {
    case substrait::Expression::kLiteral:
      text += literalToText(exp.literal());
      break;
    case substrait::Expression::kSelection:
      text += fieldReferenceToText(exp.selection());
      break;
    case substrait::Expression::kScalarFunction:
      text += scalarFunctionToText(exp.scalar_function());
      break;
    case substrait::Expression::REX_TYPE_NOT_SET:
      break;
    default:
      text += "UNSUPPORTED_EXPRESSION_TYPE(" +
          std::to_string(exp.rex_type_case()) + ")";
      break;
  }

  // text += exp.ShortDebugString();
  return text;
}

std::string PlanConverter::aggregateFunctionToText(
    const substrait::AggregateFunction& function) {
  std::string text;
  auto func = symbol_table_.nthSymbolByType(
      function.function_reference(), SymbolType::kFunction);
  if (func != nullptr) {
    text += func->name + "(";
  } else {
    text +=
        "functionref#" + std::to_string(function.function_reference()) + "(";
  }
  bool first = true;
  for (const auto& arg : function.arguments()) {
    if (!first)
      text += ", ";
    switch (arg.arg_type_case()) {
      case substrait::FunctionArgument::kEnum:
        text += "ENUM_NOT_SUPPORTED";
        break;
      case substrait::FunctionArgument::kType:
        text += typeToText(arg.type());
        break;
      case substrait::FunctionArgument::kValue:
        text += expressionToText(arg.value());
        break;
      case substrait::FunctionArgument::ARG_TYPE_NOT_SET:
      default:
        text += "UNKNOWN_ARGUMENT_TYPE";
        break;
    }
    first = false;
  }
  for (const auto& arg : function.args()) {
    if (!first)
      text += ", ";
    text += expressionToText(arg);
    first = false;
  }
  text += ")";
#if 0
  for (const auto& option : function.options()) {
    text += "#" + option.name();
    for (const auto& pref : option.preference()) {
      text += ";" + pref;
    }
  }
#endif
  text += "->" + typeToText(function.output_type());
  // MEGAHACK -- Handle sorts here.
  text += "@" + substrait::AggregationPhase_Name(function.phase());
  return text;
}

std::string PlanConverter::readRelationToText(
    const SymbolInfo& parent,
    const substrait::ReadRel& rel) {
  std::string text;
  if (rel.has_base_schema()) {
    const std::string& name = symbol_table_.getUniqueName("schema");
    symbol_table_.defineSymbol(
        name,
        parent.location,
        SymbolType::kSchema,
        substrait::Rel::RelTypeCase::REL_TYPE_NOT_SET,
        rel.base_schema());
    text += "  base_schema " + name + ";\n";
  }
  // text += rel->read().common().ShortDebugString() + "\n";
  if (rel.has_filter()) {
    text += "  filter " + expressionToText(rel.filter()) + ";\n";
  }
  // text += rel->read().base_effort_filter().ShortDebugString() + "\n";
  // text += rel->read().projection().ShortDebugString() + "\n";
  // text += rel->read().advanced_extension().ShortDebugString() + "\n";
  return text;
}

std::string PlanConverter::filterRelationToText(
    const substrait::FilterRel& rel) {
  std::string text;
  if (rel.has_condition()) {
    text += "  condition " + expressionToText(rel.condition()) + ";\n";
  }
  return text;
}

std::string PlanConverter::aggregateRelationToText(
    const substrait::AggregateRel& rel) {
  std::string text;
  for (const auto& group : rel.groupings()) {
    for (const auto& expr : group.grouping_expressions()) {
      text += "  grouping " + expressionToText(expr) + ";\n";
    }
  }
  for (const auto& measure : rel.measures()) {
    if (!measure.has_measure())
      continue;
    text += "  measure {\n";
    text += "    measure " + aggregateFunctionToText(measure.measure()) + ";\n";
    if (measure.has_filter()) {
      text += "    filter " + expressionToText(measure.filter()) + ";\n";
    }
    text += "  }\n";
  }
  return text;
}

std::string PlanConverter::projectRelationToText(
    const substrait::ProjectRel& rel) {
  std::string text;
  for (const auto& expr : rel.expressions()) {
    text += "  expression " + expressionToText(expr) + ";\n";
  }
  return text;
}

std::string PlanConverter::relationToText(const SymbolInfo& info) {
  if (!info.blob.has_value())
    return "";

  std::string text;
  text += "relation " + info.name + " {\n";

  const auto& rel = std::any_cast<substrait::Rel>(info.blob);
  switch (rel.rel_type_case()) {
    case substrait::Rel::RelTypeCase::kRead:
      text += readRelationToText(info, rel.read());
      break;
    case substrait::Rel::RelTypeCase::kFilter:
      text += filterRelationToText(rel.filter());
      break;
    case substrait::Rel::RelTypeCase::kAggregate:
      text += aggregateRelationToText(rel.aggregate());
      break;
    case substrait::Rel::RelTypeCase::kProject:
      text += projectRelationToText(rel.project());
      break;
    case substrait::Rel::RelTypeCase::REL_TYPE_NOT_SET:
    default:
      break;
  }

  text += "}\n";
  return text;
}

std::string PlanConverter::relationsToText() {
  std::string text;
  // MEGAHACK -- Need to figure out what the proper order for these symbols are
  // (it is easy to argue that at a minimum they appear in reverse order).
  bool first = true;
  for (const SymbolInfo& info : symbol_table_) {
    if (info.type != SymbolType::kRelation)
      continue;
    if (!first)
      text += "\n";
    text += relationToText(info);
    first = false;
  }
  return text;
}

// MEGAHACK -- The location for the symbol table should probably be the path
// without the field.
#define VISIT_LEAF(type, name)                             \
  case substrait::Rel::RelTypeCase::type: {                \
    auto unique_name = symbol_table_.getUniqueName(#name); \
    pipeline->add(unique_name);                            \
    symbol_table_.defineSymbol(                            \
        unique_name,                                       \
        location,                                          \
        SymbolType::kRelation,                             \
        substrait::Rel::RelTypeCase::type,                 \
        relation);                                         \
    break;                                                 \
  }

#define VISIT_SINGLE(type, name)                                              \
  case substrait::Rel::RelTypeCase::type: {                                   \
    auto unique_name = symbol_table_.getUniqueName(#name);                    \
    pipeline->add(unique_name);                                               \
    symbol_table_.defineSymbol(                                               \
        unique_name,                                                          \
        location,                                                             \
        SymbolType::kRelation,                                                \
        substrait::Rel::RelTypeCase::type,                                    \
        relation);                                                            \
    visitPipelines(                                                           \
        relation.name().input(), collector, location.visit(#name), pipeline); \
    break;                                                                    \
  }

#define VISIT_DOUBLE(type, name)                           \
  case substrait::Rel::RelTypeCase::type: {                \
    auto unique_name = symbol_table_.getUniqueName(#name); \
    pipeline->add(unique_name);                            \
    symbol_table_.defineSymbol(                            \
        unique_name,                                       \
        location,                                          \
        SymbolType::kRelation,                             \
        substrait::Rel::RelTypeCase::type,                 \
        relation);                                         \
    auto new_loc = location.visit(#name);                  \
    visitPipelines(                                        \
        relation.name().left(),                            \
        collector,                                         \
        new_loc,                                           \
        collector->add(unique_name));                      \
    visitPipelines(                                        \
        relation.name().right(),                           \
        collector,                                         \
        new_loc,                                           \
        collector->add(unique_name));                      \
    break;                                                 \
  }

#define VISIT_MULTIPLE(type, name)                                          \
  case substrait::Rel::RelTypeCase::type: {                                 \
    auto unique_name = symbol_table_.getUniqueName(#name);                  \
    pipeline->add(unique_name);                                             \
    symbol_table_.defineSymbol(                                             \
        unique_name,                                                        \
        location,                                                           \
        SymbolType::kRelation,                                              \
        substrait::Rel::RelTypeCase::type,                                  \
        relation);                                                          \
    auto new_loc = location.visit(#name);                                   \
    for (const auto& rel : relation.name().inputs()) {                      \
      visitPipelines(rel, collector, new_loc, collector->add(unique_name)); \
    }                                                                       \
    break;                                                                  \
  }

void PlanConverter::visitPipelines(
    const substrait::Rel& relation,
    PipelineCollector* collector,
    const Location& location,
    Pipeline* pipeline) {
  switch (relation.rel_type_case()) {
    VISIT_LEAF(kRead, read);
    VISIT_SINGLE(kFilter, filter);
    VISIT_SINGLE(kFetch, fetch);
    VISIT_SINGLE(kAggregate, aggregate);
    VISIT_SINGLE(kSort, sort);
    VISIT_DOUBLE(kJoin, join);
    VISIT_SINGLE(kProject, project);
    VISIT_MULTIPLE(kSet, set);
    VISIT_SINGLE(kExtensionSingle, extension_single);
    VISIT_MULTIPLE(kExtensionMulti, extension_multi);
    VISIT_LEAF(kExtensionLeaf, extension_leaf);
    VISIT_DOUBLE(kCross, cross);
#if 0
    VISIT_DOUBLE(kHashJoin, hash_join);
    VISIT_DOUBLE(kMergeJoin, merge_join);
#endif
    case substrait::Rel::REL_TYPE_NOT_SET:
    default:
      break;
  }
}

#undef VISIT_LEAF
#undef VISIT_SINGLE
#undef VISIT_DOUBLE
#undef VISIT_MULTIPLE

void PlanConverter::visitPipelines(
    const substrait::PlanRel& relation,
    PipelineCollector* collector) {
  Location location;
  switch (relation.rel_type_case()) {
    case substrait::PlanRel::kRoot: {
      // MEGAHACK -- Figure out whether this "node" should be listed as being
      // part of the pipeline.
      auto unique_name = symbol_table_.getUniqueName("root");
      symbol_table_.defineSymbol(
          unique_name,
          location,
          SymbolType::kPlanRelation,
          substrait::Rel::RelTypeCase::REL_TYPE_NOT_SET,
          relation);
      visitPipelines(
          relation.root().input(),
          collector,
          location.visit("root"),
          collector->add(unique_name));
      break;
    }
    case substrait::PlanRel::kRel: {
      // MEGAHACK -- Figure out whether this "node" should be listed as being
      // part of the pipeline.
      auto unique_name = symbol_table_.getUniqueName("rel");
      symbol_table_.defineSymbol(
          unique_name,
          location,
          SymbolType::kPlanRelation,
          substrait::Rel::RelTypeCase::REL_TYPE_NOT_SET,
          relation);
      visitPipelines(
          relation.rel(),
          collector,
          location.visit("rel"),
          collector->add(unique_name));
      break;
    }
    case substrait::PlanRel::REL_TYPE_NOT_SET:
    default:
      return;
  }
}

std::string PlanConverter::pipelinesToText(const substrait::Plan& plan) {
  PipelineCollector collector;
  for (const auto& relation : plan.relations()) {
    visitPipelines(relation, &collector);
  }
  return collector.toString();
}

std::string PlanConverter::toString() {
  std::string text;

  // Since we don't have a separate visitor pass, create the side effects now and emit later.
  std::string functionsText = functionsToText(plan_);

  text += pipelinesToText(plan_);
  if (!text.empty()) {
    text += "\n";
  }
  text += relationsToText();
  if (!text.empty()) {
    text += "\n";
  }
  text += schemasToText();
  if (!text.empty()) {
    text += "\n";
  }
  text += sourcesToText(plan_);
  if (!text.empty()) {
    text += "\n";
  }
  text += functionsText;
  return text;
}

} // namespace substrait