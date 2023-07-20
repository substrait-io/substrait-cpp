/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace io::substrait::textplan {

// Yields true if the string 'haystack' starts with the string 'needle'.
bool startsWith(std::string_view haystack, std::string_view needle);

// Returns true if the string 'haystack' ends with the string 'needle'.
bool endsWith(std::string_view haystack, std::string_view needle);

// Joins a vector of strings into a single string separated by separator.
std::string joinLines(
    std::vector<std::string> lines,
    std::string_view separator = "\n");

} // namespace io::substrait::textplan
