/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/SymbolTablePrinter.h"

#include <set>
#include <sstream>

#include "substrait/common/Exceptions.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/extensions/extensions.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/StructuredSymbolData.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/PlanPrinterVisitor.h"

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
    const SymbolInfo& info) {
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, info.blob);
  if (relationData->relation.rel_type_case() ==
      ::substrait::proto::Rel::REL_TYPE_NOT_SET) {
    // The relation isn't defined, so we don't have anything to print.
    return "";
  }

  PlanPrinterVisitor printer(symbolTable);
  return printer.printRelation(info);
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
  }
  if (hasPreviousText) {
    return "pipelines {\n" + text.str() + "}\n";
  }
  return text.str();
}

std::string outputRelationsSection(const SymbolTable& symbolTable) {
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
    text << relationToText(symbolTable, info);
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
        auto table =
            ANY_CAST(const ::substrait::proto::ReadRel_NamedTable*, info.blob);
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
std::string SymbolTablePrinter::outputToText(const SymbolTable& symbolTable) {
  std::stringstream text;
  bool hasPreviousText = false;

  std::string newText = outputPipelinesSection(symbolTable);
  if (!newText.empty()) {
    text << newText;
    hasPreviousText = true;
  }

  newText = outputRelationsSection(symbolTable);
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
    const SymbolInfo& symbolInfo,
    ::substrait::proto::Rel* relation) {
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, symbolInfo.blob);

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
    case RelationType::kProject:
      if (relationData->continuingPipeline != nullptr) {
        auto continuingRelationData = ANY_CAST(
            std::shared_ptr<RelationData>,
            relationData->continuingPipeline->blob);
        *relation->mutable_project()->mutable_input() =
            continuingRelationData->relation;
        addInputsToRelation(
            *relationData->continuingPipeline,
            relation->mutable_project()->mutable_input());
      }
      break;
    case RelationType::kJoin:
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_join()->mutable_left() = leftRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[0],
            relation->mutable_join()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_join()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[1],
            relation->mutable_join()->mutable_right());
      }
      break;
    case RelationType::kCross:
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_cross()->mutable_left() = leftRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[0],
            relation->mutable_cross()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_cross()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[1],
            relation->mutable_cross()->mutable_right());
      }
      break;
    case RelationType::kFetch:
      if (relationData->continuingPipeline != nullptr) {
        auto continuingRelationData = ANY_CAST(
            std::shared_ptr<RelationData>,
            relationData->continuingPipeline->blob);
        *relation->mutable_fetch()->mutable_input() =
            continuingRelationData->relation;
        addInputsToRelation(
            *relationData->continuingPipeline,
            relation->mutable_fetch()->mutable_input());
      }
      break;
    case RelationType::kAggregate:
      if (relationData->continuingPipeline != nullptr) {
        auto continuingRelationData = ANY_CAST(
            std::shared_ptr<RelationData>,
            relationData->continuingPipeline->blob);
        *relation->mutable_aggregate()->mutable_input() =
            continuingRelationData->relation;
        addInputsToRelation(
            *relationData->continuingPipeline,
            relation->mutable_aggregate()->mutable_input());
      }
      break;
    case RelationType::kSort:
      if (relationData->continuingPipeline != nullptr) {
        auto continuingRelationData = ANY_CAST(
            std::shared_ptr<RelationData>,
            relationData->continuingPipeline->blob);
        *relation->mutable_sort()->mutable_input() =
            continuingRelationData->relation;
        addInputsToRelation(
            *relationData->continuingPipeline,
            relation->mutable_sort()->mutable_input());
      }
      break;
    case RelationType::kFilter:
      if (relationData->continuingPipeline != nullptr) {
        auto continuingRelationData = ANY_CAST(
            std::shared_ptr<RelationData>,
            relationData->continuingPipeline->blob);
        *relation->mutable_filter()->mutable_input() =
            continuingRelationData->relation;
        addInputsToRelation(
            *relationData->continuingPipeline,
            relation->mutable_filter()->mutable_input());
      }
      break;
    case RelationType::kSet:
      for (const auto& pipeline : relationData->newPipelines) {
        auto inputRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, pipeline->blob);
        auto* input = relation->mutable_set()->add_inputs();
        *input = inputRelationData->relation;
        addInputsToRelation(*pipeline, input);
      }
      break;
    case RelationType::kHashJoin:
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_hash_join()->mutable_left() =
            leftRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[0],
            relation->mutable_hash_join()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_hash_join()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[1],
            relation->mutable_hash_join()->mutable_right());
      }
      break;
    case RelationType::kMergeJoin:
      if (relationData->newPipelines.size() == kBinaryRelationInputCount) {
        auto leftRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[0]->blob);
        *relation->mutable_merge_join()->mutable_left() =
            leftRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[0],
            relation->mutable_merge_join()->mutable_left());

        auto rightRelationData = ANY_CAST(
            std::shared_ptr<RelationData>, relationData->newPipelines[1]->blob);
        *relation->mutable_merge_join()->mutable_right() =
            rightRelationData->relation;
        addInputsToRelation(
            *relationData->newPipelines[1],
            relation->mutable_merge_join()->mutable_right());
      }
      break;
    case RelationType::kExchange:
    case RelationType::kDdl:
    case RelationType::kWrite:
      // Not yet possible to reach these relations in plans.
      break;
    case RelationType::kExtensionLeaf:
      break;
    case RelationType::kExtensionSingle:
      if (relationData->continuingPipeline != nullptr) {
        auto continuingRelationData = ANY_CAST(
            std::shared_ptr<RelationData>,
            relationData->continuingPipeline->blob);
        *relation->mutable_extension_single()->mutable_input() =
            continuingRelationData->relation;
        addInputsToRelation(
            *relationData->continuingPipeline,
            relation->mutable_extension_single()->mutable_input());
      }
      break;
    case RelationType::kExtensionMulti:
      for (const auto& pipeline : relationData->newPipelines) {
        auto inputRelationData =
            ANY_CAST(std::shared_ptr<RelationData>, pipeline->blob);
        auto* input = relation->mutable_extension_multi()->add_inputs();
        *input = inputRelationData->relation;
        addInputsToRelation(*pipeline, input);
      }
      break;
    case RelationType::kUnknown:
      break;
  }
}

::substrait::proto::Plan SymbolTablePrinter::outputToBinaryPlan(
    const SymbolTable& symbolTable) {
  ::substrait::proto::Plan plan;
  outputExtensionSpacesToBinaryPlan(symbolTable, &plan);
  outputFunctionsToBinaryPlan(symbolTable, &plan);
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kRelation) {
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
