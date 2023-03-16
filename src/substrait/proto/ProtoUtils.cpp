/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/proto/ProtoUtils.h"

namespace substrait::proto {

std::string PlanRelTypeCaseName(::substrait::proto::PlanRel::RelTypeCase num) {
  static std::vector<std::string> case_names = {
      "unknown",
      "rel",
      "root",
  };

  if (num >= case_names.size()) {
    return "unknown";
  }

  return case_names[num];
}

std::string RelTypeCaseName(::substrait::proto::Rel::RelTypeCase num) {
  static std::vector<std::string> case_names = {
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

  if (num >= case_names.size()) {
    return "unknown";
  }

  return case_names[num];
}

} // namespace substrait::proto