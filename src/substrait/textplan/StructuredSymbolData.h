/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <memory>
#include <optional>

#include "substrait/proto/algebra.pb.h"

namespace io::substrait::textplan {

class SymbolInfo;

// Used by the PlanRelation and Relation concepts to track connectivity.
struct RelationData {
  // Keeps track of the first node in a pipeline.  For relations starting a
  // pipeline this will not be a self-reference -- it will be nullptr unless
  // it is in another pipeline (which in that case the value will be the node
  // that starts that pipeline).  As such this will only have nullptr as a value
  // when it is a root node.
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

  // Each field reference here was generated within the current relation.
  std::vector<const SymbolInfo*> generatedFieldReferences;

  // Local aliases for field references in this relation.  Used to replace the
  // normal form symbols would take for this relation's use only.  (Later
  // references to the symbol would use the alias.)
  std::map<size_t, std::string> generatedFieldReferenceAlternativeExpression;

  // If populated, supersedes the combination of fieldReferences and
  // generatedFieldReferences for the field symbols exposed by this relation.
  std::vector<const SymbolInfo*> outputFieldReferences;

  // Contains the field reference names seen so far while processing this
  // relation along with the id of the first occurrence.  Used to detect when
  // fully qualified references are necessary.
  std::map<std::string, size_t> seenFieldReferenceNames;
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
