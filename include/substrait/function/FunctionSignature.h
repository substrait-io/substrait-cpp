/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/type/Type.h"

namespace substrait {

struct FunctionSignature {
  std::string name;
  std::vector<TypePtr> arguments;
  TypePtr returnType;
};

} // namespace substrait
