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

#include <utility>

#include "substrait/function/Extension.h"
#include "substrait/function/FunctionMapping.h"
#include "substrait/function/FunctionSignature.h"

namespace io::substrait {

class FunctionLookup {
 public:
  FunctionLookup(ExtensionPtr extension, FunctionMappingPtr functionMapping)
      : extension_(std::move(extension)),
        functionMapping_(std::move(functionMapping)) {}

  [[nodiscard]] virtual FunctionVariantPtr lookupFunction(
      const FunctionSignature& signature) const;

  virtual ~FunctionLookup() = default;

 protected:
  [[nodiscard]] virtual FunctionMap getFunctionMap() const = 0;

  [[nodiscard]] virtual FunctionVariantMap getFunctionVariants() const = 0;

  const FunctionMappingPtr functionMapping_;

  ExtensionPtr extension_{};
};

using FunctionLookupPtr = std::shared_ptr<const FunctionLookup>;

class ScalarFunctionLookup : public FunctionLookup {
 public:
  ScalarFunctionLookup(
      const ExtensionPtr& extension,
      const FunctionMappingPtr& functionMapping)
      : FunctionLookup(extension, functionMapping) {}

 protected:
  [[nodiscard]] FunctionMap getFunctionMap() const override {
    return functionMapping_->scalaMapping();
  }

  [[nodiscard]] FunctionVariantMap getFunctionVariants() const override {
    return extension_->scalaFunctionVariantMap();
  }
};

class AggregateFunctionLookup : public FunctionLookup {
 public:
  AggregateFunctionLookup(
      const ExtensionPtr& extension,
      const FunctionMappingPtr& functionMapping)
      : FunctionLookup(extension, functionMapping) {}

 protected:
  [[nodiscard]] FunctionMap getFunctionMap() const override {
    return functionMapping_->aggregateMapping();
  }

  [[nodiscard]] FunctionVariantMap getFunctionVariants() const override {
    return extension_->aggregateFunctionVariantMap();
  }
};

class WindowFunctionLookup : public FunctionLookup {
 public:
  WindowFunctionLookup(
      const ExtensionPtr& extension,
      const FunctionMappingPtr& functionMapping)
      : FunctionLookup(extension, functionMapping) {}

 protected:
  [[nodiscard]] FunctionMap getFunctionMap() const override {
    return functionMapping_->windowMapping();
  }

  [[nodiscard]] FunctionVariantMap getFunctionVariants() const override {
    return extension_->windowFunctionVariantMap();
  }
};

} // namespace io::substrait
