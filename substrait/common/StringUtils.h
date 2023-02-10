/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <algorithm>
#include <iostream>
#include <string_view>

namespace io::substrait::common {
class StringUtils final {
 public:
  static std::string ltrim(std::string_view s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    std::string_view res = (start == std::string::npos) ? "" : s.substr(start);
    return static_cast<std::string>(res);
  }

  static std::string rtrim(std::string_view s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    std::string_view res = (end == std::string::npos) ? "" : s.substr(0, end + 1);
    return static_cast<std::string>(res);
  }

  static std::string trim(std::string_view s) {
    return rtrim(ltrim(s));
  }

  static const std::string WHITESPACE;
};

const std::string StringUtils::WHITESPACE = " \n\r\t\f\v";

} // namespace io::substrait::common
