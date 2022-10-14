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

#pragma once

#include "Extension.h"
#include "FunctionMapping.h"

namespace io::substrait {

class FunctionLookup {
public:
  FunctionLookup(const ExtensionPtr &extension,
                 const FunctionMappingPtr &functionMapping)
      : extension_(extension), functionMapping_(functionMapping) {}

  FunctionVariantPtr
  lookupScalarFunction(const std::string &functionName,
                       const std::vector<TypePtr> &types) const;

  FunctionVariantPtr
  lookupAggregateFunction(const std::string &functionName,
                          const std::vector<TypePtr> &types) const;

  FunctionVariantPtr
  lookupWindowFunction(const std::string &functionName,
                       const std::vector<TypePtr> &types) const;

private:
  ExtensionPtr extension_;
  FunctionMappingPtr functionMapping_;
};

using FunctionLookupPtr = std::shared_ptr<const FunctionLookup>;

} // namespace io::substrait
