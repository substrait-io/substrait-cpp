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

#include "Function.h"
#include "Type.h"

namespace io::substrait {

struct TypeVariant {
  std::string name;
  std::string uri;
};

using TypeVariantPtr = std::shared_ptr<TypeVariant>;

using FunctionVariantMap =
    std::unordered_map<std::string, std::vector<FunctionVariantPtr>>;

using TypeVariantMap = std::unordered_map<std::string, TypeVariantPtr>;

class Extension {
public:
  /// deserialize default substrait extension.
  static std::shared_ptr<Extension> load();

  /// deserialize substrait extension by given basePath and extensionFiles.
  static std::shared_ptr<Extension>
  load(const std::string &basePath,
       const std::vector<std::string> &extensionFiles);

  /// deserialize substrait extension by given extensionFiles.
  static std::shared_ptr<Extension>
  load(const std::vector<std::string> &extensionFiles);

  /// Add a function variant.
  void addFunctionVariant(const FunctionVariantPtr &functionVariant);

  /// Add a type variant.
  void addTypeVariant(const TypeVariantPtr &functionVariant);

  /// Lookup function variant by given function name and function types.
  /// @return matched function variant.
  FunctionVariantPtr lookupFunction(const std::string &name,
                                    const std::vector<TypePtr> &types) const;

  /// Lookup type variant by given type name.
  /// @return matched type variant
  TypeVariantPtr lookupType(const std::string &typeName) const;

private:
  /// Deserialize default substrait extension.
  static std::shared_ptr<Extension> loadDefault();
  /// Function variants loaded in registry.
  FunctionVariantMap functionVariantMap_;
  /// Type variants loaded in registry.
  TypeVariantMap typeVariantMap_;
};

using ExtensionPtr = std::shared_ptr<const Extension>;

} // namespace io::substrait
