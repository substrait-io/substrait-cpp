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

#include "yaml-cpp/yaml.h"

#include "SubstraitExtension.h"

namespace YAML {

using namespace io::substrait;

static bool decodeFunctionVariant(
    const Node& node,
    SubstraitFunctionVariant& function) {
  auto& returnType = node["return"];
  if (returnType && returnType.IsScalar()) {
    /// Return type can be an expression.
    const auto& returnExpr = returnType.as<std::string>();
    std::stringstream ss(returnExpr);

    // TODO: currently we only parse the last sentence of type definition, use
    // ANTLR in future.
    std::string lastReturnType;
    while (std::getline(ss, lastReturnType, '\n')) {
    }
    function.returnType = SubstraitType::decode(lastReturnType);
  }
  auto& args = node["args"];
  if (args && args.IsSequence()) {
    for (auto& arg : args) {
      if (arg["options"]) { // enum argument
        auto enumArgument = std::make_shared<SubstraitEnumArgument>(
            arg.as<SubstraitEnumArgument>());
        function.arguments.emplace_back(enumArgument);
      } else if (arg["value"]) { // value argument
        auto valueArgument = std::make_shared<SubstraitValueArgument>(
            arg.as<SubstraitValueArgument>());
        function.arguments.emplace_back(valueArgument);
      } else { // type argument
        auto typeArgument = std::make_shared<SubstraitTypeArgument>(
            arg.as<SubstraitTypeArgument>());
        function.arguments.emplace_back(typeArgument);
      }
    }
  }

  auto& variadic = node["variadic"];
  if (variadic) {
    auto& min = variadic["min"];
    auto& max = variadic["max"];
    if (min) {
      function.variadic = std::make_optional<SubstraitFunctionVariadic>(
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
struct convert<SubstraitEnumArgument> {
  static bool decode(const Node& node, SubstraitEnumArgument& argument) {
    // 'options' is required property
    auto& options = node["options"];
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
struct convert<SubstraitValueArgument> {
  static bool decode(const Node& node, SubstraitValueArgument& argument) {
    auto& value = node["value"];
    if (value && value.IsScalar()) {
      auto valueType = value.as<std::string>();
      argument.type = SubstraitType::decode(valueType);
      return true;
    }
    return false;
  }
};

template <>
struct convert<SubstraitTypeArgument> {
  static bool decode(const Node& node, SubstraitTypeArgument& argument) {
    // no properties need to populate for type argument, just return true if
    // 'type' element exists.
    return node["type"];
  }
};

template <>
struct convert<SubstraitScalarFunctionVariant> {
  static bool decode(
      const Node& node,
      SubstraitScalarFunctionVariant& function) {
    return decodeFunctionVariant(node, function);
  };
};

template <>
struct convert<SubstraitAggregateFunctionVariant> {
  static bool decode(
      const Node& node,
      SubstraitAggregateFunctionVariant& function) {
    const auto& res = decodeFunctionVariant(node, function);
    if (res) {
      const auto& intermediate = node["intermediate"];
      if (intermediate) {
        function.intermediate =
            SubstraitType::decode(intermediate.as<std::string>());
      }
    }
    return res;
  }
};

template <>
struct convert<io::substrait::SubstraitTypeVariant> {
  static bool decode(
      const Node& node,
      io::substrait::SubstraitTypeVariant& typeAnchor) {
    auto& name = node["name"];
    if (name && name.IsScalar()) {
      typeAnchor.name = name.as<std::string>();
      return true;
    }
    return false;
  }
};

} // namespace YAML

namespace io::substrait {

namespace {

std::string getSubstraitExtensionAbsolutePath() {
  const std::string absolute_path = __FILE__;
  auto const pos = absolute_path.find_last_of('/');
  return absolute_path.substr(0, pos) + "/../third_party/substrait/extensions/";
}

} // namespace

std::shared_ptr<SubstraitExtension> SubstraitExtension::load() {
  static const auto registry = loadDefault();
  return registry;
}

std::shared_ptr<SubstraitExtension> SubstraitExtension::loadDefault() {
  static const std::vector<std::string> extensionFiles = {
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
      "unknown.yaml",
  };
  const auto& extensionRootPath = getSubstraitExtensionAbsolutePath();
  return load(extensionRootPath, extensionFiles);
}

std::shared_ptr<SubstraitExtension> SubstraitExtension::load(
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

std::shared_ptr<SubstraitExtension> SubstraitExtension::load(
    const std::vector<std::string>& extensionFiles) {
  auto registry = std::make_shared<SubstraitExtension>();
  for (const auto& extensionUri : extensionFiles) {
    const auto& node = YAML::LoadFile(extensionUri);

    auto& scalarFunctions = node["scalar_functions"];
    if (scalarFunctions && scalarFunctions.IsSequence()) {
      for (auto& scalarFunctionNode : scalarFunctions) {
        const auto functionName = scalarFunctionNode["name"].as<std::string>();
        for (auto& scalaFunctionVariantNode : scalarFunctionNode["impls"]) {
          auto scalarFunctionVariant =
              scalaFunctionVariantNode.as<SubstraitScalarFunctionVariant>();
          scalarFunctionVariant.name = functionName;
          scalarFunctionVariant.uri = extensionUri;
          registry->addFunctionVariant(
              std::make_shared<SubstraitScalarFunctionVariant>(
                  scalarFunctionVariant));
        }
      }
    }

    auto& aggregateFunctions = node["aggregate_functions"];
    if (aggregateFunctions && aggregateFunctions.IsSequence()) {
      for (auto& aggregateFunctionNode : aggregateFunctions) {
        const auto functionName =
            aggregateFunctionNode["name"].as<std::string>();
        for (auto& aggregateFunctionVariantNode :
             aggregateFunctionNode["impls"]) {
          auto aggregateFunctionVariant =
              aggregateFunctionVariantNode
                  .as<SubstraitAggregateFunctionVariant>();
          aggregateFunctionVariant.name = functionName;
          aggregateFunctionVariant.uri = extensionUri;
          registry->addFunctionVariant(
              std::make_shared<SubstraitAggregateFunctionVariant>(
                  aggregateFunctionVariant));
        }
      }
    }

    auto& types = node["types"];
    if (types && types.IsSequence()) {
      for (auto& type : types) {
        auto typeAnchor = type.as<SubstraitTypeVariant>();
        typeAnchor.uri = extensionUri;
        registry->addTypeVariant(
            std::make_shared<SubstraitTypeVariant>(typeAnchor));
      }
    }
  }
  return registry;
}

void SubstraitExtension::addFunctionVariant(
    const SubstraitFunctionVariantPtr& functionVariant) {
  const auto& functionVariants =
      functionVariantMap_.find(functionVariant->name);
  if (functionVariants != functionVariantMap_.end()) {
    auto& variants = functionVariants->second;
    variants.emplace_back(functionVariant);
  } else {
    std::vector<SubstraitFunctionVariantPtr> variants;
    variants.emplace_back(functionVariant);
    functionVariantMap_.insert({functionVariant->name, variants});
  }
}

void SubstraitExtension::addTypeVariant(
    const SubstraitTypeVariantPtr& functionVariant) {
  typeVariantMap_.insert({functionVariant->name, functionVariant});
}

SubstraitTypeVariantPtr SubstraitExtension::lookupType(
    const std::string& typeName) const {
  auto typeVariantIter = typeVariantMap_.find(typeName);
  if (typeVariantIter != typeVariantMap_.end()) {
    return typeVariantIter->second;
  }
  return nullptr;
}

SubstraitFunctionVariantPtr SubstraitExtension::lookupFunction(
    const std::string& name,
    const std::vector<SubstraitTypePtr>& types) const {
  auto functionVariantIter = functionVariantMap_.find(name);
  if (functionVariantIter != functionVariantMap_.end()) {
    for (const auto& candidateFunctionVariant : functionVariantIter->second) {
      if (candidateFunctionVariant->tryMatch(types)) {
        return candidateFunctionVariant;
      }
    }
  }
  return nullptr;
}

} // namespace io::substrait
