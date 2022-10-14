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

#include "memory"
#include "unordered_map"
#include "vector"

namespace io::substrait {

using FunctionMap = std::unordered_map<std::string, std::string>;

/// An interface describe the function names in difference between engine-own
/// and substrait system.
class FunctionMapping {
public:
  /// Scalar function names in difference between engine own and substrait.
  virtual const FunctionMap scalaMapping() const {
    static const FunctionMap scalaFunctionMap{};
    return scalaFunctionMap;
  }

  /// Scalar function names in difference between engine own and substrait.
  virtual const FunctionMap aggregateMapping() const {
    static const FunctionMap aggregateFunctionMap{};
    return aggregateFunctionMap;
  }

  virtual const FunctionMap windowMapping() const {
    static const FunctionMap windowFunctionMap{};
    return windowFunctionMap;
  }
};

using FunctionMappingPtr = std::shared_ptr<const FunctionMapping >;
} // namespace io::substrait
