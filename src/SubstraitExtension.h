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

#include "SubstraitFunction.h"
#include "SubstraitType.h"

namespace io::substrait {

struct SubstraitTypeVariant {
  std::string name;
  std::string uri;
};

using SubstraitTypeVariantPtr = std::shared_ptr<SubstraitTypeVariant>;

using FunctionVariantMap =
    std::unordered_map<std::string, std::vector<SubstraitFunctionVariantPtr>>;

using TypeVariantMap = std::unordered_map<std::string, SubstraitTypeVariantPtr>;

class SubstraitExtension {
 public:
  /// deserialize default substrait extension.
  static std::shared_ptr<SubstraitExtension> load();

  /// deserialize substrait extension by given basePath and extensionFiles.
  static std::shared_ptr<SubstraitExtension> load(
      const std::string& basePath,
      const std::vector<std::string>& extensionFiles);

  /// deserialize substrait extension by given extensionFiles.
  static std::shared_ptr<SubstraitExtension> load(
      const std::vector<std::string>& extensionFiles);

  /// Add a function variant
  void addFunctionVariant(const SubstraitFunctionVariantPtr& functionVariant);

  /// Add a type variant
  void addTypeVariant(const SubstraitTypeVariantPtr& functionVariant);

  /// lookup function variant by given function name and function types.
  /// @return matched function variant
  SubstraitFunctionVariantPtr lookupFunction(
      const std::string& name,
      const std::vector<SubstraitTypePtr>& types) const;

  /// lookup type variant by given type name.
  /// @return matched type variant
  SubstraitTypeVariantPtr lookupType(const std::string& typeName) const;

 private:
  /// deserialize default substrait extension.
  static std::shared_ptr<SubstraitExtension> loadDefault();
  /// function variants loaded in registry.
  FunctionVariantMap functionVariantMap_;
  /// type variants loaded in registry.
  TypeVariantMap typeVariantMap_;
};

using SubstraitExtensionPtr = std::shared_ptr<const SubstraitExtension>;

} // namespace io::substrait
