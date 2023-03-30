/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/function/FunctionLookup.h"

namespace io::substrait {

FunctionImplementationPtr FunctionLookup::lookupFunction(
    const FunctionSignature& signature) const {
  const auto& functionImpls = getFunctionImpls();
  auto functionImplsIter = functionImpls.find(signature.name);
  if (functionImplsIter != functionImpls.end()) {
    for (const auto& candidateFunctionImpl : functionImplsIter->second) {
      if (candidateFunctionImpl->tryMatch(signature)) {
        return candidateFunctionImpl;
      }
    }
  }
  return nullptr;
}

} // namespace io::substrait
