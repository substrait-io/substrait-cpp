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

#include <yaml-cpp/yaml.h>
#include "substrait/function/Extension.h"

bool decodeFunctionVariant(
    const YAML::Node& node,
    substrait::FunctionVariant& function) {
  const auto& returnType = node["return"];
  if (returnType && returnType.IsScalar()) {
    /// Return type can be an expression.
    const auto& returnExpr = returnType.as<std::string>();
    std::stringstream ss(returnExpr);

    // TODO: currently we only parse the last sentence of type definition, use
    // ANTLR in future.
    std::string lastReturnType;
    while (std::getline(ss, lastReturnType, '\n')) {
    }
    function.returnType = substrait::Type::decode(lastReturnType);
  }
  const auto& args = node["args"];
  if (args && args.IsSequence()) {
    for (auto& arg : args) {
      if (arg["options"]) { // enum argument
        auto enumArgument = std::make_shared<substrait::EnumArgument>(
            arg.as<substrait::EnumArgument>());
        function.arguments.emplace_back(enumArgument);
      } else if (arg["value"]) { // value argument
        auto valueArgument = std::make_shared<substrait::ValueArgument>(
            arg.as<substrait::ValueArgument>());
        function.arguments.emplace_back(valueArgument);
      } else { // type argument
        auto typeArgument = std::make_shared<substrait::TypeArgument>(
            arg.as<substrait::TypeArgument>());
        function.arguments.emplace_back(typeArgument);
      }
    }
  }

  const auto& variadic = node["variadic"];
  if (variadic) {
    auto& min = variadic["min"];
    auto& max = variadic["max"];
    if (min) {
      function.variadic = std::make_optional<substrait::FunctionVariadic>(
          {min.as<int>(),
           max ? std::make_optional<int>(max.as<int>()) : std::nullopt});
    } else {
      function.variadic = std::nullopt;
    }
  } else {
    function.variadic = std::nullopt;
  }

  return true;
}

template <>
struct YAML::convert<substrait::EnumArgument> {
  static bool decode(const Node& node, substrait::EnumArgument& argument) {
    // 'options' is required property
    const auto& options = node["options"];
    if (options && options.IsSequence()) {
      auto& required = node["required"];
      argument.required = required && required.as<bool>();
      return true;
    } else {
      return false;
    }
  }
};

template <>
struct YAML::convert<substrait::ValueArgument> {
  static bool decode(const Node& node, substrait::ValueArgument& argument) {
    const auto& value = node["value"];
    if (value && value.IsScalar()) {
      auto valueType = value.as<std::string>();
      argument.type = substrait::Type::decode(valueType);
      return true;
    }
    return false;
  }
};

template <>
struct YAML::convert<substrait::TypeArgument> {
  static bool decode(
      const YAML::Node& node,
      substrait::TypeArgument& argument) {
    // no properties need to populate for type argument, just return true if
    // 'type' element exists.
    if (node["type"]) {
      return true;
    }
    return false;
  }
};

template <>
struct YAML::convert<substrait::ScalarFunctionVariant> {
  static bool decode(
      const Node& node,
      substrait::ScalarFunctionVariant& function) {
    return decodeFunctionVariant(node, function);
  };
};

template <>
struct YAML::convert<substrait::AggregateFunctionVariant> {
  static bool decode(
      const Node& node,
      substrait::AggregateFunctionVariant& function) {
    const auto& res = decodeFunctionVariant(node, function);
    if (res) {
      const auto& intermediate = node["intermediate"];
      if (intermediate) {
        function.intermediate =
            substrait::ParameterizedType::decode(intermediate.as<std::string>());
      }
    }
    return res;
  }
};

template <>
struct YAML::convert<substrait::TypeVariant> {
  static bool decode(const Node& node, substrait::TypeVariant& typeAnchor) {
    const auto& name = node["name"];
    if (name && name.IsScalar()) {
      typeAnchor.name = name.as<std::string>();
      return true;
    }
    return false;
  }
};

namespace substrait {

std::shared_ptr<Extension> Extension::load(const std::string& basePath) {
  static const std::vector<std::string> extensionFiles{
      "functions_aggregate_approx.yaml",
      "functions_aggregate_generic.yaml",
      "functions_arithmetic.yaml",
      "functions_arithmetic_decimal.yaml",
      "functions_boolean.yaml",
      "functions_comparison.yaml",
      "functions_datetime.yaml",
      "functions_logarithmic.yaml",
      "functions_rounding.yaml",
      "functions_string.yaml",
      "functions_set.yaml",
  };
  return load(basePath, extensionFiles);
}

std::shared_ptr<Extension> Extension::load(
    const std::string& basePath,
    const std::vector<std::string>& extensionFiles) {
  std::vector<std::string> yamlExtensionFiles;
  yamlExtensionFiles.reserve(extensionFiles.size());
  for (auto& extensionFile : extensionFiles) {
    auto const pos = basePath.find_last_of('/');
    const auto& extensionUri = basePath.substr(0, pos) + "/" + extensionFile;
    yamlExtensionFiles.emplace_back(extensionUri);
  }
  return load(yamlExtensionFiles);
}

std::shared_ptr<Extension> Extension::load(
    const std::vector<std::string>& extensionFiles) {
  auto extension = std::make_shared<Extension>();
  for (const auto& extensionUri : extensionFiles) {
    const auto& node = YAML::LoadFile(extensionUri);

    const auto& scalarFunctions = node["scalar_functions"];
    if (scalarFunctions && scalarFunctions.IsSequence()) {
      for (auto& scalarFunctionNode : scalarFunctions) {
        const auto functionName = scalarFunctionNode["name"].as<std::string>();
        for (auto& scalaFunctionVariantNode : scalarFunctionNode["impls"]) {
          auto scalarFunctionVariant =
              scalaFunctionVariantNode.as<ScalarFunctionVariant>();
          scalarFunctionVariant.name = functionName;
          scalarFunctionVariant.uri = extensionUri;
          extension->addScalarFunctionVariant(
              std::make_shared<ScalarFunctionVariant>(scalarFunctionVariant));
        }
      }
    }

    const auto& aggregateFunctions = node["aggregate_functions"];
    if (aggregateFunctions && aggregateFunctions.IsSequence()) {
      for (auto& aggregateFunctionNode : aggregateFunctions) {
        const auto functionName =
            aggregateFunctionNode["name"].as<std::string>();
        for (auto& aggregateFunctionVariantNode :
             aggregateFunctionNode["impls"]) {
          auto aggregateFunctionVariant =
              aggregateFunctionVariantNode.as<AggregateFunctionVariant>();
          aggregateFunctionVariant.name = functionName;
          aggregateFunctionVariant.uri = extensionUri;
          extension->addAggregateFunctionVariant(
              std::make_shared<AggregateFunctionVariant>(
                  aggregateFunctionVariant));
        }
      }
    }

    const auto& types = node["types"];
    if (types && types.IsSequence()) {
      for (auto& type : types) {
        auto typeAnchor = type.as<TypeVariant>();
        typeAnchor.uri = extensionUri;
        extension->addTypeVariant(std::make_shared<TypeVariant>(typeAnchor));
      }
    }
  }
  return extension;
}

void Extension::addWindowFunctionVariant(
    const FunctionVariantPtr& functionVariant) {
  const auto& functionVariants =
      windowFunctionVariantMap_.find(functionVariant->name);
  if (functionVariants != windowFunctionVariantMap_.end()) {
    auto& variants = functionVariants->second;
    variants.emplace_back(functionVariant);
  } else {
    std::vector<FunctionVariantPtr> variants;
    variants.emplace_back(functionVariant);
    windowFunctionVariantMap_.insert(
        {functionVariant->name, std::move(variants)});
  }
}

void Extension::addTypeVariant(const TypeVariantPtr& functionVariant) {
  typeVariantMap_.insert({functionVariant->name, functionVariant});
}

TypeVariantPtr Extension::lookupType(const std::string& typeName) const {
  auto typeVariantIter = typeVariantMap_.find(typeName);
  if (typeVariantIter != typeVariantMap_.end()) {
    return typeVariantIter->second;
  }
  return nullptr;
}

void Extension::addScalarFunctionVariant(
    const FunctionVariantPtr& functionVariant) {
  const auto& functionVariants =
      scalarFunctionVariantMap_.find(functionVariant->name);
  if (functionVariants != scalarFunctionVariantMap_.end()) {
    auto& variants = functionVariants->second;
    variants.emplace_back(functionVariant);
  } else {
    std::vector<FunctionVariantPtr> variants;
    variants.emplace_back(functionVariant);
    scalarFunctionVariantMap_.insert(
        {functionVariant->name, std::move(variants)});
  }
}

void Extension::addAggregateFunctionVariant(
    const FunctionVariantPtr& functionVariant) {
  const auto& functionVariants =
      aggregateFunctionVariantMap_.find(functionVariant->name);
  if (functionVariants != aggregateFunctionVariantMap_.end()) {
    auto& variants = functionVariants->second;
    variants.emplace_back(functionVariant);
  } else {
    std::vector<FunctionVariantPtr> variants;
    variants.emplace_back(functionVariant);
    aggregateFunctionVariantMap_.insert(
        {functionVariant->name, std::move(variants)});
  }
}

} // namespace substrait
