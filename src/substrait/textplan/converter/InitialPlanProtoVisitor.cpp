/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/textplan/converter/InitialPlanProtoVisitor.h"

#include <fstream>
#include <iterator>
#include <sstream>
#include <string>

#include "substrait/common/Exceptions.h"
#include "substrait/proto/algebra.pb.h"
#include "substrait/proto/plan.pb.h"
#include "substrait/textplan/Location.h"
#include "substrait/textplan/SymbolTable.h"

namespace io::substrait::textplan {

namespace {

std::string ShortName(std::string str) {
  auto loc = str.find(':');
  if (loc != std::string::npos) {
    return str.substr(0, loc);
  }
  return str;
}

} // namespace

std::any InitialPlanProtoVisitor::visitExtension(
    const ::substrait::proto::extensions::SimpleExtensionDeclaration&
        extension) {
  if (extension.mapping_type_case() !=
      ::substrait::proto::extensions::SimpleExtensionDeclaration::
          kExtensionFunction) {
    SUBSTRAIT_FAIL(
        "Unknown mapping type case " +
        std::to_string(extension.mapping_type_case()) + " encountered.");
  }
  const auto& unique_name = symbol_table_->GetUniqueName(
      ShortName(extension.extension_function().name()));
  symbol_table_->DefineSymbol(
      unique_name,
      Location(ProtoLocation()),
      SymbolType::kFunction,
      std::nullopt,
      extension.extension_function());
  return std::nullopt;
}

std::any InitialPlanProtoVisitor::visitExtensionUri(
    const ::substrait::proto::extensions::SimpleExtensionURI& uri) {
  symbol_table_->DefineSymbol(
      uri.uri(),
      Location(ProtoLocation()),
      SymbolType::kExtensionSpace,
      std::nullopt,
      uri.extension_uri_anchor());
  return std::nullopt;
}

} // namespace io::substrait::textplan