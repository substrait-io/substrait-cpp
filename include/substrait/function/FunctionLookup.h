/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/function/Extension.h"
#include "substrait/function/FunctionSignature.h"

namespace io::substrait {

class FunctionLookup {
 public:
  explicit FunctionLookup(ExtensionPtr extension)
      : extension_(std::move(extension)) {}

  [[nodiscard]] virtual FunctionImplementationPtr lookupFunction(
      const FunctionSignature& signature) const;

  virtual ~FunctionLookup() = default;

 protected:
  [[nodiscard]] virtual FunctionImplMap getFunctionImpls() const = 0;

  ExtensionPtr extension_{};
};

using FunctionLookupPtr = std::shared_ptr<const FunctionLookup>;

class ScalarFunctionLookup : public FunctionLookup {
 public:
  explicit ScalarFunctionLookup(const ExtensionPtr& extension)
      : FunctionLookup(extension) {}

 protected:
  [[nodiscard]] FunctionImplMap getFunctionImpls() const override {
    return extension_->scalaFunctionImplMap();
  }
};

class AggregateFunctionLookup : public FunctionLookup {
 public:
  explicit AggregateFunctionLookup(const ExtensionPtr& extension)
      : FunctionLookup(extension) {}

 protected:
  [[nodiscard]] FunctionImplMap getFunctionImpls() const override {
    return extension_->aggregateFunctionImplMap();
  }
};

class WindowFunctionLookup : public FunctionLookup {
 public:
  explicit WindowFunctionLookup(const ExtensionPtr& extension)
      : FunctionLookup(extension) {}

 protected:
  [[nodiscard]] FunctionImplMap getFunctionImpls() const override {
    return extension_->windowFunctionImplMap();
  }
};

} // namespace io::substrait
