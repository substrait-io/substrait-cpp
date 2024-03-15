/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/SymbolTablePrinter.h"

#include <set>
#include <sstream>

#include "substrait/common/Exceptions.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/PlanPrinterVisitor.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

namespace {

const int kBinaryRelationInputCount = 2;

void localFileToText(
    const ::substrait::proto::ReadRel::LocalFiles::FileOrFiles& item,
    std::stringstream* text) {
  switch (item.path_type_case()) {
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kUriPath:
      *text << "uri_path: \"" << item.uri_path() << "\"";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kUriPathGlob:
      *text << "uri_path_glob: \"" + item.uri_path_glob() << "\"";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kUriFile:
      *text << "uri_file: \"" << item.uri_file() << "\"";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kUriFolder:
      *text << "uri_folder: \"" << item.uri_folder() << "\"";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::
        PATH_TYPE_NOT_SET:
    default:
      SUBSTRAIT_UNSUPPORTED(
          "Unknown path type " + std::to_string(item.path_type_case()) +
          "provided.");
  }
  if (item.partition_index() != 0) {
    *text << " partition_index: " << std::to_string(item.partition_index());
  }
  if (item.start() != 0 || item.length() != 0) {
    *text << " start: " << std::to_string(item.start());
    *text << " length: " << std::to_string(item.length());
  }
  switch (item.file_format_case()) {
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kParquet:
      *text << " parquet: {}";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kArrow:
      *text << " arrow: {}";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kOrc:
      *text << " orc: {}";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kExtension:
      SUBSTRAIT_UNSUPPORTED("Extension file format type not yet supported.");
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::kDwrf:
      *text << " dwrf: {}";
      break;
    case ::substrait::proto::ReadRel::LocalFiles::FileOrFiles::
        FILE_FORMAT_NOT_SET:
    default:
      SUBSTRAIT_UNSUPPORTED(
          "Unknown file format type " +
          std::to_string(item.file_format_case()) + "provided.");
  }
}

std::string typeToText(const ::substrait::proto::Type& type) {
  SymbolTable symbolTable;
  PlanPrinterVisitor visitor(symbolTable);
  return visitor.typeToText(type);
};

std::string relationToText(
    const SymbolTable& symbolTable,
    const SymbolInfo& info,
    SubstraitErrorListener* errorListener) {
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, info.blob);
  if (relationData->relation.rel_type_case() ==
      ::substrait::proto::Rel::REL_TYPE_NOT_SET) {
    // The relation isn't defined, so we don't have anything to print.
    return "";
  }

  PlanPrinterVisitor printer(symbolTable);
  auto result = printer.printRelation(info);
  errorListener->addErrorInstances(printer.getErrorListener()->getErrors());
  return result;
}

const SymbolInfo* getNextSinglePipeline(
    const std::shared_ptr<RelationData>& relationData) {
  if (relationData->continuingPipeline != nullptr) {
    return relationData->continuingPipeline;
  } else if (relationData->newPipelines.size() == 1) {
    return relationData->newPipelines[0];
  }
  return nullptr;
}

std::vector<std::string> pipelineToPath(
    const SymbolTable& symbolTable,
    const SymbolInfo* info) {
  std::vector<std::string> pipeline;
  if (*info == SymbolInfo::kUnknown) {
    return pipeline;
  }
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, info->blob);
  pipeline.push_back(info->name);
  if (relationData->continuingPipeline != nullptr) {
    auto tailPipe =
        pipelineToPath(symbolTable, relationData->continuingPipeline);
    pipeline.insert(pipeline.end(), tailPipe.begin(), tailPipe.end());
  }
  return pipeline;
}

std::string outputPipelinesSection(const SymbolTable& symbolTable) {
  std::stringstream text;
  bool hasPreviousText = false;
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kPlanRelation &&
        info.type != SymbolType::kRelation) {
      continue;
    }
    auto relationData = ANY_CAST(std::shared_ptr<RelationData>, info.blob);
    for (auto pipelineStart : relationData->newPipelines) {
      auto pipeline = pipelineToPath(symbolTable, pipelineStart);
      pipeline.insert(pipeline.begin(), info.name);
      bool isFirstPipe = true;
      for (auto pipe = pipeline.rbegin(); pipe != pipeline.rend(); pipe++) {
        if (isFirstPipe) {
          text << "  " << *pipe;
        } else {
          text << " -> " << *pipe;
        }
        isFirstPipe = false;
      }
      text << ";\n";
      hasPreviousText = true;
    }
    for (auto pipelineStart : relationData->subQueryPipelines) {
      auto pipeline = pipelineToPath(symbolTable, pipelineStart);
      // No need to include this node since the subquery already points to the
      // end of this pipeline.
      bool isFirstPipe = true;
      for (auto pipe = pipeline.rbegin(); pipe != pipeline.rend(); pipe++) {
        if (isFirstPipe) {
          text << "  " << *pipe;
        } else {
          text << " -> " << *pipe;
        }
        isFirstPipe = false;
      }
      text << ";\n";
      hasPreviousText = true;
    }
  }
  if (hasPreviousText) {
    return "pipelines {\n" + text.str() + "}\n";
  }
  return text.str();
}

std::string outputRelationsSection(
    const SymbolTable& symbolTable,
    SubstraitErrorListener* errorListener) {
  std::stringstream text;
  bool hasPreviousText = false;
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kRelation) {
      continue;
    }
    // TODO: Put handling this into the PlanPrinterVisitor.
    if (hasPreviousText) {
      text << "\n";
    }
    text << relationToText(symbolTable, info, errorListener);
    hasPreviousText = true;
  }
  return text.str();
}

std::string outputRootSection(const SymbolTable& symbolTable) {
  std::stringstream text;
  bool hasPreviousText = false;
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kRoot) {
      continue;
    }
    auto names = ANY_CAST(std::vector<std::string>, info.blob);
    if (names.empty()) {
      // No point in printing an empty section.
      continue;
    }
    if (hasPreviousText) {
      text << "\n";
    }
    text << "root {"
         << "\n";
    text << "  names = [";
    bool hadName = false;
    for (const auto& name : names) {
      if (hadName) {
        text << ",\n";
      } else {
        text << "\n";
      }
      text << "    " << name;
      hadName = true;
    }
    if (hadName) {
      text << "\n";
    }
    text << "  ]\n";
    text << "}\n";
    hasPreviousText = true;
  }
  return text.str();
}

std::string outputSchemaSection(const SymbolTable& symbolTable) {
  std::stringstream text;
  bool hasPreviousText = false;
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kSchema) {
      continue;
    }
    if (!info.blob.has_value()) {
      continue;
    }

    if (hasPreviousText) {
      text << "\n";
    }

    const auto* schema =
        ANY_CAST(const ::substrait::proto::NamedStruct*, info.blob);
    text << "schema " << info.name << " {\n";
    int idx = 0;
    while (idx < schema->names_size() && idx < schema->struct_().types_size()) {
      // TODO -- Handle potential whitespace in the names here or elsewhere.
      text << "  " << schema->names(idx);
      text << " " << typeToText(schema->struct_().types(idx));
      text << ";\n";
      ++idx;
    }
    text << "}\n";
    hasPreviousText = true;
  }
  return text.str();
}

std::string outputSourcesSection(const SymbolTable& symbolTable) {
  std::stringstream text;
  bool hasPreviousText = false;
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kSource) {
      continue;
    }

    if (hasPreviousText) {
      text << "\n";
    }
    auto subtype = ANY_CAST(SourceType, info.subtype);
    switch (subtype) {
      case SourceType::kNamedTable: {
        if (info.blob.has_value()) {
          // We are using the proto as is in lieu of a disciplined structure.
          auto table = ANY_CAST(
              const ::substrait::proto::ReadRel_NamedTable*, info.blob);
          text << "source named_table " << info.name << " {\n";
          text << "  names = [\n";
          for (const auto& name : table->names()) {
            text << "    \"" << name << "\",\n";
          }
          text << "  ]\n";
          text << "}\n";
          hasPreviousText = true;
          break;
        }
        // We are using the new style data structure.
        text << "source named_table " << info.name << " {\n";
        text << "  names = [\n";
        for (const auto& sym :
             symbolTable.lookupSymbolsByLocation(info.sourceLocation)) {
          if (sym->type == SymbolType::kSourceDetail) {
            text << "    \"" << sym->name << "\",\n";
          }
        }
        text << "  ]\n";
        text << "}\n";
        hasPreviousText = true;

        break;
      }
      case SourceType::kLocalFiles: {
        // TODO: Put handling this into the PlanPrinterVisitor.
        auto files =
            ANY_CAST(const ::substrait::proto::ReadRel_LocalFiles*, info.blob);
        text << "source local_files " << info.name << " {\n";
        text << "  items = [\n";
        for (const auto& item : files->items()) {
          text << "    {";
          localFileToText(item, &text);
          text << "}\n";
        }
        text << "  ]\n";
        text << "}\n";
        hasPreviousText = true;
        break;
      }
      case SourceType::kVirtualTable:
        SUBSTRAIT_FAIL("Printing of virtual tables not yet implemented.");
      case SourceType::kExtensionTable:
        SUBSTRAIT_FAIL("Printing of extension tables not yet implemented.");
      case SourceType::kUnknown:
      default:
        SUBSTRAIT_FAIL("Printing of an unknown read source requested.");
    }
  }
  return text.str();
}

std::string outputFunctionsSection(const SymbolTable& symbolTable) {
  std::stringstream text;

  std::map<uint32_t, std::string> spaceNames;
  auto cmp = [&](uint32_t a, uint32_t b) {
    if (spaceNames.find(a) == spaceNames.end()) {
      return spaceNames.find(b) != spaceNames.end();
    } else if (spaceNames.find(b) == spaceNames.end()) {
      return false;
    }
    return spaceNames.at(a) < spaceNames.at(b);
  };
  // Sorted by name if we have one, otherwise by space id.
  std::set<uint32_t, decltype(cmp)> usedSpaces(cmp);

  // Look at the existing spaces.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kExtensionSpace) {
      continue;
    }

    auto extensionData =
        ANY_CAST(std::shared_ptr<ExtensionSpaceData>, info.blob);
    spaceNames.insert(
        std::make_pair(extensionData->anchorReference, info.name));
  }

  // Find any spaces that are used but undefined.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kFunction) {
      continue;
    }

    auto extension = ANY_CAST(std::shared_ptr<FunctionData>, info.blob);
    usedSpaces.insert(extension->extensionUriReference.value());
  }

  // Finally output the extensions by space in alphabetical order.
  bool hasPreviousOutput = false;
  for (const uint32_t space : usedSpaces) {
    if (hasPreviousOutput) {
      text << "\n";
    }
    if (spaceNames.find(space) == spaceNames.end()) {
      // TODO: Handle this case as a warning.
      text << "extension_space {\n";
    } else {
      text << "extension_space " << spaceNames[space] << " {\n";
    }

    std::vector<std::pair<std::string, std::string>> functionsToOutput;
    for (const SymbolInfo& info : symbolTable) {
      if (info.type != SymbolType::kFunction) {
        continue;
      }

      auto functionData = ANY_CAST(std::shared_ptr<FunctionData>, info.blob);
      if (functionData->extensionUriReference.has_value() &&
          functionData->extensionUriReference.value() != space) {
        continue;
      }

      functionsToOutput.emplace_back(info.name, functionData->name);
    }
    std::sort(functionsToOutput.begin(), functionsToOutput.end());
    for (auto [shortName, canonicalName] : functionsToOutput) {
      text << "  function " << canonicalName << " as " << shortName << ";\n";
    }
    text << "}\n";
    hasPreviousOutput = true;
  }

  return text.str();
}

void outputExtensionSpacesToBinaryPlan(
    const SymbolTable& symbolTable,
    ::substrait::proto::Plan* plan) {
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kExtensionSpace) {
      continue;
    }

    auto extensionData =
        ANY_CAST(std::shared_ptr<ExtensionSpaceData>, info.blob);
    auto uri = plan->add_extension_uris();
    uri->set_uri(info.name);
    uri->set_extension_uri_anchor(extensionData->anchorReference);
  }
}

void outputFunctionsToBinaryPlan(
    const SymbolTable& symbolTable,
    ::substrait::proto::Plan* plan) {
  std::map<uint32_t, std::string> spaceNames;
  std::set<uint32_t> usedSpaces;

  // Look at the existing spaces.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kExtensionSpace) {
      continue;
    }

    auto extensionData =
        ANY_CAST(std::shared_ptr<ExtensionSpaceData>, info.blob);
    spaceNames.insert(
        std::make_pair(extensionData->anchorReference, info.name));
  }

  // Find any spaces that are used but undefined.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kFunction) {
      continue;
    }

    auto extension = ANY_CAST(std::shared_ptr<FunctionData>, info.blob);
    if (extension->extensionUriReference.has_value()) {
      usedSpaces.insert(extension->extensionUriReference.value());
    }
  }

  // Output the extensions by space in the order they were encountered.
  for (const uint32_t space : usedSpaces) {
    for (const SymbolInfo& info : symbolTable) {
      if (info.type != SymbolType::kFunction) {
        continue;
      }

      auto functionData = ANY_CAST(std::shared_ptr<FunctionData>, info.blob);
      if (functionData->extensionUriReference != space) {
        continue;
      }

      auto func = plan->add_extensions()->mutable_extension_function();
      func->set_function_anchor(functionData->anchor);
      func->set_name(functionData->name);

      if (spaceNames.find(space) != spaceNames.end()) {
        func->set_extension_uri_reference(space);
      }
    }
  }

  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kFunction) {
      continue;
    }

    auto functionData = ANY_CAST(std::shared_ptr<FunctionData>, info.blob);
    if (!functionData->extensionUriReference.has_value()) {
      auto func = plan->add_extensions()->mutable_extension_function();
      func->set_function_anchor(functionData->anchor);
      func->set_name(functionData->name);
    }
  }
}

} // namespace

// TODO -- Update so that errors occurring during printing are captured.
std::string SymbolTablePrinter::outputToText(
    const SymbolTable& symbolTable,
    SubstraitErrorListener* errorListener) {
  std::stringstream text;
  bool hasPreviousText = false;

  std::string newText = outputPipelinesSection(symbolTable);
  if (!newText.empty()) {
    text << newText;
    hasPreviousText = true;
  }

  newText = outputRelationsSection(symbolTable, errorListener);
  if (!newText.empty()) {
    if (hasPreviousText) {
      text << "\n";
    }
    text << newText;
    hasPreviousText = true;
  }

  newText = outputRootSection(symbolTable);
  if (!newText.empty()) {
    if (hasPreviousText) {
      text << "\n";
    }
    text << newText;
    hasPreviousText = true;
  }

  newText = outputSchemaSection(symbolTable);
  if (!newText.empty()) {
    if (hasPreviousText) {
      text << "\n";
    }
    text << newText;
    hasPreviousText = true;
  }

  newText = outputSourcesSection(symbolTable);
  if (!newText.empty()) {
    if (hasPreviousText) {
      text << "\n";
    }
    text << newText;
    hasPreviousText = true;
  }

  newText = outputFunctionsSection(symbolTable);
  if (!newText.empty()) {
    if (hasPreviousText) {
      text << "\n";
    }
    text << newText;
  }
  return text.str();
}

void SymbolTablePrinter::addInputsToRelation(
    const SymbolTable& symbolTable,
    const SymbolInfo& symbolInfo,
    ::substrait::proto::Rel* relation) {
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbolInfo.blob);
  int consumedPipelines = 0;

  // Connect up the incoming inputs in the relation data to the appropriate
  // input/left/right/inputs of this relation (which recursively also needs to
  // have its inputs added).
  if (symbolInfo.subtype.type() != typeid(RelationType)) {
    // The type isn't one we expected bail.
    return;
  }
  auto relationType = ANY_CAST(RelationType, symbolInfo.subtype);
  switch (relationType) {
    case RelationType::kRead:
      // No inputs to add.
      break;
    case RelationType::kProject: {
      auto nextPipeline = getNextSinglePipeline(relationData);
      if (nextPipeline != nullptr) {
        auto nextRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, nextPipeline->blob);
        *relation->mutable_project()->mutable_input() =
            nextRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *nextPipeline,
            relation->mutable_project()->mutable_input());
        for (auto& expr : *relation->mutable_project()->mutable_expressions()) {
          addInputsToExpression(
              symbolTable,
              relationData->subQueryPipelines,
              &expr,
              &consumedPipelines);
        }
      } else {
        SUBSTRAIT_FAIL("Internal error:  Incorrect number of pipelines.");
      }
      break;
    }
    case RelationType::kJoin:
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_join()->mutable_left() = leftRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[0],
            relation->mutable_join()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_join()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[1],
            relation->mutable_join()->mutable_right());
        if (relation->join().has_expression()) {
          addInputsToExpression(
              symbolTable,
              relationData->subQueryPipelines,
              relation->mutable_join()->mutable_expression(),
              &consumedPipelines);
        }
        if (relation->join().has_post_join_filter()) {
          addInputsToExpression(
              symbolTable,
              relationData->subQueryPipelines,
              relation->mutable_join()->mutable_post_join_filter(),
              &consumedPipelines);
        }
        break;
      }
    case RelationType::kCross:
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_cross()->mutable_left() = leftRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[0],
            relation->mutable_cross()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_cross()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[1],
            relation->mutable_cross()->mutable_right());
      }
      break;
    case RelationType::kFetch: {
      auto nextPipeline = getNextSinglePipeline(relationData);
      if (nextPipeline != nullptr) {
        auto nextRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, nextPipeline->blob);
        *relation->mutable_fetch()->mutable_input() =
            nextRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *nextPipeline,
            relation->mutable_fetch()->mutable_input());
      } else {
        SUBSTRAIT_FAIL("Internal error:  Incorrect number of pipelines.");
      }
      break;
    }
    case RelationType::kAggregate: {
      auto nextPipeline = getNextSinglePipeline(relationData);
      if (nextPipeline != nullptr) {
        auto nextRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, nextPipeline->blob);
        *relation->mutable_aggregate()->mutable_input() =
            nextRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *nextPipeline,
            relation->mutable_aggregate()->mutable_input());
      } else {
        SUBSTRAIT_FAIL("Internal error:  Incorrect number of pipelines.");
      }
      for (auto& grouping :
           *relation->mutable_aggregate()->mutable_groupings()) {
        for (auto& groupExpression : *grouping.mutable_grouping_expressions()) {
          addInputsToExpression(
              symbolTable,
              relationData->subQueryPipelines,
              &groupExpression,
              &consumedPipelines);
        }
      }
      for (auto& measure : *relation->mutable_aggregate()->mutable_measures()) {
        if (measure.has_measure()) {
          for (auto& argument :
               *measure.mutable_measure()->mutable_arguments()) {
            if (argument.arg_type_case() ==
                ::substrait::proto::FunctionArgument::kValue) {
              addInputsToExpression(
                  symbolTable,
                  relationData->subQueryPipelines,
                  argument.mutable_value(),
                  &consumedPipelines);
            }
          }
        }
        if (measure.has_filter()) {
          addInputsToExpression(
              symbolTable,
              relationData->subQueryPipelines,
              measure.mutable_filter(),
              &consumedPipelines);
        }
      }
      break;
    }
    case RelationType::kSort: {
      auto nextPipeline = getNextSinglePipeline(relationData);
      if (nextPipeline != nullptr) {
        auto nextRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, nextPipeline->blob);
        *relation->mutable_sort()->mutable_input() = nextRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *nextPipeline,
            relation->mutable_sort()->mutable_input());
        for (auto& sort : *relation->mutable_sort()->mutable_sorts()) {
          addInputsToExpression(
              symbolTable,
              relationData->subQueryPipelines,
              sort.mutable_expr(),
              &consumedPipelines);
        }
      } else {
        SUBSTRAIT_FAIL("Internal error:  Incorrect number of pipelines.");
      }
      break;
    }
    case RelationType::kFilter: {
      auto nextPipeline = getNextSinglePipeline(relationData);
      if (nextPipeline != nullptr) {
        auto nextRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, nextPipeline->blob);
        *relation->mutable_filter()->mutable_input() =
            nextRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *nextPipeline,
            relation->mutable_filter()->mutable_input());
        if (relation->filter().has_condition()) {
          addInputsToExpression(
              symbolTable,
              relationData->subQueryPipelines,
              relation->mutable_filter()->mutable_condition(),
              &consumedPipelines);
        }
      } else {
        SUBSTRAIT_FAIL("Internal error:  Incorrect number of pipelines.");
      }
      break;
    }
    case RelationType::kSet:
      for (const auto& pipeline : relationData->newPipelines) {
        auto inputRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, pipeline->blob);
        auto* input = relation->mutable_set()->add_inputs();
        *input = inputRelationData->relation;
        addInputsToRelation(symbolTable, *pipeline, input);
      }
      break;
    case RelationType::kHashJoin: {
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_hash_join()->mutable_left() =
            leftRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[0],
            relation->mutable_hash_join()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_hash_join()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[1],
            relation->mutable_hash_join()->mutable_right());
      }
      if (relation->hash_join().has_post_join_filter()) {
        addInputsToExpression(
            symbolTable,
            relationData->subQueryPipelines,
            relation->mutable_hash_join()->mutable_post_join_filter(),
            &consumedPipelines);
      }
      break;
    }
    case RelationType::kMergeJoin: {
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_merge_join()->mutable_left() =
            leftRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[0],
            relation->mutable_merge_join()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_merge_join()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[1],
            relation->mutable_merge_join()->mutable_right());
      }
      if (relation->merge_join().has_post_join_filter()) {
        addInputsToExpression(
            symbolTable,
            relationData->subQueryPipelines,
            relation->mutable_merge_join()->mutable_post_join_filter(),
            &consumedPipelines);
      }
      break;
    }
    case RelationType::kExchange:
    case RelationType::kDdl:
    case RelationType::kWrite:
      // Not yet possible to reach these relations in plans.
      break;
    case RelationType::kExtensionLeaf:
      // Nothing to do here.
      break;
    case RelationType::kExtensionSingle: {
      auto nextPipeline = getNextSinglePipeline(relationData);
      if (nextPipeline != nullptr) {
        auto nextRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, nextPipeline->blob);
        *relation->mutable_extension_single()->mutable_input() =
            nextRelationData->relation;
        addInputsToRelation(
            symbolTable,
            *nextPipeline,
            relation->mutable_extension_single()->mutable_input());
      } else {
        SUBSTRAIT_FAIL("Internal error:  Incorrect number of pipelines.");
      }
      break;
    }
    case RelationType::kExtensionMulti:
      for (const auto& pipeline : relationData->newPipelines) {
        auto inputRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, pipeline->blob);
        auto* input = relation->mutable_extension_multi()->add_inputs();
        *input = inputRelationData->relation;
        addInputsToRelation(symbolTable, *pipeline, input);
      }
      break;
    case RelationType::kUnknown:
      break;
  }
}

void SymbolTablePrinter::addInputsToExpression(
    const SymbolTable& symbolTable,
    const std::vector<const SymbolInfo*>& symbolInfos,
    ::substrait::proto::Expression* expression,
    int* consumedPipelines) {
  switch (expression->rex_type_case()) {
    case ::substrait::proto::Expression::kLiteral:
    case ::substrait::proto::Expression::kSelection:
      return;
    case ::substrait::proto::Expression::kScalarFunction: {
      for (auto& arg :
           *expression->mutable_scalar_function()->mutable_arguments()) {
        if (arg.arg_type_case() ==
            ::substrait::proto::FunctionArgument::kValue) {
          addInputsToExpression(
              symbolTable, symbolInfos, arg.mutable_value(), consumedPipelines);
        }
      }
      break;
    }
    case ::substrait::proto::Expression::kWindowFunction:
      for (auto& arg :
           *expression->mutable_window_function()->mutable_arguments()) {
        if (arg.arg_type_case() ==
            ::substrait::proto::FunctionArgument::kValue) {
          addInputsToExpression(
              symbolTable, symbolInfos, arg.mutable_value(), consumedPipelines);
        }
      }
      for (auto& partition :
           *expression->mutable_window_function()->mutable_partitions()) {
        addInputsToExpression(
            symbolTable, symbolInfos, &partition, consumedPipelines);
      }
      break;
    case ::substrait::proto::Expression::kIfThen:
      for (auto& ifThen : *expression->mutable_if_then()->mutable_ifs()) {
        addInputsToExpression(
            symbolTable, symbolInfos, ifThen.mutable_if_(), consumedPipelines);
        addInputsToExpression(
            symbolTable, symbolInfos, ifThen.mutable_then(), consumedPipelines);
      }
      if (expression->if_then().has_else_()) {
        addInputsToExpression(
            symbolTable,
            symbolInfos,
            expression->mutable_if_then()->mutable_else_(),
            consumedPipelines);
      }
      break;
    case ::substrait::proto::Expression::kSwitchExpression:
      if (expression->switch_expression().has_match()) {
        addInputsToExpression(
            symbolTable,
            symbolInfos,
            expression->mutable_switch_expression()->mutable_match(),
            consumedPipelines);
      }
      for (auto& ifValue :
           *expression->mutable_switch_expression()->mutable_ifs()) {
        addInputsToExpression(
            symbolTable,
            symbolInfos,
            ifValue.mutable_then(),
            consumedPipelines);
      }
      if (expression->switch_expression().has_else_()) {
        addInputsToExpression(
            symbolTable,
            symbolInfos,
            expression->mutable_switch_expression()->mutable_else_(),
            consumedPipelines);
      }
      break;
    case ::substrait::proto::Expression::kSingularOrList:
      if (expression->singular_or_list().has_value()) {
        addInputsToExpression(
            symbolTable,
            symbolInfos,
            expression->mutable_singular_or_list()->mutable_value(),
            consumedPipelines);
      }
      for (auto& option :
           *expression->mutable_singular_or_list()->mutable_options()) {
        addInputsToExpression(
            symbolTable, symbolInfos, &option, consumedPipelines);
      }
      break;
    case ::substrait::proto::Expression::kMultiOrList:
      for (auto& value :
           *expression->mutable_multi_or_list()->mutable_value()) {
        addInputsToExpression(
            symbolTable, symbolInfos, &value, consumedPipelines);
      }
      for (auto& option :
           *expression->mutable_multi_or_list()->mutable_options()) {
        for (auto& field : *option.mutable_fields()) {
          addInputsToExpression(
              symbolTable, symbolInfos, &field, consumedPipelines);
        }
      }
      break;
    case ::substrait::proto::Expression::kCast:
      if (expression->cast().has_input()) {
        addInputsToExpression(
            symbolTable,
            symbolInfos,
            expression->mutable_cast()->mutable_input(),
            consumedPipelines);
      }
      break;
    case ::substrait::proto::Expression::kSubquery:
      // Handled below.
      break;
    case ::substrait::proto::Expression::kNested:
      // TODO -- Implement nested expressions.
      break;
    case ::substrait::proto::Expression::kEnum:
      break;
    case ::substrait::proto::Expression::REX_TYPE_NOT_SET:
      return;
  }

  switch (expression->subquery().subquery_type_case()) {
    case ::substrait::proto::Expression_Subquery::kScalar: {
      if (*consumedPipelines >= symbolInfos.size()) {
        SUBSTRAIT_FAIL("Internal error:  Ran out of subquery symbols.");
      }
      auto subquerySymbol = symbolInfos[(*consumedPipelines)++];
      if (subquerySymbol != nullptr) {
        auto relationData =
            ANY_CAST(std::shared_ptr<RelationData>, subquerySymbol->blob);
        *expression->mutable_subquery()->mutable_scalar()->mutable_input() =
            relationData->relation;
        addInputsToRelation(
            symbolTable,
            *subquerySymbol,
            expression->mutable_subquery()->mutable_scalar()->mutable_input());
      } else {
        SUBSTRAIT_FAIL("Internal Error:  Known symbol is missing.");
      }
      break;
    }
    case ::substrait::proto::Expression_Subquery::kInPredicate: {
      // First visit the needle expressions.
      for (auto& expr : *expression->mutable_subquery()
                             ->mutable_in_predicate()
                             ->mutable_needles()) {
        addInputsToExpression(
            symbolTable, symbolInfos, &expr, consumedPipelines);
      }
      // Now visit the haystack.
      if (expression->subquery().in_predicate().has_haystack()) {
        if (*consumedPipelines >= symbolInfos.size()) {
          SUBSTRAIT_FAIL("Internal error:  Ran out of subquery symbols.");
        }
        auto subquerySymbol = symbolInfos[(*consumedPipelines)++];
        if (subquerySymbol != nullptr) {
          auto relationData =
              ANY_CAST(std::shared_ptr<RelationData>, subquerySymbol->blob);
          *expression->mutable_subquery()
               ->mutable_in_predicate()
               ->mutable_haystack() = relationData->relation;
          addInputsToRelation(
              symbolTable,
              *subquerySymbol,
              expression->mutable_subquery()
                  ->mutable_in_predicate()
                  ->mutable_haystack());
        } else {
          SUBSTRAIT_FAIL("Internal Error:  Known symbol is missing.");
        }
      }
      break;
    }
    case ::substrait::proto::Expression_Subquery::kSetPredicate: {
      if (*consumedPipelines >= symbolInfos.size()) {
        SUBSTRAIT_FAIL("Internal error:  Ran out of subquery symbols.");
      }
      auto subquerySymbol = symbolInfos[(*consumedPipelines)++];
      if (subquerySymbol != nullptr) {
        auto relationData =
            ANY_CAST(std::shared_ptr<RelationData>, subquerySymbol->blob);
        *expression->mutable_subquery()
             ->mutable_set_predicate()
             ->mutable_tuples() = relationData->relation;
        addInputsToRelation(
            symbolTable,
            *subquerySymbol,
            expression->mutable_subquery()
                ->mutable_set_predicate()
                ->mutable_tuples());
      } else {
        SUBSTRAIT_FAIL("Internal Error:  Known symbol is missing.");
      }
      break;
    }
    case ::substrait::proto::Expression_Subquery::kSetComparison: {
      addInputsToExpression(
          symbolTable,
          symbolInfos,
          expression->mutable_subquery()
              ->mutable_set_comparison()
              ->mutable_left(),
          consumedPipelines);
      if (*consumedPipelines >= symbolInfos.size()) {
        SUBSTRAIT_FAIL("Internal error:  Ran out of subquery symbols.");
      }
      auto subquerySymbol = symbolInfos[(*consumedPipelines)++];
      if (subquerySymbol != nullptr) {
        auto relationData =
            ANY_CAST(std::shared_ptr<RelationData>, subquerySymbol->blob);
        *expression->mutable_subquery()
             ->mutable_set_comparison()
             ->mutable_right() = relationData->relation;
        addInputsToRelation(
            symbolTable,
            *subquerySymbol,
            expression->mutable_subquery()
                ->mutable_set_comparison()
                ->mutable_right());
      }
    }
    case ::substrait::proto::Expression_Subquery::SUBQUERY_TYPE_NOT_SET:
      break;
  }
}

::substrait::proto::Plan SymbolTablePrinter::outputToBinaryPlan(
    const SymbolTable& symbolTable) {
  ::substrait::proto::Plan plan;
  outputExtensionSpacesToBinaryPlan(symbolTable, &plan);
  outputFunctionsToBinaryPlan(symbolTable, &plan);
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kRelation ||
        info.parentQueryLocation != Location::kUnknownLocation) {
      continue;
    }
    auto relationData = ANY_CAST(std::shared_ptr<RelationData>, info.blob);
    if (relationData->pipelineStart != nullptr &&
        *relationData->pipelineStart != SymbolInfo::kUnknown) {
      // This is not the start node for all pipelines it is a part of.
      continue;
    }

    auto relation = plan.add_relations();
    if (relationData->newPipelines.empty()) {
      *relation->mutable_root()->mutable_input() = relationData->relation;
    } else {
      if (relationData->newPipelines[0]->type != SymbolType::kRoot) {
        // This is a root node, copy the first node in before iterating.
        auto inputRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_root()->mutable_input() =
            inputRelationData->relation;

        addInputsToRelation(
            symbolTable,
            *relationData->newPipelines[0],
            relation->mutable_root()->mutable_input());
      }

      const auto& rootSymbol =
          symbolTable.nthSymbolByType(0, SymbolType::kRoot);
      if (rootSymbol != SymbolInfo::kUnknown) {
        const auto& rootNames =
            ANY_CAST(std::vector<std::string>, rootSymbol.blob);
        for (const auto& name : rootNames) {
          relation->mutable_root()->add_names(name);
        }
      }
    }
  }

  return plan;
}

} // namespace io::substrait::textplan
