/* SPDX-License-Identifier: Apache-2.0 */

#include "substrait/function/Extension.h"

#include <yaml-cpp/yaml.h>

bool DecodeFunctionImpl(
    const YAML::Node& node,
    io::substrait::FunctionImplementation& function) {
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
    function.returnType =
        io::substrait::ParameterizedType::decode(lastReturnType);
  }
  const auto& args = node["args"];
  if (args && args.IsSequence()) {
    for (const auto& arg : args) {
      if (arg["options"]) { // enum argument
        auto enumArgument = std::make_shared<io::substrait::EnumArgument>(
            arg.as<io::substrait::EnumArgument>());
        function.arguments.emplace_back(enumArgument);
      } else if (arg["value"]) { // value argument
        auto valueArgument = std::make_shared<io::substrait::ValueArgument>(
            arg.as<io::substrait::ValueArgument>());
        function.arguments.emplace_back(valueArgument);
      } else { // type argument
        auto typeArgument = std::make_shared<io::substrait::TypeArgument>(
            arg.as<io::substrait::TypeArgument>());
        function.arguments.emplace_back(typeArgument);
      }
    }
  }

  const auto& variadic = node["variadic"];
  if (variadic) {
    const auto& min = variadic["min"];
    const auto& max = variadic["max"];
    if (min) {
      function.variadic = std::make_optional<io::substrait::FunctionVariadic>(
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
struct YAML::convert<io::substrait::EnumArgument> {
  static bool decode(const Node& node, io::substrait::EnumArgument& argument) {
    // 'options' is required property
    const auto& options = node["options"];
    if (options && options.IsSequence()) {
      const auto& required = node["required"];
      argument.required = required && required.as<bool>();
      return true;
    } else {
      return false;
    }
  }
};

template <>
struct YAML::convert<io::substrait::ValueArgument> {
  static bool decode(const Node& node, io::substrait::ValueArgument& argument) {
    const auto& value = node["value"];
    if (value && value.IsScalar()) {
      auto valueType = value.as<std::string>();
      argument.type = io::substrait::ParameterizedType::decode(valueType);
      return true;
    }
    return false;
  }
};

template <>
struct YAML::convert<io::substrait::TypeArgument> {
  static bool decode(
      const YAML::Node& node,
      io::substrait::TypeArgument& argument) {
    // no properties need to populate for type argument, just return true if
    // 'type' element exists.
    return static_cast<bool>(node["type"]);
  }
};

template <>
struct YAML::convert<io::substrait::ScalarFunctionImplementation> {
  static bool decode(
      const Node& node,
      io::substrait::ScalarFunctionImplementation& function) {
    return DecodeFunctionImpl(node, function);
  };
};

template <>
struct YAML::convert<io::substrait::AggregateFunctionImplementation> {
  static bool decode(
      const Node& node,
      io::substrait::AggregateFunctionImplementation& function) {
    const auto& res = DecodeFunctionImpl(node, function);
    if (res) {
      const auto& intermediate = node["intermediate"];
      if (intermediate) {
        function.intermediate = io::substrait::ParameterizedType::decode(
            intermediate.as<std::string>());
      }
    }
    return res;
  }
};

template <>
struct YAML::convert<io::substrait::TypeVariant> {
  static bool decode(const Node& node, io::substrait::TypeVariant& typeAnchor) {
    const auto& name = node["name"];
    if (name && name.IsScalar()) {
      typeAnchor.name = name.as<std::string>();
      return true;
    }
    return false;
  }
};

namespace io::substrait {

std::shared_ptr<Extension> Extension::load(const std::string& basePath) {
  static const std::vector<std::string> kExtensionFiles{
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
  return load(basePath, kExtensionFiles);
}

std::shared_ptr<Extension> Extension::load(
    const std::string& basePath,
    const std::vector<std::string>& extensionFiles) {
  std::vector<std::string> yamlExtensionFiles;
  yamlExtensionFiles.reserve(extensionFiles.size());
  for (const auto& extensionFile : extensionFiles) {
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
      for (const auto& scalarFunctionNode : scalarFunctions) {
        const auto functionName = scalarFunctionNode["name"].as<std::string>();
        for (const auto& scalaFunctionImplNode : scalarFunctionNode["impls"]) {
          auto scalarFunctionImpl =
              scalaFunctionImplNode.as<ScalarFunctionImplementation>();
          scalarFunctionImpl.name = functionName;
          scalarFunctionImpl.uri = extensionUri;
          extension->addScalarFunctionImpl(
              std::make_shared<ScalarFunctionImplementation>(
                  scalarFunctionImpl));
        }
      }
    }

    const auto& aggregateFunctions = node["aggregate_functions"];
    if (aggregateFunctions && aggregateFunctions.IsSequence()) {
      for (const auto& aggregateFunctionNode : aggregateFunctions) {
        const auto functionName =
            aggregateFunctionNode["name"].as<std::string>();
        for (const auto& aggregateFunctionImplNode :
             aggregateFunctionNode["impls"]) {
          auto aggregateFunctionImpl =
              aggregateFunctionImplNode.as<AggregateFunctionImplementation>();
          aggregateFunctionImpl.name = functionName;
          aggregateFunctionImpl.uri = extensionUri;
          extension->addAggregateFunctionImpl(
              std::make_shared<AggregateFunctionImplementation>(
                  aggregateFunctionImpl));
        }
      }
    }

    const auto& types = node["types"];
    if (types && types.IsSequence()) {
      for (const auto& type : types) {
        auto typeAnchor = type.as<TypeVariant>();
        typeAnchor.uri = extensionUri;
        extension->addTypeVariant(std::make_shared<TypeVariant>(typeAnchor));
      }
    }
  }
  return extension;
}

void Extension::addWindowFunctionImpl(
    const FunctionImplementationPtr& functionImpl) {
  const auto& functionImpls = windowFunctionImplMap_.find(functionImpl->name);
  if (functionImpls != windowFunctionImplMap_.end()) {
    auto& impls = functionImpls->second;
    impls.emplace_back(functionImpl);
  } else {
    std::vector<FunctionImplementationPtr> impls;
    impls.emplace_back(functionImpl);
    windowFunctionImplMap_.insert({functionImpl->name, std::move(impls)});
  }
}

void Extension::addTypeVariant(const TypeVariantPtr& typeVariant) {
  typeVariantMap_.insert({typeVariant->name, typeVariant});
}

TypeVariantPtr Extension::lookupType(const std::string& typeName) const {
  auto typeVariantIter = typeVariantMap_.find(typeName);
  if (typeVariantIter != typeVariantMap_.end()) {
    return typeVariantIter->second;
  }
  return nullptr;
}

void Extension::addScalarFunctionImpl(
    const FunctionImplementationPtr& functionImpl) {
  const auto& functionImpls = scalarFunctionImplMap_.find(functionImpl->name);
  if (functionImpls != scalarFunctionImplMap_.end()) {
    auto& impls = functionImpls->second;
    impls.emplace_back(functionImpl);
  } else {
    std::vector<FunctionImplementationPtr> impls;
    impls.emplace_back(functionImpl);
    scalarFunctionImplMap_.insert({functionImpl->name, std::move(impls)});
  }
}

void Extension::addAggregateFunctionImpl(
    const FunctionImplementationPtr& functionImpl) {
  const auto& functionImpls =
      aggregateFunctionImplMap_.find(functionImpl->name);
  if (functionImpls != aggregateFunctionImplMap_.end()) {
    auto& impls = functionImpls->second;
    impls.emplace_back(functionImpl);
  } else {
    std::vector<FunctionImplementationPtr> impls;
    impls.emplace_back(functionImpl);
    aggregateFunctionImplMap_.insert({functionImpl->name, std::move(impls)});
  }
}

} // namespace io::substrait
