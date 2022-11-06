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

#include "substrait/function/Function.h"
#include <sstream>

namespace io::substrait {

std::string FunctionVariant::signature(
    const std::string& name,
    const std::vector<FunctionArgumentPtr>& arguments) {
  std::stringstream ss;
  ss << name;
  if (!arguments.empty()) {
    ss << ":";
    for (auto it = arguments.begin(); it != arguments.end(); ++it) {
      const auto& typeSign = (*it)->toTypeString();
      if (it == arguments.end() - 1) {
        ss << typeSign;
      } else {
        ss << typeSign << "_";
      }
    }
  }

  return ss.str();
}

bool FunctionVariant::tryMatch(const FunctionSignature& signature) {
  const auto& actualTypes = signature.arguments;
  if (variadic.has_value()) {
    // return false if actual types length less than min of variadic
    const auto max = variadic->max;
    if ((actualTypes.size() < variadic->min) ||
        (max.has_value() && actualTypes.size() > max.value())) {
      return false;
    }

    const auto& variadicArgument = arguments[0];
    // actual type must same as the variadicArgument
    if (const auto& variadicValueArgument =
            std::dynamic_pointer_cast<const ValueArgument>(variadicArgument)) {
      for (auto& actualType : actualTypes) {
        if (!variadicValueArgument->type->isMatch(actualType)) {
          return false;
        }
      }
    }
  } else {
    std::vector<std::shared_ptr<const ValueArgument>> valueArguments;
    for (const auto& argument : arguments) {
      if (const auto& variadicValueArgument =
              std::dynamic_pointer_cast<const ValueArgument>(argument)) {
        valueArguments.emplace_back(variadicValueArgument);
      }
    }
    // return false if size of actual types not equal to size of value
    // arguments.
    if (valueArguments.size() != actualTypes.size()) {
      return false;
    }

    for (auto i = 0; i < actualTypes.size(); i++) {
      const auto& valueArgument = valueArguments[i];
      if (!valueArgument->type->isMatch(actualTypes[i])) {
        return false;
      }
    }
  }
  const auto& sigReturnType = signature.returnType;
  if (this->returnType && sigReturnType) {
    return returnType->isMatch(sigReturnType);
  } else {
    return true;
  }
}

bool AggregateFunctionVariant::tryMatch(const FunctionSignature& signature) {
  bool matched = FunctionVariant::tryMatch(signature);
  if (!matched && intermediate) {
    const auto& actualTypes = signature.arguments;
    if (actualTypes.size() == 1) {
      return intermediate->isMatch(actualTypes[0]);
    }
  }
  return matched;
}

} // namespace io::substrait
