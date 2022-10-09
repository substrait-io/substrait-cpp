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

#include <iostream>
#include "SubstraitType.h"

namespace io::substrait {

struct SubstraitFunctionArgument {
  /// whether the argument is required or not.
  virtual const bool isRequired() const = 0;
  /// convert argument type to short type string based on
  /// https://substrait.io/extensions/#function-signature-compound-names
  virtual const std::string toTypeString() const = 0;

  virtual const bool isWildcardType() const {
    return false;
  };

  virtual const bool isValueArgument() const {
    return false;
  }
};

using SubstraitFunctionArgumentPtr = std::shared_ptr<SubstraitFunctionArgument>;

struct SubstraitEnumArgument : public SubstraitFunctionArgument {
  bool required;
  bool const isRequired() const override {
    return required;
  }

  const std::string toTypeString() const override {
    return required ? "req" : "opt";
  }
};

struct SubstraitTypeArgument : public SubstraitFunctionArgument {
  const std::string toTypeString() const override {
    return "type";
  }
  const bool isRequired() const override {
    return true;
  }
};

struct SubstraitValueArgument : public SubstraitFunctionArgument {
  SubstraitTypePtr type;

  const std::string toTypeString() const override {
    return type->signature();
  }

  const bool isRequired() const override {
    return true;
  }

  const bool isWildcardType() const override {
    return type->isWildcard();
  }

  const bool isValueArgument() const override {
    return true;
  }
};

struct SubstraitFunctionVariadic {
  int min;
  std::optional<int> max;
};

struct SubstraitFunctionVariant {
  /// function name.
  std::string name;
  /// function uri.
  std::string uri;
  /// function arguments.
  std::vector<SubstraitFunctionArgumentPtr> arguments;
  /// return type.
  SubstraitTypePtr returnType;
  /// function variadic
  std::optional<SubstraitFunctionVariadic> variadic;

  ///test if the actual types matched with this function variant.
  virtual bool tryMatch(const std::vector<SubstraitTypePtr>& actualTypes);

  /// create function signature by given function name and arguments.
  static std::string signature(
      const std::string& name,
      const std::vector<SubstraitFunctionArgumentPtr>& arguments);

  /// create function signature by function name and arguments.
  const std::string signature() const {
    return signature(name, arguments);
  }
};

using SubstraitFunctionVariantPtr = std::shared_ptr<SubstraitFunctionVariant>;

struct SubstraitScalarFunctionVariant : public SubstraitFunctionVariant {
};

struct SubstraitAggregateFunctionVariant : public SubstraitFunctionVariant {
  SubstraitTypePtr intermediate;

  bool tryMatch(const std::vector<SubstraitTypePtr>& actualTypes) override;
};

} // namespace io::substrait
