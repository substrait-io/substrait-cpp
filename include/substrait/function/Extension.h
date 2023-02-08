/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "substrait/function/Function.h"
#include "substrait/function/FunctionSignature.h"
#include "substrait/type/Type.h"

namespace io::substrait {

struct TypeVariant {
  std::string name;
  std::string uri;
};

using TypeVariantPtr = std::shared_ptr<TypeVariant>;

using FunctionImplMap =
    std::unordered_map<std::string, std::vector<FunctionImplementationPtr>>;

using TypeVariantMap = std::unordered_map<std::string, TypeVariantPtr>;

class Extension {
 public:
  /// Deserialize default substrait extension by given basePath
  /// @throws exception if file not found
  static std::shared_ptr<Extension> load(const std::string& basePath);

  /// Deserialize substrait extension by given basePath and extensionFiles.
  static std::shared_ptr<Extension> load(
      const std::string& basePath,
      const std::vector<std::string>& extensionFiles);

  /// Deserialize substrait extension by given extensionFiles.
  static std::shared_ptr<Extension> load(
      const std::vector<std::string>& extensionFiles);

  /// Add a scalar function implementation.
  void addScalarFunctionImpl(const FunctionImplementationPtr& functionImpl);

  /// Add an aggregate function implementation.
  void addAggregateFunctionImpl(const FunctionImplementationPtr& functionImpl);

  /// Add a window function implementation.
  void addWindowFunctionImpl(const FunctionImplementationPtr& functionImpl);

  /// Add a type variant.
  void addTypeVariant(const TypeVariantPtr& typeVariant);

  /// Lookup type variant by given type name.
  /// @return matched type variant
  TypeVariantPtr lookupType(const std::string& typeName) const;

  const FunctionImplMap& scalaFunctionImplMap() const {
    return scalarFunctionImplMap_;
  }

  const FunctionImplMap& windowFunctionImplMap() const {
    return windowFunctionImplMap_;
  }

  const FunctionImplMap& aggregateFunctionImplMap() const {
    return aggregateFunctionImplMap_;
  }

 private:
  FunctionImplMap scalarFunctionImplMap_;

  FunctionImplMap aggregateFunctionImplMap_;

  FunctionImplMap windowFunctionImplMap_;

  TypeVariantMap typeVariantMap_;
};

using ExtensionPtr = std::shared_ptr<const Extension>;

} // namespace io::substrait
