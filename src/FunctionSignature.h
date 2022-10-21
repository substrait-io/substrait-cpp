/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include "FunctionMapping.h"
#include "Type.h"

namespace io::substrait {

class FunctionSignature {
public:
  /// construct the substrait function signature with function name, return type
  /// and arguments.
  FunctionSignature(const std::string &name,
                    const std::vector<TypePtr> &arguments,
                    const TypePtr &returnType)
      : name_(name), arguments_(arguments), returnType_(returnType) {}

  const std::string getName() const { return name_; }

  const std::vector<TypePtr> getArguments() const { return arguments_; }

  const TypePtr getReturnType() const { return returnType_; }

private:
  const std::string name_;
  const std::vector<TypePtr> arguments_;
  const TypePtr returnType_;
};

} // namespace io::substrait
