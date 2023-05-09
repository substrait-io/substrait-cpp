/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <memory>
#include <optional>

#include "substrait/proto/algebra.pb.h"

namespace io::substrait::textplan {

class SymbolInfo;

// Used by the PlanRelation and Relation to track connectivity.
struct RelationData {
  RelationData(
      const ::substrait::proto::Rel* relation,
      const ::google::protobuf::Message* origin)
      : protoAddr(relation), originProtoAddr(origin) {
    continuingPipeline = nullptr;
  };

  const ::substrait::proto::Rel* protoAddr;
  const ::google::protobuf::Message* originProtoAddr;

  const ::substrait::proto::Rel* continuingPipeline;
  std::vector<const ::substrait::proto::Rel*> newPipelines;

  // Column name for each field known to this relation (in field order).
  std::vector<const SymbolInfo*> fieldReferences;
};

// Used by Schema symbols to keep track of assigned values.
struct SchemaData {
  explicit SchemaData(uint32_t anchor_reference)
      : anchorReference(anchor_reference){};

  uint32_t anchorReference;
};

// Used by Extension Space symbols to keep track of assigned values.
struct ExtensionSpaceData {
  explicit ExtensionSpaceData(uint32_t anchorReference)
      : anchorReference(anchorReference){};

  uint32_t anchorReference;
};

// Used by Function symbols to keep track of the name and assigned anchors.
struct FunctionData {
  FunctionData(
      std::string name,
      std::optional<uint32_t> extensionUriReference,
      uint32_t anchor)
      : name(std::move(name)),
        extensionUriReference(extensionUriReference),
        anchor(anchor){};

  std::string name;
  std::optional<uint32_t> extensionUriReference;
  uint32_t anchor;
};

} // namespace io::substrait::textplan
