/* SPDX-License-Identifier: Apache-2.0 */

#include "StringManipulation.h"

#include <numeric>
#include <string>
#include <string_view>
#include <vector>

namespace io::substrait::textplan {

bool startsWith(std::string_view haystack, std::string_view needle) {
  return haystack.size() > needle.size() &&
      haystack.substr(0, needle.size()) == needle;
}

bool endsWith(std::string_view haystack, std::string_view needle) {
  return haystack.size() > needle.size() &&
      haystack.substr(haystack.size() - needle.size(), needle.size()) == needle;
}

} // namespace io::substrait::textplan
