/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/function/Function.h"
#include <sstream>

namespace io::substrait {

bool FunctionImplementation::tryMatch(const FunctionSignature& signature) {
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
      for (const auto& actualType : actualTypes) {
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

std::string FunctionImplementation::signature() const {
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

bool AggregateFunctionImplementation::tryMatch(
    const FunctionSignature& signature) {
  bool matched = FunctionImplementation::tryMatch(signature);
  if (!matched && intermediate) {
    const auto& actualTypes = signature.arguments;
    if (actualTypes.size() == 1) {
      return intermediate->isMatch(actualTypes[0]);
    }
  }
  return matched;
}

} // namespace io::substrait
