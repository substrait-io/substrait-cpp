/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "substrait/function/Extension.h"
#include "substrait/function/FunctionSignature.h"

namespace substrait {

class FunctionLookup {
 public:
  explicit FunctionLookup(ExtensionPtr extension)
      : extension_(std::move(extension)) {}

  [[nodiscard]] virtual FunctionImplementationPtr lookupFunction(
      const FunctionSignature& signature) const;

  virtual ~FunctionLookup() = default;

 protected:
  [[nodiscard]] virtual FunctionVariantMap getFunctionVariants() const = 0;

  ExtensionPtr extension_{};
};

using FunctionLookupPtr = std::shared_ptr<const FunctionLookup>;

class ScalarFunctionLookup : public FunctionLookup {
 public:
  ScalarFunctionLookup(const ExtensionPtr& extension)
      : FunctionLookup(extension) {}

 protected:
  [[nodiscard]] FunctionVariantMap getFunctionVariants() const override {
    return extension_->scalaFunctionVariantMap();
  }
};

class AggregateFunctionLookup : public FunctionLookup {
 public:
  explicit AggregateFunctionLookup(const ExtensionPtr& extension)
      : FunctionLookup(extension) {}

 protected:
  [[nodiscard]] FunctionVariantMap getFunctionVariants() const override {
    return extension_->aggregateFunctionVariantMap();
  }
};

class WindowFunctionLookup : public FunctionLookup {
 public:
  explicit WindowFunctionLookup(const ExtensionPtr& extension)
      : FunctionLookup(extension) {}

 protected:
  [[nodiscard]] FunctionVariantMap getFunctionVariants() const override {
    return extension_->windowFunctionVariantMap();
  }
};

} // namespace substrait
