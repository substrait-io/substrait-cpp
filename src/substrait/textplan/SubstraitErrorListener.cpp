/* SPDX-License-Identifier: Apache-2.0 */

#include "SubstraitErrorListener.h"

#include <string>
#include <vector>

namespace io::substrait::textplan {

void SubstraitErrorListener::addError(
    size_t linenum,
    size_t charnum,
    const std::string& msg) {
  errors_.push_back({{linenum, charnum}, msg});
}

std::vector<std::string> SubstraitErrorListener::getErrorMessages() {
  std::vector<std::string> messages;
  for (const auto& instance : getErrors()) {
    if (instance.location.line == -1 ||
        instance.location.charPositionInLine == -1) {
      messages.push_back(instance.message);
      continue;
    }
    messages.push_back(
        std::to_string(instance.location.line) + ":" +
        std::to_string(instance.location.charPositionInLine) + " → " +
        instance.message);
  }
  return messages;
}

} // namespace io::substrait::textplan
