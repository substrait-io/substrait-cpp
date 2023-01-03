/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/type/Type.h"

namespace io::substrait {

struct FunctionSignature {
  std::string name;
  std::vector<TypePtr> arguments;
  TypePtr returnType;
};

} // namespace io::substrait
