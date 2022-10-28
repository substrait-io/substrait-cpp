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
#include "FunctionSignature.h"

namespace io::substrait {

class FunctionLookup {
 public:
  FunctionLookup(
      const ExtensionPtr& extension,
      const FunctionMappingPtr& functionMapping)
      : extension_(extension), functionMapping_(functionMapping) {}

  virtual FunctionVariantPtr lookupFunction(
      const FunctionSignature& signature) const;

  virtual ~FunctionLookup() {}

 protected:
  virtual const FunctionMap getFunctionMap() const = 0;

  virtual const FunctionVariantMap& getFunctionVariants() const = 0;

  const FunctionMappingPtr functionMapping_;

  ExtensionPtr extension_;
};

using FunctionLookupPtr = std::shared_ptr<const FunctionLookup>;

class ScalarFunctionLookup : public FunctionLookup {
 public:
  ScalarFunctionLookup(
      const ExtensionPtr& extension,
      const FunctionMappingPtr& functionMapping)
      : FunctionLookup(extension, functionMapping) {}

 protected:
  const FunctionMap getFunctionMap() const override {
    return functionMapping_->scalaMapping();
  }

  const FunctionVariantMap& getFunctionVariants() const override {
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
  const FunctionMap getFunctionMap() const override {
    return functionMapping_->aggregateMapping();
  }

  const FunctionVariantMap& getFunctionVariants() const override {
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
  const FunctionMap getFunctionMap() const override {
    return functionMapping_->windowMapping();
  }

  const FunctionVariantMap& getFunctionVariants() const override {
    return extension_->windowFunctionVariantMap();
  }
};

} // namespace io::substrait
