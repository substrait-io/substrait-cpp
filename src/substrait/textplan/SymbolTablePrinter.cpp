/* SPDX-License-Identifier: Apache-2.0 */

#include "SymbolTablePrinter.h"

#include <set>

#include "../../../cmake-build-debug/src/substrait/proto/extensions/extensions.pb.h"
#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

std::string SymbolTablePrinter::OutputToText(const SymbolTable& symbolTable) {
  std::string text;

  text = symbolTable.GetCachedOutput();
  if (!text.empty()) {
    return text;
  }

  std::map<uint32_t, std::string> space_names;
  std::set<uint32_t> used_spaces;

  // Look at the existing spaces.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kExtensionSpace)
      continue;

    auto anchor = std::any_cast<uint32_t>(info.blob);

    space_names.insert(std::make_pair(anchor, info.name));
  }

  // Find any spaces that are used but undefined.
  for (const SymbolInfo& info : symbolTable) {
    if (info.type != SymbolType::kFunction)
      continue;

    auto extension =
        std::any_cast<::substrait::proto::extensions::
                          SimpleExtensionDeclaration_ExtensionFunction>(
            info.blob);
    used_spaces.insert(extension.extension_uri_reference());
  }

  // Finally output the extensions by space in the order they were encountered.
  for (const uint32_t space : used_spaces) {
    if (space_names.find(space) == space_names.end()) {
      // TODO: Handle this case as a warning.
      text += "extension_space {\n";
    } else {
      text += "extension_space " + space_names[space] + " {\n";
    }

    for (const SymbolInfo& info : symbolTable) {
      if (info.type != SymbolType::kFunction)
        continue;

      auto extension =
          std::any_cast<::substrait::proto::extensions::
                            SimpleExtensionDeclaration_ExtensionFunction>(
              info.blob);
      if (extension.extension_uri_reference() != space)
        continue;

      text += "  function " + extension.name() + " as " + info.name + ";\n";
    }
    text += "}\n";
  }

  return text;
}

} // namespace io::substrait::textplan
