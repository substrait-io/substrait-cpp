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

#include "FunctionSignature.h"
#include "Type.h"
#include <iostream>

namespace io::substrait {

struct FunctionArgument {
  virtual const bool isRequired() const = 0;
  /// Convert argument type to short type string based on
  /// https://substrait.io/extensions/#function-signature-compound-names
  virtual const std::string toTypeString() const = 0;

  virtual const bool isWildcardType() const { return false; };

  virtual const bool isValueArgument() const { return false; }
};

using FunctionArgumentPtr = std::shared_ptr<FunctionArgument>;

struct EnumArgument : public FunctionArgument {
  bool required;
  bool const isRequired() const override { return required; }

  const std::string toTypeString() const override {
    return required ? "req" : "opt";
  }
};

struct TypeArgument : public FunctionArgument {
  const std::string toTypeString() const override { return "type"; }
  const bool isRequired() const override { return true; }
};

struct ValueArgument : public FunctionArgument {
  TypePtr type;

  const std::string toTypeString() const override { return type->signature(); }

  const bool isRequired() const override { return true; }

  const bool isWildcardType() const override { return type->isWildcard(); }

  const bool isValueArgument() const override { return true; }
};

struct FunctionVariadic {
  int min;
  std::optional<int> max;
};

struct FunctionVariant {
  std::string name;
  std::string uri;
  std::vector<FunctionArgumentPtr> arguments;
  TypePtr returnType;
  std::optional<FunctionVariadic> variadic;

  /// Test if the actual types matched with this function variant.
  virtual bool tryMatch(const FunctionSignature& signature);

  /// Create function signature by given function name and arguments.
  static std::string
  signature(const std::string &name,
            const std::vector<FunctionArgumentPtr> &arguments);

  /// Create function signature by function name and arguments.
  const std::string signature() const { return signature(name, arguments); }
};

using FunctionVariantPtr = std::shared_ptr<FunctionVariant>;

struct ScalarFunctionVariant : public FunctionVariant {};

struct AggregateFunctionVariant : public FunctionVariant {
  TypePtr intermediate;

  bool tryMatch(const FunctionSignature& signature) override;
};

} // namespace io::substrait
