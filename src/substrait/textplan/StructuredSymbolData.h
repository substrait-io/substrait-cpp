/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <memory>
#include <optional>

#include "substrait/proto/algebra.pb.h"
#include "substrait/textplan/Location.h"

namespace io::substrait::textplan {

class SymbolInfo;

// Used by by the PlanRelation and Relation concepts to track connectivity.
struct RelationData {
  // For relations not starting a pipeline, this is the relation that begins the
  // pipeline that this relation is part of.  For relations starting a pipeline
  // this value will either be set to the start of a pipeline that the node also
  // participates in or it will be nullptr if it's the start of a pipeline and
  // does not participate in any other pipelines.
  const SymbolInfo* pipelineStart{nullptr};

  // The next node in the pipeline that this node is part of.
  const SymbolInfo* continuingPipeline{nullptr};
  // The next nodes in the pipelines that this node starts.
  std::vector<const SymbolInfo*> newPipelines;

  // The information corresponding to the relation without any references to
  // other relations or inputs.
  ::substrait::proto::Rel relation;

  // Source stores the input symbol of a read relation.
  const SymbolInfo* source{nullptr};
  // Schema keeps track schema used in this relation.
  const SymbolInfo* schema{nullptr};

  // Column name for each field known to this relation (in field order).  Used
  // to determine what fields are coming in as well and fields are going out.
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
