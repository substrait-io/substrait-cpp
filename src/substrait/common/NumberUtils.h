/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <algorithm>
#include <string>

namespace io::substrait::common {

class NumberUtils {
 public:
  static bool isInteger(std::string_view s) {
    return isNonNegativeInteger(s) || isNegativeInteger(s);
  }

  static bool isNonNegativeInteger(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(),  [](unsigned char d) {
      return std::isdigit(d);
    });
  }

  static bool isNegativeInteger(std::string_view s) {
    return s.size() >= 2 && s[0] == '-' && isNonNegativeInteger(s.substr(1));
  }
};

} // namespace io::substrait::common