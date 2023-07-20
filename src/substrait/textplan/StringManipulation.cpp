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

std::string joinLines(
    std::vector<std::string> lines,
    std::string_view separator) {
  auto concatWithSeparator = [separator](std::string a, const std::string& b) {
    return std::move(a) + std::string(separator) + b;
  };

  auto result = std::accumulate(
      std::next(lines.begin()), lines.end(), lines[0], concatWithSeparator);
  return result;
}

} // namespace io::substrait::textplan
