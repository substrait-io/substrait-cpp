/* SPDX-License-Identifier: Apache-2.0 */

#include "StringManipulation.h"

namespace io::substrait::textplan {

// Yields true if the string 'haystack' starts with the string 'needle'.
bool startsWith(std::string_view haystack, std::string_view needle) {
  return haystack.size() > needle.size() &&
      haystack.substr(0, needle.size()) == needle;
}

// Returns true if the string 'haystack' ends with the string 'needle'.
bool endsWith(std::string_view haystack, std::string_view needle) {
  return haystack.size() > needle.size() &&
      haystack.substr(haystack.size() - needle.size(), needle.size()) == needle;
}

}  // namespace io::substrait::textplan
