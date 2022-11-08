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

#include "substrait/type/Type.h"
#include "substrait/function/FunctionSignature.h"

namespace substrait {

struct FunctionArgument {
  [[nodiscard]] virtual bool isRequired() const = 0;

  /// Convert argument type to short type string based on
  /// https://substrait.io/extensions/#function-signature-compound-names
  [[nodiscard]] virtual std::string toTypeString() const = 0;

  [[nodiscard]] virtual bool isWildcardType() const {
    return false;
  };

  [[nodiscard]] virtual bool isValueArgument() const {
    return false;
  }

  [[nodiscard]] virtual bool isEnumArgument() const {
    return false;
  }

  [[nodiscard]] virtual bool isTypeArgument() const {
    return false;
  }
};

using FunctionArgumentPtr = std::shared_ptr<FunctionArgument>;

struct EnumArgument : public FunctionArgument {
  bool required{};

  [[nodiscard]] bool isRequired() const override {
    return required;
  }

  [[nodiscard]] std::string toTypeString() const override {
    return required ? "req" : "opt";
  }

  [[nodiscard]] bool isEnumArgument() const override {
    return true;
  }
};

struct TypeArgument : public FunctionArgument {
  [[nodiscard]] std::string toTypeString() const override {
    return "type";
  }

  [[nodiscard]] bool isRequired() const override {
    return true;
  }

  [[nodiscard]] bool isTypeArgument() const override {
    return true;
  }
};

struct ValueArgument : public FunctionArgument {
  ParameterizedTypePtr type;

  [[nodiscard]] std::string toTypeString() const override {
    return type->signature();
  }

  [[nodiscard]] bool isRequired() const override {
    return true;
  }

  [[nodiscard]] bool isWildcardType() const override {
    return type->isWildcard();
  }

  [[nodiscard]] bool isValueArgument() const override {
    return true;
  }
};

struct FunctionVariadic {
  int min;
  std::optional<int> max;
};

struct FunctionImplementation {
  std::string name;
  std::string uri;
  std::vector<FunctionArgumentPtr> arguments;
  ParameterizedTypePtr returnType;
  std::optional<FunctionVariadic> variadic;

  /// Test if the actual types matched with this function variant.
  virtual bool tryMatch(const FunctionSignature& signature);

  /// Create function signature by function name and arguments.
  [[nodiscard]] std::string signature() const;
};

using FunctionImplementationPtr = std::shared_ptr<FunctionImplementation>;

struct ScalarFunctionImplementation : public FunctionImplementation {};

struct AggregateFunctionImplementation : public FunctionImplementation {
  ParameterizedTypePtr intermediate;
  bool deterministic;

  bool tryMatch(const FunctionSignature& signature) override;
};

} // namespace substrait
