/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <algorithm>
#include <string>
#include <string_view>

namespace io::substrait::common {
class StringUtils final {
 public:
  static std::string_view ltrim(std::string_view s) {
    size_t start = s.find_first_not_of(kWhitespace);
    return (start == std::string::npos) ? "" : s.substr(start);
  }

  static std::string_view rtrim(std::string_view s) {
    size_t end = s.find_last_not_of(kWhitespace);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
  }

  static std::string_view trim(std::string_view s) {
    return rtrim(ltrim(s));
  }

  static constexpr std::string_view kWhitespace = " \n\r\t\f\v";
};

} // namespace io::substrait::common
