/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FunctionLookup.h"

namespace io::substrait {

FunctionVariantPtr
FunctionLookup::lookupFunction(const FunctionSignature &signature) const {
  const auto &functionMappings = getFunctionMap();

  const auto &substraitFunctionName =
      functionMappings.find(signature.getName()) != functionMappings.end()
          ? functionMappings.at(signature.getName())
          : signature.getName();

  const auto &functionVariants = getFunctionVariants();
  auto functionVariantIter = functionVariants.find(substraitFunctionName);
  if (functionVariantIter != functionVariants.end()) {
    for (const auto &candidateFunctionVariant : functionVariantIter->second) {
      if (candidateFunctionVariant->tryMatch(signature)) {
        return candidateFunctionVariant;
      }
    }
  }
  return nullptr;
}

} // namespace io::substrait
