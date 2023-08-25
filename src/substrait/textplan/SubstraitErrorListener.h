/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

namespace io::substrait::textplan {

struct ErrorLocation {
  size_t line;
  size_t charPositionInLine;
};

struct ErrorInstance {
  ErrorLocation location;
  std::string message;
};

// SubstraitErrorListener is similar in behavior to an antlr4::ErrorListener to
// provide a similar error collection methodology regardless of how the input
// data is obtained.
class SubstraitErrorListener {
 public:
  SubstraitErrorListener() = default;

  void addError(size_t linenum, size_t charnum, const std::string& msg);

  void addError(const std::string& msg) {
    addError(-1, -1, msg);
  };

  void addErrorInstances(const std::vector<ErrorInstance>& errors) {
    errors_.insert(errors_.end(), errors.begin(), errors.end());
  }

  const std::vector<ErrorInstance>& getErrors() {
    return errors_;
  };

  bool hasErrors() {
    return !errors_.empty();
  }

  std::vector<std::string> getErrorMessages();

 private:
  std::vector<ErrorInstance> errors_;
};

} // namespace io::substrait::textplan
