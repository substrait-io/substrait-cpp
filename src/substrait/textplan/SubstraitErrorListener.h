/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <vector>

namespace io::substrait::textplan {

struct ErrorLocation {
  size_t line;
  size_t char_position_in_line;
};

struct ErrorInstance {
  ErrorLocation location;
  std::string message;
};

class SubstraitErrorListener {
 public:
  SubstraitErrorListener() = default;

  void AddError(size_t linenum, size_t charnum, const std::string& msg);

  const std::vector<ErrorInstance>& GetErrors() {
    return errors_;
  };

  bool HasErrors() {
    return !errors_.empty();
  }

  std::vector<std::string> GetErrorMessages();

 private:
  std::vector<ErrorInstance> errors_;
};

} // namespace io::substrait::textplan