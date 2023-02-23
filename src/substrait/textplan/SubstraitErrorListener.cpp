/* SPDX-License-Identifier: Apache-2.0 */

#include "SubstraitErrorListener.h"

#include <string>
#include <vector>

namespace io::substrait::textplan {

void SubstraitErrorListener::AddError(
    size_t linenum,
    size_t charnum,
    const std::string& msg) {
  errors_.push_back({{linenum, charnum}, msg});
}

std::vector<std::string> SubstraitErrorListener::GetErrorMessages() {
  std::vector<std::string> messages;
  for (const auto& instance : GetErrors()) {
    messages.push_back(
        std::to_string(instance.location.line) + ":" +
        std::to_string(instance.location.char_position_in_line) + " → " +
        instance.message);
  }
  return messages;
}

} // namespace io::substrait::textplan