/* SPDX-License-Identifier: Apache-2.0 */

#include "SymbolTablePrinter.h"

#include <set>
#include <sstream>

#include "substrait/common/Exceptions.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/extensions/extensions.pb.h"
#include "substrait/textplan/Any.h"
#include "substrait/textplan/RelationData.h"
#include "substrait/textplan/SymbolTable.h"
#include "substrait/textplan/converter/PlanPrinterVisitor.h"

namespace io::substrait::textplan {

namespace {

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
  switch (type.kind_case()) {
    case ::substrait::proto::Type::kBool:
      if (type.bool_().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "bool?";
      }
      return "bool";
    case ::substrait::proto::Type::kI8:
      if (type.i8().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "i8?";
      }
      return "i8";
    case ::substrait::proto::Type::kI16:
      if (type.i16().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "i16?";
      }
      return "i16";
    case ::substrait::proto::Type::kI32:
      if (type.i32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "i32?";
      }
      return "i32";
    case ::substrait::proto::Type::kI64:
      if (type.i64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "i64?";
      }
      return "i64";
    case ::substrait::proto::Type::kFp32:
      if (type.fp32().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "fp32?";
      }
      return "fp32";
    case ::substrait::proto::Type::kFp64:
      if (type.fp64().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "fp64?";
      }
      return "fp64";
    case ::substrait::proto::Type::kString:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "string?";
      }
      return "string";
    case ::substrait::proto::Type::kDecimal:
      if (type.string().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "decimal?";
      }
      return "decimal";
    case ::substrait::proto::Type::kVarchar:
      if (type.varchar().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "varchar?";
      }
      return "varchar";
    case ::substrait::proto::Type::kFixedChar:
      if (type.fixed_char().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "fixedchar?";
      }
      return "fixedchar";
    case ::substrait::proto::Type::kDate:
      if (type.date().nullability() ==
          ::substrait::proto::Type::NULLABILITY_NULLABLE) {
        return "date?";
      }
      return "date";
    case ::substrait::proto::Type::KIND_NOT_SET:
    default:
      return "UNSUPPORTED_TYPE";
  }
};

std::string relationToText(
    const SymbolTable& symbolTable,
    const SymbolInfo& info) {
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, info.blob);

  PlanPrinterVisitor printer(symbolTable);
  return printer.printRelation(relationData->protoAddr);
}

std::vector<std::string> pipelineToPath(
    const SymbolTable& symbolTable,
    const ::substrait::proto::Rel* relation) {
  std::vector<std::string> pipeline;
  auto info = symbolTable.lookupSymbolByLocation(
      Location(static_cast<const google::protobuf::Message*>(relation)));
  if (info == SymbolInfo::kUnknown) {
    return pipeline;
  }
  auto relationData = ANY_CAST(std::shared_ptr<RelationData>, info.blob);
  pipeline.push_back(info.name);
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

std::string outputSchemaSection(const SymbolTable& symbolTable) {
  std::stringstream text;
  bool hasPreviousText = false;
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kSchema) {
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

std::string outputSourceSection(const SymbolTable& symbolTable) {
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
  std::set<uint32_t> usedSpaces;

  // Look at the existing spaces.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kExtensionSpace) {
      continue;
    }

    auto anchor = ANY_CAST(uint32_t, info.blob);

    spaceNames.insert(std::make_pair(anchor, info.name));
  }

  // Find any spaces that are used but undefined.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kFunction) {
      continue;
    }

    auto extension = ANY_CAST(
        const ::substrait::proto::extensions::
            SimpleExtensionDeclaration_ExtensionFunction*,
        info.blob);
    usedSpaces.insert(extension->extension_uri_reference());
  }

  // Finally output the extensions by space in the order they were encountered.
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

    for (const SymbolInfo& info : symbolTable) {
      if (info.type != SymbolType::kFunction) {
        continue;
      }

      auto extension = ANY_CAST(
          const ::substrait::proto::extensions::
              SimpleExtensionDeclaration_ExtensionFunction*,
          info.blob);
      if (extension->extension_uri_reference() != space) {
        continue;
      }

      text << "  function " << extension->name() << " as " << info.name
           << ";\n";
    }
    text << "}\n";
    hasPreviousOutput = true;
  }

  return text.str();
}

} // namespace

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

  newText = outputSchemaSection(symbolTable);
  if (!newText.empty()) {
    if (hasPreviousText) {
      text << "\n";
    }
    text << newText;
    hasPreviousText = true;
  }

  newText = outputSourceSection(symbolTable);
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

} // namespace io::substrait::textplan
