/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/function/FunctionLookup.h"

namespace io::substrait {

FunctionImplementationPtr FunctionLookup::lookupFunction(
    const FunctionSignature& signature) const {

  const auto& functionImpls = getFunctionVariants();
  auto functionImplsIter = functionImpls.find(signature.name);
  if (functionImplsIter != functionImpls.end()) {
    for (const auto& candidateFunctionVariant : functionImplsIter->second) {
      if (candidateFunctionVariant->tryMatch(signature)) {
        return candidateFunctionVariant;
      }
    }
  }
  return nullptr;
}

} // namespace io::substrait
