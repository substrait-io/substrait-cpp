/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/proto/ProtoUtils.h"

namespace substrait::proto {

std::string planRelTypeCaseName(::substrait::proto::PlanRel::RelTypeCase num) {
  static std::vector<std::string> caseNames = {
      "unknown",
      "rel",
      "root",
  };

  if (num >= caseNames.size()) {
    return "unknown";
  }

  return caseNames[num];
}

std::string relTypeCaseName(::substrait::proto::Rel::RelTypeCase num) {
  static std::vector<std::string> caseNames = {
      "unknown",
      "read",
      "filter",
      "fetch",
      "aggregate",
      "sort",
      "join",
      "project",
      "set",
      "extensionsingle",
      "extensionmulti",
      "extensionleaf",
      "cross",
      "hashjoin",
      "mergejoin",
  };

  if (num >= caseNames.size()) {
    return "unknown";
  }

  return caseNames[num];
}

} // namespace substrait::proto
