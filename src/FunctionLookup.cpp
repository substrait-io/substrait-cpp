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
FunctionLookup::lookupScalarFunction(const std::string &functionName,
                                     const std::vector<TypePtr> &types) const {
  const auto &functionMappings = functionMapping_->scalaMapping();
  const auto &substraitFunctionName =
      functionMappings.find(functionName) != functionMappings.end()
          ? functionMappings.at(functionName)
          : functionName;
  return extension_->lookupScalarFunction(substraitFunctionName, types);
}

FunctionVariantPtr FunctionLookup::lookupAggregateFunction(
    const std::string &functionName, const std::vector<TypePtr> &types) const {
  const auto &functionMappings = functionMapping_->aggregateMapping();
  const auto &substraitFunctionName =
      functionMappings.find(functionName) != functionMappings.end()
          ? functionMappings.at(functionName)
          : functionName;
  return extension_->lookupAggregateFunction(substraitFunctionName, types);
}

FunctionVariantPtr
FunctionLookup::lookupWindowFunction(const std::string &functionName,
                                     const std::vector<TypePtr> &types) const {
  const auto &functionMappings = functionMapping_->windowMapping();
  const auto &substraitFunctionName =
      functionMappings.find(functionName) != functionMappings.end()
          ? functionMappings.at(functionName)
          : functionName;
  return extension_->lookupWindowFunction(substraitFunctionName, types);
}

} // namespace io::substrait
