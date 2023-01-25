/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
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

#include "substrait/plan.pb.h"

#include <google/protobuf/util/json_util.h>

// Utility function to read a JSON file from disk.
std::string readFromFile(const std::string& msgPath);

class PlanConverter {
 public:
  PlanConverter() {
    initialized_ = false;
  }

  // MEGAHACK -- Consider creating a version that does not require a copy.
  explicit PlanConverter(const substrait::Plan& plan) {
    plan_ = plan;
    initialized_ = true;
  }

  // Reads a plan from a json-encoded text proto.
  // Throws an exception if the JSON can not be loaded.
  void loadFromJSON(const std::string& json);

  // MEGAHACK -- Add real error handling (ideally uses an error collector).
  // Reads a plan which utilizes the text-based language for plans.
  // Throws an exception if the text cannot be parsed.
  void loadFromText(const std::string& text);

  // Loads a serialized binary text plan.
  void loadFromBinary(const std::string& blob);

  // Outputs the internal plan as a text plan.
  std::string toString();

  // Returns a reference to the internal plan.
  const substrait::Plan& plan() {
    return plan_;
  }

  // Returns true if the converter contains a valid plan.
  bool ok() const {
    return initialized_;
  }


 private:
  bool initialized_;
  substrait::Plan plan_;

  static std::string functionsToText(const substrait::Plan& plan);
  static std::string sourcesToText(const substrait::Plan& plan);
  static std::string schemasToText(const substrait::Plan& plan);
  static std::string relationsToText(const substrait::Plan& plan);
  static std::string pipelinesToText(const substrait::Plan& plan);
};
